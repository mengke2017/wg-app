#include <QCoreApplication>
#include "backstagemanager.h"
#include <csignal>
#include "mydaemon.h"

#define SIG_READDATA  (__SIGRTMIN+12)

static int setup_unix_signal_handlers()
{
    struct sigaction rea;

    rea.sa_handler = MyDaemon::reaSignalHandler;
    sigemptyset(&rea.sa_mask);
    rea.sa_flags = 0;
    rea.sa_flags |= SA_RESTART;

    if (sigaction(SIG_READDATA, &rea, 0) > 0)
       return 1;

    return 0;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    BackstageManager ser;

    setup_unix_signal_handlers(); // 安装信号

    MyDaemon daemon(NULL);

    return a.exec();
}
