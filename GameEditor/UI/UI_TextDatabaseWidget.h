/********************************************************************************
** Form generated from reading UI file 'TextDatabaseWidget.ui'
**
** Created: Wed 12. Jun 18:46:22 2013
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TEXTDATABASEWIDGET_H
#define UI_TEXTDATABASEWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QTableWidget>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TextDatabaseWidget
{
public:
    QTableWidget *m_pLocalTable;
    QLabel *label_20;

    void setupUi(QWidget *TextDatabaseWidget)
    {
        if (TextDatabaseWidget->objectName().isEmpty())
            TextDatabaseWidget->setObjectName(QString::fromUtf8("TextDatabaseWidget"));
        TextDatabaseWidget->resize(733, 505);
        m_pLocalTable = new QTableWidget(TextDatabaseWidget);
        if (m_pLocalTable->columnCount() < 1)
            m_pLocalTable->setColumnCount(1);
        if (m_pLocalTable->rowCount() < 1)
            m_pLocalTable->setRowCount(1);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        m_pLocalTable->setVerticalHeaderItem(0, __qtablewidgetitem);
        m_pLocalTable->setObjectName(QString::fromUtf8("m_pLocalTable"));
        m_pLocalTable->setGeometry(QRect(210, 100, 450, 100));
        m_pLocalTable->setFrameShape(QFrame::Box);
        m_pLocalTable->setFrameShadow(QFrame::Sunken);
        m_pLocalTable->setEditTriggers(QAbstractItemView::AllEditTriggers);
        m_pLocalTable->setGridStyle(Qt::SolidLine);
        m_pLocalTable->setRowCount(1);
        m_pLocalTable->setColumnCount(1);
        m_pLocalTable->horizontalHeader()->setVisible(false);
        m_pLocalTable->horizontalHeader()->setDefaultSectionSize(372);
        m_pLocalTable->horizontalHeader()->setMinimumSectionSize(372);
        m_pLocalTable->verticalHeader()->setDefaultSectionSize(100);
        m_pLocalTable->verticalHeader()->setMinimumSectionSize(100);
        label_20 = new QLabel(TextDatabaseWidget);
        label_20->setObjectName(QString::fromUtf8("label_20"));
        label_20->setGeometry(QRect(210, 80, 61, 21));
        QFont font;
        font.setPointSize(12);
        font.setBold(true);
        font.setWeight(75);
        label_20->setFont(font);

        retranslateUi(TextDatabaseWidget);

        QMetaObject::connectSlotsByName(TextDatabaseWidget);
    } // setupUi

    void retranslateUi(QWidget *TextDatabaseWidget)
    {
        TextDatabaseWidget->setWindowTitle(QApplication::translate("TextDatabaseWidget", "Form", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem = m_pLocalTable->verticalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("TextDatabaseWidget", "local default", 0, QApplication::UnicodeUTF8));
        label_20->setText(QApplication::translate("TextDatabaseWidget", "locals:", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class TextDatabaseWidget: public Ui_TextDatabaseWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TEXTDATABASEWIDGET_H
