/*
 * Copyright (C) 2016 Digi International Inc., All Rights Reserved
 *
 * This software contains proprietary and confidential information of Digi.
 * International Inc. By accepting transfer of this copy, Recipient agrees
 * to retain this software in confidence, to prevent disclosure to others,
 * and to make no use of this software other than that for which it was
 * delivered. This is an unpublished copyrighted work of Digi International
 * Inc. Except as permitted by federal law, 17 USC 117, copying is strictly
 * prohibited.
 *
 * Restricted Rights Legend
 *
 * Use, duplication, or disclosure by the Government is subject to restrictions
 * set forth in sub-paragraph (c)(1)(ii) of The Rights in Technical Data and
 * Computer Software clause at DFARS 252.227-7031 or subparagraphs (c)(1) and
 * (2) of the Commercial Computer Software - Restricted Rights at 48 CFR
 * 52.227-19, as applicable.
 * 
 * Digi International Inc. 11001 Bren Road East, Minnetonka, MN 55343
 */

#include <bootloader_message/bootloader_settings.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

/*------------------------------------------------------------------------------
                             D E F I N I T I O N S
------------------------------------------------------------------------------*/
#define SBIN_PATH              "/sbin/"
#define SET_SETTING_COMMAND    "fw_setenv"
#define GET_SETTING_COMMAND    "fw_printenv"
#define VALUE_ONLY_PARAMETER   "-n"
#define ERROR_GET_SETTING      "Error retrieving the value for setting %s\n"
#define ERROR_SET_SETTING      "Error setting the value of %s to %s\n"
#define GET_SETTING_SUCCESS    "Value of setting %s is %s\n"
#define SET_SETTING_SUCCESS    "Value of setting %s successfully set to %s\n"

/*------------------------------------------------------------------------------
                     F U N C T I O N  D E F I N I T I O N S
------------------------------------------------------------------------------*/

/**
 * get_cmd_output() - Execute the given command and return the output
 *
 * @cmd:	Command to be executed, must include full path.
 * @params: 	Command parameters to be passed.
 * @buff:	Buffer for the output.
 * @buff_size:	Size of the buffer for the output.
 *
 * Return:	0 if success, 1 otherwise.
 *
 * NOTE: The execution of a command is usually done using the 'popen'
 *       function, but since the Android recovery initramfs does not have a
 *       shell, a re-implementation of the 'popen' mechanism is done here.
 */
static int get_cmd_output(const char * cmd, const char * params[], char * buff, size_t buff_size)
{
	int link[2];
	int result = 0;
	pid_t pid;

	/* Create communication channel pipes. */
	if (pipe(link) == -1) {
		printf("get_cmd_output(): error creating communication pipes\n");
		return 1;
	}
	/* Fork the process here. */
	if ((pid = fork()) == -1) {
		printf("get_cmd_output(): error forking parent process\n");
		return 1;
	}
	if (pid == 0) { /* Child process. */
		/* Redirect error and output stream to the created pipe. */
		if (dup2(link[1], STDOUT_FILENO) == -1 ||
				dup2(link[1], STDERR_FILENO) == -1) {
			printf("get_cmd_output(): error redirecting streams\n");
			exit(EXIT_FAILURE);
		}
		close(link[0]);
		close(link[1]);
		/* Execute the given command with parameters. */
		execv(cmd, params);
		/* If execution reaches this point it means there was an error. */
		printf("get_cmd_output(): error on 'execv'\n");
		exit(EXIT_FAILURE);
	} else { /* Parent process. */
		int nbytes;

		/* Wait for child application to finish. */
		if (wait(&result) == -1) {
			printf("get_cmd_output(): error waiting for child process\n");
			return 1;
		}

		close(link[1]);
		if (buff) {
			nbytes = read(link[0], buff, buff_size);
			if (nbytes == -1) {
				printf("get_cmd_output(): error reading buffer\n");
				return 1;
			}

			buff[nbytes - 1] = 0;
		}
		close(link[0]);

		/* Check child application exit status. */
		if (WIFEXITED(result) && WEXITSTATUS(result)) {
			printf("get_cmd_output(): child process exit with error\n");
			return 1;
		}
	}

	return 0;
}

/**
 * get_setting_value() - Returns the value of the given bootloader setting
 *
 * @setting:	Setting to retrieve its value.
 * @buff:	Buffer to store the setting value if success.
 * @size_buff:	Size of the buff variable.
 *
 * Return: 0 on success, 1 on error.
 */
int get_setting_value(const char *setting, char * buff, size_t buff_size)
{
    const char *command = SBIN_PATH GET_SETTING_COMMAND;
    const char *params[] = { GET_SETTING_COMMAND, VALUE_ONLY_PARAMETER, setting, NULL };
    char *out = NULL;
    int error = 0;

    // Execute the command.
    if (get_cmd_output(command, params, buff, buff_size) != 0) {
        printf(ERROR_GET_SETTING, setting);
        error = 1;
        goto done;
    }

    printf(GET_SETTING_SUCCESS, setting, buff);

done:
    free(out);
    return error;
}

/**
 * set_setting_value() - Sets the value of the given bootloader setting
 *
 * @setting:	Setting to set its value.
 * @value:		The new setting value.
 *
 * Return: 0 on success, 1 on error.
 */
int set_setting_value(const char *setting, const char *value)
{
    const char *command = SBIN_PATH SET_SETTING_COMMAND;
    const char *params[] = { SET_SETTING_COMMAND, (char *)setting, (char *)value, NULL };
    char *out = NULL;
    int error = 0;

    // Execute the command.
    if (get_cmd_output(command, params, NULL, 0) != 0) {
        printf(ERROR_SET_SETTING, setting, value);
        error = 1;
        goto done;
    }
    printf(SET_SETTING_SUCCESS, setting, value);

done:
    free(out);
    return error;
}
