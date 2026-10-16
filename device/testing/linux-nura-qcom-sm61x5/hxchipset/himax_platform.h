/* SPDX-License-Identifier: GPL-2.0 */
/*  Himax Android Driver Sample Code for QCT platform
 *
 *  Copyright (C) 2019 Himax Corporation.
 *
 *  This software is licensed under the terms of the GNU General Public
 *  License version 2,  as published by the Free Software Foundation,  and
 *  may be copied,  distributed,  and modified under those terms.
 *
 *  This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 */

#ifndef HIMAX_PLATFORM_H
#define HIMAX_PLATFORM_H

#include <linux/delay.h>
#include <linux/fs.h>
#include <linux/gpio/consumer.h>
#include <linux/types.h>
#include <linux/spi/spi.h>
#include <linux/interrupt.h>

#define HIMAX_SPI_FIFO_POLLING
#define HIMAX_I2C_RETRY_TIMES 3

#if defined(CONFIG_TOUCHSCREEN_HIMAX_DEBUG)
#define D(x...) pr_debug("[HXTP] " x)
#define I(x...) pr_info("[HXTP] " x)
#define W(x...) pr_warn("[HXTP][WARNING] " x)
#define E(x...) pr_err("[HXTP][ERROR] " x)
#define DIF(x...) \
do { \
	if (debug_flag) \
		pr_debug("[HXTP][DEBUG] " x) \
	} while (0)
#else
#define D(x...)
#define I(x...)
#define W(x...)
#define E(x...)
#define DIF(x...)
#endif

#define HIMAX_common_NAME	"himax_tp"
#define HIMAX_I2C_ADDR		0x48
#define INPUT_DEV_NAME		"himax-touchscreen"

struct himax_i2c_platform_data {
	int abs_x_min;
	int abs_x_max;
	int abs_x_fuzz;
	int abs_y_min;
	int abs_y_max;
	int abs_y_fuzz;
	int abs_pressure_min;
	int abs_pressure_max;
	int abs_pressure_fuzz;
	int abs_width_min;
	int abs_width_max;
	int screenWidth;
	int screenHeight;
	uint8_t fw_version;
	uint8_t tw_id;
	uint8_t powerOff3V3;
	uint8_t cable_config[2];
	uint8_t protocol_type;
	struct gpio_desc *gpio_irq;
	struct gpio_desc *gpio_reset;
	struct gpio_desc *gpio_3v3_en;
	int (*power)(int on);
	void (*reset)(void);
	struct himax_virtual_key *virtual_key;
	struct kobject *vk_obj;
	struct kobj_attribute *vk2Use;

	int hx_config_size;
};

extern struct himax_ic_data *ic_data;
extern struct himax_ts_data *private_ts;
int himax_chip_common_init(void);
void himax_chip_common_deinit(void);

void himax_ts_work(struct himax_ts_data *ts);
enum hrtimer_restart himax_ts_timer_func(struct hrtimer *timer);
extern int himax_bus_read(uint8_t command, uint8_t *data,
		uint32_t length, uint8_t toRetry);
extern int himax_bus_write(uint8_t command, uint8_t *data,
		uint32_t length, uint8_t toRetry);
extern int himax_bus_write_command(uint8_t command, uint8_t toRetry);
extern void himax_int_enable(int enable);
extern void himax_rst_gpio_set(struct gpio_desc *gpio, uint8_t value);
extern int himax_ts_register_interrupt(void);
int himax_ts_unregister_interrupt(void);
extern uint8_t himax_int_gpio_read(struct gpio_desc *gpio);

extern int himax_gpio_power_config(struct himax_i2c_platform_data *pdata);
void himax_gpio_power_deconfig(struct himax_i2c_platform_data *pdata);

#if defined(HX_CONFIG_FB)
extern int fb_notifier_callback(struct notifier_block *self,
		unsigned long event, void *data);
#elif defined(HX_CONFIG_DRM)
extern int drm_notifier_callback(struct notifier_block *self,
		unsigned long event, void *data);
#endif

#endif
