#include <QThread>
#include <QFile>
#include <QSemaphore>
#include "common/systemutils.h"
#include "system.h"
#include "ledmanager.h"
#include "network/networkmanager.h"
#include "common/fileutils.h"
#include "common/mydebug.h"

#define FILE_TYPE_APP       0x01
#define FILE_TYPE_FONT      0x02

#define TIMER_MSEC          10      //ms
#define LED_COM_TIMEOUT     20      //单位0.01s,对应TIMER_MSEC
#define LED_BEATTIM         6000    //单位0.01s,对应TIMER_MSEC

#define MASTER_CMD_SUM  6
#define FILE_UPDATE_DATA_SIZE   256
uint8 OvertimeCmdTab[MASTER_CMD_SUM][2] = {
    {CMD_DATA_SEND,0},
    {CMD_FONT_SEND,0},
    {CMD_APP_SEND,0},
    {CMD_PARA_SEND,0},
    {CMD_QUERY_STATE,0},
    {CMD_QUERY_PARA,0},
};

QSemaphore sem(2);

LedManager::LedManager(QObject *parent) :
    QObject(parent)
{
    isUpdate = false;
    heartbeatTimer = 0;
    devIndex = 0;
    for(int32 i = 0;i < MASTER_CMD_SUM;i++) {
        OvertimeCmdTab[i][1] = 0;
    }
    sys = System::getInstance();
    sList = LedSendList::getInstance();
    ledWorker = new LedWorker();

    QThread *thread = new QThread(this);
    ledWorker->moveToThread(thread);
    connect(thread,SIGNAL(started()),ledWorker,SLOT(receive()),Qt::QueuedConnection);
    connect(ledWorker,SIGNAL(readCom(QByteArray)),this,SLOT(slotComDataHandler(QByteArray)));
    thread->start();

    mTimer = new QTimer();
    connect(mTimer,SIGNAL(timeout()),this,SLOT(slotTimeOut()));
    mTimer->start(TIMER_MSEC);

    QThread *sendthread = new QThread(this);
    sTimer = new QTimer();
    sTimer->setInterval(20*TIMER_MSEC);
    sTimer->moveToThread(sendthread);
    connect(sTimer,SIGNAL(timeout()),this,SLOT(slotSendTask()), Qt::DirectConnection);
    connect(sendthread, SIGNAL(started()),sTimer,SLOT(start()));
    sendthread->start();

    MY_SYSTEM("LedManager Init");
}

LedManager* LedManager::getInstance()
{
    static LedManager instance;
    return &instance;
}

void LedManager::sendPack(uint8 cmd,uint8 dstaddr,QByteArray data)
{
    sem.acquire(2);
    mCmd = cmd;
    mBatch = SystemUtils::rand16();

    QByteArray pack = SystemUtils::u8ToQByteArray(0);
    pack += SystemUtils::u8ToQByteArray(5);
    pack += SystemUtils::u8ToQByteArray(cmd);
    pack += SystemUtils::u8ToQByteArray(dstaddr);
    pack += SystemUtils::u8ToQByteArray(LOCAL_REAL_ID);
    uint16 size = data.size();
    pack += SystemUtils::u16ToQByteArray(size);
    pack += SystemUtils::u16ToQByteArray(mBatch);
    pack += data;

    ledWorker->send(pack);
    OvertimeCmdTab[mCmd-1][1] = 0;
    replyTimer = LED_COM_TIMEOUT;
    sem.release(1);
}

bool LedManager::fileUpdate(uint8 cmd,uint8 fonttype,uint8 addr,QString name)
{
    QString filename = LED_FILE_PATH + name;
    uint64 size = FileUtils::getFileSize(filename);
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly)) {
        FileUtils::rmFile(filename);
        return false;
    }

    QTextStream os(&file);
    static QByteArray content;
    uint64 index = 0;
    uint64 readlen;
    uint8 count = 0;
    uint16 packnum = 0;
    uint16 packsum = size/FILE_UPDATE_DATA_SIZE;
    if(size%FILE_UPDATE_DATA_SIZE) {
        packsum += 1;
    }

    while(1)
    {
        if(1 == sem.available()) {
            continue;
        }

        if(0 != OvertimeCmdTab[cmd-1][1]) {
            OvertimeCmdTab[cmd-1][1] = 0;
            if(count < 5) {
                count += 1;
            } else {
                file.close();
                MY_WARNING(QString("file update fail"));
                FileUtils::rmFile(filename);
                return false;
            }
            sendPack(cmd,addr,content);
        } else {
            count = 0;
            if(size > FILE_UPDATE_DATA_SIZE) {
                readlen = FILE_UPDATE_DATA_SIZE;
                size -= FILE_UPDATE_DATA_SIZE;
            } else if(size == 0) {
                file.close();
                MY_DEBUG(QString("file update ok"));
                emit UpdateOK(0);
                FileUtils::rmFile(filename);
                return true;
            } else {
                readlen = size;
                size = 0;
            }

            os.seek(index);
            if(CMD_FONT_SEND == cmd) {
                content = SystemUtils::u8ToQByteArray(fonttype)+SystemUtils::u16ToQByteArray(packsum)+SystemUtils::u16ToQByteArray(packnum);
            } else {
                content = SystemUtils::u16ToQByteArray(packsum)+SystemUtils::u16ToQByteArray(packnum);
            }

            content += os.read(readlen).toLatin1();
            sendPack(cmd,addr,content);
            index += readlen;
            packnum += 1;
        }
    }
    return false;
}

void LedManager::queryState()
{
    static uint8 noreplycount = 0;

    if(2 != sem.available()) {
        return;
    }

    if(0 != OvertimeCmdTab[CMD_QUERY_STATE-1][1]) {
        OvertimeCmdTab[CMD_QUERY_STATE-1][1] = 0;
        if(true == sys->peripheralState[devIndex].is_connect) {
            noreplycount += 1;
            if(3 == noreplycount) {
                noreplycount = 0;
                sys->peripheralState[devIndex].is_connect = false;
                devIndex += 1;
            }
        } else {
            devIndex += 1;
        }

        if(devIndex == LED_DEVICE_SUM) {
            devIndex = 0;
            heartbeatTimer = LED_BEATTIM;
        } else {
            sendPack(CMD_QUERY_STATE,LedDevice[devIndex],NULL);
        }
    } else {
        noreplycount = 0;
        sendPack(CMD_QUERY_STATE,LedDevice[devIndex],NULL);
    }
}

void LedManager::slotComDataHandler(QByteArray data)
{
    uint8 cmd = data.at(3);
    uint8 srcaddr = data.at(4);

    if(LOCAL_REAL_ID != srcaddr) {
        return;
    }

    switch(cmd)
    {
        case CMD_GENERAL_REPLY:
            generalReplyHandler(data);
            break;

        case CMD_STATE_REPLY:
            stateReplyHandler(data);
            break;

        case CMD_PARA_REPLY:
            paraReplyHandler(data);
            break;
    }
}

void LedManager::netWorkHandler(Message msg)
{
    if(isUpdate) {
        return;
    }

    switch(msg.id)
    {
        case SERVE_PERIPHERAL_CONTROL:
            info.type = MSG_TYPE_NET;
            info.addr = msg.data.at(1);
            netCmd = (msg.data.at(2)<<8)|msg.data.at(3);
            netBatch = msg.batch;
            switch(msg.data.at(3))
            {
                case 0:
                    info.cmd = CMD_PARA_SEND;
                    info.data = msg.data.mid(4,msg.data.size()-4);
                    if(sList->add(info)) {
                        msg.data = SystemUtils::u16ToQByteArray(netBatch)
                                +SystemUtils::u16ToQByteArray(netCmd)+SystemUtils::u8ToQByteArray(0);
                    } else {
                        msg.data = SystemUtils::u16ToQByteArray(netBatch)
                                +SystemUtils::u16ToQByteArray(netCmd)+SystemUtils::u8ToQByteArray(1);
                    }
                    emit toNetwork(msg);
                    break;

                case 1:
                case 2:
                case 3:
                case 4:
                    info.cmd = CMD_DATA_SEND;
                    info.data = msg.data.mid(3,msg.data.size()-3);
                    if(sList->add(info)) {
                        msg.data = SystemUtils::u16ToQByteArray(netBatch)
                                +SystemUtils::u16ToQByteArray(netCmd)+SystemUtils::u8ToQByteArray(0);
                    } else {
                        msg.data = SystemUtils::u16ToQByteArray(netBatch)
                                +SystemUtils::u16ToQByteArray(netCmd)+SystemUtils::u8ToQByteArray(1);
                    }
                    emit toNetwork(msg);
                    break;

                case 5:
                    info.cmd = CMD_QUERY_PARA;
                    info.data = msg.data.mid(4,msg.data.size()-4);
                    sList->add(info);
                    break;
            }
            break;

        case SERVE_FILE_UPDATE:
            message.id = msg.id;
            message.batch = msg.batch;
            message.data = msg.data;
            isUpdate = true;
            break;
    }
}

void LedManager::busboxHandler(bus_message msg)
{
    uint8 num;
    uint8 len;
    bus_data data;
    QByteArray ad;

    if(isUpdate) {
        return;
    }

    MY_DEBUG(QString("msg.type=%1").arg(msg.type));
    info.type = MSG_TYPE_BOX;
    info.cmd = CMD_DATA_SEND;
    switch(msg.add)
    {
        case MSG_ADDR_HEAD:
            info.addr = LED_HEAD_REAL_ID;
            break;

        case MSG_ADDR_WAIST:
            info.addr = LED_WAIST_REAL_ID;
            break;

        case MSG_ADDR_TAIL:
            info.addr = LED_TAIL_REAL_ID;
            break;

        case MSG_ADDR_INSIDE:
        case MSG_ADDR_BROADCAST:
            info.addr = LED_INSIDE_REAL_ID;
            break;
    }

    switch(msg.type)
    {
        case MSG_TYPE_STATION:
        {
            uint8 updownflag = 0;
            uint8 inoutflag = 0;
            uint8 num = 0;
            uint8 sum = 0;
            uint8 len = 0;
            QByteArray name;
            bus_data data;
            uint8 size = msg.list.size();
            for(uint8 i = 0;i < size;i++) {
                data = msg.list.at(i);
                switch(data.type) {
                    case DATA_TYPE_UP_DOWN_FLAG:
                        if(0x01 == data.data.at(0)) {
                            updownflag = 0;
                        } else {
                            updownflag = 1;
                        }
                        break;

                    case DATA_TYPE_IN_OUT_STATION_FLAG:
                        if(0x01 == data.data.at(0)) {
                            inoutflag = 0;
                        } else {
                            inoutflag = 1;
                        }
                        break;

                    case DATA_TYPE_STATION_NUM:
                        num = data.data.at(0);
                        break;

                    case DATA_TYPE_STATION_NAME:
                        name = data.data;
                        len = name.size();
                        break;
                }
            }

            info.data = SystemUtils::u8ToQByteArray(DATA_TYPE_SITE)+SystemUtils::u8ToQByteArray(updownflag)+SystemUtils::u8ToQByteArray(inoutflag)
                    +SystemUtils::u8ToQByteArray(num)+SystemUtils::u8ToQByteArray(sum)+SystemUtils::u8ToQByteArray(len)+name;
            sList->add(info);
            break;
        }

        case MSG_TYPE_LINE:
        {
            uint8 linelen = 0;
            QByteArray line;
            uint8 firstlen = 0;
            QByteArray first;
            uint8 endlen = 0;
            QByteArray end;
            bus_data data;
            uint8 size = msg.list.size();
            for(uint8 i = 0;i < size;i++) {
                data = msg.list.at(i);
                switch(data.type) {
                    case DATA_TYPE_LINE_NAME:
                        linelen = data.len;
                        line = data.data;
                        break;

                    case DATA_TYPE_FIRST_NAME:
                        firstlen = data.len;
                        first = data.data;
                        break;

                    case DATA_TYPE_END_NAME:
                        endlen = data.len;
                        end = data.data;
                        break;
                }
            }
            info.data = SystemUtils::u8ToQByteArray(DATA_TYPE_LINE)+SystemUtils::u8ToQByteArray(linelen)+line
                    +SystemUtils::u8ToQByteArray(firstlen)+first+SystemUtils::u8ToQByteArray(0)
                    +SystemUtils::u8ToQByteArray(endlen)+end+SystemUtils::u8ToQByteArray(0);
            sList->add(info);
            break;
        }

        case MSG_TYPE_LINE_INFO:
        {
//            bus_data data;
            uint8 size = msg.list.size();
            for(uint8 i = 0;i < size;i++) {
                data = msg.list.at(i);
                switch(data.type) {
                    case DATA_TYPE_CUE://宣传用语设置
                    {   // QByteArray ad;
                       // MY_WARNING("*************************************************");
                        //qWarning()<<"data:"<<data.data.toHex()<<"\n";
                        for(uint16 j = 0;j < data.len;) {
                            num = data.data.at(j);
                            len = data.data.at(j+2);
                            ad = data.data.mid(j+3,len);

                            if(0x02 == data.data.at(j+1)){
                                info.data = SystemUtils::u8ToQByteArray(DATA_TYPE_SLG)+SystemUtils::u8ToQByteArray(num)
                                        +SystemUtils::u8ToQByteArray(len)+ad;
                            } else {
                                info.data = SystemUtils::u8ToQByteArray(DATA_TYPE_AD)+SystemUtils::u8ToQByteArray(num)
                                        +SystemUtils::u8ToQByteArray(len)+ad;
                            }
                            sList->add(info);
                            j += len + 3;
                        }
                    break;
                    }
                    /*
                    case DATA_TYPE_REAL://实时下发宣传用语
                    {
                        QByteArray ad;
                        for(uint16 j = 0;j < data.len;) {
                            num = data.data.at(j);
                            len = data.data.at(j+2);
                            ad = data.data.mid(j+3,len);

                            info.data = SystemUtils::u8ToQByteArray(DATA_TYPE_AD)+SystemUtils::u8ToQByteArray(num)
                                    +SystemUtils::u8ToQByteArray(len)+ad;
                            sList->add(info);
                            j += len + 3;
                        }
                        break;
                    }*/
                }
            }
        break;
        }
        case MSG_TYPE_KEYSLG:  // 调用宣传用语编号
        {
             data = msg.list.at(0);
            num = data.data.at(0);
            len = 0;
            ad = data.data.mid(3,0);
            info.data = SystemUtils::u8ToQByteArray(DATA_TYPE_KEY)+SystemUtils::u8ToQByteArray(num)
                    +SystemUtils::u8ToQByteArray(len)+ad;
            sList->add(info);
        break;
        }
    }
}

void LedManager::slotTimeOut()
{
    if(1 == sem.available()) {
        if(replyTimer > 0) {
            replyTimer--;
        } else {
            uint8 index = mCmd - 1;
            if(index < MASTER_CMD_SUM) {
                OvertimeCmdTab[index][1] = 1;
            }
            sList->remove(mCmd);
            sem.release(1);
        }
    }

    if(heartbeatTimer > 0) {
        heartbeatTimer--;
    }
}

void LedManager::slotSendTask()
{
    if(isUpdate) {
        uint8 cmd;
        if(FILE_TYPE_APP == message.data.at(2)) {
            cmd = CMD_APP_SEND;
        } else if(FILE_TYPE_FONT == message.data.at(2)) {
            cmd = CMD_FONT_SEND;
        } else {
            isUpdate = false;
            return;
        }
        uint8 addr = message.data.at(1);
        uint8 font = message.data.at(3);
        QString file = message.data.mid(4,message.data.size()-4);

        if(fileUpdate(cmd,font,addr,file)) {
            message.data.remove(4,message.data.size()-4);
            message.data += SystemUtils::u8ToQByteArray(0);
            emit toNetwork(message);
        } else {
            message.data.remove(4,message.data.size()-4);
            message.data += SystemUtils::u8ToQByteArray(1);
            emit toNetwork(message);
        }
        isUpdate = false;
    } else {
        if(0 == heartbeatTimer) {
               queryState();
        } else {
            if(2 == sem.available()) {
                led_data_info* info;
                info = sList->get();
                if(NULL != info) {
                    sendPack(info->cmd,info->addr,info->data);
                }
            }
        }
    }
}

void LedManager::generalReplyHandler(QByteArray data)
{
    if(1 == sem.available()) {
        uint16 batch = (data.at(10)<<8)|data.at(11);
        MY_DEBUG(QString("batch=%1 mBatch=%2").arg(batch).arg(mBatch));
        if(batch == mBatch) {
            sem.release(1);
            sList->remove(mCmd);
        }
    }
}

void LedManager::stateReplyHandler(QByteArray data)
{
    if(1 == sem.available()) {
        uint16 batch = (data.at(10)<<8)|data.at(11);
        MY_DEBUG(QString("batch=%1 mBatch=%2").arg(batch).arg(mBatch));
        if(batch == mBatch) {
            sem.release(1);
            if(false == sys->peripheralState[devIndex].is_connect) {
                sys->peripheralState[devIndex].is_connect = true;
            }
            devIndex += 1;
            if(devIndex == LED_DEVICE_SUM) {
                devIndex = 0;
                heartbeatTimer = LED_BEATTIM;
            }
        }
    }
}

void LedManager::paraReplyHandler(QByteArray data)
{
    if(1 == sem.available()) {
        uint16 batch = (data.at(10)<<8)|data.at(11);
        MY_DEBUG(QString("batch=%1 mBatch=%2").arg(batch).arg(mBatch));
        if(batch == mBatch) {
            sem.release(1);
            sList->remove(mCmd);
            Message msg;
            msg.id = SERVE_PERIPHERAL_CONTROL;
            msg.data = SystemUtils::u16ToQByteArray(netBatch)+SystemUtils::u16ToQByteArray(netCmd)
                    +SystemUtils::u8ToQByteArray(0)+data.mid(12,data.size()-14);
            emit toNetwork(msg);
        }
    }
}

uint8 LedManager::send(uint8 type,uint8 device,QList<QByteArray> data)
{
    uint8 ret = 0;

    int32 size = data.size();
    if(0 == size) {
        return ret;
    }

    switch(type) {
    case DATA_TYPE_LINE:
        info.type = MSG_TYPE_UI;
        info.cmd = CMD_DATA_SEND;
        info.data = SystemUtils::u8ToQByteArray(type);
        info.data += data[0];
        if(LED_TYPE_HEAD == (device&LED_TYPE_HEAD)) {
            info.addr = LED_HEAD_REAL_ID;
            if(sList->add(info)) {
                ret |= LED_TYPE_HEAD;
            }
        }

        if(LED_TYPE_WAIST == (device&LED_TYPE_WAIST)) {
            info.addr = LED_WAIST_REAL_ID;
            if(sList->add(info)) {
                ret |= LED_TYPE_WAIST;
            }
        }

        if(LED_TYPE_TAIL == (device&LED_TYPE_TAIL)) {
            info.addr = LED_TAIL_REAL_ID;
            if(sList->add(info)) {
                ret |= LED_TYPE_TAIL;
            }
        }
        break;

    case DATA_TYPE_AD:
        if(LED_TYPE_INSIDE == device) {
            info.type = MSG_TYPE_UI;
            info.cmd = CMD_DATA_SEND;
            info.addr = LED_INSIDE_REAL_ID;
            for(int8 i = 0;i < size;i++) {
                info.data = SystemUtils::u8ToQByteArray(type);
                info.data += SystemUtils::u8ToQByteArray(i);
                info.data += SystemUtils::u8ToQByteArray(data[i].size());
                info.data += data[i];
                if(sList->add(info)) {
                    ret |= LED_TYPE_INSIDE;
                } else {
                    ret &= ~LED_TYPE_INSIDE;
                }
            }
        }
        break;
    }
    return ret;
}
