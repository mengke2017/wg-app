#ifndef GPSPORT_H
#define GPSPORT_H

#include <QObject>
#include "type.h"
#include "sim/simport.h"
#include "uart/serialport.h"
#include "constant.h"

class GpsPort : public QObject
{
    Q_OBJECT
public:
    explicit GpsPort(QObject *parent = 0);
    ~GpsPort();
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

#endif // GPSPORT_H
