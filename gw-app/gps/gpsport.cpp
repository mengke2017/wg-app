#include "gpsport.h"
#include "common/mydebug.h"

#define TIME_OUT    10

#define PACKET_START_1  "$GNGGA"
#define PACKET_START_2  "$GNRMC"
#define PACKET_END      "\r\n"
#define PACKET_END_SIZE 2

GpsPort::GpsPort(QObject *parent) :
    QObject(parent)
{
    comData.resize(0);
    com = new SerialPort();
}

GpsPort::~GpsPort()
{
    close();
}

void GpsPort::receive()
{
    connect(com, SIGNAL(hasdata()), this, SLOT(readMyCom()));
    open();
}

//open port
void GpsPort::open()
{
    QString portName = GPS_UART;//CMD_PORT;

#ifdef GPS_MODULE
    if(!com->openPort(portName,BAUD9600,DATA_8,PAR_NONE,STOP_1,FLOW_OFF,TIME_OUT)) {
#else
    if(!com->openPort(portName,BAUD115200,DATA_8,PAR_NONE,STOP_1,FLOW_OFF,TIME_OUT)) {
#endif
        MY_WARNING("port open fail!");
        return;
    }
    MY_WARNING("port open success!");
}

//close port
void GpsPort::close()
{
    com->close();
}

//read data
void GpsPort::readMyCom()
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

bool GpsPort::parsePacket()
{
    static QByteArray tmp;

    int32 startIndex = comData.indexOf(PACKET_START_1);
//    qWarning()<<"gpsData:"<<comData.toHex();
//    qWarning()<<"comData:"<<comData;
    if(startIndex < 0) {
        startIndex = comData.indexOf(PACKET_START_2);
        if(startIndex < 0) {
            MY_DEBUG("start");
            comData.remove(0,comData.size());
            return false;
        }
    }
//    qWarning("!!!!!!!!!!!!!!!!!!!");
    comData = comData.mid(startIndex);
    int32 endIndex = comData.indexOf(PACKET_END);
    if(endIndex < 0) {
        MY_DEBUG("end");
        return false;
    }
//    qWarning("!!!!!!!!!!!!!!!!!!!@");
    tmp = comData.mid(0,endIndex);
//    qWarning()<<"gps: "<<tmp;
    emit readCom(tmp);
    comData.remove(0,endIndex+PACKET_END_SIZE);
    if(comData.indexOf(PACKET_START_1) < 0) {
        if(comData.indexOf(PACKET_START_2) < 0) {
            return false;
        } else {
            return true;
        }
    } else {
        return true;
    }
}

//continue send data
void GpsPort::send(QByteArray data)
{
    com->write(data);
}
