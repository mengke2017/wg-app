#include <sys/ioctl.h>
#include <fcntl.h>
#include <QThread>
#include "posmanager.h"
#include "common/mydebug.h"

PosManager::PosManager(QObject *parent) :
    QObject(parent)
{
    posUart = new PosUart();
    canPort = new CanPort(1,false);

    QThread *thread = new QThread(this);
    posUart->moveToThread(thread);
    connect(thread,SIGNAL(started()),posUart,SLOT(receive()),Qt::QueuedConnection);
    connect(posUart,SIGNAL(readCom(QByteArray)),this,SLOT(slotUartDataHandler(QByteArray)));
    thread->start();
    MY_SYSTEM("PosManager Init");
}

PosManager::~PosManager()
{
    posUart->deleteLater();
    posUart = NULL;
    canPort->deleteLater();
    canPort = NULL;
}

void PosManager::slotUartDataHandler(QByteArray data)
{
    static QString str;
    str = data.mid(7,8);

    uint8 tmp[4];
    tmp[0] = (data[2]&0x80)>>2;//3
    tmp[0] |= (data[3]&0xf8)>>3;//5
    tmp[1] = (data[3]&0x07)<<5;//3
    tmp[1] |= (data[4]&0xf8)>>3;//5
    tmp[2] = (data[4]&0x07)<<5;
    tmp[2] |= (data[5]&0xf8)>>3;
    tmp[3] = (data[5]&0x07)<<5;
    tmp[3] |= (data[6]&0xf8)>>3;//0xf8

    uint32 id = tmp[0]<<24;
    id |= tmp[1]<<16;
    id |= tmp[2]<<8;
    id |= tmp[3];

    canPort->send(id,str,8);
}
