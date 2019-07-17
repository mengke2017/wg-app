#include "simport.h"
#include "common/mydebug.h"

#define TIME_OUT    10
#define CMD_PORT_2  "/dev/ttyUSB2"
#define PACKET_IMSI_START "AT+CIMI\r\r\n"
#define PACKET_IMSI_END   "\r\n\r\nOK"
#define CMD_IMSI_NUMBER   "echo -e \"AT+CIMI\r\n\" > /dev/ttyUSB2"

#define PACKET_IMSI_START_SIZE 10
#define PACKET_IMSI_END_SIZE 6

SimPort::SimPort(QObject *parent) :
    QObject(parent)
{
    sys = System::getInstance();  //  声明静态sys变量
    comData.resize(0);
    com = new SerialPort();
    sim_Timer = new QTimer();
    connect(sim_Timer,SIGNAL(timeout()),this,SLOT(slotTimeOut()));
    sim_Timer->start(5000);
}

SimPort::~SimPort()
{
    close();
}

void SimPort::sim_start()
{
    connect(com, SIGNAL(hasdata()), this, SLOT(readMyCom()));
    open();
}

//open port
void SimPort::open()
{
    QString portName = CMD_PORT_2;
    if(!com->openPort(portName,BAUD115200,DATA_8,PAR_NONE,STOP_1,FLOW_OFF,TIME_OUT)) {
        MY_WARNING("port open fail!");
        return;
    }
}

//close port
void SimPort::close()
{
    com->close();
}

//read data
void SimPort::readMyCom()
{
    comData += com->read();
    int32 size = comData.size();
    if(0 != size) {
//        qWarning()<<"USB2:"<<comData.data()<<"\n";
        if (parsePacket() == true) {
//            qWarning()<<"sys->sim_IMSI:"<<sys->sim_IMSI.toHex()<<"\n";
            disconnect(com, SIGNAL(hasdata()), this, SLOT(readMyCom()));
            close();
            sim_Timer->stop();
            emit getIMSISuccess();
        }
    }
}

bool SimPort::parsePacket()
{
    static QByteArray tmp;

    int32 startIndex = comData.indexOf(PACKET_IMSI_START);

    if (startIndex < 0)
        return false;

    comData = comData.mid(startIndex + PACKET_IMSI_START_SIZE);
    int32 endIndex = comData.indexOf(PACKET_IMSI_END);
    if (endIndex < 0) {
        return false;
    }

    tmp = comData.mid(0, 15);

    if ((tmp.at(0) >= '0' && tmp.at(0) <= '9') && (
         tmp.at(14) >= '0' && tmp.at(14) <= '9')) {
        sys->sim_IMSI = tmp;

        sys->sim[0] = (sys->sim_IMSI[0]<<4)|(sys->sim_IMSI[1]&0xf);
        sys->sim[1] = (sys->sim_IMSI[2]<<4)|(sys->sim_IMSI[3]&0xf);
        sys->sim[2] = (sys->sim_IMSI[4]<<4)|(sys->sim_IMSI[5]&0xf);
        sys->sim[3] = (sys->sim_IMSI[6]<<4)|(sys->sim_IMSI[7]&0xf);
        sys->sim[4] = (sys->sim_IMSI[8]<<4)|(sys->sim_IMSI[9]&0xf);
        sys->sim[5] = (sys->sim_IMSI[10]<<4)|(sys->sim_IMSI[11]&0xf);
        sys->sim[6] = (sys->sim_IMSI[12]<<4)|(sys->sim_IMSI[13]&0xf);
        sys->sim[7] = (sys->sim_IMSI[14]<<4)|(0x00&0xf);
        comData.remove(0, endIndex+PACKET_IMSI_END_SIZE);
//        qWarning()<<">>sys->sim_IMSI>>"<<sys->sim_IMSI.toHex()<<"\n";
        return true;
    } else {
        comData.remove(0, endIndex+PACKET_IMSI_END_SIZE);
        return false;
    }
}

//continue send data
void SimPort::send(QByteArray data)
{
    com->write(data);
}

void SimPort::slotTimeOut() {
    system(CMD_IMSI_NUMBER);
//    qWarning(CMD_IMSI_NUMBER);
}
