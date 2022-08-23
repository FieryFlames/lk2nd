// SPDX-License-Identifier: BSD-3-Clause
/* Copyright (c) 2022, Jack Matthews <jm5112356@gmail.com> */

#include <boot.h>
#include <libfdt.h>

static int lk2nd_serialno_dt_update(void *dtb, const char *cmdline,
				 enum boot_type boot_type)
{
	extern char sn_buf[];
	int dtb_root;

	if (sn_buf[0] == '\0' || boot_type & (BOOT_DOWNSTREAM | BOOT_LK2ND))
		return 0;

	dtb_root = fdt_path_offset(dtb, "/");

	dprintf(INFO, "DTB root offset is %d\n", dtb_root);

	return fdt_setprop_string(dtb, dtb_root, "serial-number", sn_buf);
}
DEV_TREE_UPDATE(lk2nd_serialno_dt_update);
