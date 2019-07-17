#ifndef LEDWORKER_H
#define LEDWORKER_H

#include <QObject>
#include <QMutex>
#include "type.h"
#include "uart/serialport.h"
#include "gpio/gpio.h"

class LedWorker : public QObject
{
    Q_OBJECT
public:
    explicit LedWorker(QObject *parent = 0);
    ~LedWorker();
    void send(QByteArray data);

private:
    SerialPort  *mCom;
    QByteArray mComData;
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

#endif // LEDWORKER_H
