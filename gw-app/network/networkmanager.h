#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>
#include <QTimer>
#include "constant.h"
#include "system.h"
#include "type.h"
#include "network/networkmessage.h"
#include "network/httptask.h"
#include "network/socketclient.h"
#include "network/networkmessage.h"
#include "network/downloadqueue.h"
#include "gps/gps.h"
#include "led/ledworker.h"

class NetworkManager : public QObject
{
    Q_OBJECT
protected:
    explicit NetworkManager(QObject *parent = 0);

public:
    static NetworkManager* getInstance();
    void networkConnect();

private:
    void clientGeneralReply(uint16 msg_id,uint16 msg_batch,uint8 result);    //通用应答
    void generalReplyHandler(Message msg);      //服务器应答处理
    void clientHeartbeat();                     //终端心跳
    void clientSignUp();                        //终端注册
    bool signUpReplyHandler(Message msg);       //服务器注册应答处理
    void clientLogout();                        //终端注销
    void clientAuthentication();                //终端鉴权
    void setClientParaHandler(Message msg);     //服务器设置参数处理
    void queryClientParaHandler(Message msg);   //服务器查询参数处理
    void queryClientInfoHandler(Message msg);   //服务器查询设备信息处理
    void fileUpdateHandler(Message msg);        //文件更新处理
    void peripheralControlHandler(Message msg); //外设控制处理
    void peripheralControlReply(Message msg);   //外设控制应答
    void dataTransparentHandler(Message msg);   //服务器数据透传处理

private:
    HttpTask* mHttpTask;
    SocketClient* mSocketClient;
    NetworkMessage* mMessage;
    DownloadQueue* mDownloadQueue;
    QTimer* mTimer;
    System* sys;
    sys_para para;
    QString authentication_code;
//    LedWorker RE485_1;

    volatile bool isDial;
    volatile bool mSocketClientFlag;
    volatile int32 mSocketClientTime;
    volatile bool mSignUpFlag;
    volatile int32 mSignUpTime;
    volatile bool mSignInFlag;
    volatile int32 mSignInTime;
    volatile uint16 mHeartbeatBatch;
    volatile int32 mHeartbeatTime;
    volatile int32 mHeartbeatOvertime;
    volatile int32 mHeartbeatCount;
    volatile bool mHeartbeatFlag;
    volatile int32 gpsUploadTime;
    
signals:
    void toLed(Message msg);
    void toLcd(Message msg);
    void toLocal(Message msg);
    
public slots:
    void ledHandler(Message msg);
    void lcdHandler(Message msg);
    void canHandler(Message msg);
    void gpsHandler(gps_data data);
    void localHandler(Message msg);

private slots:
    void timeout();
    void socketMsg(Message msg);
    void socketConnect(bool is);
    void haveIMSI();
};

#endif // NETWORKMANAGER_H
