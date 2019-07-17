#include <QThread>
#include "lcdmanager.h"
#include "common/mydebug.h"
#include "common/systemutils.h"

//cmd code
#define GENERAL_REPLY   0x0d
#define LOCAL_SEND_GPS  0xa0
#define LOCAL_REPLY_SPEED   0xd2
#define LOCAL_REPLY_IP  0xd4
#define LOCAL_REPLY_NUMBER  0xd5
#define LCD_SET_IP      0x03
#define LCD_QUERY_IP    0x58
#define LCD_QUERY_SPEED 0x59
#define LCD_SET_SPEED   0x5a
#define LCD_SET_POS_MODE    0x5c
#define LCD_QUERY_NUMBER    0x5d

#define TRANSMISSION_TYPE   0x01

LcdManager::LcdManager(QObject *parent) :
    QObject(parent)
{
    sys = System::getInstance();
    sys->getSystemPara(&para);
    lcdPort = new LcdUart();
    QThread* thread = new QThread();
    lcdPort->moveToThread(thread);
    connect(thread,SIGNAL(started()),lcdPort,SLOT(receive()),Qt::QueuedConnection);
    connect(lcdPort,SIGNAL(readCom(QByteArray)),this,SLOT(portHandler(QByteArray)));
    thread->start();
    MY_SYSTEM("LcdManager Init!");
}

void LcdManager::networkHandler(Message msg)
{
    if(TRANSMISSION_TYPE == msg.data.at(0)) {
        QByteArray pack = msg.data.mid(1,msg.data.size()-1);
        lcdPort->send(pack);
    }
}

void LcdManager::portHandler(QByteArray data)
{
    switch(data.at(1))
    {
        case LCD_SET_IP:
        {
            int32 start = data.indexOf("\"",3);
            int32 end = data.indexOf("\"",start+1);
            QString ip = data.mid(start+1,end-start-1);
            int32 i = end+2;
            int32 count = 0;
            uint32 port = 0;
            while('>' != data.at(i)) {
                count += 1;
                if(count > 4) {
                    break;
                }
                port = port*10 + data.at(i) - 0x30;
                i += 1;
            }
            MY_DEBUG(QString("ip:%s port:%d").arg(ip,port));
            para.ip = ip;
            para.port = port;
            sys->setSystemPara(para);

            QByteArray pack;
            pack[0] = 0x24;
            pack[1] = GENERAL_REPLY;
            pack[2] = 2;
            pack[3] = SystemUtils::XOR(&pack,0,3);
            pack[4] = 0x0a;
            lcdPort->send(pack);
            break;
        }

        case LCD_QUERY_IP:
        {
            MY_DEBUG("LCD_QUERY_IP");
            QByteArray pack;
            int32 size = para.ip.size();

            pack[0] = 0x24;
            pack[1] = LOCAL_REPLY_IP;
            pack[2] = size+13;
            pack[3] = '<';
            pack[4] = '0';
            pack[5] = ',';
            pack[6] = '\"';
            pack += para.ip.toLatin1();
            pack[size+7] = '\"';
            pack[size+8] = ',';
            pack[size+9] = para.port/1000+0x30;
            pack[size+10] = para.port/100%10+0x30;
            pack[size+11] = para.port/10%10+0x30;
            pack[size+12] = para.port%10+0x30;
            pack[size+13] = '>';
            pack[size+14] = SystemUtils::XOR(&pack,0,size+14);
            pack[size+15] = 0x0a;

            lcdPort->send(pack);
            break;
        }

        case LCD_QUERY_SPEED:
        {MY_DEBUG("LCD_QUERY_SPEED");
            QByteArray pack;

            pack[0] = 0x24;
            pack[1] = LOCAL_REPLY_SPEED;
            pack[2] = 3;
            pack[3] = para.max_speed;
            pack[4] = SystemUtils::XOR(&pack,0,4);
            pack[5] = 0x0a;

            lcdPort->send(pack);
            break;
        }

        case LCD_SET_SPEED:
        {MY_DEBUG(QString("LCD_SET_SPEED speed=%d").arg(data.at(3)));
            para.max_speed = data.at(3);
            sys->setSystemPara(para);

            QByteArray pack;
            pack[0] = 0x24;
            pack[1] = GENERAL_REPLY;
            pack[2] = 2;
            pack[3] = SystemUtils::XOR(&pack,0,3);
            pack[4] = 0x0a;
            lcdPort->send(pack);
            break;
        }

        case LCD_SET_POS_MODE:
        {MY_DEBUG(QString("LCD_SET_POS_MODE mode=%d").arg(data.at(3)));
            QByteArray pack;

            pack[0] = 0x24;
            pack[1] = GENERAL_REPLY;
            pack[2] = 2;
            pack[3] = SystemUtils::XOR(&pack,0,3);
            pack[4] = 0x0a;

            lcdPort->send(pack);
            break;
        }

        case LCD_QUERY_NUMBER:
        {MY_DEBUG("LCD_QUERY_NUMBER");
            QByteArray pack;
            int32 size = sys->serialNumber.size();

            pack[0] = 0x24;
            pack[1] = LOCAL_REPLY_NUMBER;
            pack[2] = 20;
            uint8 fillbit = 0;
            if(size<18) {
                fillbit = 18 - size;
            }
            uint8 i;
            for(i = 0;i < fillbit;i++) {
                pack[3+i] = 0;
            }

            for(i = 0;i < (18-fillbit);i++) {
                pack[fillbit+3+i] = sys->serialNumber.at(i);
            }

            pack[21] = SystemUtils::XOR(&pack,0,21);
            pack[22] = 0x0a;

            lcdPort->send(pack);
            break;
        }

        default:
        {MY_DEBUG("LCD_TRANSMISSION_TYPE");
            Message msg;
            msg.data[0] = TRANSMISSION_TYPE;
            msg.data += data;
            emit toNetwork(msg);
            break;
        }
    }
}

void LcdManager::gpsHandler(struct gps_data data)
{
    QByteArray pack;

    pack[0] = 0x24;
    pack[1] = 0xa0;
    pack[2] = 0x1c;

    {
        pack[3] = data.time[3];
        pack[4] = data.time[4];
        pack[5] = data.time[5];
    }

    {
        int32 dd = data.lat/1000000;
        int32 mm = (data.lat%1000000)*60/1000;
        pack[6] = (data.sn<<7)|((dd/100)<<4)|(dd%100/10);
        pack[7] = ((dd%10)<<4)|(mm/10000);
        pack[8] = ((mm/1000%10)<<4)|(mm/100%10);
        pack[9] = ((mm/10%10)<<4)|(mm%10);
    }

    {
        int32 dd = data.lng/1000000;
        int32 mm = (data.lng%1000000)*60/1000;
        pack[10] = (data.ew<<7)|((dd/100)<<4)|(dd%100/10);
        pack[11] = ((dd%10)<<4)|(mm/10000);
        pack[12] = ((mm/1000%10)<<4)|(mm/100%10);
        pack[13] = ((mm/10%10)<<4)|(mm%10);
    }

    {
        pack[14] = ((data.speed/1000)<<4)|(data.speed/100%10);
        pack[15] = ((data.speed/10%10)<<4)|(data.speed%10);
    }

    {
        pack[16] = ((data.direction/1000)<<4)|(data.direction/100%10);
        pack[17] = ((data.direction/10%10)<<4)|(data.direction%10);
    }

    {
        pack[18] = (data.state<<7)|(data.satellites&0x1f);
    }

    pack[19] = 0x01;
    pack[20] = para.max_speed;
    pack[21] = 0x01;
    pack[22] = 0x00;
    pack[23] = 0x16;
    pack[24] = 0x01;
    pack[25] = 0x01;

    {
        pack[26] = data.time[0];
        pack[27] = data.time[1];
        pack[28] = data.time[2];
    }

    pack[29] = SystemUtils::XOR(&pack,0,29);
    pack[30] = 0x0a;

    lcdPort->send(pack);
}
