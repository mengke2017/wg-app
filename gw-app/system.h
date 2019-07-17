#ifndef SYSTEM_H
#define SYSTEM_H

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QMutex>
#include "type.h"

#define DEVICE_SUM  5

#define PARA_TYPE_NULL              0x0000
#define PARA_TYPE_HEARTBEAT         0x0001
#define PARA_TYPE_IP                0x0002
#define PARA_TYPE_PORT              0x0004
#define PARA_TYPE_REPLY             0x0008
#define PARA_TYPE_RETRANS           0x0010
#define PARA_TYPE_SPEED             0x0020
#define PARA_TYPE_OVERSPEED_TIME    0x0040
#define PARA_TYPE_CAN_COLLECT       0x0080
#define PARA_TYPE_CAN_UPLOAD        0x0100
#define PARA_TYPE_GPS_UPLOAD        0x0200
#define PARA_TYPE_PROVINCE          0x0400
#define PARA_TYPE_CITY              0x0800
#define PARA_TYPE_LICENSE_PLATE     0x1000
#define PARA_TYPE_LICENSE_PLATE_COLOR   0x2000
#define PARA_TYPE_DEV_ID            0x4000
#define PARA_TYPE_CONF_NAME_PASS    0x8000

typedef struct {
//    uint8 name[12];  // 设备名
    uint8 date[8];  // 生产日期
    uint8 manufacturer[5];  // 制造商
    uint8 model[10];  // 型号
    uint8 number[18];  // 设备序列号
    uint8 version[8];  // 硬件版本号

//    uint8 conf_user[10]; //配置用户名
//    uint8 conf_pass[10];  //配置密码
    uint16 crc;
}device_info;

typedef struct {
    volatile uint8 id;
    bool is_connect;
} Device_S;

//typedef struct {
//    QString dial_user;  // 拨号用户名
//    QString dial_pass;  // 密码
//    QString dial_apn;
//    QString dial_pin;
//} DIAL;

typedef struct {
    volatile uint32 heartbeat_cycle;        //心跳间隔
    QString ip;                     //服务器IP
    QString dev_id;                 //设备ID 12位
    volatile uint32 port;                   //服务器端口
    volatile uint32 reply_timeout;          //应答超时时间
    volatile uint32 max_retrans;            //最大重传次数
    volatile uint32 max_speed;              //超速值
    volatile uint32 overspeed_time;         //超速持续时间
    volatile uint32 can_collect_interval;   //can数据采集间隔
    volatile uint16 can_upload_interval;    //can数据上传间隔
    volatile uint16 gps_upload_interval;    //gps数据上传间隔
    volatile uint16 province_id;            //省域ID
    volatile uint16 city_id;                //市域ID
    QString vin;                    //VIN
    QString license_plate;          //车牌号
    volatile uint8 license_plate_color;     //车牌颜色

 //   DIAL dial;

}sys_para;

class System
{
public:
    //eeprom
//    QByteArray deviceName;          //设备名
    QByteArray productionDate;      //生产日期
    QByteArray manufacturer;        //制造商
    QByteArray model;               //终端型号
    QByteArray serialNumber;        //设备序列号
    QByteArray hardwareVersion;     //硬件版本

    //system
    QByteArray softwareVersion;     //软件版本  0190513  ->
    QByteArray firmwareVersion;     //固件版本
    QByteArray sim;                 //sim卡号
    QByteArray sim_IMSI;            //sim卡的IMSI码    
    QByteArray dev_id;              //网络ID

    Device_S peripheralState[DEVICE_SUM];
    volatile uint32 paraFlag;

private:
    //sql
    QSqlDatabase db;
    QSqlQuery query;
    QMutex mutex;
    sys_para para;
    QString buildDate;

protected:
    System();

public:
    static System* getInstance();
    void getSystemPara(sys_para* data);
    void setSystemPara(sys_para data);
    QString toUiVersion();
    int readConf(sys_para* data);
    int writeConf(sys_para data);
//    void TEXT_connectDevice(System data, uint8 id);
//    void toBCD_6(char **asc, char asc[]);
//    void toASC_12(char **bcd, char bcd[]);

private:
    void initDeviceInfo();
    void initSystemPara();
    void sysParaDefault();
    void devInfoDefault();
    void getBuildDate();
};

#endif // SYSTEM_H
