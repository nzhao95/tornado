/****************************************************************************
** Meta object code from reading C++ file 'MyViewer.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/MyViewer.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MyViewer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MyViewer_t {
    QByteArrayData data[15];
    char stringdata0[193];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MyViewer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MyViewer_t qt_meta_stringdata_MyViewer = {
    {
QT_MOC_LITERAL(0, 0, 8), // "MyViewer"
QT_MOC_LITERAL(1, 9, 18), // "windowTitleUpdated"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 9), // "open_mesh"
QT_MOC_LITERAL(4, 39, 9), // "save_mesh"
QT_MOC_LITERAL(5, 49, 12), // "showControls"
QT_MOC_LITERAL(6, 62, 16), // "saveCameraInFile"
QT_MOC_LITERAL(7, 79, 8), // "filename"
QT_MOC_LITERAL(8, 88, 18), // "openCameraFromFile"
QT_MOC_LITERAL(9, 107, 10), // "openCamera"
QT_MOC_LITERAL(10, 118, 10), // "saveCamera"
QT_MOC_LITERAL(11, 129, 20), // "saveSnapShotPlusPlus"
QT_MOC_LITERAL(12, 150, 14), // "toggle_drawing"
QT_MOC_LITERAL(13, 165, 21), // "toggle_control_points"
QT_MOC_LITERAL(14, 187, 5) // "clear"

    },
    "MyViewer\0windowTitleUpdated\0\0open_mesh\0"
    "save_mesh\0showControls\0saveCameraInFile\0"
    "filename\0openCameraFromFile\0openCamera\0"
    "saveCamera\0saveSnapShotPlusPlus\0"
    "toggle_drawing\0toggle_control_points\0"
    "clear"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MyViewer[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   74,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   77,    2, 0x0a /* Public */,
       4,    0,   78,    2, 0x0a /* Public */,
       5,    0,   79,    2, 0x0a /* Public */,
       6,    1,   80,    2, 0x0a /* Public */,
       8,    1,   83,    2, 0x0a /* Public */,
       9,    0,   86,    2, 0x0a /* Public */,
      10,    0,   87,    2, 0x0a /* Public */,
      11,    0,   88,    2, 0x0a /* Public */,
      12,    0,   89,    2, 0x0a /* Public */,
      13,    0,   90,    2, 0x0a /* Public */,
      14,    0,   91,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MyViewer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MyViewer *_t = static_cast<MyViewer *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->windowTitleUpdated((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->open_mesh(); break;
        case 2: _t->save_mesh(); break;
        case 3: _t->showControls(); break;
        case 4: _t->saveCameraInFile((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 5: _t->openCameraFromFile((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 6: _t->openCamera(); break;
        case 7: _t->saveCamera(); break;
        case 8: _t->saveSnapShotPlusPlus(); break;
        case 9: _t->toggle_drawing(); break;
        case 10: _t->toggle_control_points(); break;
        case 11: _t->clear(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (MyViewer::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MyViewer::windowTitleUpdated)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject MyViewer::staticMetaObject = {
    { &QGLViewer::staticMetaObject, qt_meta_stringdata_MyViewer.data,
      qt_meta_data_MyViewer,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *MyViewer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MyViewer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MyViewer.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "QOpenGLFunctions_4_3_Core"))
        return static_cast< QOpenGLFunctions_4_3_Core*>(this);
    return QGLViewer::qt_metacast(_clname);
}

int MyViewer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGLViewer::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void MyViewer::windowTitleUpdated(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
