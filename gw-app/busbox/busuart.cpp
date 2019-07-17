#include "busuart.h"
#include "constant.h"
#include "common/mydebug.h"
#include "common/systemutils.h"

#define TIME_OUT    10

#define PACKET_START        "\x7e"
#define PACKET_END          0x7f

#define PACHET_START_SIZE   1

BusUart::BusUart(QObject *parent) :
    QObject(parent)
{
    mData.resize(0);
    mCom = new SerialPort();
    gpio = Gpio::getInstance();
}

BusUart::~BusUart()
{
    close();
}

void BusUart::receive()
{
    connect(mCom, SIGNAL(hasdata()), this, SLOT(readMyCom()));
    open();
}

//open port
void BusUart::open()
{
    uint8 value = 0;
    gpio->ioctl(RS485_EN2,&value);

    QString portName = BUS_UART;
    if(!mCom->openPort(portName,BAUD19200,DATA_8,PAR_NONE,STOP_1,FLOW_OFF,TIME_OUT)) {
        MY_WARNING("uart open fail!");
        return;
    }
}

//close port
void BusUart::close()
{
    mCom->close();
    gpio->close();
}

//read data
void BusUart::readMyCom()
{
    mData += mCom->read();
    int32 size = mData.size();
    if(0 != size)
    {
        bool flag = false;
        do {
            flag = parsePacket();
        }while(flag);
    }
}

bool BusUart::parsePacket()
{
    static QByteArray tmp;

    int32 startIndex = mData.indexOf(PACKET_START);
    if(startIndex < 0) {
        mData.remove(0,mData.size());
        MY_DEBUG("start");
        return false;
    }

    mData.remove(0,startIndex);
    if(mData.size() < 9) {
        MY_DEBUG("pack len not enough");
        return false;
    }

    int32 len = (mData.at(5)<<8)|mData.at(6);
    if(len > 1024
            ||PACKET_END != mData.at(len+8)) {
        MY_DEBUG("end");
        mData.remove(0,PACHET_START_SIZE);
        if(mData.indexOf(PACKET_START) < 0) {
            mData.remove(0,mData.size());
            return false;
        } else {
            return true;
        }
    }

    len += 9;
    if(mData.at(2) != 0x01||
            (mData.at(3) != 0x10&&mData.at(3) != 0x03&&mData.at(3) != 0x04&&mData.at(3) != 0x05)||
            (mData.at(1) != 0xff&&mData.at(1) != 0x30&&mData.at(1) != 0x31&&mData.at(1) != 0x32&&mData.at(1) != 0x10)) {
        mData.remove(0,len);
        MY_DEBUG("pack type mismatch");
        if(mData.indexOf(PACKET_START) < 0) {
            return false;
        } else {
            return true;
        }
    }

    tmp = mData.mid(0,len);
    if (tmp.size() <= len - 1)
        return false;
    if(tmp.at(len-2) != SystemUtils::XOR(&tmp,0,len-2)) {
        mData.remove(0,len);
        MY_DEBUG("XOR");
        if(mData.indexOf(PACKET_START) < 0) {
            return false;
        } else {
            return true;
        }
    }
    emit readCom(tmp);
    mData.remove(0,len);
    if(mData.indexOf(PACKET_START) < 0) {
        return false;
    } else {
        return true;
    }
}
