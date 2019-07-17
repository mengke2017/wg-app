#ifndef LEDSENDLIST_H
#define LEDSENDLIST_H

#include <QReadWriteLock>
#include <QList>
#include <QByteArray>
#include "type.h"

typedef struct {
    volatile uint8 type;
    volatile uint8 cmd;
    volatile uint8 addr;
    QByteArray data;
}led_data_info;

class LedSendList
{
protected:
    LedSendList();

public:
    static LedSendList* getInstance();
    led_data_info* get();
    bool add(led_data_info info);
    void remove(uint8 cmd);

private:
    QList<led_data_info> mList;
    QReadWriteLock mLock;
};

#endif // LEDSENDLIST_H
