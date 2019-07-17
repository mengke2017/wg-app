#ifndef POSCONTROL_H
#define POSCONTROL_H

#include <QObject>
#include "type.h"
#include "pos/posuart.h"
#include "can/canport.h"

class PosManager : public QObject
{
    Q_OBJECT
public:
    explicit PosManager(QObject *parent = 0);
    ~PosManager();

private:
    CanPort* canPort;
    PosUart* posUart;

private slots:
    void slotUartDataHandler(QByteArray data);
};

#endif // POSCONTROL_H
