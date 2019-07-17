#include <QVariant>
#include <QTime>
#include <QtGlobal>
#include <QFile>
#include <QTextStream>
#include "system.h"
#include "constant.h"
#include "eeprom/eeprom.h"
#include "common/systemutils.h"
#include "led/ledmanager.h"
#include "common/mydebug.h"

#define CONFIG_FILE "/etc/boa/data/config.conf"
#define CONFIG_FILE_BACK "/etc/boa/data/config.conf.back"
#define CONFIG_NAME "gw-zb"
#define CONFIG_PASS "12345678"

#define PARA_DEV_ID "000000100053"

System::System()
{
    paraFlag = PARA_TYPE_NULL;
    sim = "88888888";
    getBuildDate();
    softwareVersion = buildDate.toLatin1();
    firmwareVersion = SystemUtils::getSystemVersion();

    peripheralState[0].id = LED_HEAD_REAL_ID;
    peripheralState[0].is_connect = false;
    peripheralState[1].id = LED_WAIST_REAL_ID;
    peripheralState[1].is_connect = false;
    peripheralState[2].id = LED_TAIL_REAL_ID;
    peripheralState[2].is_connect = false;
    peripheralState[3].id = LED_INSIDE_REAL_ID;
    peripheralState[3].is_connect = false;
    peripheralState[4].id = LED_AD_REAL_ID;
    peripheralState[4].is_connect = false;

    initSystemPara();
    initDeviceInfo();
}

System* System::getInstance()
{
    static System instance;
    return &instance;
}

void System::initDeviceInfo()
{
    device_info info;
    int32 size = sizeof(device_info);
    memset(&info,0,size);
    Eeprom* eeprom = new Eeprom();
    QByteArray defdev_id = para.dev_id.toLatin1();
    if(eeprom->open()) {
        int32 len = eeprom->read(&info,size);
        if(size == len) {
            QByteArray tmp = SystemUtils::toQByteArray((uint8*)&info,size);
            uint16 crc = SystemUtils::CRC16(&tmp,0,size-2);
            if(crc == info.crc) {
                productionDate = SystemUtils::toQByteArray(info.date,sizeof(info.date));  // 生产日期
                manufacturer = SystemUtils::toQByteArray(info.manufacturer,sizeof(info.manufacturer));  // 制造商
                model = SystemUtils::toQByteArray(info.model,sizeof(info.model));  // 型号 10
                serialNumber = SystemUtils::toQByteArray(info.number,sizeof(info.number));  // 设备序列号 18

                hardwareVersion = SystemUtils::toQByteArray(info.version,sizeof(info.version));  // 硬件版本号

                dev_id[0] = (defdev_id[0]<<4)|(defdev_id[1]&0xf);
                dev_id[1] = (defdev_id[2]<<4)|(defdev_id[3]&0xf);
                dev_id[2] = (defdev_id[4]<<4)|(defdev_id[5]&0xf);
                dev_id[3] = (defdev_id[6]<<4)|(defdev_id[7]&0xf);
                dev_id[4] = (defdev_id[8]<<4)|(defdev_id[9]&0xf);
                dev_id[5] = (defdev_id[10]<<4)|(defdev_id[11]&0xf);

                eeprom->close();
                return;
            }
        }
    }
    eeprom->close();
    devInfoDefault();
}

void System::initSystemPara()
{
    db = QSqlDatabase::addDatabase("QSQLITE","config");
    db.setDatabaseName(APP_PATH + "/CONFIG.db");
    db.setUserName("root");
    db.setPassword("12345678");
    if(!db.open()) {
        sysParaDefault();
        return;
    }
    query = QSqlQuery(db);

    query.exec("create table if not exists M_USER_MANAGE(heartbeat int,ip varchar,port int,reply int,retrans int,speed int,overspeed int,cancollect int,canupload int,gpsupload int,"
               "province int,city int,vin varchar,licenseplate varchar,licenseplatecolor int,dev_id varchar)");

    query.prepare("SELECT * FROM M_USER_MANAGE");
    query.exec();
    if(query.first()) {
        bool ok;
        if (readConf(&para) < 0) {
            para.ip = query.value(1).toString();
            para.port = query.value(2).toInt(&ok);
            para.dev_id = query.value(15).toString();
        }
        para.heartbeat_cycle = query.value(0).toInt(&ok);
        para.reply_timeout = query.value(3).toInt(&ok);
        para.max_retrans = query.value(4).toInt(&ok);
        para.max_speed = query.value(5).toInt(&ok);
        para.overspeed_time = query.value(6).toInt(&ok);
        para.can_collect_interval = query.value(7).toInt(&ok);
        para.can_upload_interval = query.value(8).toInt(&ok);
        para.gps_upload_interval = query.value(9).toInt(&ok);
        para.province_id = query.value(10).toInt(&ok);
        para.city_id = query.value(11).toInt(&ok);
        para.vin = query.value(12).toString();
        para.license_plate = query.value(13).toString();
        para.license_plate_color = query.value(14).toInt(&ok);
    } else {
        sysParaDefault();
        query.prepare("INSERT INTO M_USER_MANAGE (heartbeat,ip,port,reply,retrans,speed,overspeed,cancollect,canupload,gpsupload,"
                "province,city,vin,licenseplate,licenseplatecolor,dev_id) VALUES (:heartbeat,:ip,:port,:reply,:retrans,:speed,"
                ":overspeed,:cancollect,:canupload,:gpsupload,:province,:city,:vin,:licenseplate,:licenseplatecolor,:dev_id)");
        query.bindValue(0,para.heartbeat_cycle);
        query.bindValue(1,para.ip);
        query.bindValue(2,para.port);
        query.bindValue(3,para.reply_timeout);
        query.bindValue(4,para.max_retrans);
        query.bindValue(5,para.max_speed);
        query.bindValue(6,para.overspeed_time);
        query.bindValue(7,para.can_collect_interval);
        query.bindValue(8,para.can_upload_interval);
        query.bindValue(9,para.gps_upload_interval);
        query.bindValue(10,para.province_id);
        query.bindValue(11,para.city_id);
        query.bindValue(12,para.vin);
        query.bindValue(13,para.license_plate);
        query.bindValue(14,para.license_plate_color);
        query.bindValue(15,para.dev_id);
        query.exec();
    }

    QByteArray defdev_id = para.dev_id.toLatin1();

    dev_id.resize(6);
    dev_id[0] = (defdev_id[0]<<4)|(defdev_id[1]&0xf);
    dev_id[1] = (defdev_id[2]<<4)|(defdev_id[3]&0xf);
    dev_id[2] = (defdev_id[4]<<4)|(defdev_id[5]&0xf);
    dev_id[3] = (defdev_id[6]<<4)|(defdev_id[7]&0xf);
    dev_id[4] = (defdev_id[8]<<4)|(defdev_id[9]&0xf);
    dev_id[5] = (defdev_id[10]<<4)|(defdev_id[11]&0xf);

//    if(para.dial_user.isEmpty() || para.dial_pass.isEmpty()) {
//         para.dial_user = CONFIG_NAME;
//         para.dial_pass = CONFIG_PASS;
//     }

}

void System::sysParaDefault()
{
    para.heartbeat_cycle = 60;
    if (readConf(&para) < 0) {
        para.ip = "60.191.59.11";//"122.225.198.118";//"192.168.1.144";//
        para.port = 7799;//7767;//8080;
        para.dev_id = PARA_DEV_ID;
    }

    para.reply_timeout = 3;
    para.max_retrans = 3;
    para.max_speed = 100;
    para.overspeed_time = 15;
    para.can_collect_interval = 500;
    para.can_upload_interval = 5;
    para.gps_upload_interval = 10;
    para.province_id = 0x0033;
    para.city_id = 0x0100;
    para.vin = SystemUtils::utf8ToUnicode("");
    para.license_plate = SystemUtils::utf8ToUnicode("浙A.123456");
    para.license_plate_color = 0xff;

}

void System::devInfoDefault()
{
    manufacturer = "TOYOU";
    productionDate = "20190701";
    model = "gw-zb";
    serialNumber.fill(0x38,18);
    hardwareVersion = "REV1.01";
}

void System::getSystemPara(sys_para* data)
{
    data->heartbeat_cycle = para.heartbeat_cycle;
    data->ip = para.ip;
    data->port = para.port;
    data->reply_timeout = para.reply_timeout;
    data->max_retrans = para.max_retrans;
    data->max_speed = para.max_speed;
    data->overspeed_time = para.overspeed_time;
    data->can_collect_interval = para.can_collect_interval;
    data->can_upload_interval = para.can_upload_interval;
    data->gps_upload_interval = para.gps_upload_interval;
    data->province_id = para.province_id;
    data->city_id = para.city_id;
    data->vin = para.vin;
    data->license_plate = para.license_plate;
    data->license_plate_color = para.license_plate_color;
    data->dev_id = para.dev_id;
//    data->dial.dial_user = para.dial.dial_user;
//    data->dial.dial_pass = para.dial.dial_pass;
}

void System::setSystemPara(sys_para data)
{
    bool flag = false;
    int l = 0;
    QByteArray defdev_id = data.dev_id.toLatin1();

    if(para.heartbeat_cycle != data.heartbeat_cycle) {
        para.heartbeat_cycle = data.heartbeat_cycle;
        paraFlag |= PARA_TYPE_HEARTBEAT;
        flag = true;
    }

    if(0 != para.ip.compare(data.ip)) {
        para.ip = data.ip;
        paraFlag |= PARA_TYPE_IP;
        flag = true;
    }

    if(para.port != data.port) {
        para.port = data.port;
        paraFlag |= PARA_TYPE_PORT;
        flag = true;
    }

    if(para.reply_timeout != data.reply_timeout) {
        para.reply_timeout = data.reply_timeout;
        paraFlag |= PARA_TYPE_REPLY;
        flag = true;
    }

    if(para.max_retrans != data.max_retrans) {
        para.max_retrans = data.max_retrans;
        paraFlag |= PARA_TYPE_RETRANS;
        flag = true;
    }

    if(para.max_speed != data.max_speed) {
        para.max_speed = data.max_speed;
        paraFlag |= PARA_TYPE_SPEED;
        flag = true;
    }

    if(para.overspeed_time != data.overspeed_time) {
        para.overspeed_time = data.overspeed_time;
        paraFlag |= PARA_TYPE_OVERSPEED_TIME;
        flag = true;
    }

    if(para.can_collect_interval != data.can_collect_interval) {
        para.can_collect_interval = data.can_collect_interval;
        paraFlag |= PARA_TYPE_CAN_COLLECT;
        flag = true;
    }

    if(para.can_upload_interval != data.can_upload_interval) {
        para.can_upload_interval = data.can_upload_interval;
        paraFlag |= PARA_TYPE_CAN_UPLOAD;
        flag = true;
    }

    if(para.gps_upload_interval != data.gps_upload_interval) {
        para.gps_upload_interval = data.gps_upload_interval;
        paraFlag |= PARA_TYPE_GPS_UPLOAD;
        flag = true;
    }

    if(para.province_id != data.province_id) {
        para.province_id = data.province_id;
        paraFlag |= PARA_TYPE_PROVINCE;
        flag = true;
    }

    if(para.city_id != data.city_id) {
        para.city_id = data.city_id;
        paraFlag |= PARA_TYPE_CITY;
        flag = true;
    }

    if(0 != para.vin.compare(data.vin)) {
        para.vin = data.vin;
        flag = true;
    }

    if(0 != para.license_plate.compare(data.license_plate)) {
        para.license_plate = data.license_plate;
        paraFlag |= PARA_TYPE_LICENSE_PLATE;
        flag = true;
    }

    if(para.license_plate_color != data.license_plate_color) {
        para.license_plate_color = data.license_plate_color;
        paraFlag |= PARA_TYPE_LICENSE_PLATE_COLOR;
        flag = true;
    }

//    if(para.dial_user.compare(data.dial_user) != 0) {
//        para.dial_user = data.dial_user;
//        paraFlag |= PARA_TYPE_CONF_NAME_PASS;
//        flag = true;
//    }

//    if(para.dial_pass.compare(data.dial_pass) != 0) {
//        para.dial_pass = data.dial_pass;
//        paraFlag |= PARA_TYPE_CONF_NAME_PASS;
//        qWarning()<<"***conf_pass:"<<para.dial_pass.toLatin1();
//        flag = true;
//    }

    if(0 != para.dev_id.compare(data.dev_id)) {
        para.dev_id = data.dev_id;
        paraFlag |= PARA_TYPE_DEV_ID;
        flag = true;
        l = para.dev_id.length();
        if (l < 12) {
            for(int i = (12 - l); i > 0; i--) {
                para.dev_id.insert(0,"0");
            }
        }
        defdev_id = para.dev_id.toLatin1();
        dev_id.resize(6);
        dev_id[0] = (defdev_id[0]<<4)|(defdev_id[1]&0xf);
        dev_id[1] = (defdev_id[2]<<4)|(defdev_id[3]&0xf);
        dev_id[2] = (defdev_id[4]<<4)|(defdev_id[5]&0xf);
        dev_id[3] = (defdev_id[6]<<4)|(defdev_id[7]&0xf);
        dev_id[4] = (defdev_id[8]<<4)|(defdev_id[9]&0xf);
        dev_id[5] = (defdev_id[10]<<4)|(defdev_id[11]&0xf);
    }

    if(flag) {
        query.prepare("update M_USER_MANAGE set heartbeat=?,ip=?,port=?,reply=?,retrans=?,"
                      "speed=?,overspeed=?,cancollect=?,canupload=?,gpsupload=?,province=?,"
                      "city=?,vin=?,licenseplate=?,licenseplatecolor=?,dev_id=?");
        query.bindValue(0,para.heartbeat_cycle);
        query.bindValue(1,para.ip);
        query.bindValue(2,para.port);
        query.bindValue(3,para.reply_timeout);
        query.bindValue(4,para.max_retrans);
        query.bindValue(5,para.max_speed);
        query.bindValue(6,para.overspeed_time);
        query.bindValue(7,para.can_collect_interval);
        query.bindValue(8,para.can_upload_interval);
        query.bindValue(9,para.gps_upload_interval);
        query.bindValue(10,para.province_id);
        query.bindValue(11,para.city_id);
        query.bindValue(12,para.vin);
        query.bindValue(13,para.license_plate);
        query.bindValue(14,para.license_plate_color);
        query.bindValue(15,para.dev_id);
        query.exec();
        writeConf(para);
    }
}

/*void System::TEXT_connectDevice(System data, uint8 id) {
    for (int i = 0; i < DEVICE_SUM; i++) {
        if(data.peripheralState[i].id == id)
            data.peripheralState[i].is_connect = true;
    }
}*/
void System::getBuildDate() {
    buildDate = __DATE__;
    QString pDest_y = buildDate.mid(9,2);
    QString pDest_m = buildDate.mid(0,3);
    QString pDest_d = buildDate.mid(4,2);

    if (  pDest_m.compare("Jan") == 0)  pDest_m = "01";
    else if ( pDest_m.compare("Feb") == 0)  pDest_m = "02";
    else if ( pDest_m.compare("Mar") == 0)  pDest_m = "03";
    else if ( pDest_m.compare("Apr") == 0)  pDest_m = "04";
    else if ( pDest_m.compare("May") == 0)  pDest_m = "05";
    else if ( pDest_m.compare("Jun") == 0)  pDest_m = "06";
    else if ( pDest_m.compare("Jul") == 0)  pDest_m = "07";
    else if ( pDest_m.compare("Aug") == 0)  pDest_m = "08";
    else if ( pDest_m.compare("Sep") == 0)  pDest_m = "09";
    else if ( pDest_m.compare("Oct") == 0)  pDest_m = "10";
    else if ( pDest_m.compare("Nov") == 0)  pDest_m = "11";
    else if ( pDest_m.compare("Dec") == 0)  pDest_m = "12";
    else  pDest_m = "01";

//    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
//    rang = QString::number(qrand()%(99 - 10) + 10);

    QString rang = __TIME__;  //QString::number(QTime(0,0,0).secsTo(QTime::currentTime())%(99 - 10) + 10);
    rang = rang.mid(3, 2);

    buildDate = pDest_y + pDest_m + pDest_d + rang;
}

QString System::toUiVersion() {
    QString Version = softwareVersion;

    Version = Version.insert(0,"V").insert(3,".").insert(6,".").insert(9,".");
    return Version;
}

int System::readConf(sys_para* data)
{
    QFile fd(CONFIG_FILE);

    if(!fd.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning()<<"Open failed.";
        return -1;
    }

    QTextStream txtInput(&fd);
    QString lineStr;
    QString name;
    uint flag;

    while (!txtInput.atEnd())
    {
        lineStr = txtInput.readLine();

        if(lineStr.isEmpty())
            continue;
        if(lineStr.at(0) == '#'||lineStr.at(0) == ' ')
        {
            continue;
        }
        else
        {
            int i = lineStr.indexOf('=', 0);
            if(i < 0)
                break;

            name = lineStr.mid(0, i);

            if(!name.compare("ip"))
            {
                qWarning()<<"IP ： "<<lineStr.mid(i+1).toLatin1();  // i+1 去掉 =
                data->ip = lineStr.mid(i+1);
                flag += 1;
                continue;
            }
            if(!name.compare("port"))
            {
                qWarning()<<"Port : "<<lineStr.mid(i+1).toLatin1();  // i+1 去掉 =
                data->port = lineStr.mid(i+1).toInt();
                flag += 1;
                continue;
            }
            if(!name.compare("id"))
            {
                qWarning()<<"ID ： "<<lineStr.mid(i+1).toLatin1();  // i+1 去掉 =
                data->dev_id = lineStr.mid(i+1);
                flag += 1;
                continue;
            }
//            if(!name.compare("user"))
//            {
//                qWarning()<<"USER ： "<<lineStr.mid(i+1).toLatin1();  // i+1 去掉 =
//                data->dial.dial_user = lineStr.mid(i+1);
//                continue;
//            }
//            if(!name.compare("pass"))
//            {
//                qWarning()<<"PASS ： "<<lineStr.mid(i+1).toLatin1();  // i+1 去掉 =
//                data->dial.dial_pass = lineStr.mid(i+1);
//                continue;
//            }
//            if(!name.compare("apn"))
//            {
//                qWarning()<<"APN ： "<<lineStr.mid(i+1).toLatin1();  // i+1 去掉 =
//                data->dial.dial_apn = lineStr.mid(i+1);
//                continue;
//            }
//            if(!name.compare("pin"))
//            {
//                qWarning()<<"PIN ： "<<lineStr.mid(i+1).toLatin1();  // i+1 去掉 =
//                data->dial.dial_pin = lineStr.mid(i+1);
//                continue;
//            }
        }
    }
    fd.close();

    if (flag < 3)
        return -1;

    return 0;
}

int System::writeConf(sys_para data) {
    QFile fd(CONFIG_FILE);
    QFile fd1(CONFIG_FILE_BACK);

    if(!fd.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning()<<"Open failed.";
        return -1;
    }
    if(!fd1.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning()<<"Open failed.";
        return -1;
    }

    QTextStream txtInput(&fd);
    QTextStream txtOutput(&fd1);
    QString lineStr;
    QString name;
    uint8 flag = 0;

    while (!txtInput.atEnd())
    {
        lineStr = txtInput.readLine();

        if(lineStr.isEmpty())
            continue;
        if(lineStr.at(0) != '#' && lineStr.at(0) != ' ')
        {
            int i = lineStr.indexOf('=', 0);
            if(i > 0)
            {
                name = lineStr.mid(0, i);
                if(!name.compare("ip")) {
                    flag += 1;
                    lineStr = name + "=" + data.ip;
                }
                if(!name.compare("id")) {
                    flag += 2;
                    lineStr = name + "=" + data.dev_id;
                }
                if(!name.compare("port")) {
                    flag += 4;
                    lineStr = name + "=" + data.port;
                }
            }
        }
        txtOutput<<lineStr<<endl;
    }
    if (flag == 0) {
        txtOutput<<"ip="<<data.dev_id<<endl;
        txtOutput<<"id"<<data.ip<<endl;
        txtOutput<<"port="<<data.port<<endl;
    } else if (flag == 1) {
        txtOutput<<"ip="<<data.dev_id<<endl;
        txtOutput<<"port="<<data.port<<endl;
    } else if(flag == 2) {
        txtOutput<<"ip="<<data.dev_id<<endl;
        txtOutput<<"port="<<data.port<<endl;
    } else if (flag == 3) {
        txtOutput<<"port="<<data.port<<endl;
    } else if (flag == 4) {
        txtOutput<<"ip="<<data.dev_id<<endl;
        txtOutput<<"id"<<data.ip<<endl;
    } else if (flag == 5) {
        txtOutput<<"ip="<<data.dev_id<<endl;
    } else if (flag == 6) {
        txtOutput<<"id"<<data.ip<<endl;
    }
    fd.remove();
    fd.close();
    fd1.rename(CONFIG_FILE);
    fd1.close();
}
