#include <QMetaType>
#include <QThread>
#include "busbox.h"
#include "common/mydebug.h"

BusBox::BusBox(QObject *parent) :
    QObject(parent)
{
    qRegisterMetaType<bus_message>("bus_message");
    busUart = new BusUart();

    QThread *thread = new QThread(this);
    busUart->moveToThread(thread);
    connect(thread,SIGNAL(started()),busUart,SLOT(receive()),Qt::QueuedConnection);
    connect(busUart,SIGNAL(readCom(QByteArray)),this,SLOT(slotComDataHandler(QByteArray)));
    thread->start();
    MY_SYSTEM("BusBox Init");
}

void BusBox::slotComDataHandler(QByteArray data)
{
    static bus_data busdata;
    static bus_message busMsg;

    busMsg.list.clear();
    busMsg.add = data.at(1);
    busMsg.type = data.at(3);

    uint16 size = data.size();
    uint16 index = 7;
    while((index+3) < size) {
        busdata.type = data.at(index);
        busdata.len = (data.at(index+1)<<8)|data.at(index+2);
        index += 3;
        if((index+busdata.len) <= size) {
            busdata.data = data.mid(index,busdata.len);
        }
        index += busdata.len;
        busMsg.list.append(busdata);
    }

    emit toLed(busMsg);
}
