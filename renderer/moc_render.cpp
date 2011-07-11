/****************************************************************************
** Meta object code from reading C++ file 'render.h'
**
** Created: Tue Jul 5 11:55:00 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "render.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'render.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Render[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
       8,    7,    7,    7, 0x0a,
      16,    7,    7,    7, 0x0a,
      23,    7,    7,    7, 0x0a,
      36,    7,    7,    7, 0x0a,
      49,    7,   44,    7, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Render[] = {
    "Render\0\0Pause()\0Play()\0setShowAvg()\0"
    "setup()\0bool\0connectEmotiv()\0"
};

const QMetaObject Render::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Render,
      qt_meta_data_Render, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Render::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Render::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Render::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Render))
        return static_cast<void*>(const_cast< Render*>(this));
    return QWidget::qt_metacast(_clname);
}

int Render::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: Pause(); break;
        case 1: Play(); break;
        case 2: setShowAvg(); break;
        case 3: setup(); break;
        case 4: { bool _r = connectEmotiv();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        default: ;
        }
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
