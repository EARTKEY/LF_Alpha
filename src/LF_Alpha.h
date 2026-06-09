/*
 * LF_Alpha Embedded Firmware Library
 * Copyright (c) 2025 Linked_Frequency
 * Authors: E-ARTKEY Inovations Pvt. Ltd.
 * 
 * Licensed under the MIT License (see LICENSE.md)
 * 
 * This library integrates code from multiple open-source projects:
 * - ElectronicCats MPU6050: Copyright (c) 2020 ElectronicCats, MIT License
 * - Adafruit SSD1306: Copyright (c) Adafruit Industries, BSD License  
 * - Adafruit NeoPixel: Copyright (c) Adafruit Industries, LGPL-3.0 License
 * - Adafruit APDS9960: Copyright (c) Adafruit Industries, MIT License
 * - dvarrel AHT20: Copyright (c) 2021 dvarrel, CC0-1.0 License
 * - hasenradball DS3231-RTC: Copyright (c) 2023 Frank Häfele, MIT License
 * - DFRobot MAX30102: Copyright (c) 2020 DFRobot, MIT License
 * - ESP32Servo: Copyright (c) 2017 Kevin Harrington, LGPL-2.1 License
 * 
 * ESP32-S3 Mini 1 N8 Platform
 * Version: 1.0.0
 * Date: October 2025
 */

#ifndef LF_ALPHA_H
#define LF_ALPHA_H

// Heder files
#include <Arduino.h>
#include <stdint.h>
#include <time.h>
#include <Wire.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "BluetoothSerial.h"
#include "lib/MAX30102/algorithm.h"
#include "lib/MAX30102/heartRate.h"
#include "lib/ESP32PWM/ESP32PWM.h"
#include "lib/Adafruit_SSD1306/Adafruit_SSD1306.h"
#include "lib/Adafruit_SSD1306/Adafruit_I2CDevice.h"
#include "lib/MPU6050/I2Cdev.h"
#include "lib/MPU6050/helper_3dmath.h"
#include "driver/gpio.h"
#if !defined(ESP_IDF_VERSION) || ESP_IDF_VERSION < ESP_IDF_VERSION_VAL(5, 0, 0)
#include "driver/rmt.h"
#endif

/*-------------GPIO configurations---------*/
// Buzzer GPIO
#define buzzerPin                   18

// userLEDs GPIO
#define userLED1                    21     // active LOW
#define userLED2                    26     // active HIGH

// ultrasonic sensor GPIOs
#define echo_HCSR04                 34
#define trig_HCSR04                 33

// tactile switch GPIOs
#define upButton                    13
#define downButton                  14
#define backButton                  11
#define okayButton                  12

// relay GPIOs
#define RLY1                        36
#define RLY2                        35

// servo motor GPIO (mg90)
#define servoPin                    47

// Potentiometer GPIO
#define potPin                      1

// neopixel rgbLEDs GPIO
#define rgbLEDsPin                  48

// APDS9960 Interrupt GPIO
#define APDSINT                     15

// MAX30102 Interrupt GPIO      
#define MAXINT                      17

// MPU6050 Interrupt GPIO
#define MPUINT                      16

// I2C GPIOs
#define LF_Alpha_SDA                38
#define LF_Alpha_SCL                37

/*-----------------------------------------*/

// custom settings
extern bool i2cInitFlag; // flag to check if i2c is initialized

/*-------- RTC Configurations Start ---------*/

#if !defined(UNIX_OFFSET)
// SECONDS_FROM_1970_TO_2000
// Difference between the Y2K and the UNIX epochs, in seconds.
// To convert a Y2K timestamp to UNIX...
#define UNIX_OFFSET 946684800UL
#endif

#if !defined(NTP_OFFSET)
// SECONDS_FROM_1900_TO_2000
// Difference between the Y2K and the NTP epochs, in seconds.
// To convert a Y2K timestamp to NTP...
#define NTP_OFFSET 3155673600UL
#endif
/*-------- RTC Configurations End ---------*/

/* Configurations required for MPU6050 Sensor*/
// /*--------BEGIN OF MPU6050 Sensor Configurations-----------*/

#define MPU6050_ADDRESS_AD0_LOW     0x68 // address pin low (GND), default for InvenSense evaluation board
#define MPU6050_ADDRESS_AD0_HIGH    0x69 // address pin high (VCC)
#define MPU6050_DEFAULT_ADDRESS     MPU6050_ADDRESS_AD0_HIGH

#define MPU6050_RA_XG_OFFS_TC       0x00 //[7] PWR_MODE, [6:1] XG_OFFS_TC, [0] OTP_BNK_VLD
#define MPU6050_RA_YG_OFFS_TC       0x01 //[7] PWR_MODE, [6:1] YG_OFFS_TC, [0] OTP_BNK_VLD
#define MPU6050_RA_ZG_OFFS_TC       0x02 //[7] PWR_MODE, [6:1] ZG_OFFS_TC, [0] OTP_BNK_VLD
#define MPU6050_RA_X_FINE_GAIN      0x03 //[7:0] X_FINE_GAIN
#define MPU6050_RA_Y_FINE_GAIN      0x04 //[7:0] Y_FINE_GAIN
#define MPU6050_RA_Z_FINE_GAIN      0x05 //[7:0] Z_FINE_GAIN
#define MPU6050_RA_XA_OFFS_H        0x06 //[15:0] XA_OFFS
#define MPU6050_RA_XA_OFFS_L_TC     0x07
#define MPU6050_RA_YA_OFFS_H        0x08 //[15:0] YA_OFFS
#define MPU6050_RA_YA_OFFS_L_TC     0x09
#define MPU6050_RA_ZA_OFFS_H        0x0A //[15:0] ZA_OFFS
#define MPU6050_RA_ZA_OFFS_L_TC     0x0B
#define MPU6050_RA_SELF_TEST_X      0x0D //[7:5] XA_TEST[4-2], [4:0] XG_TEST[4-0]
#define MPU6050_RA_SELF_TEST_Y      0x0E //[7:5] YA_TEST[4-2], [4:0] YG_TEST[4-0]
#define MPU6050_RA_SELF_TEST_Z      0x0F //[7:5] ZA_TEST[4-2], [4:0] ZG_TEST[4-0]
#define MPU6050_RA_SELF_TEST_A      0x10 //[5:4] XA_TEST[1-0], [3:2] YA_TEST[1-0], [1:0] ZA_TEST[1-0]
#define MPU6050_RA_XG_OFFS_USRH     0x13 //[15:0] XG_OFFS_USR
#define MPU6050_RA_XG_OFFS_USRL     0x14
#define MPU6050_RA_YG_OFFS_USRH     0x15 //[15:0] YG_OFFS_USR
#define MPU6050_RA_YG_OFFS_USRL     0x16
#define MPU6050_RA_ZG_OFFS_USRH     0x17 //[15:0] ZG_OFFS_USR
#define MPU6050_RA_ZG_OFFS_USRL     0x18
#define MPU6050_RA_SMPLRT_DIV       0x19
#define MPU6050_RA_CONFIG           0x1A
#define MPU6050_RA_GYRO_CONFIG      0x1B
#define MPU6050_RA_ACCEL_CONFIG     0x1C
#define MPU6050_RA_FF_THR           0x1D
#define MPU6050_RA_FF_DUR           0x1E
#define MPU6050_RA_MOT_THR          0x1F
#define MPU6050_RA_MOT_DUR          0x20
#define MPU6050_RA_ZRMOT_THR        0x21
#define MPU6050_RA_ZRMOT_DUR        0x22
#define MPU6050_RA_FIFO_EN          0x23
#define MPU6050_RA_I2C_MST_CTRL     0x24
#define MPU6050_RA_I2C_SLV0_ADDR    0x25
#define MPU6050_RA_I2C_SLV0_REG     0x26
#define MPU6050_RA_I2C_SLV0_CTRL    0x27
#define MPU6050_RA_I2C_SLV1_ADDR    0x28
#define MPU6050_RA_I2C_SLV1_REG     0x29
#define MPU6050_RA_I2C_SLV1_CTRL    0x2A
#define MPU6050_RA_I2C_SLV2_ADDR    0x2B
#define MPU6050_RA_I2C_SLV2_REG     0x2C
#define MPU6050_RA_I2C_SLV2_CTRL    0x2D
#define MPU6050_RA_I2C_SLV3_ADDR    0x2E
#define MPU6050_RA_I2C_SLV3_REG     0x2F
#define MPU6050_RA_I2C_SLV3_CTRL    0x30
#define MPU6050_RA_I2C_SLV4_ADDR    0x31
#define MPU6050_RA_I2C_SLV4_REG     0x32
#define MPU6050_RA_I2C_SLV4_DO      0x33
#define MPU6050_RA_I2C_SLV4_CTRL    0x34
#define MPU6050_RA_I2C_SLV4_DI      0x35
#define MPU6050_RA_I2C_MST_STATUS   0x36
#define MPU6050_RA_INT_PIN_CFG      0x37
#define MPU6050_RA_INT_ENABLE       0x38
#define MPU6050_RA_DMP_INT_STATUS   0x39
#define MPU6050_RA_INT_STATUS       0x3A
#define MPU6050_RA_ACCEL_XOUT_H     0x3B
#define MPU6050_RA_ACCEL_XOUT_L     0x3C
#define MPU6050_RA_ACCEL_YOUT_H     0x3D
#define MPU6050_RA_ACCEL_YOUT_L     0x3E
#define MPU6050_RA_ACCEL_ZOUT_H     0x3F
#define MPU6050_RA_ACCEL_ZOUT_L     0x40
#define MPU6050_RA_TEMP_OUT_H       0x41
#define MPU6050_RA_TEMP_OUT_L       0x42
#define MPU6050_RA_GYRO_XOUT_H      0x43
#define MPU6050_RA_GYRO_XOUT_L      0x44
#define MPU6050_RA_GYRO_YOUT_H      0x45
#define MPU6050_RA_GYRO_YOUT_L      0x46
#define MPU6050_RA_GYRO_ZOUT_H      0x47
#define MPU6050_RA_GYRO_ZOUT_L      0x48
#define MPU6050_RA_EXT_SENS_DATA_00 0x49
#define MPU6050_RA_EXT_SENS_DATA_01 0x4A
#define MPU6050_RA_EXT_SENS_DATA_02 0x4B
#define MPU6050_RA_EXT_SENS_DATA_03 0x4C
#define MPU6050_RA_EXT_SENS_DATA_04 0x4D
#define MPU6050_RA_EXT_SENS_DATA_05 0x4E
#define MPU6050_RA_EXT_SENS_DATA_06 0x4F
#define MPU6050_RA_EXT_SENS_DATA_07 0x50
#define MPU6050_RA_EXT_SENS_DATA_08 0x51
#define MPU6050_RA_EXT_SENS_DATA_09 0x52
#define MPU6050_RA_EXT_SENS_DATA_10 0x53
#define MPU6050_RA_EXT_SENS_DATA_11 0x54
#define MPU6050_RA_EXT_SENS_DATA_12 0x55
#define MPU6050_RA_EXT_SENS_DATA_13 0x56
#define MPU6050_RA_EXT_SENS_DATA_14 0x57
#define MPU6050_RA_EXT_SENS_DATA_15 0x58
#define MPU6050_RA_EXT_SENS_DATA_16 0x59
#define MPU6050_RA_EXT_SENS_DATA_17 0x5A
#define MPU6050_RA_EXT_SENS_DATA_18 0x5B
#define MPU6050_RA_EXT_SENS_DATA_19 0x5C
#define MPU6050_RA_EXT_SENS_DATA_20 0x5D
#define MPU6050_RA_EXT_SENS_DATA_21 0x5E
#define MPU6050_RA_EXT_SENS_DATA_22 0x5F
#define MPU6050_RA_EXT_SENS_DATA_23 0x60
#define MPU6050_RA_MOT_DETECT_STATUS    0x61
#define MPU6050_RA_I2C_SLV0_DO      0x63
#define MPU6050_RA_I2C_SLV1_DO      0x64
#define MPU6050_RA_I2C_SLV2_DO      0x65
#define MPU6050_RA_I2C_SLV3_DO      0x66
#define MPU6050_RA_I2C_MST_DELAY_CTRL   0x67
#define MPU6050_RA_SIGNAL_PATH_RESET    0x68
#define MPU6050_RA_MOT_DETECT_CTRL      0x69
#define MPU6050_RA_USER_CTRL        0x6A
#define MPU6050_RA_PWR_MGMT_1       0x6B
#define MPU6050_RA_PWR_MGMT_2       0x6C
#define MPU6050_RA_BANK_SEL         0x6D
#define MPU6050_RA_MEM_START_ADDR   0x6E
#define MPU6050_RA_MEM_R_W          0x6F
#define MPU6050_RA_DMP_CFG_1        0x70
#define MPU6050_RA_DMP_CFG_2        0x71
#define MPU6050_RA_FIFO_COUNTH      0x72
#define MPU6050_RA_FIFO_COUNTL      0x73
#define MPU6050_RA_FIFO_R_W         0x74
#define MPU6050_RA_WHO_AM_I         0x75

#define MPU6050_SELF_TEST_XA_1_BIT     0x07
#define MPU6050_SELF_TEST_XA_1_LENGTH  0x03
#define MPU6050_SELF_TEST_XA_2_BIT     0x05
#define MPU6050_SELF_TEST_XA_2_LENGTH  0x02
#define MPU6050_SELF_TEST_YA_1_BIT     0x07
#define MPU6050_SELF_TEST_YA_1_LENGTH  0x03
#define MPU6050_SELF_TEST_YA_2_BIT     0x03
#define MPU6050_SELF_TEST_YA_2_LENGTH  0x02
#define MPU6050_SELF_TEST_ZA_1_BIT     0x07
#define MPU6050_SELF_TEST_ZA_1_LENGTH  0x03
#define MPU6050_SELF_TEST_ZA_2_BIT     0x01
#define MPU6050_SELF_TEST_ZA_2_LENGTH  0x02

#define MPU6050_SELF_TEST_XG_1_BIT     0x04
#define MPU6050_SELF_TEST_XG_1_LENGTH  0x05
#define MPU6050_SELF_TEST_YG_1_BIT     0x04
#define MPU6050_SELF_TEST_YG_1_LENGTH  0x05
#define MPU6050_SELF_TEST_ZG_1_BIT     0x04
#define MPU6050_SELF_TEST_ZG_1_LENGTH  0x05

#define MPU6050_TC_PWR_MODE_BIT     7
#define MPU6050_TC_OFFSET_BIT       6
#define MPU6050_TC_OFFSET_LENGTH    6
#define MPU6050_TC_OTP_BNK_VLD_BIT  0

#define MPU6050_VDDIO_LEVEL_VLOGIC  0
#define MPU6050_VDDIO_LEVEL_VDD     1

#define MPU6050_CFG_EXT_SYNC_SET_BIT    5
#define MPU6050_CFG_EXT_SYNC_SET_LENGTH 3
#define MPU6050_CFG_DLPF_CFG_BIT    2
#define MPU6050_CFG_DLPF_CFG_LENGTH 3

#define MPU6050_EXT_SYNC_DISABLED       0x0
#define MPU6050_EXT_SYNC_TEMP_OUT_L     0x1
#define MPU6050_EXT_SYNC_GYRO_XOUT_L    0x2
#define MPU6050_EXT_SYNC_GYRO_YOUT_L    0x3
#define MPU6050_EXT_SYNC_GYRO_ZOUT_L    0x4
#define MPU6050_EXT_SYNC_ACCEL_XOUT_L   0x5
#define MPU6050_EXT_SYNC_ACCEL_YOUT_L   0x6
#define MPU6050_EXT_SYNC_ACCEL_ZOUT_L   0x7

#define MPU6050_DLPF_BW_256         0x00
#define MPU6050_DLPF_BW_188         0x01
#define MPU6050_DLPF_BW_98          0x02
#define MPU6050_DLPF_BW_42          0x03
#define MPU6050_DLPF_BW_20          0x04
#define MPU6050_DLPF_BW_10          0x05
#define MPU6050_DLPF_BW_5           0x06

#define MPU6050_GCONFIG_FS_SEL_BIT      4
#define MPU6050_GCONFIG_FS_SEL_LENGTH   2

#define MPU6050_GYRO_FS_250         0x00
#define MPU6050_GYRO_FS_500         0x01
#define MPU6050_GYRO_FS_1000        0x02
#define MPU6050_GYRO_FS_2000        0x03

#define MPU6050_ACONFIG_XA_ST_BIT           7
#define MPU6050_ACONFIG_YA_ST_BIT           6
#define MPU6050_ACONFIG_ZA_ST_BIT           5
#define MPU6050_ACONFIG_AFS_SEL_BIT         4
#define MPU6050_ACONFIG_AFS_SEL_LENGTH      2
#define MPU6050_ACONFIG_ACCEL_HPF_BIT       2
#define MPU6050_ACONFIG_ACCEL_HPF_LENGTH    3

#define MPU6050_ACCEL_FS_2          0x00
#define MPU6050_ACCEL_FS_4          0x01
#define MPU6050_ACCEL_FS_8          0x02
#define MPU6050_ACCEL_FS_16         0x03

#define MPU6050_DHPF_RESET          0x00
#define MPU6050_DHPF_5              0x01
#define MPU6050_DHPF_2P5            0x02
#define MPU6050_DHPF_1P25           0x03
#define MPU6050_DHPF_0P63           0x04
#define MPU6050_DHPF_HOLD           0x07

#define MPU6050_TEMP_FIFO_EN_BIT    7
#define MPU6050_XG_FIFO_EN_BIT      6
#define MPU6050_YG_FIFO_EN_BIT      5
#define MPU6050_ZG_FIFO_EN_BIT      4
#define MPU6050_ACCEL_FIFO_EN_BIT   3
#define MPU6050_SLV2_FIFO_EN_BIT    2
#define MPU6050_SLV1_FIFO_EN_BIT    1
#define MPU6050_SLV0_FIFO_EN_BIT    0

#define MPU6050_MULT_MST_EN_BIT     7
#define MPU6050_WAIT_FOR_ES_BIT     6
#define MPU6050_SLV_3_FIFO_EN_BIT   5
#define MPU6050_I2C_MST_P_NSR_BIT   4
#define MPU6050_I2C_MST_CLK_BIT     3
#define MPU6050_I2C_MST_CLK_LENGTH  4

#define MPU6050_CLOCK_DIV_348       0x0
#define MPU6050_CLOCK_DIV_333       0x1
#define MPU6050_CLOCK_DIV_320       0x2
#define MPU6050_CLOCK_DIV_308       0x3
#define MPU6050_CLOCK_DIV_296       0x4
#define MPU6050_CLOCK_DIV_286       0x5
#define MPU6050_CLOCK_DIV_276       0x6
#define MPU6050_CLOCK_DIV_267       0x7
#define MPU6050_CLOCK_DIV_258       0x8
#define MPU6050_CLOCK_DIV_500       0x9
#define MPU6050_CLOCK_DIV_471       0xA
#define MPU6050_CLOCK_DIV_444       0xB
#define MPU6050_CLOCK_DIV_421       0xC
#define MPU6050_CLOCK_DIV_400       0xD
#define MPU6050_CLOCK_DIV_381       0xE
#define MPU6050_CLOCK_DIV_364       0xF

#define MPU6050_I2C_SLV_RW_BIT      7
#define MPU6050_I2C_SLV_ADDR_BIT    6
#define MPU6050_I2C_SLV_ADDR_LENGTH 7
#define MPU6050_I2C_SLV_EN_BIT      7
#define MPU6050_I2C_SLV_BYTE_SW_BIT 6
#define MPU6050_I2C_SLV_REG_DIS_BIT 5
#define MPU6050_I2C_SLV_GRP_BIT     4
#define MPU6050_I2C_SLV_LEN_BIT     3
#define MPU6050_I2C_SLV_LEN_LENGTH  4

#define MPU6050_I2C_SLV4_RW_BIT         7
#define MPU6050_I2C_SLV4_ADDR_BIT       6
#define MPU6050_I2C_SLV4_ADDR_LENGTH    7
#define MPU6050_I2C_SLV4_EN_BIT         7
#define MPU6050_I2C_SLV4_INT_EN_BIT     6
#define MPU6050_I2C_SLV4_REG_DIS_BIT    5
#define MPU6050_I2C_SLV4_MST_DLY_BIT    4
#define MPU6050_I2C_SLV4_MST_DLY_LENGTH 5

#define MPU6050_MST_PASS_THROUGH_BIT    7
#define MPU6050_MST_I2C_SLV4_DONE_BIT   6
#define MPU6050_MST_I2C_LOST_ARB_BIT    5
#define MPU6050_MST_I2C_SLV4_NACK_BIT   4
#define MPU6050_MST_I2C_SLV3_NACK_BIT   3
#define MPU6050_MST_I2C_SLV2_NACK_BIT   2
#define MPU6050_MST_I2C_SLV1_NACK_BIT   1
#define MPU6050_MST_I2C_SLV0_NACK_BIT   0

#define MPU6050_INTCFG_INT_LEVEL_BIT        7
#define MPU6050_INTCFG_INT_OPEN_BIT         6
#define MPU6050_INTCFG_LATCH_INT_EN_BIT     5
#define MPU6050_INTCFG_INT_RD_CLEAR_BIT     4
#define MPU6050_INTCFG_FSYNC_INT_LEVEL_BIT  3
#define MPU6050_INTCFG_FSYNC_INT_EN_BIT     2
#define MPU6050_INTCFG_I2C_BYPASS_EN_BIT    1
#define MPU6050_INTCFG_CLKOUT_EN_BIT        0

#define MPU6050_INTMODE_ACTIVEHIGH  0x00
#define MPU6050_INTMODE_ACTIVELOW   0x01

#define MPU6050_INTDRV_PUSHPULL     0x00
#define MPU6050_INTDRV_OPENDRAIN    0x01

#define MPU6050_INTLATCH_50USPULSE  0x00
#define MPU6050_INTLATCH_WAITCLEAR  0x01

#define MPU6050_INTCLEAR_STATUSREAD 0x00
#define MPU6050_INTCLEAR_ANYREAD    0x01

#define MPU6050_INTERRUPT_FF_BIT            7
#define MPU6050_INTERRUPT_MOT_BIT           6
#define MPU6050_INTERRUPT_ZMOT_BIT          5
#define MPU6050_INTERRUPT_FIFO_OFLOW_BIT    4
#define MPU6050_INTERRUPT_I2C_MST_INT_BIT   3
#define MPU6050_INTERRUPT_PLL_RDY_INT_BIT   2
#define MPU6050_INTERRUPT_DMP_INT_BIT       1
#define MPU6050_INTERRUPT_DATA_RDY_BIT      0

// TODO: figure out what these actually do
// UMPL source code is not very obivous
#define MPU6050_DMPINT_5_BIT            5
#define MPU6050_DMPINT_4_BIT            4
#define MPU6050_DMPINT_3_BIT            3
#define MPU6050_DMPINT_2_BIT            2
#define MPU6050_DMPINT_1_BIT            1
#define MPU6050_DMPINT_0_BIT            0

#define MPU6050_MOTION_MOT_XNEG_BIT     7
#define MPU6050_MOTION_MOT_XPOS_BIT     6
#define MPU6050_MOTION_MOT_YNEG_BIT     5
#define MPU6050_MOTION_MOT_YPOS_BIT     4
#define MPU6050_MOTION_MOT_ZNEG_BIT     3
#define MPU6050_MOTION_MOT_ZPOS_BIT     2
#define MPU6050_MOTION_MOT_ZRMOT_BIT    0

#define MPU6050_DELAYCTRL_DELAY_ES_SHADOW_BIT   7
#define MPU6050_DELAYCTRL_I2C_SLV4_DLY_EN_BIT   4
#define MPU6050_DELAYCTRL_I2C_SLV3_DLY_EN_BIT   3
#define MPU6050_DELAYCTRL_I2C_SLV2_DLY_EN_BIT   2
#define MPU6050_DELAYCTRL_I2C_SLV1_DLY_EN_BIT   1
#define MPU6050_DELAYCTRL_I2C_SLV0_DLY_EN_BIT   0

#define MPU6050_PATHRESET_GYRO_RESET_BIT    2
#define MPU6050_PATHRESET_ACCEL_RESET_BIT   1
#define MPU6050_PATHRESET_TEMP_RESET_BIT    0

#define MPU6050_DETECT_ACCEL_ON_DELAY_BIT       5
#define MPU6050_DETECT_ACCEL_ON_DELAY_LENGTH    2
#define MPU6050_DETECT_FF_COUNT_BIT             3
#define MPU6050_DETECT_FF_COUNT_LENGTH          2
#define MPU6050_DETECT_MOT_COUNT_BIT            1
#define MPU6050_DETECT_MOT_COUNT_LENGTH         2

#define MPU6050_DETECT_DECREMENT_RESET  0x0
#define MPU6050_DETECT_DECREMENT_1      0x1
#define MPU6050_DETECT_DECREMENT_2      0x2
#define MPU6050_DETECT_DECREMENT_4      0x3

#define MPU6050_USERCTRL_DMP_EN_BIT             7
#define MPU6050_USERCTRL_FIFO_EN_BIT            6
#define MPU6050_USERCTRL_I2C_MST_EN_BIT         5
#define MPU6050_USERCTRL_I2C_IF_DIS_BIT         4
#define MPU6050_USERCTRL_DMP_RESET_BIT          3
#define MPU6050_USERCTRL_FIFO_RESET_BIT         2
#define MPU6050_USERCTRL_I2C_MST_RESET_BIT      1
#define MPU6050_USERCTRL_SIG_COND_RESET_BIT     0

#define MPU6050_PWR1_DEVICE_RESET_BIT   7
#define MPU6050_PWR1_SLEEP_BIT          6
#define MPU6050_PWR1_CYCLE_BIT          5
#define MPU6050_PWR1_TEMP_DIS_BIT       3
#define MPU6050_PWR1_CLKSEL_BIT         2
#define MPU6050_PWR1_CLKSEL_LENGTH      3

#define MPU6050_CLOCK_INTERNAL          0x00
#define MPU6050_CLOCK_PLL_XGYRO         0x01
#define MPU6050_CLOCK_PLL_YGYRO         0x02
#define MPU6050_CLOCK_PLL_ZGYRO         0x03
#define MPU6050_CLOCK_PLL_EXT32K        0x04
#define MPU6050_CLOCK_PLL_EXT19M        0x05
#define MPU6050_CLOCK_KEEP_RESET        0x07

#define MPU6050_PWR2_LP_WAKE_CTRL_BIT       7
#define MPU6050_PWR2_LP_WAKE_CTRL_LENGTH    2
#define MPU6050_PWR2_STBY_XA_BIT            5
#define MPU6050_PWR2_STBY_YA_BIT            4
#define MPU6050_PWR2_STBY_ZA_BIT            3
#define MPU6050_PWR2_STBY_XG_BIT            2
#define MPU6050_PWR2_STBY_YG_BIT            1
#define MPU6050_PWR2_STBY_ZG_BIT            0

#define MPU6050_WAKE_FREQ_1P25      0x0
#define MPU6050_WAKE_FREQ_2P5       0x1
#define MPU6050_WAKE_FREQ_5         0x2
#define MPU6050_WAKE_FREQ_10        0x3

#define MPU6050_BANKSEL_PRFTCH_EN_BIT       6
#define MPU6050_BANKSEL_CFG_USER_BANK_BIT   5
#define MPU6050_BANKSEL_MEM_SEL_BIT         4
#define MPU6050_BANKSEL_MEM_SEL_LENGTH      5

#define MPU6050_WHO_AM_I_BIT        6
#define MPU6050_WHO_AM_I_LENGTH     6

#define MPU6050_DMP_MEMORY_BANKS        8
#define MPU6050_DMP_MEMORY_BANK_SIZE    256
#define MPU6050_DMP_MEMORY_CHUNK_SIZE   16

#define MPU6050_FIFO_DEFAULT_TIMEOUT 11000

enum class ACCEL_FS {
    A2G,
    A4G,
    A8G,
    A16G
};

enum class GYRO_FS {
    G250DPS,
    G500DPS,
    G1000DPS,
    G2000DPS
};

// ----- MPU6050 calibartion constants and some variables -----
#define     usDelay                 3150       // Delay in ms to hold the sampling at 200Hz
#define     LinesBetweenHeaders     5
#define     iAx                     0
#define     iAy                     1
#define     iAz                     2
#define     iGx                     3
#define     iGy                     4
#define     iGz                     5
#define     NFast                   1000
#define     NSlow                   10000

// /*-----------END OF MPU6050 Sensor Configurations--------------*/

/*------------------Begin of MAX30102 Configurations---------------------- */
#define MAX30102_IIC_ADDRESS            0x57 // I2C Address

// Status Registers
#define MAX30102_INTSTAT1               0x00   // Interrupt Status1
#define MAX30102_INTSTAT2               0x01   // Interrupt Status2
#define MAX30102_INTENABLE1             0x02 // Interrupt Enable1
#define MAX30102_INTENABLE2             0x03 // Interrupt Enable2

// FIFO Registers
#define MAX30102_FIFOWRITEPTR           0x04 // FIFO Write Pointer, The FIFO Write Pointer points to the location where the MAX30102 writes the next sample. This pointer advances for each sample pushed on to the FIFO. It can also be changed through the I2C interface when MODE[2:0] is 010, 011, or 111.

#define MAX30102_FIFOOVERFLOW           0x05 // FIFO Overflow Counter, When the FIFO is full, samples are not pushed on to the FIFO, samples are lost. OVF_COUNTER counts the number of samples lost. It saturates at 0x1F. When a complete sample is â€œpoppedâ€ (i.e., removal of old FIFO data and shifting the samples down) from the FIFO (when the read pointer advances), OVF_COUNTER is reset to zero.

#define MAX30102_FIFOREADPTR            0x06 // FIFO Read Pointer, The FIFO Read Pointer points to the location from where the processor gets the next sample from the FIFO through the I2C interface. This advances each time a sample is popped from the FIFO. The processor can also write to this pointer after reading the samples to allow rereading samples from the FIFO if there is a data communication error.

#define MAX30102_FIFODATA               0x07 // FIFO Data Register, The circular FIFO depth is 32 and can hold up to 32 samples of data. The sample size depends on the number of LED channels (a.k.a. channels) configured as active. As each channel signal is stored as a 3-byte data signal, the FIFO width can be 3 bytes or 6 bytes in size.

// Configuration Registers
#define MAX30102_FIFOCONFIG             0x08      // FIFO Configuration
#define MAX30102_MODECONFIG             0x09      // Mode Configuration
#define MAX30102_PARTICLECONFIG         0x0A  // SpO2 Configuration
#define MAX30102_LED1_PULSEAMP          0x0C   // LED1 Pulse Amplitude
#define MAX30102_LED2_PULSEAMP          0x0D   // LED2 Pulse Amplitude
#define MAX30102_LED3_PULSEAMP          0x0E   // RESERVED
#define MAX30102_LED_PROX_AMP           0x10    // RESERVED
#define MAX30102_MULTILEDCONFIG1        0x11 // Multi-LED Mode Control Registers
#define MAX30102_MULTILEDCONFIG2        0x12 // Multi-LED Mode Control Registers

// Die Temperature Registers
#define MAX30102_DIETEMPINT             0x1F    // Die Temp Integer
#define MAX30102_DIETEMPFRAC            0x20   // Die Temp Fraction
#define MAX30102_DIETEMPCONFIG          0x21 // Die Temperature Config
#define MAX30102_PROXINTTHRESH          0x30 // RESERVED

// Part ID Registers
#define MAX30102_REVISIONID             0xFE // Revision ID
#define MAX30102_PARTID                 0xFF     // Part ID:0x15
#define MAX30102_EXPECTED_PARTID        0x15
#define MAX30102_SENSE_BUF_SIZE         30

/*------------------End of MAX30102 Configurations---------------------- */

/*------------------ Begin ESP32 Servo Configurations ------------------- */
#define ENFORCE_PINS
#define DEFAULT_uS_LOW                  544
#define DEFAULT_uS_HIGH                 2400
#define MINIMUM_TIMER_WIDTH             10
#define MAXIMUM_TIMER_WIDTH             20
#define DEFAULT_TIMER_WIDTH             10
#define DEFAULT_TIMER_WIDTH_TICKS       1024
#define MIN_PULSE_WIDTH                 500
#define MAX_PULSE_WIDTH                 2500
#define DEFAULT_PULSE_WIDTH             1500
#define DEFAULT_PULSE_WIDTH_TICKS       4825
#define REFRESH_USEC                    20000
#define MAX_SERVOS                      16
static const char *TAG = "ESP32Servo";

/*------------------ End ESP32 Servo Configurations ------------------- */


/*-------- LF_Alpha_APDS9960 configurations begin ---------*/

#define APDS9960_ADDRESS (0x39) /**< I2C Address */
/** I2C Registers */
enum {
  APDS9960_RAM = 0x00,
  APDS9960_ENABLE = 0x80,
  APDS9960_ATIME = 0x81,
  APDS9960_WTIME = 0x83,
  APDS9960_AILTIL = 0x84,
  APDS9960_AILTH = 0x85,
  APDS9960_AIHTL = 0x86,
  APDS9960_AIHTH = 0x87,
  APDS9960_PILT = 0x89,
  APDS9960_PIHT = 0x8B,
  APDS9960_PERS = 0x8C,
  APDS9960_CONFIG1 = 0x8D,
  APDS9960_PPULSE = 0x8E,
  APDS9960_CONTROL = 0x8F,
  APDS9960_CONFIG2 = 0x90,
  APDS9960_ID = 0x92,
  APDS9960_STATUS = 0x93,
  APDS9960_CDATAL = 0x94,
  APDS9960_CDATAH = 0x95,
  APDS9960_RDATAL = 0x96,
  APDS9960_RDATAH = 0x97,
  APDS9960_GDATAL = 0x98,
  APDS9960_GDATAH = 0x99,
  APDS9960_BDATAL = 0x9A,
  APDS9960_BDATAH = 0x9B,
  APDS9960_PDATA = 0x9C,
  APDS9960_POFFSET_UR = 0x9D,
  APDS9960_POFFSET_DL = 0x9E,
  APDS9960_CONFIG3 = 0x9F,
  APDS9960_GPENTH = 0xA0,
  APDS9960_GEXTH = 0xA1,
  APDS9960_GCONF1 = 0xA2,
  APDS9960_GCONF2 = 0xA3,
  APDS9960_GOFFSET_U = 0xA4,
  APDS9960_GOFFSET_D = 0xA5,
  APDS9960_GOFFSET_L = 0xA7,
  APDS9960_GOFFSET_R = 0xA9,
  APDS9960_GPULSE = 0xA6,
  APDS9960_GCONF3 = 0xAA,
  APDS9960_GCONF4 = 0xAB,
  APDS9960_GFLVL = 0xAE,
  APDS9960_GSTATUS = 0xAF,
  APDS9960_IFORCE = 0xE4,
  APDS9960_PICLEAR = 0xE5,
  APDS9960_CICLEAR = 0xE6,
  APDS9960_AICLEAR = 0xE7,
  APDS9960_GFIFO_U = 0xFC,
  APDS9960_GFIFO_D = 0xFD,
  APDS9960_GFIFO_L = 0xFE,
  APDS9960_GFIFO_R = 0xFF,
};

/** ADC gain settings */
typedef enum {
  APDS9960_AGAIN_1X = 0x00,  /**< No gain */
  APDS9960_AGAIN_4X = 0x01,  /**< 2x gain */
  APDS9960_AGAIN_16X = 0x02, /**< 16x gain */
  APDS9960_AGAIN_64X = 0x03  /**< 64x gain */
} apds9960AGain_t;

/** Proxmity gain settings */
typedef enum {
  APDS9960_PGAIN_1X = 0x00, /**< 1x gain */
  APDS9960_PGAIN_2X = 0x01, /**< 2x gain */
  APDS9960_PGAIN_4X = 0x02, /**< 4x gain */
  APDS9960_PGAIN_8X = 0x03  /**< 8x gain */
} apds9960PGain_t;

/** Pulse length settings */
typedef enum {
  APDS9960_PPULSELEN_4US = 0x00,  /**< 4uS */
  APDS9960_PPULSELEN_8US = 0x01,  /**< 8uS */
  APDS9960_PPULSELEN_16US = 0x02, /**< 16uS */
  APDS9960_PPULSELEN_32US = 0x03  /**< 32uS */
} apds9960PPulseLen_t;

/** LED drive settings */
typedef enum {
  APDS9960_LEDDRIVE_100MA = 0x00, /**< 100mA */
  APDS9960_LEDDRIVE_50MA = 0x01,  /**< 50mA */
  APDS9960_LEDDRIVE_25MA = 0x02,  /**< 25mA */
  APDS9960_LEDDRIVE_12MA = 0x03   /**< 12.5mA */
} apds9960LedDrive_t;

/** LED boost settings */
typedef enum {
  APDS9960_LEDBOOST_100PCNT = 0x00, /**< 100% */
  APDS9960_LEDBOOST_150PCNT = 0x01, /**< 150% */
  APDS9960_LEDBOOST_200PCNT = 0x02, /**< 200% */
  APDS9960_LEDBOOST_300PCNT = 0x03  /**< 300% */
} apds9960LedBoost_t;

/** Dimensions */
enum {
  APDS9960_DIMENSIONS_ALL = 0x00,        // All dimensions
  APDS9960_DIMENSIONS_UP_DOWN = 0x01,    // Up/Down dimensions
  APDS9960_DIMENSIONS_LEFT_RIGHT = 0x02, // Left/Right dimensions
};

/** FIFO Interrupts */
enum {
  APDS9960_GFIFO_1 = 0x00,  // Generate interrupt after 1 dataset in FIFO
  APDS9960_GFIFO_4 = 0x01,  // Generate interrupt after 2 datasets in FIFO
  APDS9960_GFIFO_8 = 0x02,  // Generate interrupt after 3 datasets in FIFO
  APDS9960_GFIFO_16 = 0x03, // Generate interrupt after 4 datasets in FIFO
};

/** Gesture Gain */
enum {
  APDS9960_GGAIN_1 = 0x00, // Gain 1x
  APDS9960_GGAIN_2 = 0x01, // Gain 2x
  APDS9960_GGAIN_4 = 0x02, // Gain 4x
  APDS9960_GGAIN_8 = 0x03, // Gain 8x
};

/** Pulse Lenghts */
enum {
  APDS9960_GPULSE_4US = 0x00,  // Pulse 4us
  APDS9960_GPULSE_8US = 0x01,  // Pulse 8us
  APDS9960_GPULSE_16US = 0x02, // Pulse 16us
  APDS9960_GPULSE_32US = 0x03, // Pulse 32us
};

#define APDS9960_UP 0x01    /**< Gesture Up */
#define APDS9960_DOWN 0x02  /**< Gesture Down */
#define APDS9960_LEFT 0x03  /**< Gesture Left */
#define APDS9960_RIGHT 0x04 /**< Gesture Right */

/*-------- LF_Alpha_APDS9960 configurations end ---------*/


/*-------- LF_Alpha_rgbLEDs configurations start ---------*/
// RGB NeoPixel permutations; white and red offsets are always same
// Offset:        W          R          G          B
#define NEO_RGB ((0 << 6) | (0 << 4) | (1 << 2) | (2)) ///< Transmit as R,G,B
#define NEO_RBG ((0 << 6) | (0 << 4) | (2 << 2) | (1)) ///< Transmit as R,B,G
#define NEO_GRB ((1 << 6) | (1 << 4) | (0 << 2) | (2)) ///< Transmit as G,R,B
#define NEO_GBR ((2 << 6) | (2 << 4) | (0 << 2) | (1)) ///< Transmit as G,B,R
#define NEO_BRG ((1 << 6) | (1 << 4) | (2 << 2) | (0)) ///< Transmit as B,R,G
#define NEO_BGR ((2 << 6) | (2 << 4) | (1 << 2) | (0)) ///< Transmit as B,G,R

// RGBW NeoPixel permutations; all 4 offsets are distinct
// Offset:         W          R          G          B
#define NEO_WRGB ((0 << 6) | (1 << 4) | (2 << 2) | (3)) ///< Transmit as W,R,G,B
#define NEO_WRBG ((0 << 6) | (1 << 4) | (3 << 2) | (2)) ///< Transmit as W,R,B,G
#define NEO_WGRB ((0 << 6) | (2 << 4) | (1 << 2) | (3)) ///< Transmit as W,G,R,B
#define NEO_WGBR ((0 << 6) | (3 << 4) | (1 << 2) | (2)) ///< Transmit as W,G,B,R
#define NEO_WBRG ((0 << 6) | (2 << 4) | (3 << 2) | (1)) ///< Transmit as W,B,R,G
#define NEO_WBGR ((0 << 6) | (3 << 4) | (2 << 2) | (1)) ///< Transmit as W,B,G,R

#define NEO_RWGB ((1 << 6) | (0 << 4) | (2 << 2) | (3)) ///< Transmit as R,W,G,B
#define NEO_RWBG ((1 << 6) | (0 << 4) | (3 << 2) | (2)) ///< Transmit as R,W,B,G
#define NEO_RGWB ((2 << 6) | (0 << 4) | (1 << 2) | (3)) ///< Transmit as R,G,W,B
#define NEO_RGBW ((3 << 6) | (0 << 4) | (1 << 2) | (2)) ///< Transmit as R,G,B,W
#define NEO_RBWG ((2 << 6) | (0 << 4) | (3 << 2) | (1)) ///< Transmit as R,B,W,G
#define NEO_RBGW ((3 << 6) | (0 << 4) | (2 << 2) | (1)) ///< Transmit as R,B,G,W

#define NEO_GWRB ((1 << 6) | (2 << 4) | (0 << 2) | (3)) ///< Transmit as G,W,R,B
#define NEO_GWBR ((1 << 6) | (3 << 4) | (0 << 2) | (2)) ///< Transmit as G,W,B,R
#define NEO_GRWB ((2 << 6) | (1 << 4) | (0 << 2) | (3)) ///< Transmit as G,R,W,B
#define NEO_GRBW ((3 << 6) | (1 << 4) | (0 << 2) | (2)) ///< Transmit as G,R,B,W
#define NEO_GBWR ((2 << 6) | (3 << 4) | (0 << 2) | (1)) ///< Transmit as G,B,W,R
#define NEO_GBRW ((3 << 6) | (2 << 4) | (0 << 2) | (1)) ///< Transmit as G,B,R,W

#define NEO_BWRG ((1 << 6) | (2 << 4) | (3 << 2) | (0)) ///< Transmit as B,W,R,G
#define NEO_BWGR ((1 << 6) | (3 << 4) | (2 << 2) | (0)) ///< Transmit as B,W,G,R
#define NEO_BRWG ((2 << 6) | (1 << 4) | (3 << 2) | (0)) ///< Transmit as B,R,W,G
#define NEO_BRGW ((3 << 6) | (1 << 4) | (2 << 2) | (0)) ///< Transmit as B,R,G,W
#define NEO_BGWR ((2 << 6) | (3 << 4) | (1 << 2) | (0)) ///< Transmit as B,G,W,R
#define NEO_BGRW ((3 << 6) | (2 << 4) | (1 << 2) | (0)) ///< Transmit as B,G,R,W

// Add NEO_KHZ400 to the color order value to indicate a 400 KHz device.
// All but the earliest v1 NeoPixels expect an 800 KHz data stream, this is
// the default if unspecified. Because flash space is very limited on ATtiny
// devices (e.g. Trinket, Gemma), v1 NeoPixels aren't handled by default on
// those chips, though it can be enabled by removing the ifndef/endif below,
// but code will be bigger. Conversely, can disable the NEO_KHZ400 line on
// other MCUs to remove v1 support and save a little space.

#define NEO_KHZ800 0x0000 ///< 800 KHz data transmission
#ifndef __AVR_ATtiny85__
#define NEO_KHZ400 0x0100 ///< 400 KHz data transmission
#endif

// If 400 KHz support is enabled, the third parameter to the constructor
// requires a 16-bit value (in order to select 400 vs 800 KHz speed).
// If only 800 KHz is enabled (as is default on ATtiny), an 8-bit value
// is sufficient to encode pixel color order, saving some space.

#ifdef NEO_KHZ400
typedef uint16_t neoPixelType; ///< 3rd arg to Adafruit_NeoPixel constructor
#else
typedef uint8_t neoPixelType; ///< 3rd arg to Adafruit_NeoPixel constructor
#endif

// These two tables are declared outside the Adafruit_NeoPixel class
// because some boards may require oldschool compilers that don't
// handle the C++11 constexpr keyword.

/* A PROGMEM (flash mem) table containing 8-bit unsigned sine wave (0-255).
   Copy & paste this snippet into a Python REPL to regenerate:
import math
for x in range(256):
    print("{:3},".format(int((math.sin(x/128.0*math.pi)+1.0)*127.5+0.5))),
    if x&15 == 15: print
*/
static const uint8_t PROGMEM _NeoPixelSineTable[256] = {
    128, 131, 134, 137, 140, 143, 146, 149, 152, 155, 158, 162, 165, 167, 170,
    173, 176, 179, 182, 185, 188, 190, 193, 196, 198, 201, 203, 206, 208, 211,
    213, 215, 218, 220, 222, 224, 226, 228, 230, 232, 234, 235, 237, 238, 240,
    241, 243, 244, 245, 246, 248, 249, 250, 250, 251, 252, 253, 253, 254, 254,
    254, 255, 255, 255, 255, 255, 255, 255, 254, 254, 254, 253, 253, 252, 251,
    250, 250, 249, 248, 246, 245, 244, 243, 241, 240, 238, 237, 235, 234, 232,
    230, 228, 226, 224, 222, 220, 218, 215, 213, 211, 208, 206, 203, 201, 198,
    196, 193, 190, 188, 185, 182, 179, 176, 173, 170, 167, 165, 162, 158, 155,
    152, 149, 146, 143, 140, 137, 134, 131, 128, 124, 121, 118, 115, 112, 109,
    106, 103, 100, 97,  93,  90,  88,  85,  82,  79,  76,  73,  70,  67,  65,
    62,  59,  57,  54,  52,  49,  47,  44,  42,  40,  37,  35,  33,  31,  29,
    27,  25,  23,  21,  20,  18,  17,  15,  14,  12,  11,  10,  9,   7,   6,
    5,   5,   4,   3,   2,   2,   1,   1,   1,   0,   0,   0,   0,   0,   0,
    0,   1,   1,   1,   2,   2,   3,   4,   5,   5,   6,   7,   9,   10,  11,
    12,  14,  15,  17,  18,  20,  21,  23,  25,  27,  29,  31,  33,  35,  37,
    40,  42,  44,  47,  49,  52,  54,  57,  59,  62,  65,  67,  70,  73,  76,
    79,  82,  85,  88,  90,  93,  97,  100, 103, 106, 109, 112, 115, 118, 121,
    124};

/* Similar to above, but for an 8-bit gamma-correction table.
   Copy & paste this snippet into a Python REPL to regenerate:
import math
gamma=2.6
for x in range(256):
    print("{:3},".format(int(math.pow((x)/255.0,gamma)*255.0+0.5))),
    if x&15 == 15: print
*/
static const uint8_t PROGMEM _NeoPixelGammaTable[256] = {
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   1,   1,   1,   1,   1,
    1,   1,   1,   1,   1,   1,   2,   2,   2,   2,   2,   2,   2,   2,   3,
    3,   3,   3,   3,   3,   4,   4,   4,   4,   5,   5,   5,   5,   5,   6,
    6,   6,   6,   7,   7,   7,   8,   8,   8,   9,   9,   9,   10,  10,  10,
    11,  11,  11,  12,  12,  13,  13,  13,  14,  14,  15,  15,  16,  16,  17,
    17,  18,  18,  19,  19,  20,  20,  21,  21,  22,  22,  23,  24,  24,  25,
    25,  26,  27,  27,  28,  29,  29,  30,  31,  31,  32,  33,  34,  34,  35,
    36,  37,  38,  38,  39,  40,  41,  42,  42,  43,  44,  45,  46,  47,  48,
    49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,
    64,  65,  66,  68,  69,  70,  71,  72,  73,  75,  76,  77,  78,  80,  81,
    82,  84,  85,  86,  88,  89,  90,  92,  93,  94,  96,  97,  99,  100, 102,
    103, 105, 106, 108, 109, 111, 112, 114, 115, 117, 119, 120, 122, 124, 125,
    127, 129, 130, 132, 134, 136, 137, 139, 141, 143, 145, 146, 148, 150, 152,
    154, 156, 158, 160, 162, 164, 166, 168, 170, 172, 174, 176, 178, 180, 182,
    184, 186, 188, 191, 193, 195, 197, 199, 202, 204, 206, 209, 211, 213, 215,
    218, 220, 223, 225, 227, 230, 232, 235, 237, 240, 242, 245, 247, 250, 252,
    255};

/* Declare external methods required by the Adafruit_NeoPixel implementation
    for specific hardware/library versions
*/
#if defined(ESP32)
#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5, 0, 0)
extern "C" void espInit();
#endif
#endif

/*-------- LF_Alpha_rgbLEDs configurations end ---------*/

// Buzzer class
class LF_Alpha_Buzzer
{
private:
    int customBuzzerPin = buzzerPin;

public:
    LF_Alpha_Buzzer()
    {
        pinMode(userLED2, OUTPUT);
        digitalWrite(userLED2, LOW);
    }
    bool begin(void);
    bool begin(uint8_t customGPIO);
    void write(bool state);
    void beep(uint16_t duration = 1000);
    bool end();
};

// userLEDs Class implimetation here..
class LF_Alpha_userLEDs
{
private:
    bool selectLED(uint8_t ledNo)
    {
        if (ledNo == 1)
        {
            pinMode(userLED1, OUTPUT);
            return true;
        }

        else if (ledNo == 2)
        {
            pinMode(userLED2, OUTPUT);
            return true;
        }

        else if (ledNo == 3)
        {
            pinMode(userLED1, OUTPUT);
            pinMode(userLED2, OUTPUT);
            return true;
        }
    }

public:
    LF_Alpha_userLEDs() {
        pinMode(userLED2, OUTPUT);
        digitalWrite(userLED2, LOW);
    }
    ~LF_Alpha_userLEDs() {}

    bool begin(uint8_t pin);
    bool begin(uint8_t led1, uint8_t led2);
    void blink(uint8_t led, uint16_t duration);
    void writeLED1(bool state);
    void writeLED2(bool state);
    void write(int led, bool state);
    bool end();
};

// Buttons class
class LF_Alpha_Buttons
{
private:
    // flags for each switch
    bool buttonFlag[4] = {false, false, false, false};

public:
    LF_Alpha_Buttons(){
        pinMode(userLED2, OUTPUT);
        digitalWrite(userLED2, LOW);
    }
    bool begin(uint8_t pin);
    bool begin(uint8_t pin1, uint8_t pin2);
    bool begin(uint8_t pin1, uint8_t pin2, uint8_t pin3);
    bool begin(uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4);
    bool read(uint8_t button);
    bool end();
};

// HCSR04 Ultrasonic Sensor class
class LF_Alpha_HCSR04
{
public:
    LF_Alpha_HCSR04(){
        pinMode(userLED2, OUTPUT);
        digitalWrite(userLED2, LOW);
    }
    bool begin();
    long readCM();
    long readINCH();
    bool end();
};

// Resistive Potentiometer class
class LF_Alpha_resPot
{
private:
    uint8_t configured_ADC_bits = 12;  // defualt ADC resolution    

public:
    LF_Alpha_resPot() {
        pinMode(userLED2, OUTPUT);
        digitalWrite(userLED2, LOW);
    }
    ~LF_Alpha_resPot() {}

    bool begin();
    int16_t readAnalog();
    int8_t readVoltage();
    uint8_t readPercentage();
    uint8_t getAdcResolution();
    void setAdcResolution(uint8_t res);
    bool end();
};

// rgbLEDs Class implimetation here (WS2812B)
class LF_Alpha_rgbLEDs
{
protected:

#if defined(ESP32)
#if defined(ESP_IDF_VERSION) && ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5, 0, 0)
    rmt_data_t *rmtItems = nullptr; // RMT items buffer
#else
    rmt_item32_t *rmtItems = nullptr; // RMT items buffer
    rmt_channel_t rmtChannel = RMT_CHANNEL_0; // Default RMT channel
#endif
#endif

#ifdef NEO_KHZ400  // If 400 KHz NeoPixel support enabled...
    bool is800KHz; ///< true if 800 KHz pixels
#endif

    bool begun;         ///< true if begin() previously called successfully
    uint16_t numLEDs;   ///< Number of RGB LEDs in strip
    uint16_t numBytes;  ///< Size of 'pixels' buffer below
    int16_t pin;        ///< Output pin number (-1 if not yet set)
    uint8_t brightness; ///< Strip brightness 0-255 (stored as +1)
    uint8_t *pixels;    ///< Holds LED color values (3 or 4 bytes each)
    uint8_t rOffset;    ///< Red index within each 3- or 4-byte pixel
    uint8_t gOffset;    ///< Index of green byte
    uint8_t bOffset;    ///< Index of blue byte
    uint8_t wOffset;    ///< Index of white (==rOffset if no white)
    uint32_t endTime;   ///< Latch timing reference
#if defined(ESP32)
    void espShow();  // Internal RMT sender
    void espInit();     
#endif

public:
    LF_Alpha_rgbLEDs(uint16_t n = 5, int16_t pin = rgbLEDsPin, neoPixelType type = NEO_GRB + NEO_KHZ800);
    ~LF_Alpha_rgbLEDs() {}

    bool begin(void);
    void setPin(int16_t p);
    void show(void);
    void write(uint16_t n, uint8_t r, uint8_t g, uint8_t b);
    void write(uint16_t n, uint8_t r, uint8_t g, uint8_t b, uint8_t w);
    void write(uint16_t n, uint32_t c);
    void setBrightness(uint8_t);
    void clear(void);
    void updateLength(uint16_t n);
    void updateType(neoPixelType t);
    uint16_t numPixels(void) const { return numLEDs; }
    uint32_t getPixelColor(uint16_t n) const;
    void rainbow(uint16_t first_hue = 0, int8_t reps = 1, uint8_t saturation = 255, uint8_t brightness = 255, bool gammify = true);
    void blink(uint8_t noOfLED, uint8_t re, uint8_t gr, uint8_t bl, uint16_t duration = 1000);
    bool end(void);

    static uint32_t ColorHSV(uint16_t hue, uint8_t sat = 255, uint8_t val = 255);
    static uint32_t gamma32(uint32_t x);
    bool canShow(void);

};

// LF_Alpha_Relays class implementation here..
class LF_Alpha_Relays
{
public :
    LF_Alpha_Relays() {
        pinMode(userLED2, OUTPUT);
        digitalWrite(userLED2, LOW);
    }
    ~LF_Alpha_Relays() {}

    bool begin(uint8_t pin);
    bool begin(uint8_t pin1, uint8_t pin2);
    void writeRLY1(bool state);
    void writeRLY2(bool state);
    void write(int relay, bool state);
    bool end();
};

// ESP32 Servo Motor Control Class (MG90)
class LF_Alpha_Servo
{
public:
    LF_Alpha_Servo();
    ~LF_Alpha_Servo(){}

    bool begin(uint servoGPIO = servoPin);
    void write(int value);
    int read();  // returns current angle 0 - 180 degrees
    void sweep(uint32_t duration = 5000);
    bool end();
    void setPeriodHertz(int hertz);

private:
    int attach(int pin);                   // attach the given pin to the next free channel, returns channel number or 0 if failure
    int attach(int pin, int min, int max); // as above but also sets min and max values for writes.
    void detach();

    void writeMicroseconds(int value);
    void writeTicks(int value);
    bool attached();
    int readMicroseconds();

    // ESP32 only functions
    void setTimerWidth(int value); // set the PWM timer width (ESP32 ONLY)
    int readTimerWidth();          // get the PWM timer width (ESP32 ONLY)

    int usToTicks(int usec);
    int ticksToUs(int ticks);
    int min = DEFAULT_uS_LOW;                          // minimum pulse width for this servo
    int max = DEFAULT_uS_HIGH;                         // maximum pulse width for this servo
    int pinNumber = 0;                                 // GPIO pin assigned to this channel
    int timer_width = DEFAULT_TIMER_WIDTH;             // ESP32 allows variable width PWM timers
    int ticks = DEFAULT_PULSE_WIDTH_TICKS;             // current pulse width on this channel
    int timer_width_ticks = DEFAULT_TIMER_WIDTH_TICKS; // no. of ticks at rollover; varies with width
    ESP32PWM *getPwm();                                // get the PWM object
    ESP32PWM pwm;
    int REFRESH_CPS = 50;
    int pwmChannel = -1;
};

// LF_Alpha_RTC class implementation here..
class DateTime
{
public:
    DateTime(int16_t year, int8_t month, int8_t day, int8_t hour, int8_t min, int8_t sec);
    DateTime(); // Default constructor

    int16_t year() const { return _tm.tm_year + 1900; }
    int8_t month() const { return _tm.tm_mon + 1; }
    int8_t day() const { return _tm.tm_mday; }
    int8_t hour() const { return _tm.tm_hour; }
    int8_t minute() const { return _tm.tm_min; }
    int8_t second() const { return _tm.tm_sec; }

private:
    struct tm _tm;
};

class LF_Alpha_RTC
{
private:
    int _i2cAddr = 0x68;
    TwoWire &_Wire = Wire;
    uint8_t decToBcd(uint8_t val);
    uint8_t bcdToDec(uint8_t val);
    byte readControlByte(bool which);
    void writeControlByte(byte control, bool which);

    // This function retrieves current values of the registers.
    byte getRegisterValue()
    {
        _Wire.requestFrom(_i2cAddr, 1);
        return bcdToDec(_Wire.read());
    }

    void setTime12(uint8_t hour, uint8_t min, uint8_t sec, bool isPM);

    // Set clock mode: true = 12h, false = 24h
    void setClockMode(bool h12);

    // Get clock mode: true = 12h, false = 24h
    bool getClockMode();

public:
    LF_Alpha_RTC();
    ~LF_Alpha_RTC() {}

    // Get time and date
    DateTime now();

    bool begin(uint clockAddress = 0x68);
    void setDateTime(String date = __DATE__, String time = __TIME__);
    void setDateTime(int16_t year, int8_t month, int8_t day, int8_t hour, int8_t min, int8_t sec);
    String readTime();
    String readDate();
    uint8_t getDayNumber(void);
    String getDayName(void);
    String getMonthName(void);
    uint8_t getMonthNumber();
    uint16_t getYear(void);
    bool isLeapYear(int16_t year);
    /**
     * @brief Check if the RTC oscillator is running
     * @return true if oscillator is running, false if stopped
     */
    bool isRunning(void);
    bool end();
};

// LF_Alpha_AHT20 class implimented here..
class LF_Alpha_AHT20
{
private:
    uint8_t _deviceAddress;

    enum REG
    {
        DEFAULT_ADDRESS = 0x38,
        CMD_CALIBRATE = 0xE1,  ///< Calibration command
        CMD_INITIALIZE = 0xBE, ///< Calibration command
        CMD_TRIGGER = 0xAC,    ///< Trigger reading command
        CMD_SOFTRESET = 0xBA   ///< Soft reset command
    };

    enum bits
    {
        BIT_CALIBRATED = 3,
        BIT_BUSY = 7
    };

    struct
    {
        uint32_t humidity;
        uint32_t temperature;
        uint8_t crc;
    } _sensorData;

    struct
    {
        bool temperature : true;
        bool humidity : true;
    } _sensorQueried;

    bool getStatus(uint8_t bit); // Returns the status byte
    bool initialize();           // Initialize for taking measurement
    bool triggerMeasurement();   // Trigger the AHT20 to take a measurement
    void readData();             // Read and parse the 6 bytes of data into raw humidity and temp
    bool softReset();            // Restart the sensor system without turning power off and on

public:
    LF_Alpha_AHT20(){
        _deviceAddress = DEFAULT_ADDRESS;
        if(i2cInitFlag == false) {
            i2cInitFlag = true;
            Wire.begin(LF_Alpha_SDA, LF_Alpha_SCL);     // Initialize I2C for all instances
        }
        pinMode(userLED2, OUTPUT);
        digitalWrite(userLED2, LOW);
    }
    ~LF_Alpha_AHT20(){
        end();
    }

    bool begin(const uint8_t deviceAddress = DEFAULT_ADDRESS);
    float readTempC(); //  returns temperature in degrees celcius
    float readTempF() { return readTempC() * 1.8 + 32; }
    float readHumi();   //  returns humidity in % RH
    bool end();
};

// LF_Alpha_OLED class implimented here..
// OLED Display Class from AdafruitSSD1306 library
// Inherits the Adafruit_SSD1306 class with private access specifier
class LF_Alpha_OLED : public Adafruit_SSD1306
{
public:
    LF_Alpha_OLED(int WIDTH = 128, int HEIGHT = 64) : Adafruit_SSD1306(WIDTH, HEIGHT, &Wire, -1) {
        if(i2cInitFlag == false) {
            i2cInitFlag = true;
            Wire.begin(LF_Alpha_SDA, LF_Alpha_SCL);     // Initialize I2C for all instances
        }
        pinMode(userLED2, OUTPUT);
        digitalWrite(userLED2, LOW);
    }
    bool begin(uint8_t I2C_Address = 0x3C)
    {
        if(i2cInitFlag == false) {
            i2cInitFlag = true;
            Wire.begin(LF_Alpha_SDA, LF_Alpha_SCL);     // Initialize I2C for all instances
        }
        return Adafruit_SSD1306::begin(SSD1306_SWITCHCAPVCC, I2C_Address);
    }
    using Adafruit_SSD1306::clearDisplay;
    using Adafruit_SSD1306::display;
    using Adafruit_SSD1306::drawCircle;
    using Adafruit_SSD1306::drawLine;
    using Adafruit_SSD1306::drawPixel;
    using Adafruit_SSD1306::drawRect;
    using Adafruit_SSD1306::drawRoundRect;
    using Adafruit_SSD1306::drawTriangle;
    using Adafruit_SSD1306::fillCircle;
    using Adafruit_SSD1306::fillRect;
    using Adafruit_SSD1306::fillRoundRect;
    using Adafruit_SSD1306::fillTriangle;
    using Adafruit_SSD1306::print;
    using Adafruit_SSD1306::println;
    using Adafruit_SSD1306::setCursor;
    using Adafruit_SSD1306::setTextColor;
    using Adafruit_SSD1306::setTextSize;
    using Adafruit_SSD1306::setFont;

    bool end()
    {
        clearDisplay();
        if (buffer)
        {
            free(buffer);
            buffer = nullptr;
        }
        display();
        return true;
    }
};

// LF_Alpha_MPU6050 class implimented here..
class LF_Alpha_MPU6050
{
public:
    LF_Alpha_MPU6050(uint8_t address = MPU6050_DEFAULT_ADDRESS, void *wireObj = &Wire);
    ~LF_Alpha_MPU6050() {}

    bool begin();
    void calibrateSensor();
    void deepCalibrateSensor();
    int16_t readAccX();
    int16_t readAccY();
    int16_t readAccZ();
    int16_t readGyroX();
    int16_t readGyroY();
    int16_t readGyroZ();
    bool getMotionState();
    int16_t getCalibratedThreshold(){
        return gyroOffsetValue;
    }
    void setIdleThresholdGyro(int16_t threshold = 250);
    bool end();

private:
    // private variable
    int16_t idleThresholdGyro = 250;
    int16_t offsets[6];
    int16_t ax, ay, az, gx, gy, gz;
    uint8_t devAddr;
    void *wireObj;
    uint8_t buffer[14];
    uint32_t fifoTimeout = MPU6050_FIFO_DEFAULT_TIMEOUT;

    float accelerationResolution;
    float gyroscopeResolution;

    // private methods
    void initialize();
    void initialize(ACCEL_FS accelRange, GYRO_FS gyroRange);

    float get_acce_resolution();
    float get_gyro_resolution();

    bool testConnection();

    // AUX_VDDIO register
    uint8_t getAuxVDDIOLevel();
    void setAuxVDDIOLevel(uint8_t level);

    // SMPLRT_DIV register
    uint8_t getRate();
    void setRate(uint8_t rate);

    // CONFIG register
    uint8_t getExternalFrameSync();
    void setExternalFrameSync(uint8_t sync);
    uint8_t getDLPFMode();
    void setDLPFMode(uint8_t bandwidth);

    // GYRO_CONFIG register
    uint8_t getFullScaleGyroRange();
    void setFullScaleGyroRange(uint8_t range);

    // SELF_TEST registers
    uint8_t getAccelXSelfTestFactoryTrim();
    uint8_t getAccelYSelfTestFactoryTrim();
    uint8_t getAccelZSelfTestFactoryTrim();

    uint8_t getGyroXSelfTestFactoryTrim();
    uint8_t getGyroYSelfTestFactoryTrim();
    uint8_t getGyroZSelfTestFactoryTrim();

    // ACCEL_CONFIG register
    bool getAccelXSelfTest();
    void setAccelXSelfTest(bool enabled);
    bool getAccelYSelfTest();
    void setAccelYSelfTest(bool enabled);
    bool getAccelZSelfTest();
    void setAccelZSelfTest(bool enabled);
    uint8_t getFullScaleAccelRange();
    void setFullScaleAccelRange(uint8_t range);
    uint8_t getDHPFMode();
    void setDHPFMode(uint8_t mode);

    // FF_THR register
    uint8_t getFreefallDetectionThreshold();
    void setFreefallDetectionThreshold(uint8_t threshold);

    // FF_DUR register
    uint8_t getFreefallDetectionDuration();
    void setFreefallDetectionDuration(uint8_t duration);

    // MOT_THR register
    uint8_t getMotionDetectionThreshold();
    void setMotionDetectionThreshold(uint8_t threshold);

    // MOT_DUR register
    uint8_t getMotionDetectionDuration();
    void setMotionDetectionDuration(uint8_t duration);

    // ZRMOT_THR register
    uint8_t getZeroMotionDetectionThreshold();
    void setZeroMotionDetectionThreshold(uint8_t threshold);

    // ZRMOT_DUR register
    uint8_t getZeroMotionDetectionDuration();
    void setZeroMotionDetectionDuration(uint8_t duration);

    // FIFO_EN register
    bool getTempFIFOEnabled();
    void setTempFIFOEnabled(bool enabled);
    bool getXGyroFIFOEnabled();
    void setXGyroFIFOEnabled(bool enabled);
    bool getYGyroFIFOEnabled();
    void setYGyroFIFOEnabled(bool enabled);
    bool getZGyroFIFOEnabled();
    void setZGyroFIFOEnabled(bool enabled);
    bool getAccelFIFOEnabled();
    void setAccelFIFOEnabled(bool enabled);
    bool getSlave2FIFOEnabled();
    void setSlave2FIFOEnabled(bool enabled);
    bool getSlave1FIFOEnabled();
    void setSlave1FIFOEnabled(bool enabled);
    bool getSlave0FIFOEnabled();
    void setSlave0FIFOEnabled(bool enabled);

    // I2C_MST_CTRL register
    bool getMultiMasterEnabled();
    void setMultiMasterEnabled(bool enabled);
    bool getWaitForExternalSensorEnabled();
    void setWaitForExternalSensorEnabled(bool enabled);
    bool getSlave3FIFOEnabled();
    void setSlave3FIFOEnabled(bool enabled);
    bool getSlaveReadWriteTransitionEnabled();
    void setSlaveReadWriteTransitionEnabled(bool enabled);
    uint8_t getMasterClockSpeed();
    void setMasterClockSpeed(uint8_t speed);

    // I2C_SLV* registers (Slave 0-3)
    uint8_t getSlaveAddress(uint8_t num);
    void setSlaveAddress(uint8_t num, uint8_t address);
    uint8_t getSlaveRegister(uint8_t num);
    void setSlaveRegister(uint8_t num, uint8_t reg);
    bool getSlaveEnabled(uint8_t num);
    void setSlaveEnabled(uint8_t num, bool enabled);
    bool getSlaveWordByteSwap(uint8_t num);
    void setSlaveWordByteSwap(uint8_t num, bool enabled);
    bool getSlaveWriteMode(uint8_t num);
    void setSlaveWriteMode(uint8_t num, bool mode);
    bool getSlaveWordGroupOffset(uint8_t num);
    void setSlaveWordGroupOffset(uint8_t num, bool enabled);
    uint8_t getSlaveDataLength(uint8_t num);
    void setSlaveDataLength(uint8_t num, uint8_t length);

    // I2C_SLV* registers (Slave 4)
    uint8_t getSlave4Address();
    void setSlave4Address(uint8_t address);
    uint8_t getSlave4Register();
    void setSlave4Register(uint8_t reg);
    void setSlave4OutputByte(uint8_t data);
    bool getSlave4Enabled();
    void setSlave4Enabled(bool enabled);
    bool getSlave4InterruptEnabled();
    void setSlave4InterruptEnabled(bool enabled);
    bool getSlave4WriteMode();
    void setSlave4WriteMode(bool mode);
    uint8_t getSlave4MasterDelay();
    void setSlave4MasterDelay(uint8_t delay);
    uint8_t getSlate4InputByte();

    // I2C_MST_STATUS register
    bool getPassthroughStatus();
    bool getSlave4IsDone();
    bool getLostArbitration();
    bool getSlave4Nack();
    bool getSlave3Nack();
    bool getSlave2Nack();
    bool getSlave1Nack();
    bool getSlave0Nack();

    // INT_PIN_CFG register
    bool getInterruptMode();
    void setInterruptMode(bool mode);
    bool getInterruptDrive();
    void setInterruptDrive(bool drive);
    bool getInterruptLatch();
    void setInterruptLatch(bool latch);
    bool getInterruptLatchClear();
    void setInterruptLatchClear(bool clear);
    bool getFSyncInterruptLevel();
    void setFSyncInterruptLevel(bool level);
    bool getFSyncInterruptEnabled();
    void setFSyncInterruptEnabled(bool enabled);
    bool getI2CBypassEnabled();
    void setI2CBypassEnabled(bool enabled);
    bool getClockOutputEnabled();
    void setClockOutputEnabled(bool enabled);

    // INT_ENABLE register
    uint8_t getIntEnabled();
    void setIntEnabled(uint8_t enabled);
    bool getIntFreefallEnabled();
    void setIntFreefallEnabled(bool enabled);
    bool getIntMotionEnabled();
    void setIntMotionEnabled(bool enabled);
    bool getIntZeroMotionEnabled();
    void setIntZeroMotionEnabled(bool enabled);
    bool getIntFIFOBufferOverflowEnabled();
    void setIntFIFOBufferOverflowEnabled(bool enabled);
    bool getIntI2CMasterEnabled();
    void setIntI2CMasterEnabled(bool enabled);
    bool getIntDataReadyEnabled();
    void setIntDataReadyEnabled(bool enabled);

    // INT_STATUS register
    uint8_t getIntStatus();
    bool getIntFreefallStatus();
    bool getIntMotionStatus();
    bool getIntZeroMotionStatus();
    bool getIntFIFOBufferOverflowStatus();
    bool getIntI2CMasterStatus();
    bool getIntDataReadyStatus();

    // ACCEL_*OUT_* registers
    void getMotion9(int16_t *ax, int16_t *ay, int16_t *az, int16_t *gx, int16_t *gy, int16_t *gz, int16_t *mx, int16_t *my, int16_t *mz);
    void getMotion6(int16_t *ax, int16_t *ay, int16_t *az, int16_t *gx, int16_t *gy, int16_t *gz);
    void getAcceleration(int16_t *x, int16_t *y, int16_t *z);
    int16_t getAccelerationX();
    int16_t getAccelerationY();
    int16_t getAccelerationZ();

    // TEMP_OUT_* registers
    int16_t getTemperature();

    // GYRO_*OUT_* registers
    void getRotation(int16_t *x, int16_t *y, int16_t *z);
    int16_t getRotationX();
    int16_t getRotationY();
    int16_t getRotationZ();

    // EXT_SENS_DATA_* registers
    uint8_t getExternalSensorByte(int position);
    uint16_t getExternalSensorWord(int position);
    uint32_t getExternalSensorDWord(int position);

    // MOT_DETECT_STATUS register
    uint8_t getMotionStatus();
    bool getXNegMotionDetected();
    bool getXPosMotionDetected();
    bool getYNegMotionDetected();
    bool getYPosMotionDetected();
    bool getZNegMotionDetected();
    bool getZPosMotionDetected();
    bool getZeroMotionDetected();

    // I2C_SLV*_DO register
    void setSlaveOutputByte(uint8_t num, uint8_t data);

    // I2C_MST_DELAY_CTRL register
    bool getExternalShadowDelayEnabled();
    void setExternalShadowDelayEnabled(bool enabled);
    bool getSlaveDelayEnabled(uint8_t num);
    void setSlaveDelayEnabled(uint8_t num, bool enabled);

    // SIGNAL_PATH_RESET register
    void resetGyroscopePath();
    void resetAccelerometerPath();
    void resetTemperaturePath();

    // MOT_DETECT_CTRL register
    uint8_t getAccelerometerPowerOnDelay();
    void setAccelerometerPowerOnDelay(uint8_t delay);
    uint8_t getFreefallDetectionCounterDecrement();
    void setFreefallDetectionCounterDecrement(uint8_t decrement);
    uint8_t getMotionDetectionCounterDecrement();
    void setMotionDetectionCounterDecrement(uint8_t decrement);

    // USER_CTRL register
    bool getFIFOEnabled();
    void setFIFOEnabled(bool enabled);
    bool getI2CMasterModeEnabled();
    void setI2CMasterModeEnabled(bool enabled);
    void switchSPIEnabled(bool enabled);
    void resetFIFO();
    void resetI2CMaster();
    void resetSensors();

    // PWR_MGMT_1 register
    void reset();
    bool getSleepEnabled();
    void setSleepEnabled(bool enabled);
    bool getWakeCycleEnabled();
    void setWakeCycleEnabled(bool enabled);
    bool getTempSensorEnabled();
    void setTempSensorEnabled(bool enabled);
    uint8_t getClockSource();
    void setClockSource(uint8_t source);

    // PWR_MGMT_2 register
    uint8_t getWakeFrequency();
    void setWakeFrequency(uint8_t frequency);
    bool getStandbyXAccelEnabled();
    void setStandbyXAccelEnabled(bool enabled);
    bool getStandbyYAccelEnabled();
    void setStandbyYAccelEnabled(bool enabled);
    bool getStandbyZAccelEnabled();
    void setStandbyZAccelEnabled(bool enabled);
    bool getStandbyXGyroEnabled();
    void setStandbyXGyroEnabled(bool enabled);
    bool getStandbyYGyroEnabled();
    void setStandbyYGyroEnabled(bool enabled);
    bool getStandbyZGyroEnabled();
    void setStandbyZGyroEnabled(bool enabled);

    // FIFO_COUNT_* registers
    uint16_t getFIFOCount();

    // FIFO_R_W register
    uint8_t getFIFOByte();
    int8_t GetCurrentFIFOPacket(uint8_t *data, uint8_t length);
    void setFIFOByte(uint8_t data);
    void getFIFOBytes(uint8_t *data, uint8_t length);
    void setFIFOTimeout(uint32_t fifoTimeout);
    uint32_t getFIFOTimeout();

    // WHO_AM_I register
    uint8_t getDeviceID();
    void setDeviceID(uint8_t id);

    // ======== UNDOCUMENTED/DMP REGISTERS/METHODS ========

    // XG_OFFS_TC register
    uint8_t getOTPBankValid();
    void setOTPBankValid(bool enabled);
    int8_t getXGyroOffsetTC();
    void setXGyroOffsetTC(int8_t offset);

    // YG_OFFS_TC register
    int8_t getYGyroOffsetTC();
    void setYGyroOffsetTC(int8_t offset);

    // ZG_OFFS_TC register
    int8_t getZGyroOffsetTC();
    void setZGyroOffsetTC(int8_t offset);

    // X_FINE_GAIN register
    int8_t getXFineGain();
    void setXFineGain(int8_t gain);

    // Y_FINE_GAIN register
    int8_t getYFineGain();
    void setYFineGain(int8_t gain);

    // Z_FINE_GAIN register
    int8_t getZFineGain();
    void setZFineGain(int8_t gain);

    // XA_OFFS_* registers
    int16_t getXAccelOffset();
    void setXAccelOffset(int16_t offset);

    // YA_OFFS_* register
    int16_t getYAccelOffset();
    void setYAccelOffset(int16_t offset);

    // ZA_OFFS_* register
    int16_t getZAccelOffset();
    void setZAccelOffset(int16_t offset);

    // XG_OFFS_USR* registers
    int16_t getXGyroOffset();
    void setXGyroOffset(int16_t offset);

    // YG_OFFS_USR* register
    int16_t getYGyroOffset();
    void setYGyroOffset(int16_t offset);

    // ZG_OFFS_USR* register
    int16_t getZGyroOffset();
    void setZGyroOffset(int16_t offset);

    // INT_ENABLE register (DMP functions)
    bool getIntPLLReadyEnabled();
    void setIntPLLReadyEnabled(bool enabled);
    bool getIntDMPEnabled();
    void setIntDMPEnabled(bool enabled);

    // DMP_INT_STATUS
    bool getDMPInt5Status();
    bool getDMPInt4Status();
    bool getDMPInt3Status();
    bool getDMPInt2Status();
    bool getDMPInt1Status();
    bool getDMPInt0Status();

    // INT_STATUS register (DMP functions)
    bool getIntPLLReadyStatus();
    bool getIntDMPStatus();

    // USER_CTRL register (DMP functions)
    bool getDMPEnabled();
    void setDMPEnabled(bool enabled);
    void resetDMP();

    // BANK_SEL register
    void setMemoryBank(uint8_t bank, bool prefetchEnabled = false, bool userBank = false);

    // MEM_START_ADDR register
    void setMemoryStartAddress(uint8_t address);

    // MEM_R_W register
    uint8_t readMemoryByte();
    void writeMemoryByte(uint8_t data);
    void readMemoryBlock(uint8_t *data, uint16_t dataSize, uint8_t bank = 0, uint8_t address = 0);
    bool writeMemoryBlock(const uint8_t *data, uint16_t dataSize, uint8_t bank = 0, uint8_t address = 0, bool verify = true, bool useProgMem = false);
    bool writeProgMemoryBlock(const uint8_t *data, uint16_t dataSize, uint8_t bank = 0, uint8_t address = 0, bool verify = true);

    bool writeDMPConfigurationSet(const uint8_t *data, uint16_t dataSize, bool useProgMem = false);
    bool writeProgDMPConfigurationSet(const uint8_t *data, uint16_t dataSize);

    // DMP_CFG_1 register
    uint8_t getDMPConfig1();
    void setDMPConfig1(uint8_t config);

    // DMP_CFG_2 register
    uint8_t getDMPConfig2();
    void setDMPConfig2(uint8_t config);

    // Calibration Routines
    void CalibrateGyro(uint8_t Loops = 15);                           // Fine tune after setting offsets with less Loops.
    void CalibrateAccel(uint8_t Loops = 15);                          // Fine tune after setting offsets with less Loops.
    void PID(uint8_t ReadAddress, float kP, float kI, uint8_t Loops); // Does the math
    void PrintActiveOffsets();                                        // See the results of the Calibration
    int16_t *GetActiveOffsets();

    // some supporting functions and variables
    int gyroOffsetValue = 0;
    int LowValue[6];
    int HighValue[6];
    int Smoothed[6];
    int LowOffset[6];
    int HighOffset[6];
    int Target[6];
    int LinesOut;
    int N;
    int i;

    void PullBracketsOut();
    void PullBracketsIn();
    void ForceHeader();
    void GetSmoothed();
    void SetOffsets(int TheOffsets[6]);
};

// LF_Alpha_APDS9960 class implimented here..
class LF_Alpha_APDS9960
{
private:
    Adafruit_I2CDevice *i2c_dev = NULL;
    uint32_t read32(uint8_t reg);
    uint16_t read16(uint8_t reg);
    uint16_t read16R(uint8_t reg);

    void write8(byte reg, byte value);
    uint8_t read8(byte reg);

    uint8_t gestCnt;

    uint8_t UCount;
    uint8_t DCount;

    uint8_t LCount;
    uint8_t RCount;

    uint8_t read(uint8_t reg, uint8_t *buf, uint8_t num);
    void write(uint8_t reg, uint8_t *buf, uint8_t num);

    struct enable
        {

            // power on
            uint8_t PON : 1;

            // ALS enable
            uint8_t AEN : 1;

            // Proximity detect enable
            uint8_t PEN : 1;

            // wait timer enable
            uint8_t WEN : 1;

            // ALS interrupt enable
            uint8_t AIEN : 1;

            // proximity interrupt enable
            uint8_t PIEN : 1;

            // gesture enable
            uint8_t GEN : 1;

            uint8_t get()
            {
                return (GEN << 6) | (PIEN << 5) | (AIEN << 4) | (WEN << 3) | (PEN << 2) |
                       (AEN << 1) | PON;
            };
        };
    struct enable _enable;

    struct pers
        {
            // ALS Interrupt Persistence. Controls rate of Clear channel interrupt to
            // the host processor
            uint8_t APERS : 4;

            // proximity interrupt persistence, controls rate of prox interrupt to host
            // processor
            uint8_t PPERS : 4;

            uint8_t get() { return (PPERS << 4) | APERS; };
        };
    pers _pers;

    struct config1
        {
            uint8_t WLONG : 1;

            uint8_t get() { return WLONG << 1; };
        };
    config1 _config1;

    struct ppulse
        {

            /*Proximity Pulse Count. Specifies the number of proximity pulses to be
            generated on LDR. Number of pulses is set by PPULSE value plus 1.
            */
            uint8_t PPULSE : 6;

            // Proximity Pulse Length. Sets the LED-ON pulse width during a proximity
            // LDR pulse.
            uint8_t PPLEN : 2;

            uint8_t get() { return (PPLEN << 6) | PPULSE; }
        };
    ppulse _ppulse;

    struct control
        {
            // ALS and Color gain control
            uint8_t AGAIN : 2;

            // proximity gain control
            uint8_t PGAIN : 2;

            // led drive strength
            uint8_t LDRIVE : 2;

            uint8_t get() { return (LDRIVE << 6) | (PGAIN << 2) | AGAIN; }
        };
    control _control;

    struct config2
        {
            /* Additional LDR current during proximity and gesture LED pulses. Current
            value, set by LDRIVE, is increased by the percentage of LED_BOOST.
            */
            uint8_t LED_BOOST : 2;

            // clear photodiode saturation int enable
            uint8_t CPSIEN : 1;

            // proximity saturation interrupt enable
            uint8_t PSIEN : 1;

            uint8_t get()
            {
                return (PSIEN << 7) | (CPSIEN << 6) | (LED_BOOST << 4) | 1;
            }
        };
    config2 _config2;

    struct status
        {
            /* ALS Valid. Indicates that an ALS cycle has completed since AEN was
            asserted or since a read from any of the ALS/Color data registers.
            */
            uint8_t AVALID : 1;

            /* Proximity Valid. Indicates that a proximity cycle has completed since PEN
            was asserted or since PDATA was last read. A read of PDATA automatically
            clears PVALID.
            */
            uint8_t PVALID : 1;

            /* Gesture Interrupt. GINT is asserted when GFVLV becomes greater than
            GFIFOTH or if GVALID has become asserted when GMODE transitioned to zero.
            The bit is reset when FIFO is completely emptied (read).
            */
            uint8_t GINT : 1;

            // ALS Interrupt. This bit triggers an interrupt if AIEN in ENABLE is set.
            uint8_t AINT : 1;

            // Proximity Interrupt. This bit triggers an interrupt if PIEN in ENABLE is
            // set.
            uint8_t PINT : 1;

            /* Indicates that an analog saturation event occurred during a previous
            proximity or gesture cycle. Once set, this bit remains set until cleared by
            clear proximity interrupt special function command (0xE5 PICLEAR) or by
            disabling Prox (PEN=0). This bit triggers an interrupt if PSIEN is set.
            */
            uint8_t PGSAT : 1;

            /* Clear Photodiode Saturation. When asserted, the analog sensor was at the
            upper end of its dynamic range. The bit can be de-asserted by sending a
            Clear channel interrupt command (0xE6 CICLEAR) or by disabling the ADC
            (AEN=0). This bit triggers an interrupt if CPSIEN is set.
            */
            uint8_t CPSAT : 1;

            void set(uint8_t data)
            {
                AVALID = data & 0x01;
                PVALID = (data >> 1) & 0x01;
                GINT = (data >> 2) & 0x01;
                AINT = (data >> 4) & 0x01;
                PINT = (data >> 5) & 0x01;
                PGSAT = (data >> 6) & 0x01;
                CPSAT = (data >> 7) & 0x01;
            }
        };
    status _status;

    struct config3
        {
            // proximity mask
            uint8_t PMASK_R : 1;
            uint8_t PMASK_L : 1;
            uint8_t PMASK_D : 1;
            uint8_t PMASK_U : 1;

            /* Sleep After Interrupt. When enabled, the device will automatically enter
            low power mode when the INT pin is asserted and the state machine has
            progressed to the SAI decision block. Normal operation is resumed when INT
            pin is cleared over I2C.
            */
            uint8_t SAI : 1;

            /* Proximity Gain Compensation Enable. This bit provides gain compensation
            when proximity photodiode signal is reduced as a result of sensor masking.
            If only one diode of the diode pair is contributing, then only half of the
            signal is available at the ADC; this results in a maximum ADC value of 127.
            Enabling PCMP enables an additional gain of 2X, resulting in a maximum ADC
            value of 255.
            */
            uint8_t PCMP : 1;

            uint8_t get()
            {
                return (PCMP << 5) | (SAI << 4) | (PMASK_U << 3) | (PMASK_D << 2) |
                       (PMASK_L << 1) | PMASK_R;
            }
        };
    config3 _config3;

    struct gconf1
        {
            /* Gesture Exit Persistence. When a number of consecutive gesture end
            occurrences become equal or greater to the GEPERS value, the Gesture state
            machine is exited.
            */
            uint8_t GEXPERS : 2;

            /* Gesture Exit Mask. Controls which of the gesture detector photodiodes
            (UDLR) will be included to determine a gesture end and subsequent exit
            of the gesture state machine. Unmasked UDLR data will be compared with the
            value in GTHR_OUT. Field value bits correspond to UDLR detectors.
            */
            uint8_t GEXMSK : 4;

            /* Gesture FIFO Threshold. This value is compared with the FIFO Level (i.e.
            the number of UDLR datasets) to generate an interrupt (if enabled).
            */
            uint8_t GFIFOTH : 2;

            uint8_t get() { return (GFIFOTH << 6) | (GEXMSK << 2) | GEXPERS; }
        };
    gconf1 _gconf1;

    struct gconf2
        {
            /* Gesture Wait Time. The GWTIME controls the amount of time in a low power
            mode between gesture detection cycles.
            */
            uint8_t GWTIME : 3;

            // Gesture LED Drive Strength. Sets LED Drive Strength in gesture mode.
            uint8_t GLDRIVE : 2;

            // Gesture Gain Control. Sets the gain of the proximity receiver in gesture
            // mode.
            uint8_t GGAIN : 2;

            uint8_t get() { return (GGAIN << 5) | (GLDRIVE << 3) | GWTIME; }
        };
    gconf2 _gconf2;

    struct gpulse
        {
            /* Number of Gesture Pulses. Specifies the number of pulses to be generated
            on LDR. Number of pulses is set by GPULSE value plus 1.
            */
            uint8_t GPULSE : 6;

            // Gesture Pulse Length. Sets the LED_ON pulse width during a Gesture LDR
            // Pulse.
            uint8_t GPLEN : 2;

            uint8_t get() { return (GPLEN << 6) | GPULSE; }
        };
    gpulse _gpulse;

    struct gconf3
        {
            /* Gesture Dimension Select. Selects which gesture photodiode pairs are
            enabled to gather results during gesture.
            */
            uint8_t GDIMS : 2;

            uint8_t get() { return GDIMS; }
        };
    gconf3 _gconf3;

    struct gconf4
        {
            /* Gesture Mode. Reading this bit reports if the gesture state machine is
            actively running, 1 = Gesture, 0= ALS, Proximity, Color. Writing a 1 to this
            bit causes immediate entry in to the gesture state machine (as if GPENTH had
            been exceeded). Writing a 0 to this bit causes exit of gesture when current
            analog conversion has finished (as if GEXTH had been exceeded).
            */
            uint8_t GMODE : 1;

            /* Gesture interrupt enable. Gesture Interrupt Enable. When asserted, all
            gesture related interrupts are unmasked.
            */
            uint8_t GIEN : 2;

            uint8_t get() { return (GIEN << 1) | GMODE; }
            void set(uint8_t data)
            {
                GIEN = (data >> 1) & 0x01;
                GMODE = data & 0x01;
            }
        };
    gconf4 _gconf4;

    struct gstatus
        {
            /* Gesture FIFO Data. GVALID bit is sent when GFLVL becomes greater than
            GFIFOTH (i.e. FIFO has enough data to set GINT). GFIFOD is reset when GMODE
            = 0 and the GFLVL=0 (i.e. All FIFO data has been read).
            */
            uint8_t GVALID : 1;

            /* Gesture FIFO Overflow. A setting of 1 indicates that the FIFO has filled
            to capacity and that new gesture detector data has been lost.
            */
            uint8_t GFOV : 1;

            void set(uint8_t data)
            {
                GFOV = (data >> 1) & 0x01;
                GVALID = data & 0x01;
            }
        };
    gstatus _gstatus;

public:
    LF_Alpha_APDS9960() {
        if(i2cInitFlag == false) {
            i2cInitFlag = true;
            Wire.begin(LF_Alpha_SDA, LF_Alpha_SCL);     // Initialize I2C for all instances
        }
        pinMode(userLED2, OUTPUT);
        digitalWrite(userLED2, LOW);
    }
    ~LF_Alpha_APDS9960() {}

    bool begin(uint16_t iTimeMS = 10, apds9960AGain_t = APDS9960_AGAIN_4X,
               uint8_t addr = APDS9960_ADDRESS, TwoWire *theWire = &Wire);

    // configuration methods
    void setADCIntegrationTime(uint16_t iTimeMS);
    float getADCIntegrationTime();
    void setADCGain(apds9960AGain_t gain);
    apds9960AGain_t getADCGain();
    void setLED(apds9960LedDrive_t drive, apds9960LedBoost_t boost);

    // proximity
    void enableProximity(boolean en = true);
    void setProxGain(apds9960PGain_t gain);
    apds9960PGain_t getProxGain();
    void setProxPulse(apds9960PPulseLen_t pLen, uint8_t pulses);
    void enableProximityInterrupt();
    void disableProximityInterrupt();
    uint8_t readProximity();
    void setProximityInterruptThreshold(uint8_t low, uint8_t high,
                                        uint8_t persistence = 4);
    bool getProximityInterrupt();

    // gesture
    void enableGesture(boolean en = true);
    bool gestureValid();
    void setGestureDimensions(uint8_t dims);
    void setGestureFIFOThreshold(uint8_t thresh);
    void setGestureGain(uint8_t gain);
    void setGestureProximityThreshold(uint8_t thresh);
    void setGestureOffset(uint8_t offset_up, uint8_t offset_down,
                          uint8_t offset_left, uint8_t offset_right);
    uint8_t readGesture();
    void resetCounts();

    // light & color
    void enableColor(boolean en = true);
    bool colorDataReady();
    void readColors(uint16_t *r, uint16_t *g, uint16_t *b, uint16_t *c);
    uint16_t calculateColorTemperature(uint16_t r, uint16_t g, uint16_t b);
    uint16_t calculateLux(uint16_t r, uint16_t g, uint16_t b);
    void enableColorInterrupt();
    void disableColorInterrupt();
    void clearInterrupt();
    void setIntLimits(uint16_t l, uint16_t h);
    
    // turn on/off elements
    void enable(boolean en = true);

    bool end(void){
        if (i2c_dev)
            delete i2c_dev;
        i2c_dev = 0;
        return true;
    }
};

// LF_Alpha_MAX30102 class implimented here..
class LF_Alpha_MAX30102
{

public:
    /*
          Interrupt Status(0x00â€“0x01) (pg 12)
          * ------------------------------------------------------------------------------------------
          * |    b7    |    b6    |    b5    |    b4    |    b3    |    b2    |    b1     |    b0    |
          * ------------------------------------------------------------------------------------------
          * |  A_FULL  | PPG_RDY  |  ALC_OVF |                    NONE                    | PWR_RDY  |
          * ------------------------------------------------------------------------------------------
          * ------------------------------------------------------------------------------------------
          * |    b7    |    b6    |    b5    |    b4    |    b3    |    b2   |     b1     |    b0    |
          * ------------------------------------------------------------------------------------------
          * |                           NONE                                 |DIE_TEMP_RDY|   NONE   |
          * ------------------------------------------------------------------------------------------
        */
    /*
      Interrupt Enable(0x02â€“0x03) (pg 13)
      * ------------------------------------------------------------------------------------------
      * |    b7    |    b6    |    b5    |    b4    |    b3    |    b2    |    b1     |    b0    |
      * ------------------------------------------------------------------------------------------
      * |A_FULL_EN |PPG_RDY_EN|ALC_OVF_EN|                         NONE                          |
      * ------------------------------------------------------------------------------------------
      * ------------------------------------------------------------------------------------------
      * |    b7    |    b6    |    b5    |    b4    |    b3    |    b2   |       b1      |   b0  |
      * ------------------------------------------------------------------------------------------
      * |                           NONE                                 |DIE_TEMP_RDY_EN|  NONE |
      * ------------------------------------------------------------------------------------------
    */
    typedef struct
    {
        uint8_t NONE : 1;
        uint8_t dieTemp : 1; // Internal Temperature Ready Flag
        uint8_t NONE1 : 6;
        uint8_t NONE2 : 5;
        uint8_t ALCOverflow : 1; // Ambient Light Cancellation Overflow
        uint8_t dataReady : 1;   // New FIFO Data Ready
        uint8_t almostFull : 1;  // FIFO Almost Full Flag
    } __attribute__((packed)) sEnable_t;

    /*
      FIFO Configuration(0x08) (pg 17)
      * ------------------------------------------------------------------------------------------
      * |    b7    |    b6    |    b5    |    b4          | b3 |    b2    |    b1     |    b0    |
      * ------------------------------------------------------------------------------------------
      * |            SMP_AVE             |FIFO_ROLLOVER_EN|               FIFO_A_FULL            |
      * ------------------------------------------------------------------------------------------
    */
    typedef struct
    {
        uint8_t almostFull : 4;   // FIFO Almost Full Value
        uint8_t RollOver : 1;     // FIFO Rolls on Full
        uint8_t sampleAverag : 3; // Sample Averaging
    } __attribute__((packed)) sFIFO_t;

    /*
      Mode configuration(0x09) (pg 18)
      * ------------------------------------------------------------------------------------------
      * |    b7    |    b6    |    b5    |    b4    |    b3    |    b2    |    b1     |    b0    |
      * ------------------------------------------------------------------------------------------
      * |   SHDN   |   RESET  |             NONE               |              MODE               |
      * ------------------------------------------------------------------------------------------
    */
    typedef struct
    {
        uint8_t ledMode : 6;  /*!< 010:Heart Rate mode, Red only. 011:SpO2 mode, Red and IR. 111:Multi-LED mode, Red and IR*/
        uint8_t reset : 1;    /*!< 1:reset */
        uint8_t shutDown : 1; /*!< 0: wake up 1: put IC into low power mode*/
    } __attribute__((packed)) sMode_t;

    /*
      Particle sensing configuration(0x0A) (pg 18)
      * ------------------------------------------------------------------------------------------
      * |    b7    |    b6    |    b5    |    b4    |    b3    |    b2    |    b1     |    b0    |
      * ------------------------------------------------------------------------------------------
      * |   NONE   |     SPO2_ADC_RGE    |             SPO2_SR            |        LED_PW        |
      * ------------------------------------------------------------------------------------------
    */
    typedef struct
    {
        uint8_t pulseWidth : 2;
        uint8_t sampleRate : 3;
        uint8_t adcRange : 3;
    } __attribute__((packed)) sParticle_t;

    /*
      LED Pulse Amplitude(0x0Câ€“0x0D) (pg 20)
      * ------------------------------------------------------------------------------------------
      * |    b7    |    b6    |    b5    |    b4    |    b3    |    b2    |    b1     |    b0    |
      * ------------------------------------------------------------------------------------------
      * |                                         LED1_PA                                        |
      * ------------------------------------------------------------------------------------------
      * ------------------------------------------------------------------------------------------
      * |    b7    |    b6    |    b5    |    b4    |    b3    |    b2    |    b1     |    b0    |
      * ------------------------------------------------------------------------------------------
      * |                                         LED2_PA                                        |
      * ------------------------------------------------------------------------------------------
    */
    /*
      Multi-LED Mode Control Registers(0x011) (pg 21)
      * ------------------------------------------------------------------------------------------
      * |    b7    |    b6    |    b5    |    b4    |    b3    |    b2    |    b1     |    b0    |
      * ------------------------------------------------------------------------------------------
      * |   NONE   |              SLOT2             |   NONE   |             SLOT1               |
      * ------------------------------------------------------------------------------------------
    */
    typedef struct
    {
        uint8_t SLOT1 : 4;
        uint8_t SLOT2 : 4;
    } __attribute__((packed)) sMultiLED_t;

    /*!
     *@brief Circular buffer for storing samples
     */
    typedef struct
    {
        uint32_t red[MAX30102_SENSE_BUF_SIZE];
        uint32_t IR[MAX30102_SENSE_BUF_SIZE];
        uint8_t head;
        uint8_t tail;
    } sSenseBuf_t;

private:
/*----------Configuration Options---------*/
// FIFO Configuration(Register address 0x08)
// sampleAverage(Table 3. Sample Averaging)
#define SAMPLEAVG_1         0
#define SAMPLEAVG_2         1
#define SAMPLEAVG_4         2
#define SAMPLEAVG_8         3
#define SAMPLEAVG_16        4
#define SAMPLEAVG_32        5

// Mode configuration(Register address 0x09)
// ledMode(Table 4. Mode Control)
#define MODE_REDONLY        2
#define MODE_RED_IR         3
#define MODE_MULTILED       7

// Particle sensing configuration(Register address 0x0A)
// adcRange(Table 5. SpO2 ADC Range Control)
#define ADCRANGE_2048       0
#define ADCRANGE_4096       1
#define ADCRANGE_8192       2
#define ADCRANGE_16384      3

// sampleRate(Table 6. SpO2 Sample Rate Control)
#define SAMPLERATE_50       0
#define SAMPLERATE_100      1
#define SAMPLERATE_200      2
#define SAMPLERATE_400      3
#define SAMPLERATE_800      4
#define SAMPLERATE_1000     5
#define SAMPLERATE_1600     6
#define SAMPLERATE_3200     7

// pulseWidth(Table 7. LED Pulse Width Control)
#define PULSEWIDTH_69       0
#define PULSEWIDTH_118      1
#define PULSEWIDTH_215      2
#define PULSEWIDTH_411      3

// Multi-LED Mode Control Registers(Register address 0x011)
#define SLOT_NONE           0
#define SLOT_RED_LED        1
#define SLOT_IR_LED         2

    void softReset();     // Reset all configuration, threshold, and data regsiter. When reset completed, auto reset the reset bit
    void shutDown();     // Enter power-saving mode. In this mode, all registers keep their values, and read and write normally, clear all interrupts to 0.
    void wakeUp();     // Wake-up mode, work normally
    void setLEDMode(uint8_t mode);  // pass mode Configure the mode by the macro definition with LEDMode comment
    void setADCRange(uint8_t adcRange);    // pass adcRange Configure ADC measuring range by the macro definition with adcRange comment
    void setSampleRate(uint8_t sampleRate);   // pass sampleRate Configure sampling rate by the macro definition with sampleRate comment
    void setPulseWidth(uint8_t pulseWidth);  // pass pulseWidth Configure LED pulse width by the macro definition with pulseWidth comment
    void setPulseAmplitudeRed(uint8_t amplitude);  // pass amplitude Amplitude: 0x00 = 0mA, 0x7F = 25.4mA, 0xFF = 50mA
    void setPulseAmplitudeIR(uint8_t amplitude);    // pass amplitude Amplitude: 0x00 = 0mA, 0x7F = 25.4mA, 0xFF = 50mA
    void enableSlot(uint8_t slotNumber, uint8_t device);   // Configure led device according to the given number, 4 timeslot in total. We will only use slot1 and slot2. There are red light and IR light , slotNumber Slot Number, can be 1, 2, device LED Device nameï¼šSLOT_RED_LED or SLOT_IR_LED
    void disableAllSlots(void);       // Disable all slots
    void enableAlmostFull(void);   // Enable FIFO Almost Full Flag. This interrupt is triggered when FIFO write pointer has a certian amount of free space.
    void disableAlmostFull(void);  // Disable FIFO Almost Full Flag
    void enableDataReady(void);   // Enable New FIFO Data Ready. This interrupt is triggered when there is a new sample in data FIFO
    void disableDataReady(void); // Disable New FIFO Data Ready
    void enableALCOverflow(void);      // Enable Ambient Light Cancellation Overflow. This interrupt is triggered when the ambient light cancellation function of SpO2/HR photodiode reaches the maximum
    void disableALCOverflow(void); // Disable Ambient Light Cancellation Overflow
    void enableDieTempReady(void);                // Enable Internal Temperature Ready Flag. This interrupt is triggered when the internal mold temperature conversion is done.
    void disableDieTempReady(void);       // Disable Internal Temperature Ready Flag
    void setFIFOAverage(uint8_t samples);           // pass samples Averaged sample. Configure it by the macro definition with the comment sampleAverage
    void enableFIFORollover();       // Enable FIFO Rolls on Full. If the FIFO is full, the FIFO address will return to 0 and FIFO will continue to fill in with new data
    void disableFIFORollover();    // Disable FIFO Rolls on Full. New sample will be lost if FIFO is full.
    void setFIFOAlmostFull(uint8_t numberOfSamples);    // numberOfSamples The size of free sample space. Trigger the interrupt when free samples reached this value. : paas If set to 2, the interrupt will be triggered by filling in 30 samples. If set to 0, that will be triggered in 32 samples.
    uint8_t getPartID();               // Read chip ID
    uint8_t getWritePointer(void);     // Get FIFO write pointer
    uint8_t getReadPointer(void);      // Get FIFO read pointer
    void resetFIFO(void);              // Reset FIFO
    void getNewData(void);   // Read new data and save it in consctructor buffer
    void writeReg(uint8_t reg, const void *pBuf, uint8_t size);   // Write data to the register
    uint8_t readReg(uint8_t reg, const void *pBuf, uint8_t size); // Read data from the register

    TwoWire *_pWire;
    uint8_t _i2cAddr;
    uint8_t _activeLEDs;
    sSenseBuf_t senseBuf;
/*---- End of Configurations option ----*/

public:
    // Methods declaration
    LF_Alpha_MAX30102() {
        if(i2cInitFlag == false) {
            i2cInitFlag = true;
            Wire.begin(LF_Alpha_SDA, LF_Alpha_SCL);     // Initialize I2C for all instances
        }

        pinMode(userLED2, OUTPUT);
        digitalWrite(userLED2, LOW);
    }
    ~LF_Alpha_MAX30102() {}

    bool begin(uint8_t i2cAddr = MAX30102_IIC_ADDRESS, TwoWire *pWire = &Wire);
    void configure(uint8_t ledBrightness = 0x1F, uint8_t sampleAverage = SAMPLEAVG_4,
                   uint8_t ledMode = MODE_MULTILED, uint8_t sampleRate = SAMPLERATE_400,
                   uint8_t pulseWidth = PULSEWIDTH_411, uint8_t adcRange = ADCRANGE_4096);
    void readSPO2(int32_t *SPO2, int8_t *SPO2Valid, int32_t *heartRate, int8_t *heartRateValid);
    uint32_t readBPM(void);
    float readTempC(void);
    float readTempF(void);
    uint32_t getRed(void);
    uint32_t getIR(void);

    bool end(void);
};

// LF_Alpha_WiFI class implimented here..
class LF_Alpha_WiFi
{
private:
    String ssid;
    String password;
    bool wifiStatus;

public: 
    LF_Alpha_WiFi();
    ~LF_Alpha_WiFi() { end(); }
    bool readStatus();
    String getIPAddress();
    bool checkInternet();
    void begin(void);
    void begin(const String &SSID, const String &PASSWORD);
    void disconnectWiFi();
    void configure(wifi_mode_t mode); // Set WiFi mode: WIFI_OFF, WIFI_STA, WIFI_AP, WIFI_AP_STA
    uint8_t getCurrentMode(void);
    void connect(const String &SSID, const String &PASSWORD);

    // Access point mode
    bool startAccessPoint(const String &SSID, const String &PASSWORD);
    bool stopAccessPoint();

    bool end(void);
};

// LF_Alpha_HTTP class implimeted here..
class LF_Alpha_HTTPClient
{
private:
    HTTPClient http;
    String serverUrl;

public:
    LF_Alpha_HTTPClient(const String &url)
    {
        serverUrl = url;
        pinMode(userLED2, OUTPUT);
        digitalWrite(userLED2, LOW);
    }

    LF_Alpha_HTTPClient(void)
    {
        serverUrl = "";
    }

    bool begin(void) { /* do nothing here */ return false;}

    bool configure(const String &url);
    bool begin(const String &url);
    int get(const String payload = "");
    int post(const String payload = "");
    int put(const String payload = "");
    int del(const String payload = "");
    void end();
    String getResponse();
    void addHeader(const String &name, const String &value);
};

// LF_Alpha_I2CScanner class impilimeted here..
class LF_Alpha_I2CScanner
{
private:
    int8_t error;
    int nDevices;
    String allAddresses;

    String i2cAddressToString(uint8_t address)
    {
        String result = "0x";
        result += decToHex(address >> 4);
        result += decToHex(address & 0x0F);
        return result;
    }

    String decToHex(uint8_t value)
    {
        return String(value, HEX);
    }

public:
    LF_Alpha_I2CScanner();
    void scan();
    String getAllAddresses() { return allAddresses; }
    int getDeviceCount() { return nDevices; }
    bool end(void)
    {
        return true;
    }
};

#endif