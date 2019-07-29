#ifndef SIMPORT_H
#define SIMPORT_H

#include <QObject>
#include "type.h"
#include "system.h"
#include "qtimer.h"
#include "uart/serialport.h"

class SimPort : public QObject
{
    Q_OBJECT
public:
    explicit SimPort(QObject *parent = 0);
    ~SimPort();
    void send(QByteArray data);

private:
    SerialPort* com;
    QByteArray comData;
    System* sys;
    QTimer* sim_Timer;

private:
    void open();
    void close();
    bool parsePacket();

signals:
    void readCom(QByteArray data);
    void getIMSISuccess();

public :
    void sim_start();

private slots:
    void readMyCom();
    void slotTimeOut();
public slots:
    void slotLowPower();
};

#endif // SIMPORT_H
