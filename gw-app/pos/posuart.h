#ifndef POSUART_H
#define POSUART_H

#include <QObject>
#include "type.h"
#include "uart/serialport.h"
#include "constant.h"

class PosUart : public QObject
{
    Q_OBJECT
public:
    explicit PosUart(QObject *parent = 0);
    ~PosUart();

private:
    SerialPort  *mCom;
    QByteArray mComData;

public:
    void send(QByteArray data);

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

#endif // POSUART_H
