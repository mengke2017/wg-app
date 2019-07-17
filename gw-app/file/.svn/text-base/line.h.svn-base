#ifndef LINE_H
#define LINE_H

#include <QList>
#include <QMutex>
#include "type.h"

typedef struct _line_info_
{
    QString name;       //线路名
    QString first_up;   //上行首站
    QString end_up;     //上行末站
    QString lines_up;   //上行线路语
    QString first_down; //下行首站
    QString end_down;   //下行末站
    QString lines_down; //下行线路语
}line_info;

class Line
{
protected:
    explicit Line();

public:
    static Line* getInstance();
    void init();
    void get(int32 index,line_info* info);
    int32 size();

private:
    void lock();
    void unlock();
    bool readXML();

private:
    QList<line_info> list;
    volatile int32 listSize;
    QMutex mutex;
};

#endif // LINE_H
