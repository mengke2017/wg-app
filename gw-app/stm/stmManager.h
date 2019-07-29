#ifndef STMMANAGER_H
#define STMMANAGER_H

#include <QObject>
#include <QTimer>
#include "type.h"
#include "stm/stmUart.h"
#include "system.h"

typedef struct {
    uint8 cmd;
    uint8 len;
    uint8 data;
    uint16 time;
}StmData;

class stmManager : public QObject
{
    Q_OBJECT
public:
    explicit stmManager(QObject *parent = 0);
//    void send();

private:
    void reply_send(uint8 cmd);

private:
    StmPort* stmUart;
    bool reply_flag;
    QTimer *Timer;
    uint8 repeatCount;
    StmData sendData;
    uint16 repeatTimeOut;
    System *sys;
    sys_para para;
    uint8 timeFlag;


signals:
//    void toNet(bool);
//    void toAT();

private slots:
    void slotComDataHandler(QByteArray data);
    void timeout();
};
#endif // STMMANAGER_H
