/****************************************************************************
** Meta object code from reading C++ file 'httptask.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../gw-app/network/httptask.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'httptask.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_HttpTask[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      23,   10,    9,    9, 0x05,

 // slots: signature, parameters, type, tag, flags
      49,    9,    9,    9, 0x08,
      70,    9,    9,    9, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_HttpTask[] = {
    "HttpTask\0\0url,filename\0download(QString,QString)\0"
    "onDownloadFinished()\0onDownloadError()\0"
};

void HttpTask::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        HttpTask *_t = static_cast<HttpTask *>(_o);
        switch (_id) {
        case 0: _t->download((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 1: _t->onDownloadFinished(); break;
        case 2: _t->onDownloadError(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData HttpTask::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject HttpTask::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_HttpTask,
      qt_meta_data_HttpTask, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &HttpTask::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *HttpTask::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *HttpTask::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_HttpTask))
        return static_cast<void*>(const_cast< HttpTask*>(this));
    return QObject::qt_metacast(_clname);
}

int HttpTask::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void HttpTask::download(QString _t1, QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
