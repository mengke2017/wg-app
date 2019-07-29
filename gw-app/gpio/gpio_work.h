#ifndef GPIO_WORK_H
#define GPIO_WORK_H
#include <QObject>
#include <QMutex>
#include "type.h"
#include "uart/serialport.h"
#include "gpio/gpio.h"

#define GPIO_INPUT_1  GPIO_1
#define GPIO_INPUT_2  GPIO_2
#define GPIO_INPUT_3  GPIO_3
#define GPIO_INPUT_4  GPIO_4
#define GPIO_INPUT_5  GPIO_5
#define GPIO_INPUT_6  GPIO_6
#define GPIO_INPUT_7  GPIO_7
#define GPIO_INPUT_8  GPIO_8

#define GPIO_INPUT(n) GPIO_INPUT_##n

class gpioWork : public QObject
{
    Q_OBJECT
public:
    explicit gpioWork(QObject *parent = 0);
    ~gpioWork();

private:
    Gpio* gpio;
    bool running;
private:
    void getGpioValue(uint16 io, uint8* value);

signals:
    void msg(unsigned char data);

public slots:
    void run();
    void stop();
};
#endif // GPIO_WORK_H
