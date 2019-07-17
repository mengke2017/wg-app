/****************************************************************************
** Meta object code from reading C++ file 'networkmanager.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../gw-app/network/networkmanager.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'networkmanager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_NetworkManager[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      20,   16,   15,   15, 0x05,
      35,   16,   15,   15, 0x05,
      50,   16,   15,   15, 0x05,

 // slots: signature, parameters, type, tag, flags
      67,   16,   15,   15, 0x0a,
      87,   16,   15,   15, 0x0a,
     107,   16,   15,   15, 0x0a,
     132,  127,   15,   15, 0x0a,
     153,   16,   15,   15, 0x0a,
     175,   15,   15,   15, 0x08,
     185,   16,   15,   15, 0x08,
     207,  204,   15,   15, 0x08,
     227,   15,   15,   15, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_NetworkManager[] = {
    "NetworkManager\0\0msg\0toLed(Message)\0"
    "toLcd(Message)\0toLocal(Message)\0"
    "ledHandler(Message)\0lcdHandler(Message)\0"
    "canHandler(Message)\0data\0gpsHandler(gps_data)\0"
    "localHandler(Message)\0timeout()\0"
    "socketMsg(Message)\0is\0socketConnect(bool)\0"
    "haveIMSI()\0"
};

void NetworkManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        NetworkManager *_t = static_cast<NetworkManager *>(_o);
        switch (_id) {
        case 0: _t->toLed((*reinterpret_cast< Message(*)>(_a[1]))); break;
        case 1: _t->toLcd((*reinterpret_cast< Message(*)>(_a[1]))); break;
        case 2: _t->toLocal((*reinterpret_cast< Message(*)>(_a[1]))); break;
        case 3: _t->ledHandler((*reinterpret_cast< Message(*)>(_a[1]))); break;
        case 4: _t->lcdHandler((*reinterpret_cast< Message(*)>(_a[1]))); break;
        case 5: _t->canHandler((*reinterpret_cast< Message(*)>(_a[1]))); break;
        case 6: _t->gpsHandler((*reinterpret_cast< gps_data(*)>(_a[1]))); break;
        case 7: _t->localHandler((*reinterpret_cast< Message(*)>(_a[1]))); break;
        case 8: _t->timeout(); break;
        case 9: _t->socketMsg((*reinterpret_cast< Message(*)>(_a[1]))); break;
        case 10: _t->socketConnect((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 11: _t->haveIMSI(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData NetworkManager::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject NetworkManager::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_NetworkManager,
      qt_meta_data_NetworkManager, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &NetworkManager::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *NetworkManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *NetworkManager::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_NetworkManager))
        return static_cast<void*>(const_cast< NetworkManager*>(this));
    return QObject::qt_metacast(_clname);
}

int NetworkManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void NetworkManager::toLed(Message _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void NetworkManager::toLcd(Message _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void NetworkManager::toLocal(Message _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
