#ifndef RECEIVETHREAD_H
#define RECEIVETHREAD_H

#include <QThread>
#include <sys/ioctl.h>
#include <net/if.h>
#include <linux/can.h>
#include "type.h"

#ifndef PF_CAN
#define PF_CAN 29
#endif

#ifndef AF_CAN
#define AF_CAN PF_CAN
#endif

#define CAN_LIST_LEN  50

class ReceiveThread : public QThread
{
    Q_OBJECT
public:
    explicit ReceiveThread(int32 s,QObject *parent = 0);
    
signals:
    void msg(can_frame frame);

public slots:
    void run();
    void stop();
    bool filter(canid_t id);

private:
    int32 socket;
    bool running;
    canid_t canid_list[CAN_LIST_LEN];

private:
    bool getConfigCanid(canid_t id_list[]);
};

#endif // RECEIVETHREAD_H
