#include "stm/stmUart.h"
#include "common/mydebug.h"
#include "common/systemutils.h"

#define TIME_OUT    10

#define PACKET_START_SEND  0xA5
#define PACKET_START_REC   0x5A
#define PACKET_END         0xFE

StmPort::StmPort(QObject *parent) :
    QObject(parent)
{
    comData.resize(0);
    com = new SerialPort();
}

StmPort::~StmPort()
{
    close();
}

void StmPort::receive()
{
    connect(com, SIGNAL(hasdata()), this, SLOT(readMyCom()));
    open();
}

//open port
void StmPort::open()
{
    QString portName = STM_UART;//CMD_PORT;

    if(!com->openPort(portName,BAUD9600,DATA_8,PAR_NONE,STOP_1,FLOW_OFF,TIME_OUT)) {
        MY_WARNING("port open fail!");
        return;
    }
    MY_WARNING("port open success!");
}

//close port
void StmPort::close()
{
    com->close();
}

//read data
void StmPort::readMyCom()
{
    comData += com->read();
    int32 size = comData.size();
    if(0 != size) {
        bool flag = false;
        do {
            flag = parsePacket();
        }while(flag);
    }
}

bool StmPort::parsePacket()
{
    static QByteArray tmp;

    int32 startIndex = comData.indexOf(PACKET_START_REC);
    if(comData.length() < 6) {
        return false;
    }
//    qWarning()<<"#####################:"<<comData.toHex();

    if(startIndex < 0) {
        MY_DEBUG("start");
        comData.remove(0,comData.size());
        return false;
    }

    comData = comData.mid(startIndex);
    int32 endIndex = comData.indexOf(PACKET_END);
    if(endIndex < 0) {
        MY_DEBUG("end");
        return false;
    }

    tmp = comData.mid(0,endIndex+1);
//    qWarning()<<"tmp:"<<tmp.toHex();
    if(tmp.length() < 6) {
        return false;
    }
    uint8 crc = tmp.at(2) + tmp.at(3);
    if (tmp.at(5) == PACKET_END && tmp.at(4) == crc)
        emit readCom(tmp);
    qWarning()<<"@@@data: "<<tmp.toHex();
    comData.remove(0,endIndex);
    if(comData.indexOf(PACKET_START_REC) < 0) {
        return false;
    } else {
        return true;
    }
}

//continue send data
void StmPort::send(uint8 cmd, uint8 len, uint8 data)
{
    QByteArray pack;
    mutex.lock();
    pack = SystemUtils::u8ToQByteArray(PACKET_START_SEND);
    pack += SystemUtils::u8ToQByteArray(cmd) + SystemUtils::u8ToQByteArray(len)
            + SystemUtils::u8ToQByteArray(data) + SystemUtils::u8ToQByteArray(data + len);
    pack += SystemUtils::u8ToQByteArray(PACKET_END);
    com->write(pack);
    mutex.unlock();
    qWarning()<<"send data: "<<pack.toHex();
}
