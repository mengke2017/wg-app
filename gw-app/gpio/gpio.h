#ifndef GPIO_H
#define GPIO_H

#include <QMutex>
#include "type.h"

#define GPIO_1          0
#define GPIO_2	    	1
#define GPIO_3	    	2
#define GPIO_4		    3
#define GPIO_5	    	4
#define GPIO_6	    	5
#define GPIO_7   		6
#define GPIO_8	    	7
#define RS485_EN1	    8
#define RS485_EN2	    9
#define GEN_3V3_EN  	10
#define GPS_POWER_EN	11
#define ENET1_RST    	12
#define ENET2_RST	    13
#define OTG_VBUS_EN		14
#define NET_HUB_RST     15
#define RESET_MCU       16
#define POWER_MCU       17
#define AP_READY_MCU    18
#define OTG_VBUS_DRV    19
#define VBUS_CTRL       20
#define EC20_BOOT       21


class Gpio
{
protected:
    Gpio();

public:
    static Gpio* getInstance();
    bool close();
    void ioctl(int32 io,uint8* ch);

private:
    bool open();

private:
    int fd;
    QMutex mutex;
};

#endif // GPIO_H
