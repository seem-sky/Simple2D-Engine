/********************************************************************************
** Form generated from reading UI file 'SpriteDatabaseWidget.ui'
**
** Created: Wed 12. Jun 18:46:23 2013
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SPRITEDATABASEWIDGET_H
#define UI_SPRITEDATABASEWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SpriteDatabaseWidget
{
public:

    void setupUi(QWidget *SpriteDatabaseWidget)
    {
        if (SpriteDatabaseWidget->objectName().isEmpty())
            SpriteDatabaseWidget->setObjectName(QString::fromUtf8("SpriteDatabaseWidget"));
        SpriteDatabaseWidget->resize(773, 430);

        retranslateUi(SpriteDatabaseWidget);

        QMetaObject::connectSlotsByName(SpriteDatabaseWidget);
    } // setupUi

    void retranslateUi(QWidget *SpriteDatabaseWidget)
    {
        SpriteDatabaseWidget->setWindowTitle(QApplication::translate("SpriteDatabaseWidget", "Form", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SpriteDatabaseWidget: public Ui_SpriteDatabaseWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SPRITEDATABASEWIDGET_H
