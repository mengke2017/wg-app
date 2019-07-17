#include <QThread>
#include <QtCore/qmath.h>
#include "gps.h"

#include "sim/simport.h"
#include "common/mydebug.h"

//#define CMD_GNSS_ON     "echo -e \"AT+QGPS=1\r\n\" > /dev/ttyUSB2"
//#define CMD_GNSS_OFF    "echo -e \"AT+QGPS=0\r\n\" > /dev/ttyUSB2"

#define GNRMC_MARK  0x01
#define GNGGA_MARK  0x02
#define FRAME_MARK  0x03

const uint8 CalendarTab[4][12] = {
    {31,29,31,30,31,30,31,31,30,31,30,31},
    {31,28,31,30,31,30,31,31,30,31,30,31},
    {31,28,31,30,31,30,31,31,30,31,30,31},
    {31,28,31,30,31,30,31,31,30,31,30,31}
};

Gps::Gps(QObject *parent) :
    QObject(parent)
{
    timezone = false;
    gpsPort = new GpsPort();

    QThread* thread = new QThread();
    gpsPort->moveToThread(thread);
    connect(thread,SIGNAL(started()),gpsPort,SLOT(receive()),Qt::QueuedConnection);
    connect(gpsPort,SIGNAL(readCom(QByteArray)),this,SLOT(slotPortHandler(QByteArray)));
    thread->start();
//    system(CMD_GNSS_ON);
    MY_SYSTEM("GPS Init");
}

Gps* Gps::getInstance()
{
    static Gps instance;
    return &instance;
}

QByteArray Gps::getGPSDataIndex(QByteArray data)
{
    static QByteArray in;
    uint8 i = 0;
    int32 index = 0;
    in.clear();
    do {
        index = data.indexOf(",",index+1);
        if(index > 0) {
            in[i] = index;
            i += 1;
        }
    }while(index > 0);
    return in;
}

void Gps::getSatellites(QByteArray data)
{
    uint8 i;
    uint8 size = data.size();

    gpsData.satellites = 0;
    for(i = 0;i < size;i++) {
        gpsData.satellites = gpsData.satellites*10 + data.at(i) - 0x30;
    }
}

void Gps::getSpeed(QByteArray data)
{
    float speed = 0;
    uint8 i;
    uint8 bit = 0;
    bool flag = false;
    uint8 size = data.size();

    for(i = 0;i < size;i++) {
        if('.' == data.at(i)) {
            flag = true;
            continue;
        }

        speed = speed*10 + data.at(i) - 0x30;
        if(flag) {
            bit += 1;
        }
    }
    speed = speed/qPow(10,bit);
    speed = speed*1.852;
    gpsData.speed = qRound(speed);
}

void Gps::getLatitude(QByteArray data)
{
    int32 pointL = 0;
    int32 pointR = 0;
    bool flag = false;
    double lat = 0;
    uint8 i;
    uint8 bit = 0;
    uint8 size = data.size();

    for(i = 0;i < size;i++) {
        if('.' == data.at(i)) {
            flag = true;
            continue;
        }

        if(flag) {
            pointR = pointR*10 + data.at(i) - 0x30;
            bit += 1;
        } else {
            pointL = pointL*10 + data.at(i) - 0x30;
        }
    }

    lat = pointL/100 + (double)(pointL%100)/60 + (double)(pointR)/qPow(10,bit)/60;
    lat = lat*qPow(10,6);
    gpsData.lat = qRound(lat);
}

void Gps::getSN(QByteArray data)
{
    int32 size = data.size();
    if(1 != size) {
        gpsData.sn = 0;
        return;
    }
    if('S' == data.at(0)) {
        gpsData.sn = 1;
    } else {
        gpsData.sn = 0;
    }
}

void Gps::getLongitude(QByteArray data)
{
    int32 pointL = 0;
    int32 pointR = 0;
    bool flag = false;
    double lng = 0;
    uint8 i;
    uint8 bit = 0;
    uint8 size = data.size();

    for(i = 0;i < size;i++) {
        if('.' == data.at(i)) {
            flag = true;
            continue;
        }

        if(flag) {
            pointR = pointR*10 + data.at(i) - 0x30;
            bit += 1;
        } else {
            pointL = pointL*10 + data.at(i) - 0x30;
        }
    }

    lng = pointL/100 + (double)(pointL%100)/60 + (double)(pointR)/qPow(10,bit)/60;
    lng = lng*qPow(10,6);
    gpsData.lng = qRound(lng);
}

void Gps::getEW(QByteArray data)
{
    int32 size = data.size();
    if(1 != size) {
        gpsData.ew = 0;
        return;
    }

    if('W' == data.at(0)) {
        gpsData.ew = 1;
    } else {
        gpsData.ew = 0;
    }
}

void Gps::getAltitude(QByteArray data)
{
    float altitude = 0;
    uint8 i;
    uint8 bit = 0;
    bool flag = false;
    bool sign = false;
    uint8 size = data.size();

    for(i = 0;i < size;i++) {
        if('.' == data.at(i)) {
            flag = true;
            continue;
        }

        if('-' == data.at(i)) {
            sign = true;
            continue;
        }

        altitude = altitude*10 + data.at(i) - 0x30;
        if(flag) {
            bit += 1;
        }
    }
    altitude = altitude/qPow(10,bit);
    gpsData.altitude = qRound(altitude);
    if(sign) {
        gpsData.altitude |= 0x8000;
    }
}

void Gps::getDirection(QByteArray data)
{
    float dir = 0;
    uint8 i;
    uint8 bit = 0;
    bool flag = false;
    uint8 size = data.size();

    for(i = 0;i < size;i++) {
        if('.' == data.at(i)) {
            flag = true;
            continue;
        }

        dir = dir*10 + data.at(i) - 0x30;
        if(flag) {
            bit += 1;
        }
    }
    dir = dir/qPow(10,bit);
    gpsData.direction = qRound(dir);
}

void Gps::getState(QByteArray data)
{
    int32 size = data.size();
    if(1 != size) {
        gpsData.state = 0;
        return;
    }

    if('A' == data.at(0)) {
        gpsData.state = 1;
    } else {
        gpsData.state = 0;
    }
}

void Gps::getDate(QByteArray data)
{
    int32 size = data.size();
    if(6 != size) {
        gpsData.time[0] = 0;
        gpsData.time[1] = 0;
        gpsData.time[2] = 0;
        return;
    }

    uint8 year = (data.at(4) - 0x30)*10 + data.at(5) - 0x30;
    uint8 month = (data.at(2) - 0x30)*10 + data.at(3) - 0x30;
    uint8 day = (data.at(0) - 0x30)*10 + data.at(1) - 0x30;
    if(timezone) {
        timezone = false;
        day += 1;
        if(day > CalendarTab[year%4][month-1]) {
            day -= CalendarTab[year%4][month-1];
            month += 1;
            if(month > 12) {
                month -= 12;
                year += 1;
                year %= 100;
            }
        }
    }

    gpsData.time[0] = ((year/10)<<4)|(year%10);
    gpsData.time[1] = ((month/10)<<4)|(month%10);
    gpsData.time[2] = ((day/10)<<4)|(day%10);
}

void Gps::getTime(QByteArray data)
{
    int32 size = data.size();
    if(9 > size) {
        gpsData.time[3] = 0;
        gpsData.time[4] = 0;
        gpsData.time[5] = 0;
        return;
    }

    uint8 hour = (data.at(0) - 0x30)*10 + data.at(1) - 0x30;
    hour += 8;
    if(hour >= 24) {
        timezone = true;
        hour -= 24;
    }
    gpsData.time[3] = ((hour/10)<<4)|(hour%10);
    gpsData.time[4] = (data.at(2)<<4)|(data.at(3)&0xf);
    gpsData.time[5] = (data.at(4)<<4)|(data.at(5)&0xf);
}

void Gps::slotPortHandler(QByteArray data)
{
    static uint8 mark = 0;
    static QByteArray index;
    static QByteArray datatmp;
    switch(data.at(3))
    {
        case 'G':
        {
            index = getGPSDataIndex(data);
            int32 size = index.size();
            if(14 == size) {
                datatmp = data.mid(index[6]+1,index[7]-index[6]-1);
                getSatellites(datatmp);
                datatmp = data.mid(index[8]+1,index[9]-index[8]-1);
                getAltitude(datatmp);
                mark |= GNGGA_MARK;
            }
//            qWarning("!!!!!!!!!!!!!!!!!!!@");
            break;
        }

        case 'R':
        {
            index = getGPSDataIndex(data);
            int32 size = index.size();
            if(12 == size) {
                datatmp = data.mid(index[0]+1,index[1]-index[0]-1);
                getTime(datatmp);
                datatmp = data.mid(index[1]+1,index[2]-index[1]-1);
                getState(datatmp);
                datatmp = data.mid(index[2]+1,index[3]-index[2]-1);
                getLatitude(datatmp);
                datatmp = data.mid(index[3]+1,index[4]-index[3]-1);
                getSN(datatmp);
                datatmp = data.mid(index[4]+1,index[5]-index[4]-1);
                getLongitude(datatmp);
                datatmp = data.mid(index[5]+1,index[6]-index[5]-1);
                getEW(datatmp);
                datatmp = data.mid(index[6]+1,index[7]-index[6]-1);
                getSpeed(datatmp);
                datatmp = data.mid(index[7]+1,index[8]-index[7]-1);
                getDirection(datatmp);
                datatmp = data.mid(index[8]+1,index[9]-index[8]-1);
                getDate(datatmp);
                mark |= GNRMC_MARK;
            }
//            qWarning("!!!!!!!!!!!!!!!!!!!@@");
            break;
        }

        default:
            break;
    }

    if(FRAME_MARK == mark) {
        mark = 0;
        emit read(gpsData);
    }
}

