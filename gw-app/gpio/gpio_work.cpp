#include "gpio/gpio_work.h"
#include "common/mydebug.h"
#include "constant.h"
#include "common/systemutils.h"

gpioWork::gpioWork(QObject *parent) :
    QObject(parent)
{
    gpio = Gpio::getInstance();
    running = true;
}
gpioWork::~gpioWork() {
    stop();
}
void gpioWork::run() {
    uint8 data;
    uint8 value;
    QByteArray input;
//    uint8 a = 0;
//    gpio->ioctl(GPS_POWER_EN, &a);
    while(running) {
        for(int16 i = 1; i <= 8; i++) {
            getGpioValue(i, &value);
            input.append(value);
            if(i < 5) {
                if(value == 1) {
                    value = 0;
                } else {
                    value = 1;
                }
            }
            data |= (value<<(8-i));
        }
//        qWarning("input: %02x", data);
//        qWarning()<<"input: "<<input.toHex();
        input.clear();
        emit msg(data);
        sleep(10);
        data = 0;
    }
}

void gpioWork::stop() {
    running = false;
}

void gpioWork::getGpioValue(uint16 io, uint8* value) {
    switch(io) {
    case 1:
         gpio->ioctl(GPIO_INPUT(1), value);
        break;
    case 2:
         gpio->ioctl(GPIO_INPUT(2), value);
        break;
    case 3:
         gpio->ioctl(GPIO_INPUT(3), value);
        break;
    case 4:
         gpio->ioctl(GPIO_INPUT(4), value);
        break;
    case 5:
         gpio->ioctl(GPIO_INPUT(5), value);
        break;
    case 6:
         gpio->ioctl(GPIO_INPUT(6), value);
        break;
    case 7:
         gpio->ioctl(GPIO_INPUT(7), value);
        break;
    case 8:
         gpio->ioctl(GPIO_INPUT(8), value);
        break;
    }
}
