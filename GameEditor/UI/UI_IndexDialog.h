/********************************************************************************
** Form generated from reading UI file 'IndexDialog.ui'
**
** Created: Wed 12. Jun 18:46:20 2013
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INDEXDIALOG_H
#define UI_INDEXDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_IndexDialog
{
public:
    QDialogButtonBox *m_pButtons;
    QLabel *label_19;
    QSpinBox *m_pIndex;

    void setupUi(QDialog *IndexDialog)
    {
        if (IndexDialog->objectName().isEmpty())
            IndexDialog->setObjectName(QString::fromUtf8("IndexDialog"));
        IndexDialog->resize(159, 102);
        m_pButtons = new QDialogButtonBox(IndexDialog);
        m_pButtons->setObjectName(QString::fromUtf8("m_pButtons"));
        m_pButtons->setGeometry(QRect(10, 70, 141, 32));
        m_pButtons->setOrientation(Qt::Horizontal);
        m_pButtons->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        label_19 = new QLabel(IndexDialog);
        label_19->setObjectName(QString::fromUtf8("label_19"));
        label_19->setGeometry(QRect(10, 10, 181, 21));
        QFont font;
        font.setPointSize(12);
        font.setBold(true);
        font.setWeight(75);
        label_19->setFont(font);
        m_pIndex = new QSpinBox(IndexDialog);
        m_pIndex->setObjectName(QString::fromUtf8("m_pIndex"));
        m_pIndex->setGeometry(QRect(10, 40, 141, 22));
        m_pIndex->setMinimum(-2147483647);
        m_pIndex->setMaximum(2147483647);

        retranslateUi(IndexDialog);
        QObject::connect(m_pButtons, SIGNAL(accepted()), IndexDialog, SLOT(accept()));
        QObject::connect(m_pButtons, SIGNAL(rejected()), IndexDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(IndexDialog);
    } // setupUi

    void retranslateUi(QDialog *IndexDialog)
    {
        IndexDialog->setWindowTitle(QApplication::translate("IndexDialog", "Dialog", 0, QApplication::UnicodeUTF8));
        label_19->setText(QApplication::translate("IndexDialog", "Condition Index:", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class IndexDialog: public Ui_IndexDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INDEXDIALOG_H
