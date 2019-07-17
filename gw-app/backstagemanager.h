#ifndef BACKSTAGEMANAGER_H
#define BACKSTAGEMANAGER_H

#include <QObject>
#include "type.h"
#include "network/networkmanager.h"
#include "pos/posmanager.h"
#include "lcd/lcdmanager.h"
#include "gps/gps.h"
#include "can/canmanager.h"
#include "led/ledmanager.h"
#include "busbox/busbox.h"
#include "file/advertising.h"
#include "file/line.h"
#include "common/mydebug.h"
#include "common/systemutils.h"
#include "common/fileutils.h"
#include "system.h"

class BackstageManager : public QObject
{
    Q_OBJECT
public:
    explicit BackstageManager(QObject *parent = 0);
    
private:
    Gps* gps;
    LedManager* led;
//    PosManager* pos;
    LcdManager* lcd;
    CanManager* can;
    BusBox* bus;
    NetworkManager* net;
    SimPort* sim;

    QTimer* timer;
    QTimer* wtd_timer;
    System* sys;
    sys_para para;
    Line* lines;
    Advertising* ad;

    bool systemResetFlag;
    int wdt_fd;  // 看门狗
signals:
    void toNetwork(Message msg);

public slots:
    void start();
    void close();
//    void uiHandler(Message msg);
    void networkHandler(Message msg);
private slots:

    void slotTimeOut();
    void wtdTimeOut();
};

#endif // BACKSTAGEMANAGER_H
