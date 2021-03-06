#include "posuart.h"
#include "common/mydebug.h"
#include "common/systemutils.h"

#define TIME_OUT    10

#define PACKET_START        "~"
#define PACKET_END          "~"
#define ESCAPE_CHAR_SRC1   "}"
#define ESCAPE_CHAR_DST1   "}\1"
#define ESCAPE_CHAR_SRC2   "~"
#define ESCAPE_CHAR_DST2   "}\2"

#define PACHET_START_SIZE   1
#define PACHET_END_SIZE     1
#define ESCAPE_CHAR_SRC1_SIZE  1
#define ESCAPE_CHAR_DST1_SIZE  2
#define ESCAPE_CHAR_SRC2_SIZE  1
#define ESCAPE_CHAR_DST2_SIZE  2

PosUart::PosUart(QObject *parent) :
    QObject(parent)
{
    mComData.resize(0);
    mCom = new SerialPort();
}

PosUart::~PosUart()
{
    close();
}

void PosUart::receive()
{
    connect(mCom, SIGNAL(hasdata()), this, SLOT(readMyCom()));
    open();
}

//open port
void PosUart::open()
{
//    QString portName = POS_UART;
//    if(!mCom->openPort(portName,BAUD9600,DATA_8,PAR_NONE,STOP_1,FLOW_OFF,TIME_OUT)) {
//        MY_WARNING("uart open fail!");
        return;
//    }
}

//close port
void PosUart::close()
{
    mCom->close();
    mCom = NULL;
}

//read data
void PosUart::readMyCom()
{
    MY_WARNING("readMyCom!");
    mComData += mCom->read();
    int32 size = mComData.size();
    if(0 != size)
    {
        bool flag = false;
        do {
            flag = parsePacket();
        }while(flag);
    }
}

bool PosUart::parsePacket()
{
    static QByteArray tmp;

    int32 startIndex = mComData.indexOf(PACKET_START);
    if(startIndex < 0) {
        mComData.remove(0,mComData.size());
        MY_DEBUG("startIndex fail!");
        return false;
    }

    mComData = mComData.mid(startIndex);
    int32 endIndex = mComData.indexOf(PACKET_END,PACHET_START_SIZE);
    if(endIndex < 0) {
        MY_DEBUG("endIndex fail!");
        return false;
    }

    if(endIndex < 15) {
        MY_DEBUG("len fail!");
        mComData.remove(0,endIndex+PACHET_END_SIZE);
        if(mComData.indexOf(PACKET_START) < 0) {
            return false;
        } else {
            return true;
        }
    }

    tmp = mComData.mid(0,endIndex+PACHET_END_SIZE);
    SystemUtils::escape(&tmp,ESCAPE_CHAR_DST1,ESCAPE_CHAR_DST1_SIZE,ESCAPE_CHAR_SRC1);
    SystemUtils::escape(&tmp,ESCAPE_CHAR_DST2,ESCAPE_CHAR_DST2_SIZE,ESCAPE_CHAR_SRC2);

    if(16 != tmp.size()) {
        MY_DEBUG("size fail!");
        mComData.remove(0,endIndex+PACHET_END_SIZE);
        if(mComData.indexOf(PACKET_START) < 0) {
            return false;
        } else {
            return true;
        }
    }

    if(tmp.at(1) != 0x01
            ||(uint8)(tmp.at(2)) != 0x80
            ||(uint8)(tmp.at(3)) != 0x8A
            ||tmp.at(4) != 0x00
            ||tmp.at(5) != 0x00
            ||tmp.at(6) != 0x10
            ||tmp.at(8) != 0x1d) {
        MY_DEBUG("head fail!");
        mComData.remove(0,endIndex+PACHET_END_SIZE);
        if(mComData.indexOf(PACKET_START) < 0) {
            return false;
        } else {
            return true;
        }
    }

    uint16 effect = tmp[13];
    effect = (effect<<8)|tmp[14];
    if(effect != SystemUtils::CRC16(&tmp,8,tmp[7]+8)) {
        MY_DEBUG("effect fail!");
        mComData.remove(0,endIndex+PACHET_END_SIZE);
        if(mComData.indexOf(PACKET_START) < 0) {
            return false;
        } else {
            return true;
        }
    }

    emit readCom(tmp);
    mComData.remove(0,endIndex+PACHET_END_SIZE);
    if(mComData.indexOf(PACKET_START) < 0) {
        return false;
    } else {
        return true;
    }
}

//continue send data
void PosUart::send(QByteArray data)
{
    mCom->write(data);
}
