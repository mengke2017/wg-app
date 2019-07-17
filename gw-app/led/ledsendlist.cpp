#include "ledsendlist.h"
#include "common/mydebug.h"

#define LIST_MAX_SIZE   30
LedSendList::LedSendList()
{
}

LedSendList* LedSendList::getInstance()
{
    static LedSendList instance;
    return &instance;
}

led_data_info* LedSendList::get()
{
    if(!mList.isEmpty()){
        mLock.lockForRead();
        led_data_info* info = &mList[0];
        mLock.unlock();
        return info;
    }
    return NULL;
}

bool LedSendList::add(led_data_info info)
{
    bool ret = false;
    mLock.lockForWrite();
    int32 size = mList.size();
    if(size < LIST_MAX_SIZE) {
        mList.append(info);
        ret = true;
    }
    mLock.unlock();
    return ret;
}

void LedSendList::remove(uint8 cmd)
{
    if(!mList.isEmpty()) {
        mLock.lockForWrite();
        if(cmd == mList.at(0).cmd) {
            mList.removeAt(0);
        }
        mLock.unlock();
    }
}
