/****************************************************************************
** Meta object code from reading C++ file 'renderer.h'
**
** Created: Wed Aug 3 14:58:10 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "renderer.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'renderer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Renderer[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      10,    9,    9,    9, 0x0a,
      23,    9,    9,    9, 0x0a,
      36,    9,   31,    9, 0x0a,
      52,    9,    9,    9, 0x0a,
      65,    9,    9,    9, 0x0a,
      78,    9,    9,    9, 0x0a,
      96,    9,   92,    9, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Renderer[] = {
    "Renderer\0\0setShowAvg()\0setup()\0bool\0"
    "connectEmotiv()\0play_pause()\0setKey_Kat()\0"
    "setKey_Will()\0int\0connect_server()\0"
};

const QMetaObject Renderer::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Renderer,
      qt_meta_data_Renderer, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Renderer::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Renderer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Renderer::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Renderer))
        return static_cast<void*>(const_cast< Renderer*>(this));
    return QWidget::qt_metacast(_clname);
}

int Renderer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: setShowAvg(); break;
        case 1: setup(); break;
        case 2: { bool _r = connectEmotiv();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 3: play_pause(); break;
        case 4: setKey_Kat(); break;
        case 5: setKey_Will(); break;
        case 6: { int _r = connect_server();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        default: ;
        }
        _id -= 7;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
