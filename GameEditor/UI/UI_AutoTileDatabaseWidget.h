/********************************************************************************
** Form generated from reading UI file 'AutoTileDatabaseWidget.ui'
**
** Created: Wed 12. Jun 18:46:23 2013
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AUTOTILEDATABASEWIDGET_H
#define UI_AUTOTILEDATABASEWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QWidget>
#include "dragtreewidget.h"

QT_BEGIN_NAMESPACE

class Ui_AutoTileDatabaseWidget
{
public:
    QLabel *label_18;
    TilePrototypeDragTreeWidget *m_pTileList;
    QLabel *label_20;
    TileDropLabel *m_pTopLeft;
    TileDropLabel *m_pTopCenter;
    TileDropLabel *m_pTopRight;
    QLabel *label_21;
    TileDropLabel *m_pCenterLeft;
    TileDropLabel *m_pCenter;
    TileDropLabel *m_pCenterRight;
    TileDropLabel *m_pBottomLeft;
    TileDropLabel *m_pBottomCenter;
    TileDropLabel *m_pBottomRight;
    QLabel *label_22;
    TileDropLabel *m_pInnerEdges;

    void setupUi(QWidget *AutoTileDatabaseWidget)
    {
        if (AutoTileDatabaseWidget->objectName().isEmpty())
            AutoTileDatabaseWidget->setObjectName(QString::fromUtf8("AutoTileDatabaseWidget"));
        AutoTileDatabaseWidget->resize(636, 476);
        label_18 = new QLabel(AutoTileDatabaseWidget);
        label_18->setObjectName(QString::fromUtf8("label_18"));
        label_18->setGeometry(QRect(390, 0, 91, 21));
        QFont font;
        font.setPointSize(12);
        font.setBold(true);
        font.setWeight(75);
        label_18->setFont(font);
        m_pTileList = new TilePrototypeDragTreeWidget(AutoTileDatabaseWidget);
        m_pTileList->setObjectName(QString::fromUtf8("m_pTileList"));
        m_pTileList->setGeometry(QRect(390, 30, 231, 261));
        m_pTileList->setFrameShape(QFrame::StyledPanel);
        m_pTileList->setEditTriggers(QAbstractItemView::NoEditTriggers);
        m_pTileList->setIndentation(0);
        m_pTileList->setRootIsDecorated(false);
        m_pTileList->setSortingEnabled(true);
        m_pTileList->setHeaderHidden(false);
        m_pTileList->setExpandsOnDoubleClick(false);
        m_pTileList->setColumnCount(2);
        m_pTileList->header()->setVisible(true);
        m_pTileList->header()->setCascadingSectionResizes(false);
        m_pTileList->header()->setDefaultSectionSize(50);
        m_pTileList->header()->setHighlightSections(false);
        m_pTileList->header()->setProperty("showSortIndicator", QVariant(true));
        label_20 = new QLabel(AutoTileDatabaseWidget);
        label_20->setObjectName(QString::fromUtf8("label_20"));
        label_20->setGeometry(QRect(210, 80, 111, 21));
        label_20->setFont(font);
        m_pTopLeft = new TileDropLabel(AutoTileDatabaseWidget);
        m_pTopLeft->setObjectName(QString::fromUtf8("m_pTopLeft"));
        m_pTopLeft->setGeometry(QRect(210, 130, 32, 32));
        m_pTopLeft->setFrameShape(QFrame::Box);
        m_pTopLeft->setLineWidth(1);
        m_pTopCenter = new TileDropLabel(AutoTileDatabaseWidget);
        m_pTopCenter->setObjectName(QString::fromUtf8("m_pTopCenter"));
        m_pTopCenter->setGeometry(QRect(250, 130, 32, 32));
        m_pTopCenter->setFrameShape(QFrame::Box);
        m_pTopRight = new TileDropLabel(AutoTileDatabaseWidget);
        m_pTopRight->setObjectName(QString::fromUtf8("m_pTopRight"));
        m_pTopRight->setGeometry(QRect(290, 130, 32, 32));
        m_pTopRight->setFrameShape(QFrame::Box);
        label_21 = new QLabel(AutoTileDatabaseWidget);
        label_21->setObjectName(QString::fromUtf8("label_21"));
        label_21->setGeometry(QRect(210, 110, 111, 21));
        QFont font1;
        font1.setPointSize(8);
        font1.setBold(true);
        font1.setWeight(75);
        label_21->setFont(font1);
        m_pCenterLeft = new TileDropLabel(AutoTileDatabaseWidget);
        m_pCenterLeft->setObjectName(QString::fromUtf8("m_pCenterLeft"));
        m_pCenterLeft->setGeometry(QRect(210, 170, 32, 32));
        m_pCenterLeft->setFrameShape(QFrame::Box);
        m_pCenter = new TileDropLabel(AutoTileDatabaseWidget);
        m_pCenter->setObjectName(QString::fromUtf8("m_pCenter"));
        m_pCenter->setGeometry(QRect(250, 170, 32, 32));
        m_pCenter->setFrameShape(QFrame::Box);
        m_pCenterRight = new TileDropLabel(AutoTileDatabaseWidget);
        m_pCenterRight->setObjectName(QString::fromUtf8("m_pCenterRight"));
        m_pCenterRight->setGeometry(QRect(290, 170, 32, 32));
        m_pCenterRight->setFrameShape(QFrame::Box);
        m_pBottomLeft = new TileDropLabel(AutoTileDatabaseWidget);
        m_pBottomLeft->setObjectName(QString::fromUtf8("m_pBottomLeft"));
        m_pBottomLeft->setGeometry(QRect(210, 210, 32, 32));
        m_pBottomLeft->setFrameShape(QFrame::Box);
        m_pBottomCenter = new TileDropLabel(AutoTileDatabaseWidget);
        m_pBottomCenter->setObjectName(QString::fromUtf8("m_pBottomCenter"));
        m_pBottomCenter->setGeometry(QRect(250, 210, 32, 32));
        m_pBottomCenter->setFrameShape(QFrame::Box);
        m_pBottomRight = new TileDropLabel(AutoTileDatabaseWidget);
        m_pBottomRight->setObjectName(QString::fromUtf8("m_pBottomRight"));
        m_pBottomRight->setGeometry(QRect(290, 210, 32, 32));
        m_pBottomRight->setFrameShape(QFrame::Box);
        label_22 = new QLabel(AutoTileDatabaseWidget);
        label_22->setObjectName(QString::fromUtf8("label_22"));
        label_22->setGeometry(QRect(210, 250, 81, 21));
        label_22->setFont(font1);
        m_pInnerEdges = new TileDropLabel(AutoTileDatabaseWidget);
        m_pInnerEdges->setObjectName(QString::fromUtf8("m_pInnerEdges"));
        m_pInnerEdges->setGeometry(QRect(210, 270, 32, 32));
        m_pInnerEdges->setFrameShape(QFrame::Box);

        retranslateUi(AutoTileDatabaseWidget);

        QMetaObject::connectSlotsByName(AutoTileDatabaseWidget);
    } // setupUi

    void retranslateUi(QWidget *AutoTileDatabaseWidget)
    {
        AutoTileDatabaseWidget->setWindowTitle(QApplication::translate("AutoTileDatabaseWidget", "Form", 0, QApplication::UnicodeUTF8));
        label_18->setText(QApplication::translate("AutoTileDatabaseWidget", "tiles:", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem = m_pTileList->headerItem();
        ___qtreewidgetitem->setText(1, QApplication::translate("AutoTileDatabaseWidget", "Name", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem->setText(0, QApplication::translate("AutoTileDatabaseWidget", "ID", 0, QApplication::UnicodeUTF8));
        label_20->setText(QApplication::translate("AutoTileDatabaseWidget", "AutoTile set:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        m_pTopLeft->setToolTip(QApplication::translate("AutoTileDatabaseWidget", "<html><head/><body><p>top-left</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        m_pTopLeft->setText(QString());
#ifndef QT_NO_TOOLTIP
        m_pTopCenter->setToolTip(QApplication::translate("AutoTileDatabaseWidget", "<html><head/><body><p>top-center</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        m_pTopCenter->setText(QString());
#ifndef QT_NO_TOOLTIP
        m_pTopRight->setToolTip(QApplication::translate("AutoTileDatabaseWidget", "<html><head/><body><p>top-right</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        m_pTopRight->setText(QString());
        label_21->setText(QApplication::translate("AutoTileDatabaseWidget", "basic set:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        m_pCenterLeft->setToolTip(QApplication::translate("AutoTileDatabaseWidget", "<html><head/><body><p>center-left</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        m_pCenterLeft->setText(QString());
#ifndef QT_NO_TOOLTIP
        m_pCenter->setToolTip(QApplication::translate("AutoTileDatabaseWidget", "<html><head/><body><p>center</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        m_pCenter->setText(QString());
#ifndef QT_NO_TOOLTIP
        m_pCenterRight->setToolTip(QApplication::translate("AutoTileDatabaseWidget", "<html><head/><body><p>center-right</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        m_pCenterRight->setText(QString());
#ifndef QT_NO_TOOLTIP
        m_pBottomLeft->setToolTip(QApplication::translate("AutoTileDatabaseWidget", "<html><head/><body><p>bottom-left</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        m_pBottomLeft->setText(QString());
#ifndef QT_NO_TOOLTIP
        m_pBottomCenter->setToolTip(QApplication::translate("AutoTileDatabaseWidget", "<html><head/><body><p>bottom-center</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        m_pBottomCenter->setText(QString());
#ifndef QT_NO_TOOLTIP
        m_pBottomRight->setToolTip(QApplication::translate("AutoTileDatabaseWidget", "<html><head/><body><p>bottom-right</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        m_pBottomRight->setText(QString());
        label_22->setText(QApplication::translate("AutoTileDatabaseWidget", "inner edges:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        m_pInnerEdges->setToolTip(QApplication::translate("AutoTileDatabaseWidget", "<html><head/><body><p>inner edges</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        m_pInnerEdges->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class AutoTileDatabaseWidget: public Ui_AutoTileDatabaseWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AUTOTILEDATABASEWIDGET_H
