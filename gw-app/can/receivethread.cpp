#include <QMetaType>
#include <QFile>
#include <QTextStream>
#include "common/systemutils.h"
#include "receivethread.h"
#include "common/mydebug.h"

#define CAN_FRAME_SIZE  sizeof(struct can_frame)
#define CAN_CONFIG      "/usr/adapter/can_config.conf"

ReceiveThread::ReceiveThread(int32 s,QObject *parent) :
    QThread(parent)
{
    socket  = s;
    running = true;
    qRegisterMetaType<can_frame>("can_frame");
}

void ReceiveThread::run()
{
    int32 nbytes;
    int32 len;
    struct can_frame frame;
    struct sockaddr_can addr;

    if (!getConfigCanid(canid_list))
        qWarning("open can configfile error!");

    while(running)
    {
        nbytes=recvfrom(socket,&frame,CAN_FRAME_SIZE,0,(struct sockaddr *)&addr,(socklen_t*)&len);
        if(nbytes>0){
            if (filter(frame.can_id)) {
                emit msg(frame);
            }
        }
//        sleep(3);
    }
}

void ReceiveThread::stop()
{
    running = false;
}
bool ReceiveThread::getConfigCanid(canid_t id_list[]) {
    QFile file(CAN_CONFIG);
    uint32 value;
    if (!file.open( QIODevice::ReadOnly | QIODevice::Text ))
    {
        return false;
    }
    QTextStream txtInput(&file);
    QString lineStr;
    int i = 0;
    while(!txtInput.atEnd()) {
        lineStr = txtInput.readLine();

        if(lineStr.isEmpty())    continue;
        if(lineStr.at(0) == '#')    continue;
//        qWarning()<<lineStr.toLatin1();
        if(SystemUtils::stringToHex(lineStr, &value)) {
            if(i >= CAN_LIST_LEN) {
                file.close();
                return true;
            }
            id_list[i] = value;
            i++;
        }
    }
    file.close();
    return true;
}
bool ReceiveThread::filter(canid_t id) {
    canid_t can_id = id&0x1fffffff;
    for(int i = 0; i < CAN_LIST_LEN; i++) {
        if (canid_list[i] == can_id) {
            return true;
        }
    }
    return false;
}
