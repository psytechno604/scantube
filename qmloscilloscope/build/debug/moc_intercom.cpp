/****************************************************************************
** Meta object code from reading C++ file 'intercom.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../intercom.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'intercom.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_intercom_t {
    QByteArrayData data[14];
    char stringdata0[106];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_intercom_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_intercom_t qt_meta_stringdata_intercom = {
    {
QT_MOC_LITERAL(0, 0, 8), // "intercom"
QT_MOC_LITERAL(1, 9, 15), // "processDatagram"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 2), // "on"
QT_MOC_LITERAL(4, 29, 3), // "off"
QT_MOC_LITERAL(5, 33, 7), // "setMyIP"
QT_MOC_LITERAL(6, 41, 2), // "ip"
QT_MOC_LITERAL(7, 44, 15), // "setAccumulation"
QT_MOC_LITERAL(8, 60, 3), // "acc"
QT_MOC_LITERAL(9, 64, 8), // "setSpeed"
QT_MOC_LITERAL(10, 73, 3), // "spd"
QT_MOC_LITERAL(11, 77, 13), // "setDataSource"
QT_MOC_LITERAL(12, 91, 11), // "DataSource*"
QT_MOC_LITERAL(13, 103, 2) // "ds"

    },
    "intercom\0processDatagram\0\0on\0off\0"
    "setMyIP\0ip\0setAccumulation\0acc\0setSpeed\0"
    "spd\0setDataSource\0DataSource*\0ds"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_intercom[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   49,    2, 0x0a /* Public */,

 // methods: name, argc, parameters, tag, flags
       3,    0,   50,    2, 0x02 /* Public */,
       4,    0,   51,    2, 0x02 /* Public */,
       5,    1,   52,    2, 0x02 /* Public */,
       7,    1,   55,    2, 0x02 /* Public */,
       9,    1,   58,    2, 0x02 /* Public */,
      11,    1,   61,    2, 0x02 /* Public */,

 // slots: parameters
    QMetaType::Void,

 // methods: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void, QMetaType::QString,   10,
    QMetaType::Void, 0x80000000 | 12,   13,

       0        // eod
};

void intercom::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        intercom *_t = static_cast<intercom *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->processDatagram(); break;
        case 1: _t->on(); break;
        case 2: _t->off(); break;
        case 3: _t->setMyIP((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->setAccumulation((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: _t->setSpeed((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: _t->setDataSource((*reinterpret_cast< DataSource*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 6:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< DataSource* >(); break;
            }
            break;
        }
    }
}

const QMetaObject intercom::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_intercom.data,
      qt_meta_data_intercom,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *intercom::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *intercom::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_intercom.stringdata0))
        return static_cast<void*>(const_cast< intercom*>(this));
    return QObject::qt_metacast(_clname);
}

int intercom::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
