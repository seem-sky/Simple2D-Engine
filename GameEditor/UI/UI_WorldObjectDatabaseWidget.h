/********************************************************************************
** Form generated from reading UI file 'WorldObjectDatabaseWidget.ui'
**
** Created: Wed 12. Jun 18:46:20 2013
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WORLDOBJECTDATABASEWIDGET_H
#define UI_WORLDOBJECTDATABASEWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpinBox>
#include <QtGui/QWidget>
#include "animationviewerwidget.h"
#include "dragtreewidget.h"

QT_BEGIN_NAMESPACE

class Ui_WorldObjectDatabaseWidget
{
public:
    QLabel *m_pAnimationLabel;
    AnimationPrototypeDragTreeWidget *m_pAnimationList;
    QLabel *label_35;
    QSpinBox *m_pBoundingBottom;
    QLabel *label_33;
    QLabel *label_32;
    QSpinBox *m_pBoundingLeft;
    QSpinBox *m_pBoundingUp;
    QLabel *label_34;
    QSpinBox *m_pBoundingRight;
    QLabel *label_37;
    QSpinBox *m_pSpeedModifyer;
    QLabel *label_38;
    QLabel *label_17;
    QLineEdit *m_pScript;
    AnimationViewerTableWidget *m_pAnimationTable;
    QPushButton *m_pAddAnimationButton;
    QPushButton *m_pRemoveAnimationButton;

    void setupUi(QWidget *WorldObjectDatabaseWidget)
    {
        if (WorldObjectDatabaseWidget->objectName().isEmpty())
            WorldObjectDatabaseWidget->setObjectName(QString::fromUtf8("WorldObjectDatabaseWidget"));
        WorldObjectDatabaseWidget->resize(702, 473);
        m_pAnimationLabel = new QLabel(WorldObjectDatabaseWidget);
        m_pAnimationLabel->setObjectName(QString::fromUtf8("m_pAnimationLabel"));
        m_pAnimationLabel->setGeometry(QRect(460, 0, 91, 21));
        QFont font;
        font.setPointSize(12);
        font.setBold(true);
        font.setWeight(75);
        m_pAnimationLabel->setFont(font);
        m_pAnimationList = new AnimationPrototypeDragTreeWidget(WorldObjectDatabaseWidget);
        m_pAnimationList->setObjectName(QString::fromUtf8("m_pAnimationList"));
        m_pAnimationList->setGeometry(QRect(460, 20, 221, 291));
        m_pAnimationList->setEditTriggers(QAbstractItemView::NoEditTriggers);
        m_pAnimationList->setDragEnabled(true);
        m_pAnimationList->setDragDropMode(QAbstractItemView::DragOnly);
        m_pAnimationList->setIndentation(0);
        m_pAnimationList->setRootIsDecorated(false);
        m_pAnimationList->setSortingEnabled(true);
        m_pAnimationList->setHeaderHidden(false);
        m_pAnimationList->setExpandsOnDoubleClick(false);
        m_pAnimationList->setColumnCount(2);
        m_pAnimationList->header()->setVisible(true);
        m_pAnimationList->header()->setCascadingSectionResizes(false);
        m_pAnimationList->header()->setDefaultSectionSize(50);
        m_pAnimationList->header()->setHighlightSections(false);
        m_pAnimationList->header()->setProperty("showSortIndicator", QVariant(true));
        label_35 = new QLabel(WorldObjectDatabaseWidget);
        label_35->setObjectName(QString::fromUtf8("label_35"));
        label_35->setGeometry(QRect(210, 170, 141, 21));
        label_35->setFont(font);
        m_pBoundingBottom = new QSpinBox(WorldObjectDatabaseWidget);
        m_pBoundingBottom->setObjectName(QString::fromUtf8("m_pBoundingBottom"));
        m_pBoundingBottom->setGeometry(QRect(360, 170, 91, 21));
        QFont font1;
        font1.setPointSize(12);
        m_pBoundingBottom->setFont(font1);
        m_pBoundingBottom->setMinimum(0);
        m_pBoundingBottom->setMaximum(100000);
        m_pBoundingBottom->setValue(0);
        label_33 = new QLabel(WorldObjectDatabaseWidget);
        label_33->setObjectName(QString::fromUtf8("label_33"));
        label_33->setGeometry(QRect(210, 110, 121, 21));
        label_33->setFont(font);
        label_32 = new QLabel(WorldObjectDatabaseWidget);
        label_32->setObjectName(QString::fromUtf8("label_32"));
        label_32->setGeometry(QRect(210, 130, 111, 21));
        label_32->setFont(font);
        m_pBoundingLeft = new QSpinBox(WorldObjectDatabaseWidget);
        m_pBoundingLeft->setObjectName(QString::fromUtf8("m_pBoundingLeft"));
        m_pBoundingLeft->setGeometry(QRect(360, 110, 91, 21));
        m_pBoundingLeft->setFont(font1);
        m_pBoundingLeft->setMinimum(-100000);
        m_pBoundingLeft->setMaximum(100000);
        m_pBoundingLeft->setValue(0);
        m_pBoundingUp = new QSpinBox(WorldObjectDatabaseWidget);
        m_pBoundingUp->setObjectName(QString::fromUtf8("m_pBoundingUp"));
        m_pBoundingUp->setGeometry(QRect(360, 130, 91, 21));
        m_pBoundingUp->setFont(font1);
        m_pBoundingUp->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        m_pBoundingUp->setMinimum(-100000);
        m_pBoundingUp->setMaximum(100000);
        m_pBoundingUp->setValue(0);
        label_34 = new QLabel(WorldObjectDatabaseWidget);
        label_34->setObjectName(QString::fromUtf8("label_34"));
        label_34->setGeometry(QRect(210, 150, 141, 21));
        label_34->setFont(font);
        m_pBoundingRight = new QSpinBox(WorldObjectDatabaseWidget);
        m_pBoundingRight->setObjectName(QString::fromUtf8("m_pBoundingRight"));
        m_pBoundingRight->setGeometry(QRect(360, 150, 91, 21));
        m_pBoundingRight->setFont(font1);
        m_pBoundingRight->setMinimum(0);
        m_pBoundingRight->setMaximum(100000);
        m_pBoundingRight->setValue(0);
        label_37 = new QLabel(WorldObjectDatabaseWidget);
        label_37->setObjectName(QString::fromUtf8("label_37"));
        label_37->setGeometry(QRect(210, 200, 151, 21));
        label_37->setFont(font);
        m_pSpeedModifyer = new QSpinBox(WorldObjectDatabaseWidget);
        m_pSpeedModifyer->setObjectName(QString::fromUtf8("m_pSpeedModifyer"));
        m_pSpeedModifyer->setGeometry(QRect(360, 200, 71, 21));
        m_pSpeedModifyer->setFont(font1);
        m_pSpeedModifyer->setMinimum(1);
        m_pSpeedModifyer->setMaximum(10000);
        m_pSpeedModifyer->setValue(100);
        label_38 = new QLabel(WorldObjectDatabaseWidget);
        label_38->setObjectName(QString::fromUtf8("label_38"));
        label_38->setGeometry(QRect(430, 200, 21, 21));
        label_38->setFont(font);
        label_17 = new QLabel(WorldObjectDatabaseWidget);
        label_17->setObjectName(QString::fromUtf8("label_17"));
        label_17->setGeometry(QRect(210, 80, 61, 21));
        label_17->setFont(font);
        m_pScript = new QLineEdit(WorldObjectDatabaseWidget);
        m_pScript->setObjectName(QString::fromUtf8("m_pScript"));
        m_pScript->setGeometry(QRect(270, 80, 181, 20));
        m_pScript->setReadOnly(false);
        m_pAnimationTable = new AnimationViewerTableWidget(WorldObjectDatabaseWidget);
        m_pAnimationTable->setObjectName(QString::fromUtf8("m_pAnimationTable"));
        m_pAnimationTable->setGeometry(QRect(210, 230, 241, 201));
        m_pAddAnimationButton = new QPushButton(WorldObjectDatabaseWidget);
        m_pAddAnimationButton->setObjectName(QString::fromUtf8("m_pAddAnimationButton"));
        m_pAddAnimationButton->setGeometry(QRect(210, 440, 101, 23));
        m_pRemoveAnimationButton = new QPushButton(WorldObjectDatabaseWidget);
        m_pRemoveAnimationButton->setObjectName(QString::fromUtf8("m_pRemoveAnimationButton"));
        m_pRemoveAnimationButton->setGeometry(QRect(320, 440, 101, 23));
        QWidget::setTabOrder(m_pBoundingLeft, m_pBoundingUp);
        QWidget::setTabOrder(m_pBoundingUp, m_pBoundingRight);
        QWidget::setTabOrder(m_pBoundingRight, m_pBoundingBottom);
        QWidget::setTabOrder(m_pBoundingBottom, m_pAnimationList);

        retranslateUi(WorldObjectDatabaseWidget);

        QMetaObject::connectSlotsByName(WorldObjectDatabaseWidget);
    } // setupUi

    void retranslateUi(QWidget *WorldObjectDatabaseWidget)
    {
        WorldObjectDatabaseWidget->setWindowTitle(QApplication::translate("WorldObjectDatabaseWidget", "Form", 0, QApplication::UnicodeUTF8));
        m_pAnimationLabel->setText(QApplication::translate("WorldObjectDatabaseWidget", "animations:", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem = m_pAnimationList->headerItem();
        ___qtreewidgetitem->setText(1, QApplication::translate("WorldObjectDatabaseWidget", "Name", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem->setText(0, QApplication::translate("WorldObjectDatabaseWidget", "ID", 0, QApplication::UnicodeUTF8));
        label_35->setText(QApplication::translate("WorldObjectDatabaseWidget", "bounding height:", 0, QApplication::UnicodeUTF8));
        label_33->setText(QApplication::translate("WorldObjectDatabaseWidget", "bounding x:", 0, QApplication::UnicodeUTF8));
        label_32->setText(QApplication::translate("WorldObjectDatabaseWidget", "bounding y:", 0, QApplication::UnicodeUTF8));
        label_34->setText(QApplication::translate("WorldObjectDatabaseWidget", "bounding width:", 0, QApplication::UnicodeUTF8));
        label_37->setText(QApplication::translate("WorldObjectDatabaseWidget", "animation speed:", 0, QApplication::UnicodeUTF8));
        label_38->setText(QApplication::translate("WorldObjectDatabaseWidget", "%", 0, QApplication::UnicodeUTF8));
        label_17->setText(QApplication::translate("WorldObjectDatabaseWidget", "script:", 0, QApplication::UnicodeUTF8));
        m_pAddAnimationButton->setText(QApplication::translate("WorldObjectDatabaseWidget", "add animation", 0, QApplication::UnicodeUTF8));
        m_pRemoveAnimationButton->setText(QApplication::translate("WorldObjectDatabaseWidget", "remove animation", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class WorldObjectDatabaseWidget: public Ui_WorldObjectDatabaseWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WORLDOBJECTDATABASEWIDGET_H
