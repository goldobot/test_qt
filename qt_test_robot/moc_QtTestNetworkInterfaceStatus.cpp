/****************************************************************************
** Meta object code from reading C++ file 'QtTestNetworkInterfaceStatus.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "QtTestNetworkInterfaceStatus.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QtTestNetworkInterfaceStatus.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QT_TEST__QtTestNetworkInterfaceStatus[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: signature, parameters, type, tag, flags
      39,   38,   38,   38, 0x05,
      62,   38,   38,   38, 0x05,
      85,   38,   38,   38, 0x05,
     112,   38,   38,   38, 0x05,

 // slots: signature, parameters, type, tag, flags
     141,   38,   38,   38, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_QT_TEST__QtTestNetworkInterfaceStatus[] = {
    "QT_TEST::QtTestNetworkInterfaceStatus\0"
    "\0NetworkInterfaceIsOK()\0NetworkInterfaceIsKO()\0"
    "NetworkInterfaceIsActive()\0"
    "NetworkInterfaceIsInactive()\0Monitor()\0"
};

void QT_TEST::QtTestNetworkInterfaceStatus::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QtTestNetworkInterfaceStatus *_t = static_cast<QtTestNetworkInterfaceStatus *>(_o);
        switch (_id) {
        case 0: _t->NetworkInterfaceIsOK(); break;
        case 1: _t->NetworkInterfaceIsKO(); break;
        case 2: _t->NetworkInterfaceIsActive(); break;
        case 3: _t->NetworkInterfaceIsInactive(); break;
        case 4: _t->Monitor(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData QT_TEST::QtTestNetworkInterfaceStatus::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QT_TEST::QtTestNetworkInterfaceStatus::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QT_TEST__QtTestNetworkInterfaceStatus,
      qt_meta_data_QT_TEST__QtTestNetworkInterfaceStatus, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QT_TEST::QtTestNetworkInterfaceStatus::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QT_TEST::QtTestNetworkInterfaceStatus::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QT_TEST::QtTestNetworkInterfaceStatus::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QT_TEST__QtTestNetworkInterfaceStatus))
        return static_cast<void*>(const_cast< QtTestNetworkInterfaceStatus*>(this));
    return QObject::qt_metacast(_clname);
}

int QT_TEST::QtTestNetworkInterfaceStatus::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void QT_TEST::QtTestNetworkInterfaceStatus::NetworkInterfaceIsOK()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void QT_TEST::QtTestNetworkInterfaceStatus::NetworkInterfaceIsKO()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void QT_TEST::QtTestNetworkInterfaceStatus::NetworkInterfaceIsActive()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void QT_TEST::QtTestNetworkInterfaceStatus::NetworkInterfaceIsInactive()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}
QT_END_MOC_NAMESPACE
