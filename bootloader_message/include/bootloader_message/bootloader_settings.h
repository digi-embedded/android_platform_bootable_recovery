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

#ifndef RECOVERY_BOOTLOADER_SETTINGS_H
#define RECOVERY_BOOTLOADER_SETTINGS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>

/*------------------------------------------------------------------------------
                             D E F I N I T I O N S
------------------------------------------------------------------------------*/

#define BOOT_RECOVERY_SETTING            "boot_recovery"
#define RECOVERY_STATUS_SETTING          "recovery_status"
#define RECOVERY_COMMAND_SETTING         "recovery_command"
#define RECOVERY_STAGE_SETTING           "recovery_stage"

/*------------------------------------------------------------------------------
                    F U N C T I O N  D E C L A R A T I O N S
------------------------------------------------------------------------------*/

int get_setting_value(const char *setting, char * buff, size_t buff_size);
int set_setting_value(const char *setting, const char *value);

#ifdef __cplusplus
}
#endif

#endif
