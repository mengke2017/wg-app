#include "ledworker.h"
#include "common/mydebug.h"
#include "constant.h"
#include "common/systemutils.h"

#define TIME_OUT    10

#define PACKET_START        "~"
#define PACKET_END          "~"
#define ESCAPE_CHAR_SRC1   "}"
#define ESCAPE_CHAR_DST1   "}]"
#define ESCAPE_CHAR_SRC2   "~"
#define ESCAPE_CHAR_DST2   "}^"

#define PACHET_START_SIZE   1
#define PACHET_END_SIZE     1
#define ESCAPE_CHAR_SRC1_SIZE  1
#define ESCAPE_CHAR_DST1_SIZE  2
#define ESCAPE_CHAR_SRC2_SIZE  1
#define ESCAPE_CHAR_DST2_SIZE  2

LedWorker::LedWorker(QObject *parent) :
    QObject(parent)
{
    mComData.resize(0);
    mCom = new SerialPort();
    gpio = Gpio::getInstance();
}

LedWorker::~LedWorker()
{
    close();
}

void LedWorker::receive()
{
    connect(mCom, SIGNAL(hasdata()), this, SLOT(readMyCom()));
    open();
}

//open port
void LedWorker::open()
{
    uint8 value = 0;
    gpio->ioctl(RS485_EN1,&value);

    QString portName = LED_UART;
    if(!mCom->openPort(portName,BAUD57600,DATA_8,PAR_NONE,STOP_1,FLOW_OFF,TIME_OUT)) {
        MY_WARNING("uart open fail!");
        return;
    }
}

//close port
void LedWorker::close()
{
    mCom->close();
    gpio->close();
}

//read data
void LedWorker::readMyCom()
{
    mComData += mCom->read();
    int32 size = mComData.size();
    if(0 != size) {
        bool flag = false;
        do {
            flag = parsePacket();
        }while(flag);
    }
}

bool LedWorker::parsePacket()
{
    static QByteArray tmp;

    int32 startIndex = mComData.indexOf(PACKET_START);
    if(startIndex < 0) {
        MY_DEBUG("start");
        mComData.remove(0,mComData.size());
        return false;
    }

    mComData.remove(0,startIndex);
    int32 endIndex = mComData.indexOf(PACKET_END,PACHET_START_SIZE);
    if(endIndex < 0) {
        MY_DEBUG("end");
        return false;
    }

    if(endIndex < 11) {
        MY_DEBUG(QString("end=%1").arg(endIndex));
        mComData.remove(0,endIndex);
        if(mComData.indexOf(PACKET_START) < 0) {
            return false;
        } else {
            return true;
        }
    }

    tmp = mComData.mid(0,endIndex+PACHET_END_SIZE);
    SystemUtils::escape(&tmp,ESCAPE_CHAR_DST1,ESCAPE_CHAR_DST1_SIZE,ESCAPE_CHAR_SRC1);
    SystemUtils::escape(&tmp,ESCAPE_CHAR_DST2,ESCAPE_CHAR_DST2_SIZE,ESCAPE_CHAR_SRC2);

    int32 size = tmp.size();
    int32 len = (tmp.at(6)<<8)|tmp.at(7);
    if(len != (size - 12)) {
        MY_DEBUG(QString("size=%1 len=%2").arg(size).arg(len));
        mComData.remove(0,endIndex+PACHET_END_SIZE);
        if(mComData.indexOf(PACKET_START) < 0) {
            return false;
        } else {
            return true;
        }
    }

    if(tmp.at(1) != 0x00
            ||tmp.at(2) != 0x05) {
        MY_DEBUG("head");
        mComData.remove(0,endIndex+PACHET_END_SIZE);
        if(mComData.indexOf(PACKET_START) < 0) {
            return false;
        } else {
            return true;
        }
    }

    if(tmp.at(size-2) != SystemUtils::XOR(&tmp,1,size-2)) {
        MY_DEBUG("Effect");
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
void LedWorker::send(QByteArray data)
{
    static QByteArray pack;

    quint8 effect = SystemUtils::XOR(&data,0,data.size());
    data += SystemUtils::u8ToQByteArray(effect);
    SystemUtils::escape(&data,ESCAPE_CHAR_SRC1,ESCAPE_CHAR_SRC1_SIZE,ESCAPE_CHAR_DST1);
    SystemUtils::escape(&data,ESCAPE_CHAR_SRC2,ESCAPE_CHAR_SRC2_SIZE,ESCAPE_CHAR_DST2);
    QString start = PACKET_START;
    pack = start.toLatin1();
    pack += data;
    QString end = PACKET_END;
    pack += end.toLatin1();
    uint8 value = 1;
    gpio->ioctl(RS485_EN1,&value);
    mComData.remove(0,mComData.size());
    mCom->write(pack);

    int32 size = pack.size();
    uint32 delay = size*175;
    usleep(delay);
    value = 0;
    gpio->ioctl(RS485_EN1,&value);
}
