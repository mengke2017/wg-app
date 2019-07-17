#ifndef BUSUART_H
#define BUSUART_H

#include <QObject>
#include <QMutex>
#include "type.h"
#include "uart/serialport.h"
#include "gpio/gpio.h"

class BusUart : public QObject
{
    Q_OBJECT
public:
    explicit BusUart(QObject *parent = 0);
    ~BusUart();
    
private:
    SerialPort *mCom;
    QByteArray mData;
    Gpio* gpio;

private:
    void open();
    void close();
    bool parsePacket();

signals:
    void readCom(QByteArray data);

public slots:
    void receive();

private slots:
    void readMyCom();
};

#endif // BUSUART_H
