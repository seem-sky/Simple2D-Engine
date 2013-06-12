/********************************************************************************
** Form generated from reading UI file 'MapBrushWidget.ui'
**
** Created: Wed 12. Jun 18:46:21 2013
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAPBRUSHWIDGET_H
#define UI_MAPBRUSHWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MapBrushWidget
{
public:
    QLabel *m_pCurrentTile;
    QComboBox *m_pMode;

    void setupUi(QWidget *MapBrushWidget)
    {
        if (MapBrushWidget->objectName().isEmpty())
            MapBrushWidget->setObjectName(QString::fromUtf8("MapBrushWidget"));
        MapBrushWidget->resize(94, 63);
        MapBrushWidget->setMinimumSize(QSize(94, 63));
        MapBrushWidget->setMaximumSize(QSize(94, 63));
        m_pCurrentTile = new QLabel(MapBrushWidget);
        m_pCurrentTile->setObjectName(QString::fromUtf8("m_pCurrentTile"));
        m_pCurrentTile->setGeometry(QRect(30, 0, 32, 32));
        m_pCurrentTile->setFrameShape(QFrame::Box);
        m_pCurrentTile->setAlignment(Qt::AlignCenter);
        m_pMode = new QComboBox(MapBrushWidget);
        m_pMode->setObjectName(QString::fromUtf8("m_pMode"));
        m_pMode->setGeometry(QRect(0, 40, 91, 22));

        retranslateUi(MapBrushWidget);

        QMetaObject::connectSlotsByName(MapBrushWidget);
    } // setupUi

    void retranslateUi(QWidget *MapBrushWidget)
    {
        MapBrushWidget->setWindowTitle(QApplication::translate("MapBrushWidget", "Form", 0, QApplication::UnicodeUTF8));
        m_pCurrentTile->setText(QString());
        m_pMode->clear();
        m_pMode->insertItems(0, QStringList()
         << QApplication::translate("MapBrushWidget", "Pen", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MapBrushWidget", "Fill", 0, QApplication::UnicodeUTF8)
        );
    } // retranslateUi

};

namespace Ui {
    class MapBrushWidget: public Ui_MapBrushWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAPBRUSHWIDGET_H
