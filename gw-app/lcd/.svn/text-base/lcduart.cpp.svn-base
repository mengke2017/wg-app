#include "constant.h"
#include "lcduart.h"
#include "common/mydebug.h"
#include "common/systemutils.h"

#define TIME_OUT    10

#define PACKET_START        0x24
#define PACKET_END_1        0xff
#define PACKET_END_2        0x0a

#define PACHET_START_SIZE   1
#define PACHET_END_SIZE     1

LcdUart::LcdUart(QObject *parent) :
    QObject(parent)
{
    comData.resize(0);
    com = new SerialPort();
}

LcdUart::~LcdUart()
{
    close();
}

void LcdUart::receive()
{
    connect(com, SIGNAL(hasdata()), this, SLOT(readMyCom()));
    open();
}

//open port
void LcdUart::open()
{
    QString portName = LCD_UART;
    if(!com->openPort(portName,BAUD9600,DATA_8,PAR_NONE,STOP_1,FLOW_OFF,TIME_OUT)) {
        MY_WARNING("uart open fail!");
        return;
    }
}

//close port
void LcdUart::close()
{
    com->close();
}

//read data
void LcdUart::readMyCom()
{
    comData += com->read();
    int32 size = comData.size();
    if(0 != size)
    {
        bool flag = false;
        do {
            flag = parsePacket();
        }while(flag);
    }
}

bool LcdUart::parsePacket()
{
    static QByteArray tmp;

    int32 startIndex = comData.indexOf(PACKET_START);
    if(startIndex < 0) {
        MY_DEBUG("startIndex");
        comData.remove(0,comData.size());
        return false;
    }

    comData = comData.mid(startIndex);
    int32 size = comData.size();
    if(size < 5) {
        MY_DEBUG(QString("size=%d").arg(size));
        return false;
    }

    int32 len = comData[2] + 3;
    if(size < len) {
        MY_DEBUG(QString("size=%d len=%d").arg(size,len));
        return false;
    }

    if(PACKET_END_1 != (quint8)comData.at(len-1)
            &&PACKET_END_2 != comData.at(len-1)) {
        MY_DEBUG("PACKET_END");
        comData.remove(0,PACHET_START_SIZE);
        if(comData.indexOf(PACKET_START) < 0) {
            return false;
        } else {
            return true;
        }
    }

    tmp = comData.mid(0,len);
    if(comData.at(len-2) != SystemUtils::XOR(&tmp,0,len-2)) {
        MY_DEBUG("xorEffect");
        comData.remove(0,len);
        if(comData.indexOf(PACKET_START) < 0) {
            return false;
        } else {
            return true;
        }
    }

    emit readCom(tmp);
    comData.remove(0,len);
    if(comData.indexOf(PACKET_START) < 0) {
        return false;
    } else {
        return true;
    }
}

//continue send data
void LcdUart::send(QByteArray data)
{
    com->write(data);
}
