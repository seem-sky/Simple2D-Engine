/********************************************************************************
** Form generated from reading UI file 'TransparentColorDialog.ui'
**
** Created: Wed 12. Jun 18:46:23 2013
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TRANSPARENTCOLORDIALOG_H
#define UI_TRANSPARENTCOLORDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>

QT_BEGIN_NAMESPACE

class Ui_TransparentColorDialog
{
public:
    QLabel *m_pPic;

    void setupUi(QDialog *TransparentColorDialog)
    {
        if (TransparentColorDialog->objectName().isEmpty())
            TransparentColorDialog->setObjectName(QString::fromUtf8("TransparentColorDialog"));
        TransparentColorDialog->resize(100, 100);
        TransparentColorDialog->setMinimumSize(QSize(100, 100));
        m_pPic = new QLabel(TransparentColorDialog);
        m_pPic->setObjectName(QString::fromUtf8("m_pPic"));
        m_pPic->setGeometry(QRect(0, 0, 100, 100));

        retranslateUi(TransparentColorDialog);

        QMetaObject::connectSlotsByName(TransparentColorDialog);
    } // setupUi

    void retranslateUi(QDialog *TransparentColorDialog)
    {
        TransparentColorDialog->setWindowTitle(QApplication::translate("TransparentColorDialog", "Dialog", 0, QApplication::UnicodeUTF8));
        m_pPic->setText(QApplication::translate("TransparentColorDialog", "TextLabel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class TransparentColorDialog: public Ui_TransparentColorDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TRANSPARENTCOLORDIALOG_H
