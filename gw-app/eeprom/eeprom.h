#ifndef EEPROM_H
#define EEPROM_H

#include <QObject>
#include <QMutex>
#include "type.h"

class Eeprom : public QObject
{
    Q_OBJECT
public:
    explicit Eeprom(QObject *parent = 0);
    bool open();
    bool close();
    int32 read(void* data,int32 len);
    int32 write(void* data,int32 len);
    
private:
    int32 fd;
    QMutex mutex_r;
    QMutex mutex_w;
};

#endif // EEPROM_H
