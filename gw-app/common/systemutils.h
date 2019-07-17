/****************************************************************************
** 
**说明: 封装和系统相关的线程、进程等的功能函数
**
****************************************************************************/

#ifndef SYSTEMUTILS_H
#define SYSTEMUTILS_H

#include <QObject>
#include "type.h"

class SystemUtils: public QObject
{
	Q_OBJECT
public:
    static uint16 rand16();

    static QByteArray toQByteArray(const uint8* ch,int32 size);
    static QByteArray u32ToQByteArray(uint32 value);
    static QByteArray u16ToQByteArray(uint16 value);
    static QByteArray u8ToQByteArray(uint8 value);
    static bool stringToHex(QString str, uint32* value);

    static QString utf8ToUnicode(QString str);
    static QByteArray unicodeToGbk(QString str);
    static QString gbkToUnicode(QByteArray str);
    static QByteArray utf8ToGbk(QString str);
    static QByteArray gbkToUtf8(QByteArray str);
    static void escape(QByteArray *array,const char *s,int32 size,const char *c);

    static QString extractSuffix(QString str);
    static QString getFileMd5(QString filePath);

    static uint8 XOR(QByteArray *array,int32 start,int32 end);
    static uint16 CRC16(QByteArray* array,int32 start,int32 end);

    static QByteArray getSystemVersion();

    static QString removefirst0(QString s);
};

#endif
