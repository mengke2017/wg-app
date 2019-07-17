#ifndef FILEUTILS_H
#define FILEUTILS_H
#include <QObject>
#include <QStringList>
#include <QFileInfo>
#include "type.h"

class FileUtils : public QObject
{
	Q_OBJECT
public:
	Q_INVOKABLE static bool exists(const QString& fileName);

public:
	FileUtils();
	virtual ~FileUtils();
	static QString getAppInstancePath() ;
	static QByteArray readLocalFile(const QString& path);
	static QString readAllText(const QString &fileName);
	static QString getFullPath(const QString& fileName);
	
	static bool writeText(const QString& fileName, QString& content);
	static bool existsFullPath(const QString& fileName);
	static bool makeDir(const QString& strDir);
	static bool makePath(const QString& strPath);
	static void rmPath(const QString& strDir);
	static bool copyFile(const QString& srcFile, const QString& destFile);
	static bool saveFile(const QByteArray* content, const QString& fileName, bool bCover = true);
	static bool renameFile(const QString& fileName, const QString& newName );
    static bool createFile(const QString& fileName, int32 initSize = 0, bool bForce = false);
	static bool rmFile(const QString& fileName);
	static bool isAbsolute(const QString& filePath);
    static int64 getFileSize(const QString& filePath);

    static int32 fileCount(const QString& filePath);
	static QStringList getFileList(const QString& dir);
private:
	static void delDir(QFileInfo fileList);
    static void fileCounter(const QString& filePath, int32* pCounter);
	static QString readAllTextFullPath(const QString& fileName);
};


#endif // FILEUTILS_H
