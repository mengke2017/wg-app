#include "backstagemanager.h"
#include "can/receivethread.h"
#include <QThread>
#include "qthread.h"

#define FILE_TYPE_APP   0x0101
#define FILE_TYPE_LINE  0x0301
#define FILE_TYPE_AD    0x0401
#define FILE_TYPE_CAN_CONF 0x0501

BackstageManager::BackstageManager(QObject *parent) :
    QObject(parent)
{
    gps = NULL;
    net = NULL;
    led = NULL;
//    pos = NULL;
    lcd = NULL;
    can = NULL;
    bus = NULL;

    lines = Line::getInstance();
    ad = Advertising::getInstance();

    sys = System::getInstance();
    sys->getSystemPara(&para);

    systemResetFlag = false;
    timer = new QTimer();
    connect(timer,SIGNAL(timeout()),this,SLOT(slotTimeOut()));
    timer->start(1000);

    led = LedManager::getInstance();
    MY_SYSTEM("gw-app start");

    start();

    qRegisterMetaType<Message>("Message");
}

void BackstageManager::start()
{
//    pos = new PosManager(this);
    net = NetworkManager::getInstance();

    connect(net,SIGNAL(toLocal(Message)),this,SLOT(networkHandler(Message)));
    connect(this,SIGNAL(toNetwork(Message)),net,SLOT(localHandler(Message)));

    gps = Gps::getInstance();
    connect(gps,SIGNAL(read(gps_data)),net,SLOT(gpsHandler(gps_data)));

//    led = LedManager::getInstance();
//    connect(net,SIGNAL(toLed(Message)),led,SLOT(netWorkHandler(Message)));
//    connect(led,SIGNAL(toNetwork(Message)),net,SLOT(ledHandler(Message)));

    //lcd = new LcdManager();
    //connect(net,SIGNAL(toLcd(Message)),lcd,SLOT(networkHandler(Message)));
    //connect(lcd,SIGNAL(toNetwork(Message)),net,SLOT(lcdHandler(Message)));
    //connect(gps,SIGNAL(read(gps_data)),lcd,SLOT(gpsHandler(gps_data)));

    can = new CanManager();
    connect(can,SIGNAL(toNetwork(Message)),net,SLOT(canHandler(Message)));

    bus = new BusBox();
    connect(bus,SIGNAL(toLed(bus_message)),led,SLOT(busboxHandler(bus_message)));

    sim = new SimPort();
    sim->sim_start();
    connect(sim,SIGNAL(getIMSISuccess()),net,SLOT(haveIMSI()));

    stm = new stmManager();
//    connect(stm,SIGNAL(toAT()),sim,SLOT(slotLowPower()));
//    connect(stm,SIGNAL(toNet(bool),net,SLOT())
    net->networkConnect();
    gpioInput = new gpioWork();
    QThread *thread = new QThread(this);
    gpioInput->moveToThread(thread);
    connect(thread, SIGNAL(started()), gpioInput, SLOT(run()), Qt::QueuedConnection);
    thread->start();
    connect(gpioInput, SIGNAL(msg(unsigned char)), net, SLOT(gpioInputHandler(unsigned char)));
}

void BackstageManager::close()
{

}

void BackstageManager::networkHandler(Message msg)
{
    MY_DEBUG(QString("msg.id=%1").arg(msg.id));
    switch(msg.id)
    {
        case SERVE_FILE_UPDATE:
        {
            uint16 type = (msg.data.at(2)<<8)|msg.data.at(3);
            QString filename = msg.data.mid(4,msg.data.size()-4);
            msg.data.remove(4,msg.data.size()-4);
            if(FILE_TYPE_APP == type) {
                FileUtils::renameFile(APP_FILE_PATH+filename,APP_FILE_PATH+"gw-app");
                msg.data += SystemUtils::u8ToQByteArray(0);

                systemResetFlag = true;
            } else if(FILE_TYPE_LINE == type) {
                FileUtils::rmFile(APP_FILE_PATH+"line.xml");
                FileUtils::renameFile(APP_FILE_PATH+filename,APP_FILE_PATH+"line.xml");
                lines->init();
                msg.data += SystemUtils::u8ToQByteArray(0);
            } else if(FILE_TYPE_AD == type) {
                FileUtils::rmFile(APP_FILE_PATH+"advertising.xml");
                FileUtils::renameFile(APP_FILE_PATH+filename,APP_FILE_PATH+"advertising.xml");
                ad->init();
                msg.data += SystemUtils::u8ToQByteArray(0);

            } else if(FILE_TYPE_CAN_CONF == type) {
                FileUtils::rmFile(CAN_CONFIG_PATH + "can_config.conf");
                FileUtils::renameFile(CAN_CONFIG_PATH+filename, CAN_CONFIG_PATH+"can_config.conf");
                can->can_init();
                msg.data += SystemUtils::u8ToQByteArray(0);
            } else {
                msg.data += SystemUtils::u8ToQByteArray(1);
            }
            emit toNetwork(msg);
            break;
        }
    }
}
void BackstageManager::slotTimeOut()
{
    if(systemResetFlag) {
        ::close(wdt_fd);
        MY_SYSTEM("reboot");
        system("reboot");
    }
}
void BackstageManager::wtdTimeOut() {
    if (wdt_fd != -1)
        write(wdt_fd, "a", 1);
}
