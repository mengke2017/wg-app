/****************************************************************************
** Meta object code from reading C++ file 'ledmanager.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../gw-app/led/ledmanager.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ledmanager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_LedManager[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      16,   12,   11,   11, 0x05,
      40,   35,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
      54,   12,   11,   11, 0x0a,
      78,   12,   11,   11, 0x0a,
     110,  105,   11,   11, 0x08,
     141,   11,   11,   11, 0x08,
     155,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_LedManager[] = {
    "LedManager\0\0msg\0toNetwork(Message)\0"
    "flag\0UpdateOK(int)\0netWorkHandler(Message)\0"
    "busboxHandler(bus_message)\0data\0"
    "slotComDataHandler(QByteArray)\0"
    "slotTimeOut()\0slotSendTask()\0"
};

void LedManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        LedManager *_t = static_cast<LedManager *>(_o);
        switch (_id) {
        case 0: _t->toNetwork((*reinterpret_cast< Message(*)>(_a[1]))); break;
        case 1: _t->UpdateOK((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->netWorkHandler((*reinterpret_cast< Message(*)>(_a[1]))); break;
        case 3: _t->busboxHandler((*reinterpret_cast< bus_message(*)>(_a[1]))); break;
        case 4: _t->slotComDataHandler((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 5: _t->slotTimeOut(); break;
        case 6: _t->slotSendTask(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData LedManager::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject LedManager::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_LedManager,
      qt_meta_data_LedManager, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &LedManager::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *LedManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *LedManager::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_LedManager))
        return static_cast<void*>(const_cast< LedManager*>(this));
    return QObject::qt_metacast(_clname);
}

int LedManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void LedManager::toNetwork(Message _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void LedManager::UpdateOK(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
