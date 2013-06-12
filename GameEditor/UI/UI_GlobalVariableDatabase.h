/********************************************************************************
** Form generated from reading UI file 'GlobalVariableDatabase.ui'
**
** Created: Wed 12. Jun 18:46:24 2013
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GLOBALVARIABLEDATABASE_H
#define UI_GLOBALVARIABLEDATABASE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>
#include "customvariableboolpagewidget.h"
#include "customvariablefloatpagewidget.h"
#include "customvariableintpagewidget.h"
#include "customvariablestringpagewidget.h"

QT_BEGIN_NAMESPACE

class Ui_GlobalVariableDatabase
{
public:
    QGroupBox *groupBox;
    CustomVariableBoolPageWidget *m_pBools;
    QGroupBox *groupBox_2;
    CustomVariableFloatPageWidget *m_pFloats;
    QGroupBox *groupBox_3;
    CustomVariableIntPageWidget *m_pInts;
    QGroupBox *groupBox_4;
    CustomVariableStringPageWidget *m_pStrings;

    void setupUi(QWidget *GlobalVariableDatabase)
    {
        if (GlobalVariableDatabase->objectName().isEmpty())
            GlobalVariableDatabase->setObjectName(QString::fromUtf8("GlobalVariableDatabase"));
        GlobalVariableDatabase->resize(680, 332);
        groupBox = new QGroupBox(GlobalVariableDatabase);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(10, 0, 331, 161));
        groupBox->setMinimumSize(QSize(331, 161));
        groupBox->setMaximumSize(QSize(331, 161));
        QFont font;
        font.setPointSize(8);
        font.setBold(false);
        font.setWeight(50);
        groupBox->setFont(font);
        m_pBools = new CustomVariableBoolPageWidget(groupBox);
        m_pBools->setObjectName(QString::fromUtf8("m_pBools"));
        m_pBools->setGeometry(QRect(10, 20, 311, 132));
        groupBox_2 = new QGroupBox(GlobalVariableDatabase);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(340, 0, 331, 161));
        groupBox_2->setMinimumSize(QSize(331, 161));
        groupBox_2->setMaximumSize(QSize(331, 161));
        groupBox_2->setFont(font);
        m_pFloats = new CustomVariableFloatPageWidget(groupBox_2);
        m_pFloats->setObjectName(QString::fromUtf8("m_pFloats"));
        m_pFloats->setGeometry(QRect(10, 20, 311, 132));
        groupBox_3 = new QGroupBox(GlobalVariableDatabase);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setGeometry(QRect(10, 160, 331, 161));
        groupBox_3->setMinimumSize(QSize(331, 161));
        groupBox_3->setMaximumSize(QSize(331, 161));
        groupBox_3->setFont(font);
        m_pInts = new CustomVariableIntPageWidget(groupBox_3);
        m_pInts->setObjectName(QString::fromUtf8("m_pInts"));
        m_pInts->setGeometry(QRect(10, 20, 311, 132));
        groupBox_4 = new QGroupBox(GlobalVariableDatabase);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        groupBox_4->setGeometry(QRect(340, 160, 331, 161));
        groupBox_4->setMinimumSize(QSize(331, 161));
        groupBox_4->setMaximumSize(QSize(331, 161));
        groupBox_4->setFont(font);
        m_pStrings = new CustomVariableStringPageWidget(groupBox_4);
        m_pStrings->setObjectName(QString::fromUtf8("m_pStrings"));
        m_pStrings->setGeometry(QRect(10, 20, 311, 132));

        retranslateUi(GlobalVariableDatabase);

        QMetaObject::connectSlotsByName(GlobalVariableDatabase);
    } // setupUi

    void retranslateUi(QWidget *GlobalVariableDatabase)
    {
        GlobalVariableDatabase->setWindowTitle(QApplication::translate("GlobalVariableDatabase", "Form", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("GlobalVariableDatabase", "Bools:", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("GlobalVariableDatabase", "Floats:", 0, QApplication::UnicodeUTF8));
        groupBox_3->setTitle(QApplication::translate("GlobalVariableDatabase", "Integers:", 0, QApplication::UnicodeUTF8));
        groupBox_4->setTitle(QApplication::translate("GlobalVariableDatabase", "Strings:", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class GlobalVariableDatabase: public Ui_GlobalVariableDatabase {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GLOBALVARIABLEDATABASE_H
