/*
 * Copyright 2018, Digi International Inc.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
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
