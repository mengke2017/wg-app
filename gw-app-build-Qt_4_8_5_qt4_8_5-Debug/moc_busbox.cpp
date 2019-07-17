/****************************************************************************
** Meta object code from reading C++ file 'busbox.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../gw-app/busbox/busbox.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'busbox.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_BusBox[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      12,    8,    7,    7, 0x05,

 // slots: signature, parameters, type, tag, flags
      36,   31,    7,    7, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_BusBox[] = {
    "BusBox\0\0msg\0toLed(bus_message)\0data\0"
    "slotComDataHandler(QByteArray)\0"
};

void BusBox::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        BusBox *_t = static_cast<BusBox *>(_o);
        switch (_id) {
        case 0: _t->toLed((*reinterpret_cast< bus_message(*)>(_a[1]))); break;
        case 1: _t->slotComDataHandler((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData BusBox::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject BusBox::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_BusBox,
      qt_meta_data_BusBox, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &BusBox::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *BusBox::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *BusBox::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_BusBox))
        return static_cast<void*>(const_cast< BusBox*>(this));
    return QObject::qt_metacast(_clname);
}

int BusBox::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void BusBox::toLed(bus_message _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
