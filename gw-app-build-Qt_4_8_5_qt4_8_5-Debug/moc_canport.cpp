/****************************************************************************
** Meta object code from reading C++ file 'canport.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../gw-app/can/canport.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'canport.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CanPort[] = {

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
       9,    8,    8,    8, 0x05,

 // slots: signature, parameters, type, tag, flags
      22,   16,    8,    8, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_CanPort[] = {
    "CanPort\0\0init()\0frame\0slotMsgHandler(can_frame)\0"
};

void CanPort::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CanPort *_t = static_cast<CanPort *>(_o);
        switch (_id) {
        case 0: _t->init(); break;
        case 1: _t->slotMsgHandler((*reinterpret_cast< can_frame(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CanPort::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CanPort::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CanPort,
      qt_meta_data_CanPort, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CanPort::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CanPort::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CanPort::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CanPort))
        return static_cast<void*>(const_cast< CanPort*>(this));
    return QObject::qt_metacast(_clname);
}

int CanPort::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void CanPort::init()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
