#ifndef ADVERTISING_H
#define ADVERTISING_H

#include <QList>
#include <QMutex>
#include "type.h"

class Advertising
{
protected:
    explicit Advertising();

public:
    static Advertising* getInstance();
    void init();
    QString get(int32 index);
    int32 size();

private:
    void lock();
    void unlock();
    bool readXML();

private:
    QList<QString> list;
    volatile int32 listSize;
    QMutex mutex;
};

#endif // ADVERTISING_H
