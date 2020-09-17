/*
 * Copyright 2016-2020 Digi International Inc., All Rights Reserved
 *
 * This software contains proprietary and confidential information of Digi
 * International Inc.  By accepting transfer of this copy, Recipient agrees
 * to retain this software in confidence, to prevent disclosure to others,
 * and to make no use of this software other than that for which it was
 * delivered.  This is an unpublished copyrighted work of Digi International
 * Inc.  Except as permitted by federal law, 17 USC 117, copying is strictly
 * prohibited.
 *
 * Restricted Rights Legend
 *
 * Use, duplication, or disclosure by the Government is subject to
 * restrictions set forth in sub-paragraph (c)(1)(ii) of The Rights in
 * Technical Data and Computer Software clause at DFARS 252.227-7031 or
 * subparagraphs (c)(1) and (2) of the Commercial Computer Software -
 * Restricted Rights at 48 CFR 52.227-19, as applicable.
 *
 * Digi International Inc., 9350 Excelsior Blvd., Suite 700, Hopkins, MN 55343
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "utils.h"

/**
 * get_cmd_output() - Execute the given command and return the output
 *
 * @cmd:        Command to be executed, must include full path.
 * @params:     Command parameters to be passed.
 * @out:        Command's output buffer.
 * @outsize:    Output's buffer size
 *
 * Return:      0 if success, 1 otherwise.
 *
 * NOTE: The execution of a command is usually done using the 'popen'
 *       function, but since the Android recovery initramfs does not have a
 *       shell, a re-implementation of the 'popen' mechanism is done here.
 */
int get_cmd_output(const char *cmd, char *params[], char *out, size_t outsize)
{
	int link[2];
	int result = 0;
	pid_t pid;

	/* Create communication channel pipes. */
	if (pipe(link) == -1) {
		printf
		    ("get_cmd_output(): error creating communication pipes\n");
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
			printf
			    ("get_cmd_output(): error waiting for child process\n");
			return 1;
		}

		/* Get command's output */
		close(link[1]);
		if (out) {
			nbytes = read(link[0], out, outsize);
			if (nbytes == -1) {
				printf
				    ("get_cmd_output(): error reading output\n");
				return 1;
			}
			out[nbytes - 1] = 0;
		}
		close(link[0]);

		/* Check child application exit status. */
		if (WIFEXITED(result) && WEXITSTATUS(result)) {
			printf
			    ("get_cmd_output(): child process exit with error\n");
			return 1;
		}
	}

	return 0;
}
