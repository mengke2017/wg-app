#ifndef HTTPTASK_H
#define HTTPTASK_H

#include <QObject>
#include "type.h"
#include "network/httpworker.h"

class HttpTask : public QObject
{
    Q_OBJECT
public:
    enum TaskState
    {
        STATE_IDLE,
        STATE_START,
        STATE_ERROR,
        STATE_END
    };

protected:
    HttpTask(QObject *parent = 0);
    
private:
    QThread* thread;
    HttpWorker* mDownloadWorker;
    volatile HttpTask::TaskState mDownloadState;

public:
    static HttpTask *getInstance();
    HttpTask::TaskState getTaskState();
    bool downloadFile(QString url,QString filename);
    void stop();
    
private:
    void clearDownloadTask();

signals:
    void download(QString url, QString filename);

private slots:
    void onDownloadFinished();
    void onDownloadError();
};

#endif // HTTPTASK_H
