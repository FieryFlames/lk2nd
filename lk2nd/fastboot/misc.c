// SPDX-License-Identifier: BSD-3-Clause
/* Copyright (c) 2019-2022 Stephan Gerhold <stephan@gerhold.net> */

#include <debug.h>
#include <fastboot.h>
#include <platform.h>
#include <platform/timer.h>

#if WITH_DEBUG_LOG_BUF
static void cmd_oem_log(const char *arg, void *data, unsigned sz)
{
	fastboot_stage(data, log_copy(data));
}
FASTBOOT_REGISTER("oem log", cmd_oem_log);
#endif

static void cmd_oem_reboot_edl(const char *arg, void *data, unsigned sz)
{
	fastboot_okay("");
	reboot_device(EMERGENCY_DLOAD);
}
FASTBOOT_REGISTER("oem reboot-edl", cmd_oem_reboot_edl);


#include <dev/gpio.h>
#include <lk2nd/gpio.h>

static void cmd_oem_test(const char *arg, void *data, unsigned sz)
{
	fastboot_info("this is the test code");

	unsigned flash_pin = LK2ND_GPIO_PIN(32, LK2ND_GPIO_DEV_TLMM, 0);

	gpio_config(flash_pin, GPIO_OUTPUT);
	gpio_set(flash_pin, 1);
	mdelay(250);
	gpio_set(flash_pin, 0);
	mdelay(250);
	gpio_set(32, 1);
	mdelay(250);
	gpio_set(32, 0);

	fastboot_okay("");
}
FASTBOOT_REGISTER("oem test", cmd_oem_test);
