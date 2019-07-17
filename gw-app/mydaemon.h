#ifndef MYDAEMON_H
#define MYDAEMON_H
#include <QObject>
#include <QSocketNotifier>
#include "system.h"

class MyDaemon : public QObject
{
Q_OBJECT

public:
    MyDaemon(QObject *parent = 0);
    ~MyDaemon();

    // Unix signal handlers.
    static void reaSignalHandler(int unused);

private:
    System* sys;
    sys_para para;

public slots:
    // Qt signal handlers.
    void handleSigRea();

private:
    static int sigreaFd[2];

    QSocketNotifier *snRea;
};
#endif // MYDAEMON_H
