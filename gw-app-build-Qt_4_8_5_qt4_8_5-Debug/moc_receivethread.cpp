/****************************************************************************
** Meta object code from reading C++ file 'receivethread.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../gw-app/can/receivethread.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'receivethread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ReceiveThread[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      21,   15,   14,   14, 0x05,

 // slots: signature, parameters, type, tag, flags
      36,   14,   14,   14, 0x0a,
      42,   14,   14,   14, 0x0a,
      57,   54,   49,   14, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_ReceiveThread[] = {
    "ReceiveThread\0\0frame\0msg(can_frame)\0"
    "run()\0stop()\0bool\0id\0filter(canid_t)\0"
};

void ReceiveThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ReceiveThread *_t = static_cast<ReceiveThread *>(_o);
        switch (_id) {
        case 0: _t->msg((*reinterpret_cast< can_frame(*)>(_a[1]))); break;
        case 1: _t->run(); break;
        case 2: _t->stop(); break;
        case 3: { bool _r = _t->filter((*reinterpret_cast< canid_t(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObjectExtraData ReceiveThread::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ReceiveThread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_ReceiveThread,
      qt_meta_data_ReceiveThread, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ReceiveThread::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ReceiveThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ReceiveThread::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ReceiveThread))
        return static_cast<void*>(const_cast< ReceiveThread*>(this));
    return QThread::qt_metacast(_clname);
}

int ReceiveThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void ReceiveThread::msg(can_frame _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
