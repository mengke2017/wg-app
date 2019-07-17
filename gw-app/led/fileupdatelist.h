#ifndef FILEUPDATELIST_H
#define FILEUPDATELIST_H

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QMutex>

class FileUpdateList
{
public:
    typedef struct {
        quint8 id;      //文件更新外设ID
        quint8 type;    //文件类型
        QString name;   //文件名
    } FileInfo;

protected:
    FileUpdateList();

public:
    static FileUpdateList* getInstance();
    FileInfo* get();
    void add(FileInfo info);
    void remove(QString name);

private:
    QSqlDatabase mDb;
    QSqlQuery mQuery;
    QList<FileInfo> mFileList;
    QMutex mutex;
    bool flag;
};

#endif // FILEUPDATELIST_H
