/****************************************************************************
** Meta object code from reading C++ file 'simport.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../gw-app/sim/simport.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'simport.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SimPort[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      14,    9,    8,    8, 0x05,
      34,    8,    8,    8, 0x05,

 // slots: signature, parameters, type, tag, flags
      51,    8,    8,    8, 0x08,
      63,    8,    8,    8, 0x08,
      77,    8,    8,    8, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_SimPort[] = {
    "SimPort\0\0data\0readCom(QByteArray)\0"
    "getIMSISuccess()\0readMyCom()\0slotTimeOut()\0"
    "slotLowPower()\0"
};

void SimPort::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        SimPort *_t = static_cast<SimPort *>(_o);
        switch (_id) {
        case 0: _t->readCom((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 1: _t->getIMSISuccess(); break;
        case 2: _t->readMyCom(); break;
        case 3: _t->slotTimeOut(); break;
        case 4: _t->slotLowPower(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData SimPort::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject SimPort::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_SimPort,
      qt_meta_data_SimPort, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SimPort::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SimPort::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SimPort::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SimPort))
        return static_cast<void*>(const_cast< SimPort*>(this));
    return QObject::qt_metacast(_clname);
}

int SimPort::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void SimPort::readCom(QByteArray _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SimPort::getIMSISuccess()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
QT_END_MOC_NAMESPACE
