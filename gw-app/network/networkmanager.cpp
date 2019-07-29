#include "networkmanager.h"
#include "common/systemutils.h"
#include "network/socketconstant.h"
#include "common/mydebug.h"
#include "gpio/gpio.h"

#define CLIENT_CONNECT_INTERVAL 5*60
#define CLIENT_SIGNUP_INTERVAL  3*60
#define CLIENT_SIGNIN_INTERVAL  3*60
#define HEARTBEAT_INTERVAL      60
#define HEARTBEAT_OVERTIME      5
#define HEARTBEAT_COUNT_MAX     3

NetworkManager::NetworkManager(QObject *parent) :
    QObject(parent)
{
    sys = System::getInstance();
    sys->getSystemPara(&para);
    isDial = false;
    mSocketClientFlag = false;
    mSignInFlag = false;
    mSignUpFlag = false;
    authentication_code = "";
    mHeartbeatTime = 0;
    mHeartbeatOvertime = 0;
    mHeartbeatCount = 0;
    mHeartbeatFlag = true;
    gpsUploadTime = para.gps_upload_interval;
    MY_SYSTEM("NetworkManager Init");

}

NetworkManager* NetworkManager::getInstance()
{
    static NetworkManager instance;
    return &instance;
}

void NetworkManager::networkConnect()
{
    mMessage = NetworkMessage::getInstance();
    mHttpTask = HttpTask::getInstance();
    mDownloadQueue = DownloadQueue::getInstance();
    mSocketClient = SocketClient::getInstance();
    connect(mSocketClient,SIGNAL(dataPacketReady(Message)),this,SLOT(socketMsg(Message)));
    connect(mSocketClient,SIGNAL(socketConnected(bool)),this,SLOT(socketConnect(bool)));

    if(FileUtils::exists(DIAL_FLAG_FILE)) {
        isDial = true;
        QHostAddress address(para.ip);
        mSocketClient->connectServer(address,para.port);
        if(mSocketClient->isConnected()) {
            mSocketClientFlag = true;
            if(mSignUpFlag) {
                clientAuthentication();
                mSignInTime = CLIENT_SIGNIN_INTERVAL;
            } else {

                clientSignUp();
                mSignUpTime = CLIENT_SIGNUP_INTERVAL;
            }
            MY_SYSTEM("SocketClient Connect success!");
        } else {
            MY_WARNING("SocketClient Connect fail1!");
            mSocketClientTime = CLIENT_CONNECT_INTERVAL;
        }
    } else {
        MY_SYSTEM("4g connecting...");
    }

    mTimer = new QTimer(this);
    connect(mTimer,SIGNAL(timeout()),this,SLOT(timeout()));
    mTimer->start(1000);
}

void NetworkManager::clientGeneralReply(uint16 msg_id,uint16 msg_batch,uint8 result)   //通用应答
{
    MY_DEBUG(QString("id=%1").arg(msg_id));
    uint16 id = CLIENT_GENERAL_REPLY;
    uint16 attr = 5;
    uint16 batch = SystemUtils::rand16();
    QByteArray pack = SystemUtils::u16ToQByteArray(id) + SystemUtils::u16ToQByteArray(attr)
            + sys->dev_id + SystemUtils::u16ToQByteArray(batch) + SystemUtils::u16ToQByteArray(msg_batch)
            + SystemUtils::u16ToQByteArray(msg_id) + SystemUtils::u8ToQByteArray(result);
    mSocketClient->send(pack);
}

void NetworkManager::generalReplyHandler(Message msg)   //服务器应答处理
{
    uint16 id = (msg.data.at(2)<<8)|msg.data.at(3);
    MY_DEBUG(QString("id=%1").arg(id));
    if(CLIENT_AUTHENTICATION == id) {
        mSignInFlag = true;
    } else if (CLIENT_HEARTBEAT == id) {
        uint16 batch = (msg.data.at(0)<<8)|msg.data.at(1);
        if(mHeartbeatBatch == batch) {
            mHeartbeatFlag = true;
            mHeartbeatCount = 0;
        }
    } else {
        uint16 batch = (msg.data.at(0)<<8)|msg.data.at(1);
        NetworkMessage::NetMessage netMsg;
        netMsg.count = 0;
        mMessage->setByBatch(batch,netMsg);
    }
}

void NetworkManager::clientHeartbeat()  //终端心跳
{
    uint16 id = CLIENT_HEARTBEAT;
    mHeartbeatBatch = SystemUtils::rand16();
    uint8 sum = 0;
    QByteArray pack;

/*    for (int i = 0; i < DEVICE_SUM; i++) {
            sys->peripheralState[i].is_connect = true;
    }  // 测试设备上线状态
*/
    for(uint8 i=0;i<DEVICE_SUM;i++)
    {
        if(sys->peripheralState[i].is_connect) {
            pack += SystemUtils::u8ToQByteArray(sys->peripheralState[i].id);
            sum += 1;
        }
    }
    pack = sys->serialNumber+sys->softwareVersion + SystemUtils::u8ToQByteArray(sum) + pack;
//    qWarning()<<pack.toHex()<<"\n";
    uint16 attr = pack.size();
    pack = SystemUtils::u16ToQByteArray(id) + SystemUtils::u16ToQByteArray(attr)
            + sys->dev_id + SystemUtils::u16ToQByteArray(mHeartbeatBatch) + pack + gpioInputData;
//    qWarning()<<pack.toHex()<<"\n";
    mSocketClient->send(pack);
    mHeartbeatFlag = false;
    mHeartbeatOvertime = HEARTBEAT_OVERTIME;
    mHeartbeatTime = HEARTBEAT_INTERVAL;
}

void NetworkManager::clientSignUp() //终端注册
{
    uint16 id = CLIENT_SIGN_IN;
    uint16 batch = SystemUtils::rand16();

    QByteArray pack = SystemUtils::u16ToQByteArray(para.province_id) + SystemUtils::u16ToQByteArray(para.city_id) + sys->manufacturer
            + sys->model + sys->serialNumber + sys->sim + SystemUtils::u8ToQByteArray(para.license_plate_color);
    if(0 == para.license_plate_color) {
        pack += para.license_plate;
    } else {
        pack += para.vin;
    }
    uint16 attr = pack.size();
    pack = SystemUtils::u16ToQByteArray(id) + SystemUtils::u16ToQByteArray(attr)
            + sys->dev_id + SystemUtils::u16ToQByteArray(batch) + pack;
    mSocketClient->send(pack);
}

bool NetworkManager::signUpReplyHandler(Message msg)    //服务器注册应答处理
{
    uint8 result = msg.data.at(2);
    if(0 == result) {
        authentication_code = msg.data.mid(3,msg.data.size()-3);
        if(0 != authentication_code.size()) {

            return true;
        } else {
            MY_WARNING("authentication_code null");
        }
    } else {
        MY_WARNING(QString("result=%1").arg(result));
    }

    return false;
}

void NetworkManager::clientLogout() //终端注销
{
    uint16 id = CLIENT_LOGOUT;
    uint16 batch = SystemUtils::rand16();
    uint16 attr = 0;
    QByteArray pack = SystemUtils::u16ToQByteArray(id) + SystemUtils::u16ToQByteArray(attr)
            + sys->dev_id + SystemUtils::u16ToQByteArray(batch);
    mSocketClient->send(pack);
}

void NetworkManager::clientAuthentication() //终端鉴权
{
    uint16 id = CLIENT_AUTHENTICATION;
    uint16 batch = SystemUtils::rand16();
    QByteArray pack = authentication_code.toLatin1();
    uint16 attr = pack.size();
    pack = SystemUtils::u16ToQByteArray(id) + SystemUtils::u16ToQByteArray(attr)
            + sys->dev_id + SystemUtils::u16ToQByteArray(batch) + pack;
    mSocketClient->send(pack);
}

void NetworkManager::setClientParaHandler(Message msg) //服务器设置参数处理
{
    MY_DEBUG(QString("id=%1 batch=%2").arg(msg.id).arg(msg.batch));
    clientGeneralReply(msg.id,msg.batch,0);

    uint16 id;
    uint8 len;
    bool flag = false;
    uint32 tmp_dword;
    uint16 tmp_word;
    uint8 tmp_byte;
    QByteArray tmp_bytes;
    QString tmp_string;

    int32 i = 1;
    int32 size = msg.data.size();

    while(i<size)
    {
        id = (msg.data[i]<<8)|msg.data[i+1];
        len = msg.data[i+2];
        switch(id)
        {
            case PARA_HEARTBEAT:
                tmp_dword = (msg.data[i+3]<<24)|(msg.data[i+4]<<16)|(msg.data[i+5]<<8)|msg.data[i+6];
                para.heartbeat_cycle = tmp_dword;
                break;

            case PARA_IP:{
                tmp_bytes = msg.data.mid(i+3,len);
                tmp_string = SystemUtils::gbkToUnicode(tmp_bytes);
                if(tmp_string.compare(para.ip) != 0) {
                    para.ip = tmp_string;
                    flag = true;
                }
                break;}

            case PARA_PORT:
                tmp_dword = (msg.data[i+3]<<24)|(msg.data[i+4]<<16)|(msg.data[i+5]<<8)|msg.data[i+6];
                if(tmp_dword != para.port) {
                    para.port = tmp_dword;
                    flag = true;
                }
                break;

            case PARA_REPLY_TIMEOUT:
                tmp_dword = (msg.data[i+3]<<24)|(msg.data[i+4]<<16)|(msg.data[i+5]<<8)|msg.data[i+6];
                para.reply_timeout = tmp_dword;
                break;

            case PARA_RETRANS:
                tmp_dword = (msg.data[i+3]<<24)|(msg.data[i+4]<<16)|(msg.data[i+5]<<8)|msg.data[i+6];
                para.max_retrans = tmp_dword;
                break;

            case PARA_SPEED:
                tmp_dword = (msg.data[i+3]<<24)|(msg.data[i+4]<<16)|(msg.data[i+5]<<8)|msg.data[i+6];
                para.max_speed = tmp_dword;
                break;

            case PARA_OVERSPEED_TIME:
                tmp_dword = (msg.data[i+3]<<24)|(msg.data[i+4]<<16)|(msg.data[i+5]<<8)|msg.data[i+6];
                para.overspeed_time = tmp_dword;
                break;

            case PARA_CAN_ACQUISITION:
                tmp_dword = (msg.data[i+3]<<24)|(msg.data[i+4]<<16)|(msg.data[i+5]<<8)|msg.data[i+6];
                para.can_collect_interval = tmp_dword;
                break;

            case PARA_CAN_UPLOAD:
                tmp_word = (msg.data[i+3]<<8)|msg.data[i+4];
                para.can_upload_interval = tmp_word;
                break;

            case PARA_GPS_UPLOAD:
                tmp_word = (msg.data[i+3]<<8)|msg.data[i+4];
                para.gps_upload_interval = tmp_word;
                break;

            case PARA_PROVINCE_ID:
                tmp_word = (msg.data[i+3]<<8)|msg.data[i+4];
                para.province_id = tmp_word;
                break;

            case PARA_CITY_ID:
                tmp_word = (msg.data[i+3]<<8)|msg.data[i+4];
                para.city_id = tmp_word;
                break;

            case PARA_LICENSE_PLATE:{
                tmp_bytes = msg.data.mid(i+3,len);
                tmp_string = SystemUtils::gbkToUnicode(tmp_bytes);
                para.license_plate = tmp_string;
                break;}

            case PARA_LICENSE_PLATE_COLOR:
                tmp_byte = msg.data[i+3];
                para.license_plate_color = tmp_byte;
                break;
/*
            case PARA_CONF_NAME:
                tmp_bytes = msg.data.mid(i+3,len);
                tmp_string = SystemUtils::gbkToUnicode(tmp_bytes);
                para.conf_user = ;
                break;

            case PARA_CONF_PASS:
                tmp_bytes = msg.data.mid(i+3,len);
                tmp_string = SystemUtils::gbkToUnicode(tmp_bytes);
                para.conf_pass = ;
                break;
*/
        }
        i = i+len+3;
    }

    if(flag) {
        MY_WARNING("ip or port change");
        clientLogout();
        authentication_code = "";
    }
    sys->setSystemPara(para);
}

void NetworkManager::queryClientParaHandler(Message msg) //服务器查询参数处理
{
    int32 size = msg.data.size();
    uint8 count = msg.data[0];

    if((size !=0)&&(size != (2*count + 1)))
        return;

    MY_DEBUG(QString("para count=%1").arg(count));
    uint16 id = CLIENT_QUERY_PARA_REPLY;
    uint16 batch = SystemUtils::rand16();
    QByteArray data = SystemUtils::u16ToQByteArray(msg.batch);
    if(size == 0) {
        data += SystemUtils::u8ToQByteArray(PARA_NUMER);  // 参数个数

        data += SystemUtils::u16ToQByteArray(PARA_HEARTBEAT);
        data += SystemUtils::u8ToQByteArray(4);
        data += SystemUtils::u32ToQByteArray(para.heartbeat_cycle);

        data += SystemUtils::u16ToQByteArray(PARA_IP);
        data += SystemUtils::u8ToQByteArray(para.ip.size());
        data += para.ip.toLatin1();

        data += SystemUtils::u16ToQByteArray(PARA_PORT);
        data += SystemUtils::u8ToQByteArray(4);
        data += SystemUtils::u32ToQByteArray(para.port);

        data += SystemUtils::u16ToQByteArray(PARA_REPLY_TIMEOUT);
        data += SystemUtils::u8ToQByteArray(4);
        data += SystemUtils::u32ToQByteArray(para.reply_timeout);

        data += SystemUtils::u16ToQByteArray(PARA_RETRANS);
        data += SystemUtils::u8ToQByteArray(4);
        data += SystemUtils::u32ToQByteArray(para.max_retrans);

        data += SystemUtils::u16ToQByteArray(PARA_SPEED);
        data += SystemUtils::u8ToQByteArray(4);
        data += SystemUtils::u32ToQByteArray(para.max_speed);

        data += SystemUtils::u16ToQByteArray(PARA_OVERSPEED_TIME);
        data += SystemUtils::u8ToQByteArray(4);
        data += SystemUtils::u32ToQByteArray(para.overspeed_time);

        data += SystemUtils::u16ToQByteArray(PARA_CAN_ACQUISITION);
        data += SystemUtils::u8ToQByteArray(4);
        data += SystemUtils::u32ToQByteArray(para.can_collect_interval);

        data += SystemUtils::u16ToQByteArray(PARA_CAN_UPLOAD);
        data += SystemUtils::u8ToQByteArray(2);
        data += SystemUtils::u16ToQByteArray(para.can_upload_interval);

        data += SystemUtils::u16ToQByteArray(PARA_GPS_UPLOAD);
        data += SystemUtils::u8ToQByteArray(2);
        data += SystemUtils::u16ToQByteArray(para.gps_upload_interval);

        data += SystemUtils::u16ToQByteArray(PARA_PROVINCE_ID);
        data += SystemUtils::u8ToQByteArray(2);
        data += SystemUtils::u16ToQByteArray(para.province_id);

        data += SystemUtils::u16ToQByteArray(PARA_CITY_ID);
        data += SystemUtils::u8ToQByteArray(2);
        data += SystemUtils::u16ToQByteArray(para.city_id);

        data += SystemUtils::u16ToQByteArray(PARA_LICENSE_PLATE);
        QByteArray tmpbytes;
        if(para.license_plate_color == 0) {
            tmpbytes = SystemUtils::unicodeToGbk(para.vin);
        } else {
            tmpbytes = SystemUtils::unicodeToGbk(para.license_plate);
        }
        data += SystemUtils::u8ToQByteArray(tmpbytes.size());
        data += tmpbytes;
        data += SystemUtils::u16ToQByteArray(PARA_LICENSE_PLATE_COLOR);
        data += SystemUtils::u8ToQByteArray(1);
        data += SystemUtils::u8ToQByteArray(para.license_plate_color);

        data += SystemUtils::u16ToQByteArray(PARA_DELAY_TIME);
        data += SystemUtils::u8ToQByteArray(4);
        data += SystemUtils::u32ToQByteArray(para.delay_time);

        data += SystemUtils::u16ToQByteArray(PARA_LOGIN_PASS);
        data += SystemUtils::u8ToQByteArray(para.login_pass.size());
        data += para.login_pass.toLatin1();
    } else {
        data += SystemUtils::u8ToQByteArray(count);
        for(quint8 i=0;i<count;i++)
        {
            quint16 tmp = (msg.data[2*i+1]<<8)|msg.data[2*i+2];
            switch(tmp)
            {
                case PARA_HEARTBEAT:
                    data += SystemUtils::u16ToQByteArray(PARA_HEARTBEAT);
                    data += SystemUtils::u8ToQByteArray(4);
                    data += SystemUtils::u32ToQByteArray(para.heartbeat_cycle);
                    break;

                case PARA_IP:
                    data += SystemUtils::u16ToQByteArray(PARA_IP);
                    data += SystemUtils::u8ToQByteArray(para.ip.size());
                    data += para.ip.toLatin1();
                    break;

                case PARA_PORT:
                    data += SystemUtils::u16ToQByteArray(PARA_PORT);
                    data += SystemUtils::u8ToQByteArray(4);
                    data += SystemUtils::u32ToQByteArray(para.port);
                    break;

                case PARA_REPLY_TIMEOUT:
                    data += SystemUtils::u16ToQByteArray(PARA_REPLY_TIMEOUT);
                    data += SystemUtils::u8ToQByteArray(4);
                    data += SystemUtils::u32ToQByteArray(para.reply_timeout);
                    break;

                case PARA_RETRANS:
                    data += SystemUtils::u16ToQByteArray(PARA_RETRANS);
                    data += SystemUtils::u8ToQByteArray(4);
                    data += SystemUtils::u32ToQByteArray(para.max_retrans);
                    break;

                case PARA_SPEED:
                    data += SystemUtils::u16ToQByteArray(PARA_SPEED);
                    data += SystemUtils::u8ToQByteArray(4);
                    data += SystemUtils::u32ToQByteArray(para.max_speed);
                    break;

                case PARA_OVERSPEED_TIME:
                    data += SystemUtils::u16ToQByteArray(PARA_OVERSPEED_TIME);
                    data += SystemUtils::u8ToQByteArray(4);
                    data += SystemUtils::u32ToQByteArray(para.overspeed_time);
                    break;

                case PARA_CAN_ACQUISITION:
                    data += SystemUtils::u16ToQByteArray(PARA_CAN_ACQUISITION);
                    data += SystemUtils::u8ToQByteArray(4);
                    data += SystemUtils::u32ToQByteArray(para.can_collect_interval);
                    break;

                case PARA_CAN_UPLOAD:
                    data += SystemUtils::u16ToQByteArray(PARA_CAN_UPLOAD);
                    data += SystemUtils::u8ToQByteArray(2);
                    data += SystemUtils::u16ToQByteArray(para.can_upload_interval);
                    break;

                case PARA_GPS_UPLOAD:
                    data += SystemUtils::u16ToQByteArray(PARA_GPS_UPLOAD);
                    data += SystemUtils::u8ToQByteArray(2);
                    data += SystemUtils::u16ToQByteArray(para.gps_upload_interval);
                    break;

                case PARA_PROVINCE_ID:
                    data += SystemUtils::u16ToQByteArray(PARA_PROVINCE_ID);
                    data += SystemUtils::u8ToQByteArray(2);
                    data += SystemUtils::u16ToQByteArray(para.province_id);
                    break;

                case PARA_CITY_ID:
                    data += SystemUtils::u16ToQByteArray(PARA_CITY_ID);
                    data += SystemUtils::u8ToQByteArray(2);
                    data += SystemUtils::u16ToQByteArray(para.city_id);
                    break;

                case PARA_LICENSE_PLATE:
                {
                    data += SystemUtils::u16ToQByteArray(PARA_LICENSE_PLATE);
                    QByteArray tmpbytes;
                    if(para.license_plate_color == 0) {
                        tmpbytes = SystemUtils::unicodeToGbk(para.vin);
                    } else {
                        tmpbytes = SystemUtils::unicodeToGbk(para.license_plate);
                    }
                    data += SystemUtils::u8ToQByteArray(tmpbytes.size());
                    data += tmpbytes;
                    break;
                }

                case PARA_LICENSE_PLATE_COLOR:
                    data += SystemUtils::u16ToQByteArray(PARA_LICENSE_PLATE_COLOR);
                    data += SystemUtils::u8ToQByteArray(1);
                    data += SystemUtils::u8ToQByteArray(para.license_plate_color);
                    break;

                case PARA_DELAY_TIME:
                    data += SystemUtils::u16ToQByteArray(PARA_DELAY_TIME);
                    data += SystemUtils::u8ToQByteArray(4);
                    data += SystemUtils::u32ToQByteArray(para.delay_time);
                    break;
                case PARA_LOGIN_PASS:
                    data += SystemUtils::u16ToQByteArray(PARA_LOGIN_PASS);
                    data += SystemUtils::u8ToQByteArray(para.login_pass.size());
                    data += para.login_pass.toLatin1();
                    break;
            }
        }
    }

    uint16 attr = data.size();
    data = SystemUtils::u16ToQByteArray(id) + SystemUtils::u16ToQByteArray(attr)
            + sys->dev_id + SystemUtils::u16ToQByteArray(batch) + data;
    mSocketClient->send(data);
}

void NetworkManager::queryClientInfoHandler(Message msg)   //服务器查询设备信息处理
{
    MY_DEBUG("queryClientInfoHandler");
    uint16 id = CLIENT_QUERY_INFO_REPLY;
    uint16 batch = SystemUtils::rand16();

    QByteArray pack = SystemUtils::u16ToQByteArray(msg.batch) + sys->manufacturer + sys->productionDate
            + sys->model + sys->serialNumber + SystemUtils::u8ToQByteArray(8) + sys->hardwareVersion
            + SystemUtils::u8ToQByteArray(8) + sys->softwareVersion + SystemUtils::u8ToQByteArray(8) + sys->firmwareVersion;
    uint16 attr = pack.size();
    pack = SystemUtils::u16ToQByteArray(id) + SystemUtils::u16ToQByteArray(attr)
            + sys->dev_id + SystemUtils::u16ToQByteArray(batch) + pack;
    mSocketClient->send(pack);
    qWarning()<<pack.toHex()<<"\n";
}

void NetworkManager::fileUpdateHandler(Message msg)    //文件更新处理
{
    clientGeneralReply(msg.id,msg.batch,0);

    DownloadQueue::TaskInfo info;
    info.filetype = (msg.data[0]<<24)|(msg.data[1]<<16)|(msg.data[2]<<8)|msg.data[3];
    info.md5 = msg.data.mid(4,32);
    info.url = msg.data.mid(36,(msg.data.size()-35));
    MY_DEBUG(QString("type=%1 url=%2").arg(info.filetype).arg(info.url));
    mDownloadQueue->addDownloadTask(info);
}

void NetworkManager::peripheralControlHandler(Message msg)  //外设控制处理
{    
    int32 deviceID = (msg.data[0]<<8)|msg.data[1];
    MY_DEBUG(QString("deviceID=%1").arg(deviceID));
    switch(deviceID)
    {
        case ADAPTER_ID:
            emit toLocal(msg);
            break;

        case LED_HEAD_ID:
        case LED_WAIST_ID:
        case LED_TAIL_ID:
        case LED_INSIDE_ID:
        case LED_AD_ID:
            emit toLed(msg);
            break;
    }
}

void NetworkManager::peripheralControlReply(Message msg)
{
    MY_DEBUG("peripheralControlReply");
    uint16 id = CLIENT_PERIPHERAL_CONTROL_REPLY;
    uint16 attr = msg.data.size();
    uint16 batch = SystemUtils::rand16();
    QByteArray pack = SystemUtils::u16ToQByteArray(id) + SystemUtils::u16ToQByteArray(attr)
            + sys->dev_id + SystemUtils::u16ToQByteArray(batch) + msg.data;
    mSocketClient->send(pack);
}

void NetworkManager::dataTransparentHandler(Message msg)    //服务器数据透传处理
{
    clientGeneralReply(msg.id,msg.batch,0);
    uint8 type = msg.data[0];
    MY_DEBUG(QString("type=%d").arg(type));
    if(LCD_TRANSPARENT == type) {
        emit toLcd(msg);
    }
}

void NetworkManager::timeout()
{
    if(!isDial) {
        if(FileUtils::exists(DIAL_FLAG_FILE)) {
            isDial = true;
        } else {
            MY_SYSTEM("4g connecting...");
            return;
        }
    }

    if((sys->paraFlag&PARA_TYPE_IP)
            ||(sys->paraFlag&PARA_TYPE_PORT
               ||(sys->paraFlag&PARA_TYPE_DEV_ID))) {
        MY_WARNING("ip or port or id update!");
        sys->paraFlag &= ~PARA_TYPE_IP;
        sys->paraFlag &= ~PARA_TYPE_PORT;
        sys->paraFlag &= ~PARA_TYPE_DEV_ID;
        sys->getSystemPara(&para);
        mSocketClientFlag = false;
        mSocketClientTime = 5;
        mSignUpFlag = false;
        mSignInFlag = false;
        mSocketClient->stop();
    }

    if(!mSocketClientFlag) {
        if(0 == mSocketClientTime) {
            QHostAddress address(para.ip);
            mSocketClient->connectServer(address,para.port);
            if(mSocketClient->isConnected()) {
                mSocketClientFlag = true;
                if(mSignUpFlag) {
                    clientAuthentication();
                    mSignInTime = CLIENT_SIGNIN_INTERVAL;
                } else {
                    clientSignUp();
                    mSignUpTime = CLIENT_SIGNUP_INTERVAL;
                }
                MY_SYSTEM("SocketClient Connect success!");
            } else {
                MY_WARNING("SocketClient Connect fail2!");
                mSocketClientTime = CLIENT_CONNECT_INTERVAL;
            }
        } else {
            mSocketClientTime -= 1;
        }
    } else {
        if(mSignUpFlag) {
            if(mSignInFlag) {
                if(gpsUploadTime > 0) {
                    gpsUploadTime -= 1;
                }

                /*if(mHeartbeatOvertime > 0) {
                    mHeartbeatOvertime -= 1;
                } else {
                    if(!mHeartbeatFlag) {
                        mHeartbeatCount += 1;
                        if(HEARTBEAT_COUNT_MAX < mHeartbeatCount) {
                            mHeartbeatCount = 0;
                            mSocketClient->stop();
                            mSocketClientFlag = false;
                            mSocketClientTime = 5;
                            mSignUpFlag = false;
                            mSignInFlag = false;
                            MY_WARNING("SocketClient disconnect!");
                        } else {
                            clientHeartbeat();
                        }
                    }
                }*/

                if(mHeartbeatTime > 0) {
                    mHeartbeatTime -= 1;
                } else {
                    clientHeartbeat();
/*                    for(int i = 0; i < 8; i--) {
                        ioctl()
                    } */
                }

                NetworkMessage::NetMessage netmsg;
                int32 size = mMessage->size();
                for(int32 i=0;i<size;)
                {
                    mMessage->get(i,&netmsg);
                    if(netmsg.count == 0) {
                        mMessage->remove(i);
                        size -= 1;
                        break;
                    }

                    if(netmsg.time == 0) {
                        QByteArray tmp = SystemUtils::u16ToQByteArray(netmsg.msg.id) + SystemUtils::u16ToQByteArray(netmsg.msg.data.size())
                                + sys->dev_id + SystemUtils::u16ToQByteArray(netmsg.msg.batch) + netmsg.msg.data;
                        mSocketClient->send(tmp);
                        netmsg.time = para.reply_timeout;
                        if(netmsg.count == 1) {
                            mMessage->remove(i);
                            size -= 1;
                            break;
                        } else {
                            netmsg.count -= 1;
                            mMessage->setByIndex(i,netmsg);
                        }
                    } else {
                        netmsg.time -= 1;
                        mMessage->setByIndex(i,netmsg);
                    }
                    i += 1;
                }
            } else {
                if(0 == mSignInTime) {
                    clientAuthentication();
                    mSignInTime = CLIENT_SIGNIN_INTERVAL;
                } else {
                    mSignInTime -= 1;
                }
            }
        } else {
            if(0 == mSignUpTime) {
                clientSignUp();
                mSignUpTime = CLIENT_SIGNUP_INTERVAL;
            } else {
                mSignUpTime -= 1;
            }
        }
    }

    if(mDownloadQueue->getTaskStop()) {
        mHttpTask->stop();
        mDownloadQueue->rmDownloadTask();
    }

    switch(mHttpTask->getTaskState())
    {
        case HttpTask::STATE_ERROR:
        {
            mHttpTask->stop();
            DownloadQueue::TaskInfo info;
            info.filetype = 0;
            mDownloadQueue->getDownloadTask(&info);
            if(info.filetype != 0) {
                NetworkMessage::NetMessage netmsg;
                netmsg.msg.id = CLIENT_FILE_UPDATE_RESULT;
                netmsg.msg.data = SystemUtils::u16ToQByteArray((info.filetype>>16)&0xffff);
                netmsg.msg.data += SystemUtils::u16ToQByteArray(info.filetype&0xffff);
                netmsg.msg.data += SystemUtils::u8ToQByteArray(1);
                netmsg.msg.batch = SystemUtils::rand16();
                netmsg.time = 0;
                netmsg.count = para.max_retrans;
                mMessage->add(netmsg);
            }
            mDownloadQueue->rmDownloadTask();
            info.filetype = 0;
            mDownloadQueue->getDownloadTask(&info);
            if(info.filetype != 0) {
                mHttpTask->downloadFile(info.url,DOWNLOAD_PATH+info.md5+SystemUtils::extractSuffix(info.url));
            }
            break;
        }

        case HttpTask::STATE_END:
        {
            mHttpTask->stop();
            DownloadQueue::TaskInfo info;
            info.filetype = 0;
            mDownloadQueue->getDownloadTask(&info);
            if(info.filetype != 0) {
                QString srcfile = info.md5+SystemUtils::extractSuffix(info.url);
                QString md5 = SystemUtils::getFileMd5(DOWNLOAD_PATH+srcfile);
                if(md5.compare(QString(info.md5)) == 0) {
                    Message msg;
                    msg.id = SERVE_FILE_UPDATE;
                    uint16 DeviceID = (info.filetype>>16)&0xffff;
                    QString dstfile = info.md5+"_"+QString::number(DeviceID, 16).toUpper()+SystemUtils::extractSuffix(info.url);
                    msg.data = SystemUtils::u16ToQByteArray(DeviceID)
                           + SystemUtils::u16ToQByteArray(info.filetype&0xffff) + dstfile.toLatin1();
                    if(ADAPTER_ID == DeviceID) {
                        FileUtils::copyFile(DOWNLOAD_PATH+srcfile,APP_FILE_PATH+dstfile);
                        MY_DEBUG("toLocal");
                        emit toLocal(msg);
                    } else {
                        FileUtils::copyFile(DOWNLOAD_PATH+srcfile,LED_FILE_PATH+dstfile);
                        MY_DEBUG("toLed");
                        emit toLed(msg);
                    }
                } else {
                    MY_WARNING(QString("info.md5=%1 md5=%2").arg(QString(info.md5)).arg(md5));
                    NetworkMessage::NetMessage netmsg;
                    netmsg.msg.id = CLIENT_FILE_UPDATE_RESULT;
                    netmsg.msg.data = SystemUtils::u16ToQByteArray((info.filetype>>16)&0xffff);
                    netmsg.msg.data += SystemUtils::u16ToQByteArray(info.filetype&0xffff);
                    netmsg.msg.data += SystemUtils::u8ToQByteArray(1);
                    netmsg.msg.batch = SystemUtils::rand16();
                    netmsg.time = 0;
                    netmsg.count = para.max_retrans;
                    mMessage->add(netmsg);
                }
                FileUtils::rmFile(DOWNLOAD_PATH+srcfile);
            }
            mDownloadQueue->rmDownloadTask();
            info.filetype = 0;
            mDownloadQueue->getDownloadTask(&info);
            if(info.filetype != 0) {
                mHttpTask->downloadFile(info.url,DOWNLOAD_PATH+info.md5+SystemUtils::extractSuffix(info.url));
            }
            break;
        }

        case HttpTask::STATE_IDLE:
        {
            DownloadQueue::TaskInfo info;
            info.filetype = 0;
            mDownloadQueue->getDownloadTask(&info);
            if(info.filetype != 0) {
                mHttpTask->downloadFile(info.url,DOWNLOAD_PATH+info.md5+SystemUtils::extractSuffix(info.url));
            }
            break;
        }

        default:
            break;
    }
}

void NetworkManager::socketMsg(Message msg)
{
    switch(msg.id)
    {
        case SERVE_GENERAL_REPLY:
            generalReplyHandler(msg);
            qWarning("SERVE_GENERAL_REPLY");
            break;

        case SERVE_SIGN_IN_REPLY:
            mSignUpFlag = signUpReplyHandler(msg);
            if(mSignUpFlag) {
                clientAuthentication();
                mSignInTime = CLIENT_SIGNIN_INTERVAL;
            }
            qWarning("SERVE_SIGN_IN_REPLY");
            break;

        case SERVE_SET_CLIENT_PARA:
            setClientParaHandler(msg);
            qWarning("SERVE_SET_CLIENT_PARA");
            break;

        case SERVE_QUERY_CLIENT_PARA:
            queryClientParaHandler(msg);
            qWarning("SERVE_QUERY_CLIENT_PARA");
            break;

        case SERVE_SPECIFY_QUERY_CLIENT_PARA:
            queryClientParaHandler(msg);
            qWarning("SERVE_SPECIFY_QUERY_CLIENT_PARA");
            break;

        case SERVE_QUERY_CLIENT_INFO:
            queryClientInfoHandler(msg);
             qWarning("SERVE_QUERY_CLIENT_INFO");
            break;

        case SERVE_FILE_UPDATE:
            fileUpdateHandler(msg);
            qWarning("SERVE_FILE_UPDATE");
            break;

        case SERVE_PERIPHERAL_CONTROL:
            peripheralControlHandler(msg);
            qWarning("SERVE_PERIPHERAL_CONTROL");
            break;

        case SERVE_DATA_TRANSPARENT:
            dataTransparentHandler(msg);
            qWarning("SERVE_DATA_TRANSPARENT");
            break;
    }
}

void NetworkManager::socketConnect(bool is)
{
    if(!is) {
        MY_WARNING("SocketClient disconnect");
        mSocketClientFlag = false;
        mSocketClientTime = 5;
        mSignUpFlag = false;
        mSignInFlag = false;
    }
}

void NetworkManager::ledHandler(Message msg)
{
    if(!mSignInFlag) {
        return;
    }
    MY_DEBUG(QString("id=%1").arg(msg.id));
    switch(msg.id)
    {
        case SERVE_PERIPHERAL_CONTROL:
            peripheralControlReply(msg);
            break;

        case SERVE_FILE_UPDATE:
            NetworkMessage::NetMessage netmsg;
            netmsg.msg.id = CLIENT_FILE_UPDATE_RESULT;
            netmsg.msg.data = msg.data;
            netmsg.msg.batch = SystemUtils::rand16();
            netmsg.time = 0;
            netmsg.count = para.max_retrans;
            mMessage->add(netmsg);
            break;
    }
}

void NetworkManager::localHandler(Message msg)
{
    if(!mSignInFlag) {
        return;
    }

    MY_DEBUG(QString("id=%1").arg(msg.id));
    switch(msg.id)
    {
        case SERVE_FILE_UPDATE:
            NetworkMessage::NetMessage netmsg;
            netmsg.msg.id = CLIENT_FILE_UPDATE_RESULT;
            netmsg.msg.data = msg.data;
            netmsg.msg.batch = SystemUtils::rand16();
            netmsg.time = 0;
            netmsg.count = para.max_retrans;
            mMessage->add(netmsg);
            break;
    }
}

void NetworkManager::lcdHandler(Message msg)
{
    if(!mSignInFlag) {
        return;
    }

    MY_DEBUG("lcdHandler");
    NetworkMessage::NetMessage netmsg;
    netmsg.msg.id = CLIENT_DATA_TRANSPARENT;
    netmsg.msg.data = msg.data;
    netmsg.msg.batch = SystemUtils::rand16();
    netmsg.time = 0;
    netmsg.count = para.max_retrans;
    mMessage->add(netmsg);
}

void NetworkManager::canHandler(Message msg)
{
    if(!mSignInFlag) {
        return;
    }

    MY_DEBUG("canHandler");
    uint16 id = CLIENT_CAN_REPORT;
    uint16 attr = msg.data.size();
    uint16 batch = SystemUtils::rand16();
    QByteArray pack = SystemUtils::u16ToQByteArray(id) + SystemUtils::u16ToQByteArray(attr)
            + sys->dev_id + SystemUtils::u16ToQByteArray(batch) + msg.data;

//  qWarning()<<"candata:"<<pack.toHex();
    mSocketClient->send(pack);
}

void NetworkManager::gpsHandler(gps_data data)
{
    if(!mSignInFlag) {
        return;
    }

    if(para.gps_upload_interval == 0) {
        return;
    }

    if(gpsUploadTime > 0) {
        return;
    }

    MY_DEBUG("gpsHandler");
    QByteArray pack;
    if(data.speed >= para.max_speed) {
        pack[0] = 0x01;
    } else {
        pack[0] = 0x00;
    }
    pack[1] = 0x08|(data.ew<<2)|(data.sn<<1)|data.state;
    pack += SystemUtils::u32ToQByteArray(data.lat);
    pack += SystemUtils::u32ToQByteArray(data.lng);
    pack += SystemUtils::u16ToQByteArray(data.altitude);
    pack += SystemUtils::u16ToQByteArray(data.speed);
    pack += SystemUtils::u16ToQByteArray(data.direction);
    pack += SystemUtils::toQByteArray(data.time,6);

    uint16 id = CLIENT_COORD_REPORT;
    uint16 attr = pack.size();
    uint16 batch = SystemUtils::rand16();
    pack = SystemUtils::u16ToQByteArray(id) + SystemUtils::u16ToQByteArray(attr)
            + sys->dev_id + SystemUtils::u16ToQByteArray(batch) + pack;
    mSocketClient->send(pack);
//    qWarning()<<"gps_data: "<<pack.toHex();
    gpsUploadTime = para.gps_upload_interval;
}
void NetworkManager::haveIMSI() {
    clientSignUp();
    mSignUpTime = CLIENT_SIGNUP_INTERVAL;
}

void NetworkManager::gpioInputHandler(unsigned char data) {
    gpioInputData = SystemUtils::u8ToQByteArray(data);
}
