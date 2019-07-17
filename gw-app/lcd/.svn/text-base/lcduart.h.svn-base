#ifndef LCDUART_H
#define LCDUART_H

#include <QObject>
#include "type.h"
#include "uart/serialport.h"

class LcdUart : public QObject
{
    Q_OBJECT
public:
    explicit LcdUart(QObject *parent = 0);
    ~LcdUart();
    void send(QByteArray data);
    
private:
    SerialPort* com;
    QByteArray comData;

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

#endif // LCDUART_H
