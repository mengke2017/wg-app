#include "networkmessage.h"

NetworkMessage::NetworkMessage()
{
}

NetworkMessage* NetworkMessage::getInstance()
{
    static NetworkMessage instance;
    return &instance;
}

int NetworkMessage::size()
{
    if(!mUpList.isEmpty()){
        mUplock.lockForRead();
        int32 size = mUpList.size();
        mUplock.unlock();
        return size;
    }
    return 0;
}

void NetworkMessage::get(int32 i,NetworkMessage::NetMessage* netmsg)
{
    if(!mUpList.isEmpty()){
        mUplock.lockForRead();
        netmsg->msg.id = mUpList[i].msg.id;
        netmsg->msg.batch = mUpList[i].msg.batch;
        netmsg->msg.data = mUpList[i].msg.data;
        netmsg->msg.data = mUpList[i].msg.data;
        netmsg->time = mUpList[i].time;
        netmsg->count = mUpList[i].count;
        mUplock.unlock();
    }
}

void NetworkMessage::setByIndex(int32 i,NetworkMessage::NetMessage netmsg)
{
    if(!mUpList.isEmpty()) {
        mUplock.lockForWrite();
        mUpList.replace(i,netmsg);
        mUplock.unlock();
    }
}

void NetworkMessage::setByBatch(uint16 batch,NetworkMessage::NetMessage netmsg)
{
    if(!mUpList.isEmpty()) {
        mUplock.lockForWrite();
        int size = mUpList.size();
        for(int i=0;i<size;i++)
        {
            if(mUpList[i].msg.batch == batch) {
                mUpList.replace(i,netmsg);
                break;
            }
        }
        mUplock.unlock();
    }
}

void NetworkMessage::add(NetworkMessage::NetMessage netmsg)
{
    mUplock.lockForWrite();
    mUpList.append(netmsg);
    mUplock.unlock();
}

void NetworkMessage::remove(int32 i)
{
    if(!mUpList.isEmpty()) {
        mUplock.lockForWrite();
        mUpList.removeAt(i);
        mUplock.unlock();
    }
}
