#ifndef CONSTANT_H
#define CONSTANT_H

#include "common/fileutils.h"

#define VERSION   "00"  //"V0.19.05.14"

//GPIO
#define GPIO        "/dev/gpio-gw"

//EEPROM
#define EEPROM      "/sys/bus/i2c/devices/1-0050/eeprom"

//UART
#define RS485_1     "/dev/ttymxc2"
#define RS485_2     "/dev/ttymxc4"
//#define RS232_1     "/dev/ttymxc1"
#define RS232     "/dev/ttymxc1"

#define  GPS_MODULE  0

#ifdef  GPS_MODULE
    #define GPS_UART    "/dev/ttymxc7"
#else
    #define GPS_UART  "/dev/ttyUSB1"
#endif

#define STM_UART    "/dev/ttymxc6"
#define G2_UART     "/dev/ttymxc3"

#define LED_UART    RS485_1
#define BUS_UART    RS485_2
//#define POS_UART    RS232_1
#define LCD_UART    RS232

//Device ID
#define ADAPTER_ID      0x0001
#define LED_HEAD_ID     0x1001
#define LED_WAIST_ID    0x1002
#define LED_TAIL_ID     0x1003
#define LED_INSIDE_ID   0x1004
#define LED_AD_ID       0x1005

//Update File Type
#define FILE_APP    0x01
#define FILE_FONT   0x02
#define FILE_LINE   0x03
#define FILE_AD     0x04

#define APP_PATH    FileUtils::getAppInstancePath()
#define APP_FILE_PATH   APP_PATH + "/data/"
#define LED_FILE_PATH   APP_PATH + "/led/"
#define DOWNLOAD_PATH   APP_PATH + "/download/"
#define CAN_CONFIG_PATH APP_FILE_PATH

#define DIAL_FLAG_FILE  "/usr/adapter/eth2"
#endif // CONSTANT_H
