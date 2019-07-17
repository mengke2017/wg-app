#ifndef GPS_H
#define GPS_H

#include <QObject>
#include "type.h"
#include "gps/gpsport.h"

struct gps_data {
    uint16 speed;
    uint32 lat;
    uint8 sn;
    uint32 lng;
    uint8 ew;
    uint16 altitude;
    uint16 direction;
    uint8 satellites;
    uint8 state;
    uint8 time[6];
};

class Gps : public QObject
{
    Q_OBJECT
protected:
    explicit Gps(QObject *parent = 0);
    
public:
    static Gps* getInstance();

private:
    QByteArray getGPSDataIndex(QByteArray data);
    void getSpeed(QByteArray data);
    void getLatitude(QByteArray data);
    void getSN(QByteArray data);
    void getLongitude(QByteArray data);
    void getEW(QByteArray data);
    void getAltitude(QByteArray data);
    void getDirection(QByteArray data);
    void getSatellites(QByteArray data);
    void getState(QByteArray data);
    void getDate(QByteArray data);
    void getTime(QByteArray data);

private:
    GpsPort* gpsPort;
    struct gps_data gpsData;
    bool timezone;

signals:
    void read(struct gps_data data);

private slots:
    void slotPortHandler(QByteArray data);
};

#endif // GPS_H
