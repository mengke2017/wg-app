/****************************************************************************
** Meta object code from reading C++ file 'lcdmanager.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../gw-app/lcd/lcdmanager.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'lcdmanager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_LcdManager[] = {

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
      16,   12,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
      35,   12,   11,   11, 0x0a,
      64,   59,   11,   11, 0x0a,
      85,   59,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_LcdManager[] = {
    "LcdManager\0\0msg\0toNetwork(Message)\0"
    "networkHandler(Message)\0data\0"
    "gpsHandler(gps_data)\0portHandler(QByteArray)\0"
};

void LcdManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        LcdManager *_t = static_cast<LcdManager *>(_o);
        switch (_id) {
        case 0: _t->toNetwork((*reinterpret_cast< Message(*)>(_a[1]))); break;
        case 1: _t->networkHandler((*reinterpret_cast< Message(*)>(_a[1]))); break;
        case 2: _t->gpsHandler((*reinterpret_cast< gps_data(*)>(_a[1]))); break;
        case 3: _t->portHandler((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData LcdManager::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject LcdManager::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_LcdManager,
      qt_meta_data_LcdManager, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &LcdManager::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *LcdManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *LcdManager::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_LcdManager))
        return static_cast<void*>(const_cast< LcdManager*>(this));
    return QObject::qt_metacast(_clname);
}

int LcdManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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
void LcdManager::toNetwork(Message _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
