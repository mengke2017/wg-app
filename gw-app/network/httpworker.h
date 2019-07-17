#ifndef HTTPWORKER_H
#define HTTPWORKER_H

#include <QObject>
#include <QNetworkReply>
#include <QMutex>
#include "type.h"

class HttpWorker : public QObject
{
    Q_OBJECT
public:
    explicit HttpWorker(QObject *parent = 0);
    ~HttpWorker();

    void stop();

private:
    QNetworkAccessManager *mNetworkAccessManager;
    QNetworkReply* mReply;
    QUrl mUrl;
    QString mFileName;
    volatile bool mIsStop;

signals:
    void finished(int code);
    void error(int code);    

public slots:
    void init();
    void download(QString url,QString fileName);

private slots:
    void onReadyRead();
    void onFinished();
    void onError(QNetworkReply::NetworkError error);
};

#endif // HTTPWORKER_H
