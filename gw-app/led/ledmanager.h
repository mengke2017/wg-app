#ifndef LEDMANAGER_H
#define LEDMANAGER_H

#include <QObject>
#include <QMutex>
#include <QTimer>
#include "type.h"
#include "led/ledworker.h"
#include "common/message.h"
#include "busbox/busbox.h"
#include "led/ledsendlist.h"

#define LED_DEVICE_SUM      5
#define LOCAL_REAL_ID       0x00
#define LED_HEAD_REAL_ID    0x01
#define LED_WAIST_REAL_ID   0x02
#define LED_TAIL_REAL_ID    0x03
#define LED_INSIDE_REAL_ID  0x04
#define LED_AD_REAL_ID      0x05

#define CMD_GENERAL_REPLY   0x80
#define CMD_DATA_SEND       0x01
#define CMD_FONT_SEND       0x02
#define CMD_APP_SEND        0x03
#define CMD_PARA_SEND       0x04
#define CMD_QUERY_STATE     0x05
#define CMD_STATE_REPLY     0x85
#define CMD_QUERY_PARA      0x06
#define CMD_PARA_REPLY      0x86

#define DATA_TYPE_LINE      0x01
#define DATA_TYPE_AD        0x02
#define DATA_TYPE_TMP       0x03
#define DATA_TYPE_SITE      0x04
#define DATA_TYPE_SLG       0x05  // 广告信息 按键调用
#define DATA_TYPE_KEY       0x06  // 广告信息 按键调用

#define LED_TYPE_HEAD       0x01
#define LED_TYPE_WAIST      0x02
#define LED_TYPE_TAIL       0x04
#define LED_TYPE_INSIDE     0x08

#define MSG_TYPE_UI         0x01
#define MSG_TYPE_NET        0x02
#define MSG_TYPE_BOX        0x03

const uint8 LedDevice[LED_DEVICE_SUM] = {
    LED_HEAD_REAL_ID,LED_WAIST_REAL_ID,
    LED_TAIL_REAL_ID,LED_INSIDE_REAL_ID,LED_AD_REAL_ID
};

class NetworkManager;
class System;
class LedManager : public QObject
{
    Q_OBJECT
protected:
    explicit LedManager(QObject *parent = 0);

public:
    static LedManager* getInstance();
    uint8 send(uint8 type,uint8 device,QList<QByteArray> data);
    
private:
    void generalReplyHandler(QByteArray data);
    void fontUpdateReplyHandler(QByteArray data);
    void appUpdateReplyHandler(QByteArray data);
    void stateReplyHandler(QByteArray data);
    void paraReplyHandler(QByteArray data);
    void sendPack(uint8 cmd,uint8 dstaddr,QByteArray data);
    bool fileUpdate(uint8 cmd,uint8 fonttype,uint8 addr,QString file);
    void queryState();

private:
    System* sys;
    LedWorker* ledWorker;
    LedSendList* sList;
    QTimer* mTimer;
    QTimer* sTimer;

    volatile uint16 replyTimer;
    volatile uint16 heartbeatTimer;
    volatile bool isUpdate;
    volatile uint8 devIndex;
    volatile uint16 mCmd;
    volatile uint16 mBatch;
    volatile uint16 netCmd;
    volatile uint16 netBatch;
    Message message;
    led_data_info info;
    
signals:
    void toNetwork(Message msg);
    void UpdateOK(int flag);

public slots:
    void netWorkHandler(Message msg);
    void busboxHandler(bus_message msg);

private slots:
    void slotComDataHandler(QByteArray data);
    void slotTimeOut();
    void slotSendTask();
};

#endif // LEDMANAGER_H
