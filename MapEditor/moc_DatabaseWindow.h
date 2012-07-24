/****************************************************************************
** Meta object code from reading C++ file 'DatabaseWindow.h'
**
** Created: Tue 24. Jul 17:41:53 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "DatabaseWindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DatabaseWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DatabaseWindow[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x08,
      32,   15,   15,   15, 0x08,
      59,   51,   15,   15, 0x08,
      84,   51,   15,   15, 0x08,
     109,   15,   15,   15, 0x08,
     133,   15,   15,   15, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_DatabaseWindow[] = {
    "DatabaseWindow\0\0ClickButtonOK()\0"
    "ClickButtonApply()\0p_Index\0"
    "TextureTabChangeCur(int)\0"
    "TextureBoxChangeCur(int)\0"
    "ClickButtonNewTexture()\0"
    "ClickButtonDeleteTexture()\0"
};

void DatabaseWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        DatabaseWindow *_t = static_cast<DatabaseWindow *>(_o);
        switch (_id) {
        case 0: _t->ClickButtonOK(); break;
        case 1: _t->ClickButtonApply(); break;
        case 2: _t->TextureTabChangeCur((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->TextureBoxChangeCur((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->ClickButtonNewTexture(); break;
        case 5: _t->ClickButtonDeleteTexture(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData DatabaseWindow::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject DatabaseWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_DatabaseWindow,
      qt_meta_data_DatabaseWindow, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DatabaseWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DatabaseWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DatabaseWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DatabaseWindow))
        return static_cast<void*>(const_cast< DatabaseWindow*>(this));
    if (!strcmp(_clname, "Ui_Database"))
        return static_cast< Ui_Database*>(const_cast< DatabaseWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int DatabaseWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
