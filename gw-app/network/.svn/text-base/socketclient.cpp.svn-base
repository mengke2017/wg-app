#include <QTime>
#include "socketclient.h"
#include "common/mydebug.h"
#include "common/systemutils.h"

SocketClient::SocketClient()
{
    mClientSocket = NULL;
    mIsConnect = true;
    mDataPacket.resize(0);
}

SocketClient *SocketClient::getInstance()
{
    static SocketClient instance;
    return &instance;
}

bool SocketClient::isConnected()
{
    if(mClientSocket == NULL || (mClientSocket != NULL &&
            mClientSocket->state() != QTcpSocket::ConnectedState)) {
        return false;
    } else {
        return true;
    }
}

void SocketClient::connectServer(QHostAddress ip,int32 port)
{
    QTcpSocket *socket = new QTcpSocket();
    bool flag = true;
    mIP = ip;
    mPort = port;
    socket->connectToHost(mIP,mPort);
    flag = socket->waitForConnected();
    if(flag == true)
    if(socket->state() == QTcpSocket::ConnectedState) {
        if(mClientSocket != NULL) {
            mClientSocket->abort();
            MY_WARNING("SocketClient Connect fail!");
            emit socketConnected(false);
            delete mClientSocket;
        }

        mClientSocket = socket;
        connect(mClientSocket,SIGNAL(readyRead()),this,SLOT(onReadyRead()));
        connect(mClientSocket,SIGNAL(disconnected()),this,SLOT(onDisconnected()));
    } else {
        MY_WARNING("SocketClient Connect fail!");
        emit socketConnected(false);
        delete socket;
    }
}

void SocketClient::onReadyRead()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());

    if(socket == mClientSocket) {
        mDataPacket += socket->readAll();
        bool isOK = false;
        do{
            isOK = parsePacket(socket);
        }while(isOK);
    } else {
        MY_ERROR("socket connect abnormal");
    }
}

bool SocketClient::parsePacket(QTcpSocket *socket)
{
    static QByteArray tmp;

    int32 startIndex = mDataPacket.indexOf(NET_PACKET_START);
    if(startIndex < 0) {
        mDataPacket.remove(0,mDataPacket.size());
        MY_DEBUG("startIndex");
        return false;
    }

    mDataPacket.remove(0,startIndex);
    int32 endIndex = mDataPacket.indexOf(NET_PACKET_END,NET_PACHET_START_SIZE);
    if(endIndex < 0) {
        MY_DEBUG("endIndex");
        return false;
    }

    int32 len = endIndex - NET_PACHET_START_SIZE;
    if(len < NET_PACHET_HEADERS_SIZE + NET_PACHET_EFFECT_SIZE) {
        MY_DEBUG(QString("len=%1").arg(len));
        mDataPacket.remove(0,len+NET_PACHET_START_SIZE+NET_PACHET_END_SIZE);
        if(mDataPacket.indexOf(NET_PACKET_START) < 0) {
            return false;
        } else {
            return true;
        }
    }

    tmp = mDataPacket.mid(NET_PACHET_START_SIZE,len);
    SystemUtils::escape(&tmp,ESCAPE_CHARACTER_DST1,ESCAPE_CHARACTER_DST1_SIZE,ESCAPE_CHARACTER_SRC1);
    SystemUtils::escape(&tmp,ESCAPE_CHARACTER_DST2,ESCAPE_CHARACTER_DST2_SIZE,ESCAPE_CHARACTER_SRC2);

    int32 attr = (tmp.at(2)<<8)|tmp.at(3);
    int32 infoLen = attr&0x3ff;
    len = tmp.size();
    if(len != (infoLen + NET_PACHET_HEADERS_SIZE + NET_PACHET_EFFECT_SIZE)){
        MY_DEBUG(QString("len=%1 infoLen=%2").arg(len).arg(infoLen));
        mDataPacket.remove(0,endIndex+NET_PACHET_END_SIZE);
        if(mDataPacket.indexOf(NET_PACKET_START) < 0) {
            return false;
        } else {
            return true;
        }
    }

    if(tmp.at(len-1) != SystemUtils::XOR(&tmp,0,len-1)) {
        MY_DEBUG("xorEffect");
        mDataPacket.remove(0,endIndex+NET_PACHET_END_SIZE);
        if(mDataPacket.indexOf(NET_PACKET_START) < 0) {
            return false;
        } else {
            return true;
        }
    }

    if(socket == mClientSocket) {
        static Message msg;
        msg.id = (tmp.at(0)<<8)|tmp.at(1);
        msg.batch = (tmp.at(10)<<8)|tmp.at(11);
        msg.data = tmp.mid(12,infoLen);
        emit dataPacketReady(msg);
    }

    mDataPacket.remove(0,endIndex+NET_PACHET_END_SIZE);
    if(mDataPacket.indexOf(NET_PACKET_START) < 0) {
        return false;
    } else {
        return true;
    }
}

void SocketClient::stop()
{
    if(mClientSocket != NULL) {
        mClientSocket->abort();
        mClientSocket = NULL;
    }
    emit socketConnected(false);
    MY_DEBUG("socket stop");
}

void SocketClient::onDisconnected()
{
    if(mClientSocket != NULL) {
        mClientSocket->abort();
        mClientSocket = NULL;
    }

    emit socketConnected(false);
    MY_DEBUG("socket disconnect");
}

void SocketClient::send(QByteArray data)
{
    static QByteArray packet;

    QTcpSocket *socket = mClientSocket;
    if(socket == NULL || (socket != NULL &&
            socket->state() != QTcpSocket::ConnectedState)) {
        return;
    }

    uint8 effect = SystemUtils::XOR(&data,0,data.size());
    data = data + SystemUtils::u8ToQByteArray(effect);
    SystemUtils::escape(&data,ESCAPE_CHARACTER_SRC1,ESCAPE_CHARACTER_SRC1_SIZE,ESCAPE_CHARACTER_DST1);
    SystemUtils::escape(&data,ESCAPE_CHARACTER_SRC2,ESCAPE_CHARACTER_SRC2_SIZE,ESCAPE_CHARACTER_DST2);
    packet = QByteArray(NET_PACKET_START) + data + QByteArray(NET_PACKET_END);
    socket->write(packet);
//    qWarning()<<"************packet:"<<packet.toHex()<<"***********\n";
}
