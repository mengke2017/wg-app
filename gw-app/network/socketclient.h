#ifndef SOCKETCLIENT_H
#define SOCKETCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>
#include "network/socketconstant.h"
#include "type.h"
#include "common/message.h"

class SocketClient : public QObject
{
    Q_OBJECT
protected:
    SocketClient();
    
private:
    QTcpSocket *mClientSocket;
    QByteArray mDataPacket;
    QHostAddress mIP;
    int32 mPort;
    volatile bool mIsConnect;

public:
    static SocketClient *getInstance();
    bool isConnected();
    void connectServer(QHostAddress ip,int32 port);
    void stop();

private:
    bool parsePacket(QTcpSocket *socket);

signals:
    void dataPacketReady(Message msg);
    void socketConnected(bool);

public:
    void send(QByteArray data);

private slots:
    void onReadyRead();
    void onDisconnected();
};

#endif // SOCKETCLIENT_H
