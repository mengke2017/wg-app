#ifndef CANMANAGER_H
#define CANMANAGER_H

#include <QObject>
#include <QTimer>
#include "type.h"
#include "common/message.h"
#include "can/canport.h"
#include "system.h"

class CanManager : public QObject
{
    typedef struct _can_info_{
        uint16 sum;
        uint8 time[5];
        QList<can_frame> list;
    } can_info;

    Q_OBJECT
public:
    explicit CanManager(QObject *parent = 0);
    ~CanManager();
    
private:
    System* sys;
    sys_para para;
    CanPort* canPort;
    QTimer* timer;
    can_info info;
    volatile uint16 collectTimer;
    volatile uint16 collectTimerDef;
    volatile uint16 uploadTimer;
    volatile uint16 uploadTimerDef;

signals:
    void toNetwork(Message msg);

private:
    void rmRepeatID (can_frame can);

private slots:
    void timeout();
};

#endif // CANMANAGER_H
