/********************************************************************************
** Form generated from reading UI file 'MapEditor.ui'
**
** Created: Wed 12. Jun 19:36:35 2013
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAPEDITOR_H
#define UI_MAPEDITOR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QFrame>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSlider>
#include <QtGui/QSpinBox>
#include <QtGui/QTabWidget>
#include <QtGui/QWidget>
#include "dragtreewidget.h"
#include "mapbrushwidget.h"
#include "maptree.h"
#include "mapview.h"
#include "tileview.h"

QT_BEGIN_NAMESPACE

class Ui_MapEditor
{
public:
    MapTreeWidget *m_pMapTree;
    MapTabWidget *m_pMapEditor;
    QLabel *m_pMapTreeLabel;
    QTabWidget *m_pMappingModeTab;
    QWidget *m_pTileMapping;
    QTabWidget *m_pTileTab;
    TileView *m_pTiles;
    AutoTileView *m_pAutoTiles;
    MapBrushWidget *m_pLHBrush;
    MapBrushWidget *m_pRHBrush;
    QWidget *m_pObjectMapping;
    QTabWidget *m_pObjectTabs;
    WorldObjectPrototypeDragTreeWidget *m_pWorldObjects;
    QWidget *m_pDynamicObjects;
    QPushButton *m_pRevert;
    QCheckBox *m_pShowGrid;
    QLabel *m_pMapTreeLabel_3;
    QLabel *m_pMapTreeLabel_2;
    QSlider *m_pZoom;
    QFrame *m_pLayer;
    QSpinBox *m_pCurLayer;
    QLabel *m_pMapTreeLabel_5;
    QRadioButton *m_pLayerBackground;
    QRadioButton *m_pLayerForeground;

    void setupUi(QWidget *MapEditor)
    {
        if (MapEditor->objectName().isEmpty())
            MapEditor->setObjectName(QString::fromUtf8("MapEditor"));
        MapEditor->resize(796, 563);
        m_pMapTree = new MapTreeWidget(MapEditor);
        m_pMapTree->setObjectName(QString::fromUtf8("m_pMapTree"));
        m_pMapTree->setGeometry(QRect(0, 330, 231, 231));
        m_pMapTree->setDragEnabled(true);
        m_pMapTree->setDragDropMode(QAbstractItemView::DragDrop);
        m_pMapTree->setDefaultDropAction(Qt::MoveAction);
        m_pMapTree->setSortingEnabled(true);
        m_pMapTree->setAnimated(true);
        m_pMapTree->setAllColumnsShowFocus(false);
        m_pMapTree->setColumnCount(2);
        m_pMapTree->header()->setVisible(true);
        m_pMapTree->header()->setCascadingSectionResizes(false);
        m_pMapTree->header()->setDefaultSectionSize(75);
        m_pMapTree->header()->setHighlightSections(false);
        m_pMapTree->header()->setMinimumSectionSize(20);
        m_pMapTree->header()->setProperty("showSortIndicator", QVariant(true));
        m_pMapTree->header()->setStretchLastSection(true);
        m_pMapEditor = new MapTabWidget(MapEditor);
        m_pMapEditor->setObjectName(QString::fromUtf8("m_pMapEditor"));
        m_pMapEditor->setGeometry(QRect(250, 70, 541, 491));
        m_pMapEditor->setTabsClosable(true);
        m_pMapEditor->setMovable(true);
        m_pMapTreeLabel = new QLabel(MapEditor);
        m_pMapTreeLabel->setObjectName(QString::fromUtf8("m_pMapTreeLabel"));
        m_pMapTreeLabel->setGeometry(QRect(0, 310, 81, 21));
        QFont font;
        font.setPointSize(12);
        font.setBold(true);
        font.setWeight(75);
        m_pMapTreeLabel->setFont(font);
        m_pMappingModeTab = new QTabWidget(MapEditor);
        m_pMappingModeTab->setObjectName(QString::fromUtf8("m_pMappingModeTab"));
        m_pMappingModeTab->setGeometry(QRect(0, 10, 241, 291));
        m_pTileMapping = new QWidget();
        m_pTileMapping->setObjectName(QString::fromUtf8("m_pTileMapping"));
        m_pTileTab = new QTabWidget(m_pTileMapping);
        m_pTileTab->setObjectName(QString::fromUtf8("m_pTileTab"));
        m_pTileTab->setEnabled(true);
        m_pTileTab->setGeometry(QRect(0, 90, 231, 171));
        m_pTiles = new TileView();
        m_pTiles->setObjectName(QString::fromUtf8("m_pTiles"));
        QSizePolicy sizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(m_pTiles->sizePolicy().hasHeightForWidth());
        m_pTiles->setSizePolicy(sizePolicy);
        m_pTileTab->addTab(m_pTiles, QString());
        m_pAutoTiles = new AutoTileView();
        m_pAutoTiles->setObjectName(QString::fromUtf8("m_pAutoTiles"));
        m_pAutoTiles->setEnabled(true);
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(m_pAutoTiles->sizePolicy().hasHeightForWidth());
        m_pAutoTiles->setSizePolicy(sizePolicy1);
        m_pTileTab->addTab(m_pAutoTiles, QString());
        m_pLHBrush = new MapBrushWidget(m_pTileMapping);
        m_pLHBrush->setObjectName(QString::fromUtf8("m_pLHBrush"));
        m_pLHBrush->setGeometry(QRect(10, 10, 71, 71));
        m_pRHBrush = new MapBrushWidget(m_pTileMapping);
        m_pRHBrush->setObjectName(QString::fromUtf8("m_pRHBrush"));
        m_pRHBrush->setGeometry(QRect(130, 10, 71, 71));
        m_pMappingModeTab->addTab(m_pTileMapping, QString());
        m_pObjectMapping = new QWidget();
        m_pObjectMapping->setObjectName(QString::fromUtf8("m_pObjectMapping"));
        m_pObjectTabs = new QTabWidget(m_pObjectMapping);
        m_pObjectTabs->setObjectName(QString::fromUtf8("m_pObjectTabs"));
        m_pObjectTabs->setEnabled(true);
        m_pObjectTabs->setGeometry(QRect(0, 0, 231, 261));
        m_pWorldObjects = new WorldObjectPrototypeDragTreeWidget();
        m_pWorldObjects->setObjectName(QString::fromUtf8("m_pWorldObjects"));
        sizePolicy.setHeightForWidth(m_pWorldObjects->sizePolicy().hasHeightForWidth());
        m_pWorldObjects->setSizePolicy(sizePolicy);
        m_pObjectTabs->addTab(m_pWorldObjects, QString());
        m_pDynamicObjects = new QWidget();
        m_pDynamicObjects->setObjectName(QString::fromUtf8("m_pDynamicObjects"));
        m_pDynamicObjects->setEnabled(true);
        sizePolicy1.setHeightForWidth(m_pDynamicObjects->sizePolicy().hasHeightForWidth());
        m_pDynamicObjects->setSizePolicy(sizePolicy1);
        m_pObjectTabs->addTab(m_pDynamicObjects, QString());
        m_pMappingModeTab->addTab(m_pObjectMapping, QString());
        m_pRevert = new QPushButton(MapEditor);
        m_pRevert->setObjectName(QString::fromUtf8("m_pRevert"));
        m_pRevert->setGeometry(QRect(250, 10, 31, 41));
        m_pRevert->setLayoutDirection(Qt::LeftToRight);
        m_pRevert->setAutoFillBackground(false);
        m_pRevert->setAutoRepeat(true);
        m_pRevert->setAutoRepeatDelay(250);
        m_pShowGrid = new QCheckBox(MapEditor);
        m_pShowGrid->setObjectName(QString::fromUtf8("m_pShowGrid"));
        m_pShowGrid->setGeometry(QRect(460, 30, 16, 21));
        QFont font1;
        font1.setPointSize(14);
        m_pShowGrid->setFont(font1);
        m_pShowGrid->setChecked(true);
        m_pMapTreeLabel_3 = new QLabel(MapEditor);
        m_pMapTreeLabel_3->setObjectName(QString::fromUtf8("m_pMapTreeLabel_3"));
        m_pMapTreeLabel_3->setGeometry(QRect(450, 0, 51, 21));
        m_pMapTreeLabel_3->setFont(font);
        m_pMapTreeLabel_2 = new QLabel(MapEditor);
        m_pMapTreeLabel_2->setObjectName(QString::fromUtf8("m_pMapTreeLabel_2"));
        m_pMapTreeLabel_2->setGeometry(QRect(290, 0, 81, 21));
        m_pMapTreeLabel_2->setFont(font);
        m_pZoom = new QSlider(MapEditor);
        m_pZoom->setObjectName(QString::fromUtf8("m_pZoom"));
        m_pZoom->setGeometry(QRect(290, 20, 151, 31));
        m_pZoom->setLayoutDirection(Qt::RightToLeft);
        m_pZoom->setMinimum(30);
        m_pZoom->setMaximum(100);
        m_pZoom->setPageStep(10);
        m_pZoom->setValue(100);
        m_pZoom->setSliderPosition(100);
        m_pZoom->setTracking(true);
        m_pZoom->setOrientation(Qt::Horizontal);
        m_pZoom->setTickPosition(QSlider::TicksBothSides);
        m_pLayer = new QFrame(MapEditor);
        m_pLayer->setObjectName(QString::fromUtf8("m_pLayer"));
        m_pLayer->setGeometry(QRect(500, 0, 191, 61));
        m_pLayer->setFrameShape(QFrame::Box);
        m_pLayer->setFrameShadow(QFrame::Raised);
        m_pCurLayer = new QSpinBox(m_pLayer);
        m_pCurLayer->setObjectName(QString::fromUtf8("m_pCurLayer"));
        m_pCurLayer->setGeometry(QRect(10, 30, 41, 22));
        m_pCurLayer->setMinimum(1);
        m_pCurLayer->setMaximum(5);
        m_pMapTreeLabel_5 = new QLabel(m_pLayer);
        m_pMapTreeLabel_5->setObjectName(QString::fromUtf8("m_pMapTreeLabel_5"));
        m_pMapTreeLabel_5->setGeometry(QRect(10, 0, 71, 21));
        m_pMapTreeLabel_5->setFont(font);
        m_pLayerBackground = new QRadioButton(m_pLayer);
        m_pLayerBackground->setObjectName(QString::fromUtf8("m_pLayerBackground"));
        m_pLayerBackground->setGeometry(QRect(100, 10, 82, 17));
        m_pLayerBackground->setChecked(true);
        m_pLayerForeground = new QRadioButton(m_pLayer);
        m_pLayerForeground->setObjectName(QString::fromUtf8("m_pLayerForeground"));
        m_pLayerForeground->setGeometry(QRect(100, 30, 82, 17));

        retranslateUi(MapEditor);

        m_pMapEditor->setCurrentIndex(-1);
        m_pMappingModeTab->setCurrentIndex(0);
        m_pTileTab->setCurrentIndex(0);
        m_pObjectTabs->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MapEditor);
    } // setupUi

    void retranslateUi(QWidget *MapEditor)
    {
        MapEditor->setWindowTitle(QApplication::translate("MapEditor", "Form", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem = m_pMapTree->headerItem();
        ___qtreewidgetitem->setText(1, QApplication::translate("MapEditor", "Name", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem->setText(0, QApplication::translate("MapEditor", "ID", 0, QApplication::UnicodeUTF8));
        m_pMapTreeLabel->setText(QApplication::translate("MapEditor", "map tree:", 0, QApplication::UnicodeUTF8));
        m_pTileTab->setTabText(m_pTileTab->indexOf(m_pTiles), QApplication::translate("MapEditor", "Tiles", 0, QApplication::UnicodeUTF8));
        m_pTileTab->setTabText(m_pTileTab->indexOf(m_pAutoTiles), QApplication::translate("MapEditor", "AutoTiles", 0, QApplication::UnicodeUTF8));
        m_pMappingModeTab->setTabText(m_pMappingModeTab->indexOf(m_pTileMapping), QApplication::translate("MapEditor", "Tiles", 0, QApplication::UnicodeUTF8));
        m_pObjectTabs->setTabText(m_pObjectTabs->indexOf(m_pWorldObjects), QApplication::translate("MapEditor", "WorldObjects", 0, QApplication::UnicodeUTF8));
        m_pObjectTabs->setTabText(m_pObjectTabs->indexOf(m_pDynamicObjects), QApplication::translate("MapEditor", "DynamicObjects", 0, QApplication::UnicodeUTF8));
        m_pMappingModeTab->setTabText(m_pMappingModeTab->indexOf(m_pObjectMapping), QApplication::translate("MapEditor", "Objects", 0, QApplication::UnicodeUTF8));
        m_pRevert->setText(QApplication::translate("MapEditor", "<-", 0, QApplication::UnicodeUTF8));
        m_pRevert->setShortcut(QString());
        m_pShowGrid->setText(QString());
        m_pMapTreeLabel_3->setText(QApplication::translate("MapEditor", "grid:", 0, QApplication::UnicodeUTF8));
        m_pMapTreeLabel_2->setText(QApplication::translate("MapEditor", "zoom:", 0, QApplication::UnicodeUTF8));
        m_pMapTreeLabel_5->setText(QApplication::translate("MapEditor", "layer no.:", 0, QApplication::UnicodeUTF8));
        m_pLayerBackground->setText(QApplication::translate("MapEditor", "background", 0, QApplication::UnicodeUTF8));
        m_pLayerForeground->setText(QApplication::translate("MapEditor", "foreground", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MapEditor: public Ui_MapEditor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAPEDITOR_H
