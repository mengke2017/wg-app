#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "constant.h"
#include "eeprom.h"
#include "common/mydebug.h"

Eeprom::Eeprom(QObject *parent) :
    QObject(parent)
{
    fd = -1;
}

bool Eeprom::open()
{
    if(fd != -1) {
        MY_WARNING("is aready opened!");
        return false;
    }

    QString name = EEPROM;
    fd = ::open(name.toLatin1(),O_RDONLY|O_NOCTTY);
    if(fd == -1) {
        MY_WARNING("open erro!");
        return false;
    }

    return true;
}

bool Eeprom::close()
{
    mutex_r.lock();
    if(fd!=-1) {
        ::close(fd);
        fd=-1;
        mutex_r.unlock();
        return true;
    }
    mutex_r.unlock();
    return false;
}

int32 Eeprom::read(void* data,int32 len)
{
    int32 size = 0;

    mutex_r.lock();
    size = ::read(fd,data,len);
    mutex_r.unlock();

    return size;
}

int32 Eeprom::write(void* data,int32 len) {
    int32 size = 0;

    mutex_w.lock();
    size = ::write(fd,data,len);
    mutex_w.unlock();

    return size;

}
