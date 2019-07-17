#include "httpworker.h"
#include "common/fileutils.h"
#include "common/mydebug.h"
#include <QThread>

#define DOWNLOAD_FILE_SUFFIX    ".tmp"
HttpWorker::HttpWorker(QObject *parent)
    : QObject(parent)
    , mNetworkAccessManager(NULL)
    , mUrl(QUrl(""))
    , mFileName("")
    , mIsStop(true)
{

}

HttpWorker::~HttpWorker()
{

}

void HttpWorker::init()
{
    mNetworkAccessManager = new QNetworkAccessManager();
}

// 开始下载文件，传入下载链接和文件的路径;
void HttpWorker::download(QString url,QString fileName)
{
    // 防止多次点击开始下载按钮，进行多次下载，只有在停止标志变量为true时才进行下载;
    if(mIsStop) {
        mIsStop = false;
        mUrl = QUrl(url);

        // 将当前文件名设置为临时文件名，下载完成时修改回来;
        mFileName = fileName + DOWNLOAD_FILE_SUFFIX;
        FileUtils::rmFile(mFileName);
        QNetworkRequest request;
        request.setUrl(mUrl);

        // 请求下载;
        mReply = mNetworkAccessManager->get(request);
        connect(mReply, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
        connect(mReply, SIGNAL(finished()), this, SLOT(onFinished()));
        connect(mReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(onError(QNetworkReply::NetworkError)));
    }
}

// 获取下载内容，保存到文件中;
void HttpWorker::onReadyRead()
{
    if(!mIsStop) {
        QFile file(mFileName);
        if(file.open(QIODevice::WriteOnly | QIODevice::Append)) {
            file.write(mReply->readAll());
        }
        file.close();
    }
}

// 下载完成;
void HttpWorker::onFinished()
{
    mIsStop = true;
    // http请求状态码;
    QVariant code = mReply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

    if(mReply->error() == QNetworkReply::NoError) {
        // 重命名临时文件;
        QFileInfo fileInfo(mFileName);
        if(fileInfo.exists()) {
            int index = mFileName.lastIndexOf(DOWNLOAD_FILE_SUFFIX);
            QString realName = mFileName.left(index);
            QFile::rename(mFileName,realName);
        }
    } else {
        MY_WARNING(mReply->errorString());
    }

    stop();
    emit finished(code.toInt());
}

// 下载过程中出现错误，关闭下载，并上报错误，这里未上报错误类型，可自己定义进行上报;
void HttpWorker::onError(QNetworkReply::NetworkError code)
{
    MY_WARNING(mReply->errorString());
    stop();
    emit error(code);
}

// 停止下载工作;
void HttpWorker::stop()
{
    mIsStop = true;
    if(mReply != NULL) {
        disconnect(mReply,SIGNAL(readyRead()),this,SLOT(onReadyRead()));
        disconnect(mReply,SIGNAL(finished()),this,SLOT(onFinished()));
        disconnect(mReply,SIGNAL(error(QNetworkReply::NetworkError)),this,SLOT(onError(QNetworkReply::NetworkError)));
        mReply->abort();
        mReply->deleteLater();
        mReply = NULL;
    }
    FileUtils::rmFile(mFileName);
}
