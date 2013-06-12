/********************************************************************************
** Form generated from reading UI file 'DatabaseWidget.ui'
**
** Created: Wed 12. Jun 18:46:19 2013
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DATABASEWIDGET_H
#define UI_DATABASEWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpinBox>
#include <QtGui/QTreeWidget>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DatabaseWidget
{
public:
    QPushButton *m_pResizeList;
    QSpinBox *m_pListCount;
    QTreeWidget *m_pList;
    QLabel *label_17;
    QSpinBox *m_pID;
    QLabel *label_42;
    QLineEdit *m_pName;

    void setupUi(QWidget *DatabaseWidget)
    {
        if (DatabaseWidget->objectName().isEmpty())
            DatabaseWidget->setObjectName(QString::fromUtf8("DatabaseWidget"));
        DatabaseWidget->resize(380, 440);
        m_pResizeList = new QPushButton(DatabaseWidget);
        m_pResizeList->setObjectName(QString::fromUtf8("m_pResizeList"));
        m_pResizeList->setGeometry(QRect(130, 410, 75, 23));
        m_pListCount = new QSpinBox(DatabaseWidget);
        m_pListCount->setObjectName(QString::fromUtf8("m_pListCount"));
        m_pListCount->setGeometry(QRect(0, 410, 121, 22));
        m_pListCount->setMaximum(99999);
        m_pList = new QTreeWidget(DatabaseWidget);
        m_pList->setObjectName(QString::fromUtf8("m_pList"));
        m_pList->setGeometry(QRect(0, 0, 201, 401));
        m_pList->setIndentation(0);
        m_pList->setRootIsDecorated(false);
        m_pList->setSortingEnabled(true);
        m_pList->setHeaderHidden(false);
        m_pList->setExpandsOnDoubleClick(false);
        m_pList->setColumnCount(2);
        m_pList->header()->setVisible(true);
        m_pList->header()->setCascadingSectionResizes(false);
        m_pList->header()->setDefaultSectionSize(50);
        m_pList->header()->setHighlightSections(false);
        m_pList->header()->setProperty("showSortIndicator", QVariant(true));
        label_17 = new QLabel(DatabaseWidget);
        label_17->setObjectName(QString::fromUtf8("label_17"));
        label_17->setGeometry(QRect(210, 30, 61, 21));
        QFont font;
        font.setPointSize(12);
        font.setBold(true);
        font.setWeight(75);
        label_17->setFont(font);
        m_pID = new QSpinBox(DatabaseWidget);
        m_pID->setObjectName(QString::fromUtf8("m_pID"));
        m_pID->setGeometry(QRect(240, 0, 131, 22));
        m_pID->setFrame(true);
        m_pID->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        m_pID->setReadOnly(true);
        m_pID->setButtonSymbols(QAbstractSpinBox::NoButtons);
        m_pID->setMaximum(999999999);
        label_42 = new QLabel(DatabaseWidget);
        label_42->setObjectName(QString::fromUtf8("label_42"));
        label_42->setGeometry(QRect(210, 0, 31, 21));
        label_42->setFont(font);
        m_pName = new QLineEdit(DatabaseWidget);
        m_pName->setObjectName(QString::fromUtf8("m_pName"));
        m_pName->setGeometry(QRect(210, 50, 161, 20));
        m_pName->setReadOnly(false);

        retranslateUi(DatabaseWidget);

        QMetaObject::connectSlotsByName(DatabaseWidget);
    } // setupUi

    void retranslateUi(QWidget *DatabaseWidget)
    {
        DatabaseWidget->setWindowTitle(QApplication::translate("DatabaseWidget", "Form", 0, QApplication::UnicodeUTF8));
        m_pResizeList->setText(QApplication::translate("DatabaseWidget", "resize list", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem = m_pList->headerItem();
        ___qtreewidgetitem->setText(1, QApplication::translate("DatabaseWidget", "Name", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem->setText(0, QApplication::translate("DatabaseWidget", "ID", 0, QApplication::UnicodeUTF8));
        label_17->setText(QApplication::translate("DatabaseWidget", "name:", 0, QApplication::UnicodeUTF8));
        label_42->setText(QApplication::translate("DatabaseWidget", "ID:", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class DatabaseWidget: public Ui_DatabaseWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DATABASEWIDGET_H
