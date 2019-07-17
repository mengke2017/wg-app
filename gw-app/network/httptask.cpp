#include "httptask.h"
#include <QThread>
#include "common/mydebug.h"

HttpTask::HttpTask(QObject *parent) :
    QObject(parent)
    ,mDownloadWorker(NULL)
    ,mDownloadState(STATE_IDLE)
{
    mDownloadWorker = new HttpWorker();
    thread = new QThread(this);
    mDownloadWorker->moveToThread(thread);
    connect(thread,SIGNAL(started()),mDownloadWorker,SLOT(init()));
    thread->start();
    connect(this,SIGNAL(download(QString,QString)),mDownloadWorker,SLOT(download(QString,QString)));
    connect(mDownloadWorker,SIGNAL(finished(int)),this,SLOT(onDownloadFinished()));
    connect(mDownloadWorker,SIGNAL(error(int)),this,SLOT(onDownloadError()));
}

HttpTask *HttpTask::getInstance()
{
    static HttpTask instance;
    return &instance;
}

HttpTask::TaskState HttpTask::getTaskState()
{
    return mDownloadState;
}

bool HttpTask::downloadFile(QString url, QString filename)
{
    if(NULL == mDownloadWorker
            ||(STATE_IDLE != mDownloadState&&STATE_END != mDownloadState))
        return false;

    MY_DEBUG(QString("url=%1 file=%2").arg(url).arg(filename));
    mDownloadState = STATE_START;
    emit download(url,filename);
    return true;
}

void HttpTask::stop()
{
    MY_DEBUG("stop");
    mDownloadWorker->stop();
    mDownloadState = STATE_IDLE;
}

void HttpTask::onDownloadFinished()
{
    MY_DEBUG("finish");
    mDownloadState = STATE_END;
}

void HttpTask::onDownloadError()
{
    MY_DEBUG("finish");
    mDownloadState = STATE_ERROR;
}
