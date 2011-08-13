/****************************************************************************
** Meta object code from reading C++ file 'gyro_widget.h'
**
** Created: Thu Aug 11 12:33:55 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "gyro_widget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'gyro_widget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_gyro_widget[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

static const char qt_meta_stringdata_gyro_widget[] = {
    "gyro_widget\0"
};

const QMetaObject gyro_widget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_gyro_widget,
      qt_meta_data_gyro_widget, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &gyro_widget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *gyro_widget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *gyro_widget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_gyro_widget))
        return static_cast<void*>(const_cast< gyro_widget*>(this));
    return QWidget::qt_metacast(_clname);
}

int gyro_widget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
