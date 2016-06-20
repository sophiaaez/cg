/****************************************************************************
** Meta object code from reading C++ file 'oglwidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../fliper/oglwidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'oglwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_OGLWidget_t {
    QByteArrayData data[18];
    char stringdata0[147];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_OGLWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_OGLWidget_t qt_meta_stringdata_OGLWidget = {
    {
QT_MOC_LITERAL(0, 0, 9), // "OGLWidget"
QT_MOC_LITERAL(1, 10, 14), // "changeRotation"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 2), // "dx"
QT_MOC_LITERAL(4, 29, 2), // "dy"
QT_MOC_LITERAL(5, 32, 2), // "dz"
QT_MOC_LITERAL(6, 35, 13), // "stepAnimation"
QT_MOC_LITERAL(7, 49, 8), // "setCubeX"
QT_MOC_LITERAL(8, 58, 5), // "newrx"
QT_MOC_LITERAL(9, 64, 8), // "setCubeZ"
QT_MOC_LITERAL(10, 73, 5), // "newry"
QT_MOC_LITERAL(11, 79, 12), // "setCylinderX"
QT_MOC_LITERAL(12, 92, 5), // "newrz"
QT_MOC_LITERAL(13, 98, 12), // "setCylinderZ"
QT_MOC_LITERAL(14, 111, 8), // "setStart"
QT_MOC_LITERAL(15, 120, 3), // "bla"
QT_MOC_LITERAL(16, 124, 7), // "setFlip"
QT_MOC_LITERAL(17, 132, 14) // "setPerspective"

    },
    "OGLWidget\0changeRotation\0\0dx\0dy\0dz\0"
    "stepAnimation\0setCubeX\0newrx\0setCubeZ\0"
    "newry\0setCylinderX\0newrz\0setCylinderZ\0"
    "setStart\0bla\0setFlip\0setPerspective"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_OGLWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    3,   59,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    0,   66,    2, 0x0a /* Public */,
       7,    1,   67,    2, 0x0a /* Public */,
       9,    1,   70,    2, 0x0a /* Public */,
      11,    1,   73,    2, 0x0a /* Public */,
      13,    1,   76,    2, 0x0a /* Public */,
      14,    1,   79,    2, 0x0a /* Public */,
      16,    1,   82,    2, 0x0a /* Public */,
      17,    1,   85,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int,    3,    4,    5,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void, QMetaType::Int,   12,
    QMetaType::Void, QMetaType::Int,   12,
    QMetaType::Void, QMetaType::Bool,   15,
    QMetaType::Void, QMetaType::Bool,   15,
    QMetaType::Void, QMetaType::Bool,   15,

       0        // eod
};

void OGLWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        OGLWidget *_t = static_cast<OGLWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->changeRotation((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 1: _t->stepAnimation(); break;
        case 2: _t->setCubeX((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->setCubeZ((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->setCylinderX((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->setCylinderZ((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->setStart((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->setFlip((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: _t->setPerspective((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (OGLWidget::*_t)(int , int , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&OGLWidget::changeRotation)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject OGLWidget::staticMetaObject = {
    { &QOpenGLWidget::staticMetaObject, qt_meta_stringdata_OGLWidget.data,
      qt_meta_data_OGLWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *OGLWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *OGLWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_OGLWidget.stringdata0))
        return static_cast<void*>(const_cast< OGLWidget*>(this));
    if (!strcmp(_clname, "QOpenGLFunctions"))
        return static_cast< QOpenGLFunctions*>(const_cast< OGLWidget*>(this));
    return QOpenGLWidget::qt_metacast(_clname);
}

int OGLWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QOpenGLWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void OGLWidget::changeRotation(int _t1, int _t2, int _t3)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
