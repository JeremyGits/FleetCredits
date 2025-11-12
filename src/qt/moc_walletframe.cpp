/****************************************************************************
** Meta object code from reading C++ file 'walletframe.h'
**
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "qt/walletframe.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'walletframe.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.13. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_WalletFrame_t {
    QByteArrayData data[23];
    char stringdata0[381];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_WalletFrame_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_WalletFrame_t qt_meta_stringdata_WalletFrame = {
    {
QT_MOC_LITERAL(0, 0, 11), // "WalletFrame"
QT_MOC_LITERAL(1, 12, 24), // "requestedSyncWarningInfo"
QT_MOC_LITERAL(2, 37, 0), // ""
QT_MOC_LITERAL(3, 38, 16), // "gotoOverviewPage"
QT_MOC_LITERAL(4, 55, 15), // "gotoHistoryPage"
QT_MOC_LITERAL(5, 71, 20), // "gotoReceiveCoinsPage"
QT_MOC_LITERAL(6, 92, 17), // "gotoSendCoinsPage"
QT_MOC_LITERAL(7, 110, 4), // "addr"
QT_MOC_LITERAL(8, 115, 17), // "gotoMWEBSuitePage"
QT_MOC_LITERAL(9, 133, 21), // "gotoContributionsPage"
QT_MOC_LITERAL(10, 155, 18), // "gotoGovernancePage"
QT_MOC_LITERAL(11, 174, 18), // "gotoSignMessageTab"
QT_MOC_LITERAL(12, 193, 20), // "gotoVerifyMessageTab"
QT_MOC_LITERAL(13, 214, 13), // "encryptWallet"
QT_MOC_LITERAL(14, 228, 6), // "status"
QT_MOC_LITERAL(15, 235, 12), // "backupWallet"
QT_MOC_LITERAL(16, 248, 16), // "changePassphrase"
QT_MOC_LITERAL(17, 265, 12), // "unlockWallet"
QT_MOC_LITERAL(18, 278, 16), // "importPrivateKey"
QT_MOC_LITERAL(19, 295, 17), // "printPaperWallets"
QT_MOC_LITERAL(20, 313, 20), // "usedSendingAddresses"
QT_MOC_LITERAL(21, 334, 22), // "usedReceivingAddresses"
QT_MOC_LITERAL(22, 357, 23) // "outOfSyncWarningClicked"

    },
    "WalletFrame\0requestedSyncWarningInfo\0"
    "\0gotoOverviewPage\0gotoHistoryPage\0"
    "gotoReceiveCoinsPage\0gotoSendCoinsPage\0"
    "addr\0gotoMWEBSuitePage\0gotoContributionsPage\0"
    "gotoGovernancePage\0gotoSignMessageTab\0"
    "gotoVerifyMessageTab\0encryptWallet\0"
    "status\0backupWallet\0changePassphrase\0"
    "unlockWallet\0importPrivateKey\0"
    "printPaperWallets\0usedSendingAddresses\0"
    "usedReceivingAddresses\0outOfSyncWarningClicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_WalletFrame[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      22,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,  124,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,  125,    2, 0x0a /* Public */,
       4,    0,  126,    2, 0x0a /* Public */,
       5,    0,  127,    2, 0x0a /* Public */,
       6,    1,  128,    2, 0x0a /* Public */,
       6,    0,  131,    2, 0x2a /* Public | MethodCloned */,
       8,    0,  132,    2, 0x0a /* Public */,
       9,    0,  133,    2, 0x0a /* Public */,
      10,    0,  134,    2, 0x0a /* Public */,
      11,    1,  135,    2, 0x0a /* Public */,
      11,    0,  138,    2, 0x2a /* Public | MethodCloned */,
      12,    1,  139,    2, 0x0a /* Public */,
      12,    0,  142,    2, 0x2a /* Public | MethodCloned */,
      13,    1,  143,    2, 0x0a /* Public */,
      15,    0,  146,    2, 0x0a /* Public */,
      16,    0,  147,    2, 0x0a /* Public */,
      17,    0,  148,    2, 0x0a /* Public */,
      18,    0,  149,    2, 0x0a /* Public */,
      19,    0,  150,    2, 0x0a /* Public */,
      20,    0,  151,    2, 0x0a /* Public */,
      21,    0,  152,    2, 0x0a /* Public */,
      22,    0,  153,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   14,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void WalletFrame::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<WalletFrame *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->requestedSyncWarningInfo(); break;
        case 1: _t->gotoOverviewPage(); break;
        case 2: _t->gotoHistoryPage(); break;
        case 3: _t->gotoReceiveCoinsPage(); break;
        case 4: _t->gotoSendCoinsPage((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: _t->gotoSendCoinsPage(); break;
        case 6: _t->gotoMWEBSuitePage(); break;
        case 7: _t->gotoContributionsPage(); break;
        case 8: _t->gotoGovernancePage(); break;
        case 9: _t->gotoSignMessageTab((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 10: _t->gotoSignMessageTab(); break;
        case 11: _t->gotoVerifyMessageTab((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 12: _t->gotoVerifyMessageTab(); break;
        case 13: _t->encryptWallet((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 14: _t->backupWallet(); break;
        case 15: _t->changePassphrase(); break;
        case 16: _t->unlockWallet(); break;
        case 17: _t->importPrivateKey(); break;
        case 18: _t->printPaperWallets(); break;
        case 19: _t->usedSendingAddresses(); break;
        case 20: _t->usedReceivingAddresses(); break;
        case 21: _t->outOfSyncWarningClicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (WalletFrame::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&WalletFrame::requestedSyncWarningInfo)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject WalletFrame::staticMetaObject = { {
    QMetaObject::SuperData::link<QFrame::staticMetaObject>(),
    qt_meta_stringdata_WalletFrame.data,
    qt_meta_data_WalletFrame,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *WalletFrame::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *WalletFrame::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_WalletFrame.stringdata0))
        return static_cast<void*>(this);
    return QFrame::qt_metacast(_clname);
}

int WalletFrame::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QFrame::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 22)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 22;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 22)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 22;
    }
    return _id;
}

// SIGNAL 0
void WalletFrame::requestedSyncWarningInfo()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
