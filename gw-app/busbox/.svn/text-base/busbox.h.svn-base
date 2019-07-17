#ifndef BUSBOX_H
#define BUSBOX_H

#include <QObject>
#include "type.h"
#include "busbox/busuart.h"

typedef struct {
    uint8 type;
    uint16 len;
    QByteArray data;
}bus_data;

typedef struct {
    uint8 add;
    uint8 type;
    QList<bus_data> list;
}bus_message;

#define MSG_ADDR_HEAD       0x30
#define MSG_ADDR_WAIST      0x31
#define MSG_ADDR_TAIL       0x32
#define MSG_ADDR_INSIDE     0x10
#define MSG_ADDR_BROADCAST  0xFF

#define MSG_TYPE_STATION    0x03
#define MSG_TYPE_LINE       0x04
#define MSG_TYPE_KEYSLG     0X05    //调用宣传用语编号
#define MSG_TYPE_LINE_INFO  0x10

#define DATA_TYPE_UP_DOWN_FLAG  0x02
#define DATA_TYPE_STATION_TYPE  0x03
#define DATA_TYPE_IN_OUT_STATION_FLAG   0x04
#define DATA_TYPE_STATION_NUM   0x05
#define DATA_TYPE_STATION_NAME  0x08

//#define DATA_TYPE_CALL       0x06       //调用宣传用语编号
#define DATA_TYPE_REAL       0x07       //实时下发宣传用语
#define DATA_TYPE_CUE       0x24
#define DATA_TYPE_LINE_NAME 0x31
#define DATA_TYPE_FIRST_NAME    0x32
#define DATA_TYPE_FIRST_RESERVE_NAME    0x33
#define DATA_TYPE_END_NAME  0x34
#define DATA_TYPE_END_RESERVE_NAME      0x35

class BusBox : public QObject
{
    Q_OBJECT
public:
    explicit BusBox(QObject *parent = 0);
    
private:
    BusUart* busUart;

signals:
    void toLed(bus_message msg);

private slots:
    void slotComDataHandler(QByteArray data);
};

#endif // BUSBOX_H
