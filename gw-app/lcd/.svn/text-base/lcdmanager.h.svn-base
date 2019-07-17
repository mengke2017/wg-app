#ifndef LCDMANAGER_H
#define LCDMANAGER_H

#include <QObject>
#include "type.h"
#include "common/message.h"
#include "system.h"
#include "lcd/lcduart.h"
#include "gps/gps.h"

class LcdManager : public QObject
{
    Q_OBJECT
public:
    explicit LcdManager(QObject *parent = 0);
    
private:
    LcdUart* lcdPort;
    System* sys;
    sys_para para;

signals:
    void toNetwork(Message msg);

public slots:
    void networkHandler(Message msg);
    void gpsHandler(struct gps_data data);

private slots:
    void portHandler(QByteArray data);
};

#endif // LCDMANAGER_H
