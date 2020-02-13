// SPDX-License-Identifier: GPL-2.0-only

#include <debug.h>
#include <libfdt.h>
#include <smem.h>
#include <stdlib.h>
#include <string.h>

#include "../../device.h"
#include "motorola-unit-info.h"

#define SMEM_ID_VENDOR0			0x86
#define SMEM_KERNEL_RESERVE		SMEM_ID_VENDOR0
#define SMEM_KERNEL_RESERVE_SIZE	1024

static void readprop_u32(const void *fdt, int node, const char *name, uint32_t *val) {
	int len;
	const uint32_t *prop = fdt_getprop(fdt, node, name, &len);
	if (len == sizeof(*val))
		*val = fdt32_to_cpu(*prop);
	else
		dprintf(CRITICAL, "Failed to get prop '%s' in device tree\n", name);
}

static int motorola_unit_info(const void *fdt, int node)
{
	struct mmi_unit_info *info;
	const char *baseband;
	int chosen, len, ret;

	chosen = fdt_path_offset(fdt, "/chosen");
	if (chosen < 0) {
		dprintf(CRITICAL, "Failed to get /chosen node in device tree\n");
		return chosen;
	}

	info = malloc(SMEM_KERNEL_RESERVE_SIZE);
	memset(info, 0, SMEM_KERNEL_RESERVE_SIZE);

	info->version = MMI_UNIT_INFO_VER;

	readprop_u32(fdt, chosen, "linux,hwrev", &info->system_rev);
	readprop_u32(fdt, chosen, "linux,seriallow", &info->system_serial_low);
	readprop_u32(fdt, chosen, "linux,serialhigh", &info->system_serial_high);

	if (lk2nd_dev.serialno)
		strlcpy(info->barcode, lk2nd_dev.serialno, BARCODE_MAX_LEN);
	if (lk2nd_dev.carrier)
		strlcpy(info->carrier, lk2nd_dev.carrier, CARRIER_MAX_LEN);

	baseband = fdt_getprop(fdt, chosen, "mmi,baseband", &len);
	if (len > 0)
		strlcpy(info->baseband, baseband, MIN(len, BASEBAND_MAX_LEN));

	if (lk2nd_dev.device)
		strlcpy(info->device, lk2nd_dev.device, DEVICE_MAX_LEN);
	if (lk2nd_dev.radio) {
		info->radio = atoui(lk2nd_dev.radio);
		strlcpy(info->radio_str, lk2nd_dev.radio, RADIO_MAX_LEN);
	}

	readprop_u32(fdt, chosen, "mmi,powerup_reason", &info->powerup_reason);

	dprintf(INFO, "Setting Motorola unit info v%d: rev=%#04x, serial=%#08x%08x, "
		"machine='%s', barcode='%s', carrier='%s', baseband='%s', device='%s', "
		"radio='%s' (%#x), powerup_reason=%#08x\n",
		info->version, info->system_rev, info->system_serial_high, info->system_serial_low,
		info->machine, info->barcode, info->carrier, info->baseband, info->device,
		info->radio_str, info->radio, info->powerup_reason);

	ret = smem_alloc_write_entry(SMEM_KERNEL_RESERVE, info, SMEM_KERNEL_RESERVE_SIZE);
	free(info);
	return ret;
}
LK2ND_DEVICE_INIT("motorola,unit-info", motorola_unit_info);
