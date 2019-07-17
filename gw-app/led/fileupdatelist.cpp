#include <QCoreApplication>
#include <QVariant>
#include "fileupdatelist.h"

FileUpdateList::FileUpdateList()
{
    mDb = QSqlDatabase::addDatabase("QSQLITE");
    mDb.setDatabaseName(QCoreApplication::applicationDirPath()+"UPDATE.db");
    if(!mDb.open()) {
        flag = false;
        return;
    }

    flag = true;
}

FileUpdateList* FileUpdateList::getInstance()
{
    static FileUpdateList instance;
    return &instance;
}

FileUpdateList::FileInfo* FileUpdateList::get()
{
    if(!flag)
        return NULL;

    mutex.lock();
    mQuery.prepare("SELECT * FROM T_USER_MANAGE");
    mQuery.exec();
    if(mQuery.first()) {
        bool ok;
        FileInfo* info;
        info->id = mQuery.value(0).toInt(&ok);
        info->type = mQuery.value(1).toInt(&ok);
        info->name = mQuery.value(2).toString();
        mutex.unlock();
        return info;
    }
    mutex.unlock();
    return NULL;
}

void FileUpdateList::add(FileUpdateList::FileInfo info)
{
    if(!flag)
        return ;

    QString id = QString::number(info.id);
    QString type = QString::number(info.type);

    mutex.lock();
    mQuery.prepare("DELETE FROM T_USER_MANAGE WHERE name='" + info.name + "'");
    mQuery.exec();
    mQuery.prepare("INSERT INTO T_USER_MANAGE (id, type, name) VALUES (:id, :type, :name)");
    mQuery.bindValue(":id", id);	//给每个插入值标识符设定具体值
    mQuery.bindValue(":type", type);
    mQuery.bindValue(":name", info.name);
    mQuery.exec();
    mutex.unlock();
}

void FileUpdateList::remove(QString name)
{
    if(!flag)
        return ;

    mutex.lock();
    mQuery.prepare("DELETE FROM T_USER_MANAGE WHERE name='" + name + "'");
    mQuery.exec();
    mutex.unlock();
}
