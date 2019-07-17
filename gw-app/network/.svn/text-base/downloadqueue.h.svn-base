#ifndef DOWNLOADQUEUE_H
#define DOWNLOADQUEUE_H

#include <QMutex>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include "type.h"

class DownloadQueue
{
public:
    typedef struct {
        int32 filetype;
        QByteArray md5;
        QString url;
    } TaskInfo;

protected:
    DownloadQueue();

public:
    static DownloadQueue* getInstance();
    void getDownloadTask(TaskInfo* info);
    void addDownloadTask(TaskInfo info);
    void rmDownloadTask();
    bool getTaskStop();

private:
    QSqlDatabase mDb;
    QSqlQuery mQuery;
    QMutex mutex;
    volatile bool flag;
    volatile bool isStop;
};

#endif // DOWNLOADQUEUE_H
