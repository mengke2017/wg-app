#include <QDebug>
#include <QObject>
#include <QSocketNotifier>
#include <csignal>
#include <sys/socket.h>
#include "mydaemon.h"

//needed to not get an undefined reference to static members
int MyDaemon::sigreaFd[2];

MyDaemon::MyDaemon(QObject *parent)
: QObject(parent)
{
    sys = System::getInstance();

    if (::socketpair(AF_UNIX, SOCK_STREAM, 0, sigreaFd))
        qFatal("Couldn't create HUP socketpair");

    snRea = new QSocketNotifier(sigreaFd[1], QSocketNotifier::Read, this);
    connect(snRea, SIGNAL(activated(int)), this, SLOT(handleSigRea()));
}

MyDaemon::~MyDaemon() {}

void MyDaemon::reaSignalHandler(int)
{
//    qDebug() << "signal hup";
    char a = '1';
    ::write(sigreaFd[0], &a, sizeof(a));

}

void MyDaemon::handleSigRea()
{
    snRea->setEnabled(false);
    char tmp;
    ::read(sigreaFd[1], &tmp, sizeof(tmp));

    // do Qt stuff

    sys->getSystemPara(&para);
    sys->readConf(&para);
    sys->setSystemPara(para);

    snRea->setEnabled(true);
}
