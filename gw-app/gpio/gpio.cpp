#include "constant.h"
#include "common/mydebug.h"
#include "gpio.h"
#include "gpio_c.h"

#define _IOC_NRBITS	8
#define _IOC_TYPEBITS	8
#define _IOC_SIZEBITS	13
#define _IOC_DIRBITS	3

#define _IOC_NRSHIFT	0
#define _IOC_TYPESHIFT	(_IOC_NRSHIFT+_IOC_NRBITS)
#define _IOC_SIZESHIFT	(_IOC_TYPESHIFT+_IOC_TYPEBITS)
#define _IOC_DIRSHIFT	(_IOC_SIZESHIFT+_IOC_SIZEBITS)

#define _IOC_NONE	1U
#define _IOC_READ	2U
#define _IOC_WRITE	4U

#define _IOC(dir,type,nr,size)			\
    ((unsigned int)				\
     (((dir)  << _IOC_DIRSHIFT) |		\
      ((type) << _IOC_TYPESHIFT) |		\
      ((nr)   << _IOC_NRSHIFT) |		\
      ((size) << _IOC_SIZESHIFT)))

/* used to create numbers */
#define _IO(type,nr)		_IOC(_IOC_NONE,(type),(nr),0)

#define GPIO_U_IOCTL_BASE 'x'

#define GPIO_SIGNAL_1       _IO(GPIO_U_IOCTL_BASE,0)
#define GPIO_SIGNAL_2       _IO(GPIO_U_IOCTL_BASE,1)
#define GPIO_SIGNAL_3       _IO(GPIO_U_IOCTL_BASE,2)
#define GPIO_SIGNAL_4       _IO(GPIO_U_IOCTL_BASE,3)
#define GPIO_SIGNAL_5       _IO(GPIO_U_IOCTL_BASE,4)
#define GPIO_SIGNAL_6       _IO(GPIO_U_IOCTL_BASE,5)
#define GPIO_SIGNAL_7       _IO(GPIO_U_IOCTL_BASE,6)
#define GPIO_SIGNAL_8       _IO(GPIO_U_IOCTL_BASE,7)
#define GPIO_RS485_EN1      _IO(GPIO_U_IOCTL_BASE,8)
#define GPIO_RS485_EN2      _IO(GPIO_U_IOCTL_BASE,9)
#define GPIO_GEN_3V3_EN     _IO(GPIO_U_IOCTL_BASE,10)
#define GPIO_GPS_POWER_EN   _IO(GPIO_U_IOCTL_BASE,11)
#define GPIO_ENET1_RST      _IO(GPIO_U_IOCTL_BASE,12)
#define GPIO_ENET2_RST      _IO(GPIO_U_IOCTL_BASE,13)
#define GPIO_OTG_VBUS_EN    _IO(GPIO_U_IOCTL_BASE,14)
#define GPIO_NET_HUB_RST    _IO(GPIO_U_IOCTL_BASE,15)
#define GPIO_RESET_MCU      _IO(GPIO_U_IOCTL_BASE,16)
#define GPIO_POWER_MCU      _IO(GPIO_U_IOCTL_BASE,17)
#define GPIO_AP_READY_MCU   _IO(GPIO_U_IOCTL_BASE,18)
#define GPIO_OTG_VBUS_DRV   _IO(GPIO_U_IOCTL_BASE,19)
#define GPIO_VBUS_CTRL      _IO(GPIO_U_IOCTL_BASE,20)
#define GPIO_EC20_BOOT      _IO(GPIO_U_IOCTL_BASE,21)


Gpio::Gpio()
{
    fd = -1;
    open();
}

Gpio* Gpio::getInstance()
{
    static Gpio instance;
    return &instance;
}

bool Gpio::open()
{
    if(fd != -1) {
        MY_WARNING("is aready opened!");
        return false;
    }

    QString name = GPIO;
    fd = gpio_open(name.toLatin1().data());
    if(fd == -1) {
        MY_WARNING("gpio open erro!");
        return false;
    }

    return true;
}

bool Gpio::close()
{
    mutex.lock();
    if(fd != -1) {
        gpio_close(fd);
        fd = -1;
        mutex.unlock();
        return true;
    }
    mutex.unlock();
    return false;
}

void Gpio::ioctl(int32 io, uint8* value)
{
    mutex.lock();
    switch(io) {
    case GPIO_1:
        gpio_ioctl(fd,GPIO_SIGNAL_1,value);
        break;

    case GPIO_2:
        gpio_ioctl(fd,GPIO_SIGNAL_2,value);
        break;

    case GPIO_3:
        gpio_ioctl(fd,GPIO_SIGNAL_3,value);
        break;

    case GPIO_4:
        gpio_ioctl(fd,GPIO_SIGNAL_4,value);
        break;

    case GPIO_5:
        gpio_ioctl(fd,GPIO_SIGNAL_5,value);
        break;

    case GPIO_6:
        gpio_ioctl(fd,GPIO_SIGNAL_6,value);
        break;

    case GPIO_7:
        gpio_ioctl(fd,GPIO_SIGNAL_7,value);
        break;

    case GPIO_8:
        gpio_ioctl(fd,GPIO_SIGNAL_8,value);
        break;

    case RS485_EN1:
        gpio_ioctl(fd,GPIO_RS485_EN1,value);
        break;

    case RS485_EN2:
        gpio_ioctl(fd,GPIO_RS485_EN2,value);
        break;

    case GEN_3V3_EN:
        gpio_ioctl(fd,GPIO_GEN_3V3_EN,value);
        break;

    case GPS_POWER_EN:
        gpio_ioctl(fd,GPIO_GPS_POWER_EN,value);
        break;

    case ENET1_RST:
        gpio_ioctl(fd,GPIO_ENET1_RST,value);
        break;

    case ENET2_RST:
        gpio_ioctl(fd,GPIO_ENET2_RST,value);
        break;

    case OTG_VBUS_EN:
        gpio_ioctl(fd,GPIO_OTG_VBUS_EN,value);

    case NET_HUB_RST:
        gpio_ioctl(fd,GPIO_NET_HUB_RST,value);
        break;
    case RESET_MCU:
        gpio_ioctl(fd,GPIO_RESET_MCU,value);
        break;
    case POWER_MCU:
        gpio_ioctl(fd,GPIO_POWER_MCU,value);
        break;
    case AP_READY_MCU:
        gpio_ioctl(fd,GPIO_AP_READY_MCU,value);
        break;
    case OTG_VBUS_DRV:
        gpio_ioctl(fd,GPIO_OTG_VBUS_DRV,value);
        break;
    case VBUS_CTRL:
        gpio_ioctl(fd,GPIO_VBUS_CTRL,value);
        break;
    case EC20_BOOT:
        gpio_ioctl(fd,GPIO_EC20_BOOT,value);
        break;
    }
    mutex.unlock();
}
