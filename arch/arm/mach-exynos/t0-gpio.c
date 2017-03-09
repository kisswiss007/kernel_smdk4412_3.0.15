/*
 *  linux/arch/arm/mach-exynos/midas-gpio.c
 *
 * Copyright (c) 2010 Samsung Electronics Co., Ltd.
 *		http://www.samsung.com/
 *
 * EXYNOS - GPIO setting in set board
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/gpio.h>
#include <linux/serial_core.h>
#include <plat/devs.h>
#include <plat/gpio-cfg.h>
#include <plat/regs-serial.h>
#include <mach/gpio-midas.h>
#include <plat/cpu.h>
#include <mach/pmu.h>

struct gpio_init_data {
	uint num;
	uint cfg;
	uint val;
	uint pud;
	uint drv;
};

/*
 * M0 GPIO Init Table
 */
static struct gpio_init_data m0_init_gpios[] = {
	{EXYNOS4_GPA1(4), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1}, /* NC */
	{EXYNOS4_GPA1(5), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1}, /* NC */
	{EXYNOS4_GPD0(2), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1},
	{EXYNOS4_GPD0(3), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1},
	{EXYNOS4_GPD1(2), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1}, /* GSENSE_SDA_1.8V */
	{EXYNOS4_GPD1(3), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1}, /* GSENSE_SCL_1.8V */

	{EXYNOS4_GPX0(2), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1}, /* PS_ALS_INT */
	{EXYNOS4_GPX0(3), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1},
	{EXYNOS4_GPX0(4), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1},
	{EXYNOS4_GPX0(5), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1},
	{EXYNOS4_GPX0(6), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1},

	{EXYNOS4_GPX0(7), S3C_GPIO_SFN(0xF), S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1}, /* AP_PMIC_IRQ */

	{EXYNOS4_GPX1(4), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1},
	{EXYNOS4_GPX1(7), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1}, /* NFC_IRQ */

	{EXYNOS4_GPX2(0), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1},
	{EXYNOS4_GPX2(1), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1},
	{EXYNOS4_GPX2(4), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1}, /* V_BUS_INT */
	{EXYNOS4_GPX2(3), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1}, /* FUEL_ALERT */
	{EXYNOS4_GPX2(5), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1}, /* WLAN_HOST_WAKEUP */
	{EXYNOS4_GPX2(6), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1}, /* BT_HOST_WAKEUP */
	{EXYNOS4_GPX2(7), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1}, /* nPower */

	{EXYNOS4_GPX3(0), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1}, /* WPC_INT */
	{EXYNOS4_GPX3(1), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1}, /* BT_WAKE */
	{EXYNOS4_GPX3(2), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1}, /* CP_PMU_RST */
#if defined(CONFIG_SEC_MODEM_M0_TD)
	{EXYNOS4_GPX3(5), S3C_GPIO_OUTPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1},
#else
	{EXYNOS4_GPX3(5), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1},
#endif
	{EXYNOS4212_GPJ0(0), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_ZERO,
		S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV4}, /* WLAN_EN */
	{EXYNOS4_GPK3(1), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1}, /* WLAN_SDIO_CMD */
	{EXYNOS4_GPK3(3), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1}, /* WLAN_SDIO_D(0) */
	{EXYNOS4_GPK3(4), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1}, /* WLAN_SDIO_D(1) */
	{EXYNOS4_GPK3(5), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1}, /* WLAN_SDIO_D(2) */
	{EXYNOS4_GPK3(6), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1}, /* WLAN_SDIO_D(3) */

	{EXYNOS4212_GPM3(0), S3C_GPIO_OUTPUT, S3C_GPIO_SETPIN_ZERO,
		S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1}, /* PMIC_DVS1 */

	{EXYNOS4_GPY0(0), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1}, /* NC */
	{EXYNOS4_GPY0(1), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1}, /* NC */
	{EXYNOS4_GPY0(4), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1}, /* NC */
	{EXYNOS4_GPY0(5), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1}, /* NC */
	{EXYNOS4_GPY1(0), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1}, /* NC */
	{EXYNOS4_GPY1(1), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1}, /* NC */
	{EXYNOS4_GPY1(2), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1}, /* NC */
	{EXYNOS4_GPY1(3), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1}, /* NC */
	{EXYNOS4_GPY2(1), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1}, /* NC */

	{EXYNOS4212_GPJ1(3), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV3}, /* CAM_MCLK */
	{EXYNOS4212_GPM2(2), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV3}, /* VTCAM_MCLK */
};

/*
 * M0 GPIO Sleep Table
 */
static unsigned int m0_sleep_gpio_table[][3] = {
	{EXYNOS4_GPA0(0),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE},
	{EXYNOS4_GPA0(1),  S3C_GPIO_SLP_OUT0, S3C_GPIO_PULL_NONE},
	{EXYNOS4_GPA0(2),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE},
	{EXYNOS4_GPA0(3),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_UP},
	{EXYNOS4_GPA0(4),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE},
	{EXYNOS4_GPA0(5),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPA0(6),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPA0(7),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_UP},

	{EXYNOS4_GPA1(0),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPA1(1),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPA1(2),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPA1(3),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPA1(4),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPA1(5),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},

	{EXYNOS4_GPB(0),   S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE},
	{EXYNOS4_GPB(1),   S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE},
	{EXYNOS4_GPB(2),   S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE},
	{EXYNOS4_GPB(3),   S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE},
	{EXYNOS4_GPB(4),   S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPB(5),   S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_UP},
	{EXYNOS4_GPB(6),   S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPB(7),   S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},

	{EXYNOS4_GPC0(0),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* REC_PCM_CLK(NC) */
	{EXYNOS4_GPC0(1),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPC0(2),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* REC_PCM_SYNC(NC) */
	{EXYNOS4_GPC0(3),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* REC_PCM_IN(NC) */
	{EXYNOS4_GPC0(4),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* REC_PCM_OUT(NC) */
#if defined(CONFIG_SEC_MODEM_M0_TD)
	{EXYNOS4_GPC1(0), S3C_GPIO_SLP_PREV, S3C_GPIO_PULL_NONE},
#else
	{EXYNOS4_GPC1(0), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE},
#endif
	{EXYNOS4_GPC1(1),  S3C_GPIO_SLP_PREV, S3C_GPIO_PULL_NONE}, /* FM_RST */

	{EXYNOS4_GPC1(2),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE},
	{EXYNOS4_GPC1(3),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE},
	{EXYNOS4_GPC1(4),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE},

	{EXYNOS4_GPD0(0),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPD0(1),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPD0(2),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE},
	{EXYNOS4_GPD0(3),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE},

	{EXYNOS4_GPD1(0),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPD1(1),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPD1(2),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE},
	{EXYNOS4_GPD1(3),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE},

	{EXYNOS4_GPF0(0),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE}, /* NC */
	{EXYNOS4_GPF0(1),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE}, /* NC */
	{EXYNOS4_GPF0(2),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},

	{EXYNOS4_GPF0(3),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPF0(4),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE}, /* NC */
	{EXYNOS4_GPF0(5),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPF0(6),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE}, /* NC */
	{EXYNOS4_GPF0(7),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */

	{EXYNOS4_GPF1(0),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPF1(1),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPF1(2),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPF1(3),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPF1(4),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE}, /* NC */
	{EXYNOS4_GPF1(5),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE}, /* NC */
	/* GPF1(6) M0, C1 PDA_ACTIVE, let cp know AP sleep status*/
	{EXYNOS4_GPF1(6),  S3C_GPIO_SLP_OUT0, S3C_GPIO_PULL_NONE},
	{EXYNOS4_GPF1(7),  S3C_GPIO_SLP_PREV, S3C_GPIO_PULL_NONE}, /* NC */

	{EXYNOS4_GPF2(0),  S3C_GPIO_SLP_PREV, S3C_GPIO_PULL_NONE}, /* NC */
	{EXYNOS4_GPF2(1),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPF2(2),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPF2(3),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPF2(4),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPF2(5),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPF2(6),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE}, /* S_LED_I2C_SCL */
	{EXYNOS4_GPF2(7),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE}, /* S_LED_I2C_SDA */

	{EXYNOS4_GPF3(0),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE}, /* NC */
	{EXYNOS4_GPF3(1),  S3C_GPIO_SLP_PREV, S3C_GPIO_PULL_NONE}, /* BUCK2_SEL */
	{EXYNOS4_GPF3(2),  S3C_GPIO_SLP_PREV, S3C_GPIO_PULL_NONE}, /* BUCK3_SEL */
	{EXYNOS4_GPF3(3),  S3C_GPIO_SLP_PREV, S3C_GPIO_PULL_NONE}, /* BUCK4_SEL */
	{EXYNOS4_GPF3(4),  S3C_GPIO_SLP_OUT1,  S3C_GPIO_PULL_NONE},
	{EXYNOS4_GPF3(5),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},

	{EXYNOS4_GPK0(0),  S3C_GPIO_SLP_PREV, S3C_GPIO_PULL_NONE}, /* NAND_CLK */
	{EXYNOS4_GPK0(1),  S3C_GPIO_SLP_PREV, S3C_GPIO_PULL_NONE}, /* NAND_CMD */
	{EXYNOS4_GPK0(2),  S3C_GPIO_SLP_PREV, S3C_GPIO_PULL_NONE}, /* eMMC_EN */
	{EXYNOS4_GPK0(3),  S3C_GPIO_SLP_PREV, S3C_GPIO_PULL_NONE}, /* NAND_D(0) */
	{EXYNOS4_GPK0(4),  S3C_GPIO_SLP_PREV, S3C_GPIO_PULL_NONE}, /* NAND_D(1) */
	{EXYNOS4_GPK0(5),  S3C_GPIO_SLP_PREV, S3C_GPIO_PULL_NONE}, /* NAND_D(2) */
	{EXYNOS4_GPK0(6),  S3C_GPIO_SLP_PREV, S3C_GPIO_PULL_NONE}, /* NAND_D(3) */

	{EXYNOS4_GPK1(0),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPK1(1),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPK1(2),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPK1(3),  S3C_GPIO_SLP_PREV, S3C_GPIO_PULL_NONE}, /* NAND_D(4) */
	{EXYNOS4_GPK1(4),  S3C_GPIO_SLP_PREV, S3C_GPIO_PULL_NONE}, /* NAND_D(5) */
	{EXYNOS4_GPK1(5),  S3C_GPIO_SLP_PREV, S3C_GPIO_PULL_NONE}, /* NAND_D(6) */
	{EXYNOS4_GPK1(6),  S3C_GPIO_SLP_PREV, S3C_GPIO_PULL_NONE}, /* NAND_D(7) */

	{EXYNOS4_GPK2(0),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPK2(1),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPK2(2),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPK2(3),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPK2(4),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPK2(5),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPK2(6),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},

	{EXYNOS4_GPK3(0),  S3C_GPIO_SLP_OUT0,  S3C_GPIO_PULL_NONE},
	{EXYNOS4_GPK3(1),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE},
	{EXYNOS4_GPK3(2),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPK3(3),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE},
	{EXYNOS4_GPK3(4),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE},
	{EXYNOS4_GPK3(5),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE},
	{EXYNOS4_GPK3(6),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE},

	{EXYNOS4_GPL0(0),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPL0(1),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPL0(2),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPL0(3),  S3C_GPIO_SLP_INPUT,  S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPL0(4),  S3C_GPIO_SLP_PREV, S3C_GPIO_PULL_NONE}, /* HDMI_EN */
/*	{EXYNOS4_GPL0(5),  S3C_GPIO_SLP_OUT0,  S3C_GPIO_PULL_NONE}, */
	{EXYNOS4_GPL0(6),  S3C_GPIO_SLP_PREV,  S3C_GPIO_PULL_NONE},
/*	{EXYNOS4_GPL0(7),  S3C_GPIO_SLP_OUT0,  S3C_GPIO_PULL_NONE}, */

	{EXYNOS4_GPL1(0),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPL1(1),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
/*	{EXYNOS4_GPL1(2),  S3C_GPIO_SLP_PREV,  S3C_GPIO_PULL_NONE}, */

	{EXYNOS4_GPL2(0),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPL2(1),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPL2(2),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPL2(3),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPL2(4),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPL2(5),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPL2(6),  S3C_GPIO_SLP_PREV,  S3C_GPIO_PULL_NONE},
	{EXYNOS4_GPL2(7),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},

	{EXYNOS4_GPY0(0),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */

	{EXYNOS4_GPY0(1),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */

	{EXYNOS4_GPY0(2),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPY0(3),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */

	{EXYNOS4_GPY0(4),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPY0(5),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */

	{EXYNOS4_GPY1(0),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPY1(1),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */

	{EXYNOS4_GPY1(2),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPY1(3),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPY2(0),  S3C_GPIO_SLP_PREV, S3C_GPIO_PULL_NONE},  /* TF_EN */
	{EXYNOS4_GPY2(1),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPY2(2),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE},
	{EXYNOS4_GPY2(3),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE},
	{EXYNOS4_GPY2(4),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE},
	{EXYNOS4_GPY2(5),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE},

	{EXYNOS4_GPY3(0),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPY3(1),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPY3(2),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPY3(3),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPY3(4),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPY3(5),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPY3(6),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPY3(7),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},

	{EXYNOS4_GPY4(0),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPY4(1),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPY4(2),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPY4(3),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPY4(4),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPY4(5),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPY4(6),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPY4(7),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},

	{EXYNOS4_GPY5(0),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPY5(1),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPY5(2),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPY5(3),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPY5(4),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPY5(5),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPY5(6),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPY5(7),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},

	{EXYNOS4_GPY6(0),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPY6(1),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPY6(2),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPY6(3),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPY6(4),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPY6(5),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPY6(6),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPY6(7),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},

	{EXYNOS4_GPZ(0),   S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPZ(1),   S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPZ(2),   S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPZ(3),   S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPZ(4),   S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4_GPZ(5),   S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPZ(6),   S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */

	/* Exynos4212 specific gpio */
	{EXYNOS4212_GPJ0(0),  S3C_GPIO_SLP_PREV,  S3C_GPIO_PULL_NONE},
	{EXYNOS4212_GPJ0(1),  S3C_GPIO_SLP_PREV, S3C_GPIO_PULL_DOWN},/*USB_SEL*/
	{EXYNOS4212_GPJ0(2),  S3C_GPIO_SLP_PREV, S3C_GPIO_PULL_NONE},
	{EXYNOS4212_GPJ0(3),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4212_GPJ0(4),  S3C_GPIO_SLP_PREV,  S3C_GPIO_PULL_NONE},
	/* GPIO_PS_ALS_EN */
	{EXYNOS4212_GPJ0(5),  S3C_GPIO_SLP_PREV, S3C_GPIO_PULL_NONE},
	{EXYNOS4212_GPJ0(6),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4212_GPJ0(7),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */

	{EXYNOS4212_GPJ1(0),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4212_GPJ1(1),  S3C_GPIO_SLP_PREV, S3C_GPIO_PULL_NONE}, /* TORCH_EN */
	{EXYNOS4212_GPJ1(2),  S3C_GPIO_SLP_PREV, S3C_GPIO_PULL_NONE}, /* TORCH_SET */
	{EXYNOS4212_GPJ1(3),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4212_GPJ1(4),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},

	{EXYNOS4212_GPM0(0),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4212_GPM0(1),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4212_GPM0(2),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4212_GPM0(3),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4212_GPM0(4),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4212_GPM0(5),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4212_GPM0(6),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4212_GPM0(7),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */

	{EXYNOS4212_GPM1(0),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4212_GPM1(1),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4212_GPM1(2),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE}, /* NC */
	{EXYNOS4212_GPM1(3),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE}, /* NC */
	{EXYNOS4212_GPM1(4),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE}, /* NC */
	{EXYNOS4212_GPM1(5),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE}, /* NC */
	{EXYNOS4212_GPM1(6),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},

	{EXYNOS4212_GPM2(0),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE}, /* NC */
	{EXYNOS4212_GPM2(1),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_NONE}, /* NC */
	{EXYNOS4212_GPM2(2),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4212_GPM2(3),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4212_GPM2(4),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */

	{EXYNOS4212_GPM3(0),  S3C_GPIO_SLP_PREV, S3C_GPIO_PULL_NONE}, /* PMIC_DVS1 */
	{EXYNOS4212_GPM3(1),  S3C_GPIO_SLP_PREV, S3C_GPIO_PULL_NONE}, /* PMIC_DVS2 */
	{EXYNOS4212_GPM3(2),  S3C_GPIO_SLP_PREV, S3C_GPIO_PULL_NONE}, /* PMIC_DVS3 */
	/* GPM3(3) M0, CP_RESET_REQ hold high */
	{EXYNOS4212_GPM3(3),  S3C_GPIO_SLP_OUT1, S3C_GPIO_PULL_NONE},
	{EXYNOS4212_GPM3(4),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4212_GPM3(5),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4212_GPM3(6),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4212_GPM3(7),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* ISP_RXD */

	{EXYNOS4212_GPM4(0),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4212_GPM4(1),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4212_GPM4(2),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* VT_CAM_SCL */
	{EXYNOS4212_GPM4(3),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* VT_CAM_SDA */
	{EXYNOS4212_GPM4(4),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4212_GPM4(5),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4212_GPM4(6),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4212_GPM4(7),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */

	{EXYNOS4212_GPV0(0),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4212_GPV0(1),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4212_GPV0(2),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4212_GPV0(3),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4212_GPV0(4),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4212_GPV0(5),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4212_GPV0(6),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4212_GPV0(7),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},

	{EXYNOS4212_GPV1(0),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4212_GPV1(1),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4212_GPV1(2),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4212_GPV1(3),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4212_GPV1(4),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4212_GPV1(5),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4212_GPV1(6),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4212_GPV1(7),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},

	{EXYNOS4212_GPV2(0),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4212_GPV2(1),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4212_GPV2(2),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4212_GPV2(3),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4212_GPV2(4),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4212_GPV2(5),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4212_GPV2(6),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4212_GPV2(7),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},

	{EXYNOS4212_GPV3(0),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4212_GPV3(1),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4212_GPV3(2),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4212_GPV3(3),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4212_GPV3(4),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4212_GPV3(5),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4212_GPV3(6),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4212_GPV3(7),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},

	{EXYNOS4212_GPV4(0),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
	{EXYNOS4212_GPV4(1),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
}; /* m0_sleep_gpio_table */

/*
 * M0 Rev0.4 GPIO Sleep Table
 */
static unsigned int m0_sleep_gpio_table_rev04[][3] = {
	{EXYNOS4_GPF2(3),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4212_GPJ0(1),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4212_GPJ0(2),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
};

/*
 * M0 Rev0.5 GPIO Sleep Table
 */
static unsigned int m0_sleep_gpio_table_rev05[][3] = {
	{EXYNOS4212_GPJ1(0),  S3C_GPIO_SLP_PREV, S3C_GPIO_PULL_NONE}, /* CAM_SW_EN */
};

/*
 * M0 Rev0.6 GPIO Sleep Table
 */
static unsigned int m0_sleep_gpio_table_rev06[][3] = {
	{EXYNOS4_GPC1(4),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPL0(3),  S3C_GPIO_SLP_PREV,  S3C_GPIO_PULL_NONE}, /* RADIO_EN */
	{EXYNOS4_GPL2(3),  S3C_GPIO_SLP_INPUT,  S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4212_GPJ0(2),  S3C_GPIO_SLP_PREV, S3C_GPIO_PULL_NONE}, /* 3_MICBIAS_EN */
};

/*
 * M0 Rev0.8 GPIO Sleep Table
 */
static unsigned int m0_sleep_gpio_table_rev08[][3] = {
	{EXYNOS4212_GPJ0(1),  S3C_GPIO_SLP_PREV, S3C_GPIO_PULL_NONE}, /* AP_DUMP_INT */
	{EXYNOS4212_GPM2(2),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* VTCAM_MCLK */
};

/*
 * M0 Rev0.9 GPIO Sleep Table
 */
static unsigned int m0_sleep_gpio_table_rev09[][3] = {
#if defined(CONFIG_SEC_MODEM_M0_TD)
	{EXYNOS4_GPC1(0), S3C_GPIO_SLP_PREV, S3C_GPIO_PULL_NONE},
	{EXYNOS4212_GPJ0(1),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN},
#else
	{EXYNOS4_GPC1(0), S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
#endif
	{EXYNOS4_GPC1(2),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4_GPC1(3),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
	{EXYNOS4212_GPM0(0),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
};

/*
 * M0 Rev1.0 GPIO Sleep Table
 */
static unsigned int m0_sleep_gpio_table_rev10[][3] = {
	{EXYNOS4_GPL0(1),  S3C_GPIO_SLP_INPUT,  S3C_GPIO_PULL_DOWN}, /* 2_TOUCH_SCL_1.8V */
	{EXYNOS4_GPL0(2),  S3C_GPIO_SLP_INPUT,  S3C_GPIO_PULL_DOWN}, /* 2_TOUCH_SDA_1.8V */
	{EXYNOS4212_GPJ0(3),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* 2_TOUCH_INT */
};

/*
 * M0 Rev1.1 GPIO Sleep Table
 */
static unsigned int m0_sleep_gpio_table_rev11[][3] = {
	{EXYNOS4212_GPJ0(2),  S3C_GPIO_SLP_INPUT, S3C_GPIO_PULL_DOWN}, /* NC */
};

struct m0_sleep_table {
	unsigned int (*ptr)[3];
	int size;
};

#define GPIO_TABLE(_ptr) \
	{.ptr = _ptr, \
	.size = ARRAY_SIZE(_ptr)} \

 #define GPIO_TABLE_NULL \
	{.ptr = NULL, \
	.size = 0} \

static struct m0_sleep_table m0_sleep_table[] = {
	GPIO_TABLE(m0_sleep_gpio_table),	/* Rev0.0(0x0) - SLP */
	GPIO_TABLE_NULL,			/* Rev0.0(0x1) - Android */
	GPIO_TABLE_NULL,
	GPIO_TABLE_NULL,
	GPIO_TABLE_NULL,
	GPIO_TABLE(m0_sleep_gpio_table_rev04),	/* Rev0.4(0x5) */
	GPIO_TABLE(m0_sleep_gpio_table_rev05),	/* Rev0.5(0x6) */
	GPIO_TABLE(m0_sleep_gpio_table_rev06),	/* Rev0.6(0x7) */
	GPIO_TABLE_NULL,
	GPIO_TABLE(m0_sleep_gpio_table_rev08),	/* Rev0.8(0x9) */
	GPIO_TABLE(m0_sleep_gpio_table_rev09),	/* Rev0.9(0xA) */
	GPIO_TABLE(m0_sleep_gpio_table_rev10),	/* Rev1.0(0xB) */
	GPIO_TABLE(m0_sleep_gpio_table_rev11),	/* Rev1.1(0xC) */
};

static void config_sleep_gpio_table(int array_size,
				    unsigned int (*gpio_table)[3])
{
	u32 i, gpio;

	for (i = 0; i < array_size; i++) {
		gpio = gpio_table[i][0];
		s3c_gpio_slp_cfgpin(gpio, gpio_table[i][1]);
		s3c_gpio_slp_setpull_updown(gpio, gpio_table[i][2]);
	}
}

static void m0_config_sleep_gpio_table(void)
{
	int i;
	int index = min(ARRAY_SIZE(m0_sleep_table), system_rev + 1);

	for (i = 0; i < index; i++) {
		if (m0_sleep_table[i].ptr == NULL)
			continue;

		config_sleep_gpio_table(m0_sleep_table[i].size,
				m0_sleep_table[i].ptr);
	}
}

/* To save power consumption, gpio pin set before enterling sleep */
void midas_config_sleep_gpio_table(void)
{
	m0_config_sleep_gpio_table();
}

/* Intialize gpio set in midas board */
void midas_config_gpio_table(void)
{
	u32 i, gpio;

	printk(KERN_DEBUG "%s\n", __func__);

	for (i = 0; i < ARRAY_SIZE(m0_init_gpios); i++) {
		gpio = m0_init_gpios[i].num;
		if (gpio <= EXYNOS4212_GPV4(1)) {
			s3c_gpio_cfgpin(gpio, m0_init_gpios[i].cfg);
			s3c_gpio_setpull(gpio, m0_init_gpios[i].pud);

			if (m0_init_gpios[i].val != S3C_GPIO_SETPIN_NONE)
				gpio_set_value(gpio, m0_init_gpios[i].val);

			s5p_gpio_set_drvstr(gpio, m0_init_gpios[i].drv);
		}
	}
}
