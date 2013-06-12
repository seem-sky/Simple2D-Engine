/********************************************************************************
** Form generated from reading UI file 'MapObjectDialog.ui'
**
** Created: Wed 12. Jun 20:45:54 2013
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAPOBJECTDIALOG_H
#define UI_MAPOBJECTDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QFrame>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QRadioButton>
#include <QtGui/QSpinBox>
#include "animationviewwidget.h"

QT_BEGIN_NAMESPACE

class Ui_MapObjectDialog
{
public:
    QDialogButtonBox *buttonBox;
    AnimationViewDB *m_pView;
    QLabel *label_42;
    QSpinBox *m_pGUID;
    QSpinBox *m_pID;
    QLabel *label_43;
    QLabel *label_17;
    QLineEdit *m_pType;
    QFrame *frame;
    QLabel *label_18;
    QRadioButton *m_pDirUp;
    QRadioButton *m_pDirLeft;
    QRadioButton *m_pDirRight;
    QRadioButton *m_pDirDown;
    QLabel *label_19;
    QComboBox *m_pLayer;

    void setupUi(QDialog *MapObjectDialog)
    {
        if (MapObjectDialog->objectName().isEmpty())
            MapObjectDialog->setObjectName(QString::fromUtf8("MapObjectDialog"));
        MapObjectDialog->resize(394, 262);
        MapObjectDialog->setMinimumSize(QSize(394, 262));
        MapObjectDialog->setMaximumSize(QSize(394, 262));
        buttonBox = new QDialogButtonBox(MapObjectDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(230, 230, 161, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        m_pView = new AnimationViewDB(MapObjectDialog);
        m_pView->setObjectName(QString::fromUtf8("m_pView"));
        m_pView->setGeometry(QRect(170, 10, 211, 211));
        label_42 = new QLabel(MapObjectDialog);
        label_42->setObjectName(QString::fromUtf8("label_42"));
        label_42->setGeometry(QRect(10, 10, 51, 21));
        QFont font;
        font.setPointSize(12);
        font.setBold(true);
        font.setWeight(75);
        label_42->setFont(font);
        m_pGUID = new QSpinBox(MapObjectDialog);
        m_pGUID->setObjectName(QString::fromUtf8("m_pGUID"));
        m_pGUID->setGeometry(QRect(70, 10, 91, 22));
        m_pGUID->setFrame(true);
        m_pGUID->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        m_pGUID->setReadOnly(true);
        m_pGUID->setButtonSymbols(QAbstractSpinBox::NoButtons);
        m_pGUID->setMaximum(999999999);
        m_pID = new QSpinBox(MapObjectDialog);
        m_pID->setObjectName(QString::fromUtf8("m_pID"));
        m_pID->setGeometry(QRect(70, 40, 91, 22));
        m_pID->setFrame(true);
        m_pID->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        m_pID->setReadOnly(true);
        m_pID->setButtonSymbols(QAbstractSpinBox::NoButtons);
        m_pID->setMaximum(999999999);
        label_43 = new QLabel(MapObjectDialog);
        label_43->setObjectName(QString::fromUtf8("label_43"));
        label_43->setGeometry(QRect(10, 40, 31, 21));
        label_43->setFont(font);
        label_17 = new QLabel(MapObjectDialog);
        label_17->setObjectName(QString::fromUtf8("label_17"));
        label_17->setGeometry(QRect(10, 70, 51, 21));
        label_17->setFont(font);
        m_pType = new QLineEdit(MapObjectDialog);
        m_pType->setObjectName(QString::fromUtf8("m_pType"));
        m_pType->setGeometry(QRect(70, 70, 91, 20));
        m_pType->setReadOnly(true);
        frame = new QFrame(MapObjectDialog);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(10, 130, 151, 121));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        label_18 = new QLabel(frame);
        label_18->setObjectName(QString::fromUtf8("label_18"));
        label_18->setGeometry(QRect(0, 0, 91, 21));
        label_18->setFont(font);
        m_pDirUp = new QRadioButton(frame);
        m_pDirUp->setObjectName(QString::fromUtf8("m_pDirUp"));
        m_pDirUp->setGeometry(QRect(60, 30, 51, 17));
        QFont font1;
        font1.setPointSize(12);
        m_pDirUp->setFont(font1);
        m_pDirLeft = new QRadioButton(frame);
        m_pDirLeft->setObjectName(QString::fromUtf8("m_pDirLeft"));
        m_pDirLeft->setGeometry(QRect(10, 60, 51, 17));
        m_pDirLeft->setFont(font1);
        m_pDirRight = new QRadioButton(frame);
        m_pDirRight->setObjectName(QString::fromUtf8("m_pDirRight"));
        m_pDirRight->setGeometry(QRect(90, 60, 51, 17));
        m_pDirRight->setFont(font1);
        m_pDirDown = new QRadioButton(frame);
        m_pDirDown->setObjectName(QString::fromUtf8("m_pDirDown"));
        m_pDirDown->setGeometry(QRect(60, 90, 61, 17));
        m_pDirDown->setFont(font1);
        m_pDirDown->setChecked(true);
        label_19 = new QLabel(MapObjectDialog);
        label_19->setObjectName(QString::fromUtf8("label_19"));
        label_19->setGeometry(QRect(10, 100, 51, 21));
        label_19->setFont(font);
        m_pLayer = new QComboBox(MapObjectDialog);
        m_pLayer->setObjectName(QString::fromUtf8("m_pLayer"));
        m_pLayer->setGeometry(QRect(70, 100, 91, 22));

        retranslateUi(MapObjectDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), MapObjectDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), MapObjectDialog, SLOT(reject()));

        m_pLayer->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MapObjectDialog);
    } // setupUi

    void retranslateUi(QDialog *MapObjectDialog)
    {
        MapObjectDialog->setWindowTitle(QApplication::translate("MapObjectDialog", "Dialog", 0, QApplication::UnicodeUTF8));
        label_42->setText(QApplication::translate("MapObjectDialog", "GUID:", 0, QApplication::UnicodeUTF8));
        label_43->setText(QApplication::translate("MapObjectDialog", "ID:", 0, QApplication::UnicodeUTF8));
        label_17->setText(QApplication::translate("MapObjectDialog", "type:", 0, QApplication::UnicodeUTF8));
        label_18->setText(QApplication::translate("MapObjectDialog", "direction:", 0, QApplication::UnicodeUTF8));
        m_pDirUp->setText(QApplication::translate("MapObjectDialog", "up", 0, QApplication::UnicodeUTF8));
        m_pDirLeft->setText(QApplication::translate("MapObjectDialog", "left", 0, QApplication::UnicodeUTF8));
        m_pDirRight->setText(QApplication::translate("MapObjectDialog", "right", 0, QApplication::UnicodeUTF8));
        m_pDirDown->setText(QApplication::translate("MapObjectDialog", "down", 0, QApplication::UnicodeUTF8));
        label_19->setText(QApplication::translate("MapObjectDialog", "layer:", 0, QApplication::UnicodeUTF8));
        m_pLayer->clear();
        m_pLayer->insertItems(0, QStringList()
         << QApplication::translate("MapObjectDialog", "lower", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MapObjectDialog", "middle", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MapObjectDialog", "upper", 0, QApplication::UnicodeUTF8)
        );
    } // retranslateUi

};

namespace Ui {
    class MapObjectDialog: public Ui_MapObjectDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAPOBJECTDIALOG_H
