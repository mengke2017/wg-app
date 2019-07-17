#ifndef NETWORKMESSAGE_H
#define NETWORKMESSAGE_H

#include <qglobal.h>
#include <QReadWriteLock>
#include <QList>
#include <QByteArray>
#include "type.h"
#include "common/message.h"

class NetworkMessage
{
public:
    typedef struct {
        Message msg;
        uint8 time;
        uint8 count;
    }NetMessage;

protected:
    NetworkMessage();

public:
    static NetworkMessage* getInstance();
    int32 size();
    void get(int32 i,NetMessage* netmsg);
    void add(NetMessage msg);
    void setByIndex(int32 i,NetMessage msg);
    void setByBatch(uint16 batch,NetMessage msg);
    void remove(int32 i);

private:
    QList<NetMessage> mUpList;
    QReadWriteLock mUplock;
};

#endif // NETWORKMESSAGE_H
