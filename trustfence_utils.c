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

#include "utils.h"

#define SBIN_PATH           "/sbin/"
#define TRUSTFENCE_TOOL     "trustfence-tool"
#define NEW_KEY_COMMAND     "--newkey"
#define ERROR_MEMORY        "Error allocating memory\n"
#define ERROR_NEW_KEY       "Error configuring the new key:\n%s\n"
#define NEW_KEY_SUCCESS     "New key successfully configured\n"

/**
 * setup_key() - Set the encryption key to be used for the file system.
 *
 * @key: Base-64 encoded key to use, empty to generate a random one.
 *
 * Return: 0 on success, 1 on error.
 */
int setup_key(const char *const key)
{
	int ret = 0;
	const char *command = SBIN_PATH TRUSTFENCE_TOOL;
	char *param = NULL;
	char *params[3];
	char out[256];
	int len;

	/* Build the parameters array. */
	len = strlen(NEW_KEY_COMMAND) + 1;
	if (key)
		len += strlen(key) + 1;
	param = calloc(1, len);
	if (!param) {
		printf(ERROR_MEMORY);
		ret = 1;
		goto done;
	}
	strcpy(param, NEW_KEY_COMMAND);
	if (key && strlen(key) > 0) {
		strcat(param, "=");
		strcat(param, key);
	}
	params[0] = TRUSTFENCE_TOOL;
	params[1] = param;
	params[2] = NULL;

	/* Execute the command. */
	if (get_cmd_output(command, params, out, sizeof(out)) != 0) {
		printf(ERROR_NEW_KEY, out);
		ret = 1;
	} else {
		printf(NEW_KEY_SUCCESS);
	}

done:
	free(param);

	return ret;
}
