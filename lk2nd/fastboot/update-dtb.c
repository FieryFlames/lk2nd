// SPDX-License-Identifier: BSD-3-Clause
/* Copyright (c) 2022, Stephan Gerhold <stephan@gerhold.net> */

#include <debug.h>
#include <fastboot.h>
#include <string.h>
#include <target.h>
#include <dev_tree.h>
#include <boot.h>
#include <libfdt.h>

static void cmd_oem_update_dtb(const char *arg, void *data, unsigned sz)
{
    if (!sz)
    {
        fastboot_fail("no data staged to update");
        return;
    }

    update_device_tree(data, "lk2nd.pass-ramoops", 0, NULL, 0);

    fastboot_stage(data, fdt_totalsize(data));
}

FASTBOOT_REGISTER("oem update-dtb", cmd_oem_update_dtb);
