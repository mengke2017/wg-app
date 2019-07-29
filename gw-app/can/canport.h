#ifndef CANPORT_H
#define CANPORT_H

#include <QObject>
#include <QMutex>
#include <sys/ioctl.h>
#include <net/if.h>
#include <linux/can.h>
#include "type.h"
#include "can/receivethread.h"

#define CAN0_NAME       "can0"
#define CAN0_CMD_OPEN   "ifconfig can0 up"
#define CAN0_CMD_CLOSE  "ifconfig can0 down"
#define CAN0_CMD_SET    "ip link set can0 up type can bitrate 250000 triple-sampling on"

#define CAN1_NAME       "can1"
#define CAN1_CMD_OPEN   "ifconfig can1 up"
#define CAN1_CMD_CLOSE  "ifconfig can1 down"
#define CAN1_CMD_SET    "ip link set can1 up type can bitrate 250000 triple-sampling on"

#define CAN_SUM         2

typedef struct {
    QString name;
    QString open;
    QString close;
    QString set;
}can_opt;

const can_opt opt[CAN_SUM] = {
    {CAN0_NAME,CAN0_CMD_OPEN,CAN0_CMD_CLOSE,CAN0_CMD_SET},
    {CAN1_NAME,CAN1_CMD_OPEN,CAN1_CMD_CLOSE,CAN1_CMD_SET}
};

class CanPort : public QObject
{
    Q_OBJECT
public:
    explicit CanPort(int32 num,bool isReceive,QObject *parent = 0);
    void close(int32 num);
    void send(uint32 id,QString str,int32 len);
    int32 read(void* frame);
    QList<can_frame> read_can_list();
    void clear_can_list();

private:
    void start(int32 n,bool isReceive);
    void stop(int32 n);
signals:
    void init();
private:
    int32 socket;
    struct sockaddr_can addr;
    volatile bool isValue;
    struct can_frame frameData;
    QMutex mutex;
    ReceiveThread *recvThread;
    QList<can_frame> can_list;

public:
    void can_init();

private slots:
    void slotMsgHandler(can_frame frame);
};

#endif // CAN_H
