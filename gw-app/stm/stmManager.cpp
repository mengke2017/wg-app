#include <QMetaType>
#include <QThread>
#include "stmManager.h"
#include "common/mydebug.h"
#include "system.h"

stmManager::stmManager(QObject *parent) :
    QObject(parent)
{
//    qRegisterMetaType<bus_message>("bus_message");
    stmUart = new StmPort();

    sys = System::getInstance();

    repeatCount = 5;

    QThread *thread = new QThread(this);
    stmUart->moveToThread(thread);
    connect(thread,SIGNAL(started()),stmUart,SLOT(receive()),Qt::QueuedConnection);
    connect(stmUart,SIGNAL(readCom(QByteArray)),this,SLOT(slotComDataHandler(QByteArray)));
    thread->start();
    Timer = new QTimer(this);
    connect(Timer,SIGNAL(timeout()),this,SLOT(timeout()));
    Timer->start(3000);
    timeFlag = 0;
    timeFlag |= SET_TIME_CMD;
    MY_SYSTEM("stmPort Init");
}

void stmManager::slotComDataHandler(QByteArray data)
{
    static StmData stmData = {0, 0, 0, 0};

    stmData.cmd = data.at(1);
    stmData.len = data.at(2);
    stmData.data = data.at(3);

    if(stmData.cmd == STM_REPLY_CMD) {
        switch(stmData.data) {
        case SET_TIME_CMD:
            qWarning("set success！");
            if(timeFlag&SET_TIME_CMD)
                timeFlag &= ~SET_TIME_CMD;
            break;
        case LOW_POWERDOWN_CMD:
            qWarning("power off!");
            if(timeFlag&LOW_POWERDOWN_CMD)
                timeFlag &= ~LOW_POWERDOWN_CMD;
            system("poweroff");
            break;
        }
    } else if (stmData.cmd == STM_POWERDOWN_CMD) {
        reply_send(STM_POWERDOWN_CMD);

        timeFlag |= LOW_POWERDOWN_CMD;
    }
}

void stmManager::reply_send(uint8 cmd) {
    stmUart->send(REPLY_CMD, 1, cmd);
}

void stmManager::timeout() {
    if (timeFlag&SET_TIME_CMD) {
        static uint8 count0 = 0;
        if(count0 < repeatCount) {
            sys->getSystemPara(&para);
//            para.delay_time = 5;
//            sys->setSystemPara(para);
            stmUart->send(SET_TIME_CMD, 1, para.delay_time);
            qWarning("delayTime: %d", (int)para.delay_time);
            count0 ++;
        } else {
            timeFlag &= ~SET_TIME_CMD;
            MY_WARNING("set delaytime error! ");
            count0 = 0;
        }
    } else if (timeFlag&LOW_POWERDOWN_CMD) {
        static uint8 count1 = 0;
        if(count1 < repeatCount) {
            stmUart->send(LOW_POWERDOWN_CMD, 1, 0);
            count1 ++;
        } else {
            timeFlag &= ~LOW_POWERDOWN_CMD;
            MY_WARNING("low power error!");
            count1 = 0;
        }
    }
}
