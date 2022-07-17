
#include <debug.h>
#include <libfdt.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <lk2nd/gpio.h>
#include <dev/gpio_i2c.h>

#include "device.h"

int lk2nd_dev_gpio_i2c_add_bus(const void *dtb, int node)
{
	static int id = 0; /* Why shall I handle this >:( */
	gpio_i2c_info_t *info = malloc(sizeof(gpio_i2c_info_t));

	info->sda = lk2nd_dev_gpio_get(dtb, node, "i2c-sda");
	info->scl = lk2nd_dev_gpio_get(dtb, node, "i2c-scl");
	info->hcd = 10;
	info->qcd = 5;

	dprintf(CRITICAL, "Adding i2c bus %d with 0x%x 0x%x\n",
		id, info->sda, info->scl);

	gpio_i2c_add_bus(id, info);
	id++;

	return id - 1;
}

static int aw2013_init(const void *dtb, int node)
{
	int i2cbus;
	const uint8_t buf[] = { 0x01, 0x07, 0xff, 0x55, };

	i2cbus = lk2nd_dev_gpio_i2c_add_bus(dtb, node);

	gpio_i2c_write_reg_bytes(i2cbus, 0x45, 0x00, &buf[3], 1);
	gpio_i2c_write_reg_bytes(i2cbus, 0x45, 0x01, &buf[0], 1);
	gpio_i2c_write_reg_bytes(i2cbus, 0x45, 0x30, &buf[1], 1);
	gpio_i2c_write_reg_bytes(i2cbus, 0x45, 0x31, &buf[0], 1);
	gpio_i2c_write_reg_bytes(i2cbus, 0x45, 0x32, &buf[0], 1);
	gpio_i2c_write_reg_bytes(i2cbus, 0x45, 0x34, &buf[2], 1);
	gpio_i2c_write_reg_bytes(i2cbus, 0x45, 0x35, &buf[2], 1);

	return 0;
}
LK2ND_DEVICE_INIT("awinic,aw2013", aw2013_init);
