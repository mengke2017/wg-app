#ifndef STMUART_H
#define STMUART_H

#include <QObject>
#include "type.h"
#include "uart/serialport.h"
#include "constant.h"
#include <QMutex>

//imx6ul->stm8
#define SET_TIME_CMD       0x02
#define LOW_POWERDOWN_CMD  0x01
#define REPLY_CMD          0x3E
//stm8->imx6ul
#define STM_POWERDOWN_CMD  0x01
#define STM_WEAKUP_CMD     0x02
#define STM_REPLY_CMD      0xE3

class StmPort : public QObject
{
    Q_OBJECT
public:
    explicit StmPort(QObject *parent = 0);
    ~StmPort();
    void send(uint8 cmd, uint8 len, uint8 data);

private:
    SerialPort* com;
    QByteArray comData;
    QMutex mutex;

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
#endif // STMUART_H
