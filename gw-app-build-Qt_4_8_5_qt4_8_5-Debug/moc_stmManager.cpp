/****************************************************************************
** Meta object code from reading C++ file 'stmManager.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../gw-app/stm/stmManager.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'stmManager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_stmManager[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      17,   12,   11,   11, 0x08,
      48,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_stmManager[] = {
    "stmManager\0\0data\0slotComDataHandler(QByteArray)\0"
    "timeout()\0"
};

void stmManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        stmManager *_t = static_cast<stmManager *>(_o);
        switch (_id) {
        case 0: _t->slotComDataHandler((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 1: _t->timeout(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData stmManager::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject stmManager::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_stmManager,
      qt_meta_data_stmManager, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &stmManager::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *stmManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *stmManager::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_stmManager))
        return static_cast<void*>(const_cast< stmManager*>(this));
    return QObject::qt_metacast(_clname);
}

int stmManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
QT_END_MOC_NAMESPACE
