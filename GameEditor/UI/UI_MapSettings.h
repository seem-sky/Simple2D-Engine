/********************************************************************************
** Form generated from reading UI file 'MapSettings.ui'
**
** Created: Wed 12. Jun 18:46:22 2013
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAPSETTINGS_H
#define UI_MAPSETTINGS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_MapSettings
{
public:
    QDialogButtonBox *m_pButtons;
    QLabel *m_pMapTreeLabel;
    QLineEdit *m_pFileName;
    QLineEdit *m_pName;
    QLabel *m_pMapTreeLabel_2;
    QLineEdit *m_pScriptName;
    QLabel *m_pMapTreeLabel_3;
    QLabel *m_pMapTreeLabel_4;
    QLabel *m_pMapTreeLabel_5;
    QLabel *m_pMapTreeLabel_6;
    QSpinBox *m_pSizeX;
    QSpinBox *m_pSizeY;
    QLabel *m_pMapTreeLabel_7;
    QSpinBox *m_pID;
    QLabel *m_pMapTreeLabel_8;
    QSpinBox *m_pBackgroundLayer;
    QSpinBox *m_pForegroundLayer;
    QLabel *m_pMapTreeLabel_9;

    void setupUi(QDialog *MapSettings)
    {
        if (MapSettings->objectName().isEmpty())
            MapSettings->setObjectName(QString::fromUtf8("MapSettings"));
        MapSettings->resize(329, 232);
        MapSettings->setMinimumSize(QSize(329, 232));
        MapSettings->setMaximumSize(QSize(329, 232));
        m_pButtons = new QDialogButtonBox(MapSettings);
        m_pButtons->setObjectName(QString::fromUtf8("m_pButtons"));
        m_pButtons->setGeometry(QRect(160, 200, 161, 32));
        m_pButtons->setOrientation(Qt::Horizontal);
        m_pButtons->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        m_pMapTreeLabel = new QLabel(MapSettings);
        m_pMapTreeLabel->setObjectName(QString::fromUtf8("m_pMapTreeLabel"));
        m_pMapTreeLabel->setGeometry(QRect(10, 50, 91, 21));
        QFont font;
        font.setPointSize(12);
        font.setBold(true);
        font.setWeight(75);
        m_pMapTreeLabel->setFont(font);
        m_pFileName = new QLineEdit(MapSettings);
        m_pFileName->setObjectName(QString::fromUtf8("m_pFileName"));
        m_pFileName->setGeometry(QRect(10, 70, 151, 20));
        m_pFileName->setMouseTracking(false);
        m_pFileName->setAcceptDrops(false);
        m_pFileName->setMaxLength(25);
        m_pFileName->setFrame(true);
        m_pFileName->setReadOnly(true);
        m_pName = new QLineEdit(MapSettings);
        m_pName->setObjectName(QString::fromUtf8("m_pName"));
        m_pName->setGeometry(QRect(170, 70, 151, 20));
        m_pName->setMouseTracking(true);
        m_pName->setAcceptDrops(true);
        m_pName->setMaxLength(25);
        m_pMapTreeLabel_2 = new QLabel(MapSettings);
        m_pMapTreeLabel_2->setObjectName(QString::fromUtf8("m_pMapTreeLabel_2"));
        m_pMapTreeLabel_2->setGeometry(QRect(170, 50, 151, 21));
        m_pMapTreeLabel_2->setFont(font);
        m_pScriptName = new QLineEdit(MapSettings);
        m_pScriptName->setObjectName(QString::fromUtf8("m_pScriptName"));
        m_pScriptName->setGeometry(QRect(170, 30, 151, 20));
        m_pScriptName->setMouseTracking(false);
        m_pScriptName->setAcceptDrops(false);
        m_pScriptName->setMaxLength(25);
        m_pMapTreeLabel_3 = new QLabel(MapSettings);
        m_pMapTreeLabel_3->setObjectName(QString::fromUtf8("m_pMapTreeLabel_3"));
        m_pMapTreeLabel_3->setGeometry(QRect(170, 10, 151, 21));
        m_pMapTreeLabel_3->setFont(font);
        m_pMapTreeLabel_4 = new QLabel(MapSettings);
        m_pMapTreeLabel_4->setObjectName(QString::fromUtf8("m_pMapTreeLabel_4"));
        m_pMapTreeLabel_4->setGeometry(QRect(10, 90, 61, 21));
        m_pMapTreeLabel_4->setFont(font);
        m_pMapTreeLabel_5 = new QLabel(MapSettings);
        m_pMapTreeLabel_5->setObjectName(QString::fromUtf8("m_pMapTreeLabel_5"));
        m_pMapTreeLabel_5->setGeometry(QRect(160, 110, 16, 21));
        m_pMapTreeLabel_5->setFont(font);
        m_pMapTreeLabel_6 = new QLabel(MapSettings);
        m_pMapTreeLabel_6->setObjectName(QString::fromUtf8("m_pMapTreeLabel_6"));
        m_pMapTreeLabel_6->setGeometry(QRect(260, 90, 61, 21));
        m_pMapTreeLabel_6->setFont(font);
        m_pSizeX = new QSpinBox(MapSettings);
        m_pSizeX->setObjectName(QString::fromUtf8("m_pSizeX"));
        m_pSizeX->setGeometry(QRect(10, 110, 141, 22));
        m_pSizeX->setMinimum(1);
        m_pSizeX->setMaximum(1000);
        m_pSizeX->setValue(20);
        m_pSizeY = new QSpinBox(MapSettings);
        m_pSizeY->setObjectName(QString::fromUtf8("m_pSizeY"));
        m_pSizeY->setGeometry(QRect(180, 110, 141, 22));
        m_pSizeY->setMinimum(1);
        m_pSizeY->setMaximum(1000);
        m_pSizeY->setValue(20);
        m_pMapTreeLabel_7 = new QLabel(MapSettings);
        m_pMapTreeLabel_7->setObjectName(QString::fromUtf8("m_pMapTreeLabel_7"));
        m_pMapTreeLabel_7->setGeometry(QRect(10, 10, 81, 21));
        m_pMapTreeLabel_7->setFont(font);
        m_pID = new QSpinBox(MapSettings);
        m_pID->setObjectName(QString::fromUtf8("m_pID"));
        m_pID->setGeometry(QRect(10, 30, 151, 22));
        m_pID->setReadOnly(true);
        m_pID->setButtonSymbols(QAbstractSpinBox::NoButtons);
        m_pID->setMinimum(1);
        m_pID->setMaximum(999999999);
        m_pID->setValue(1);
        m_pMapTreeLabel_8 = new QLabel(MapSettings);
        m_pMapTreeLabel_8->setObjectName(QString::fromUtf8("m_pMapTreeLabel_8"));
        m_pMapTreeLabel_8->setGeometry(QRect(10, 140, 151, 21));
        m_pMapTreeLabel_8->setFont(font);
        m_pBackgroundLayer = new QSpinBox(MapSettings);
        m_pBackgroundLayer->setObjectName(QString::fromUtf8("m_pBackgroundLayer"));
        m_pBackgroundLayer->setGeometry(QRect(240, 140, 81, 22));
        m_pBackgroundLayer->setMinimum(1);
        m_pBackgroundLayer->setMaximum(100);
        m_pBackgroundLayer->setValue(1);
        m_pForegroundLayer = new QSpinBox(MapSettings);
        m_pForegroundLayer->setObjectName(QString::fromUtf8("m_pForegroundLayer"));
        m_pForegroundLayer->setGeometry(QRect(240, 170, 81, 22));
        m_pForegroundLayer->setMinimum(0);
        m_pForegroundLayer->setMaximum(100);
        m_pForegroundLayer->setValue(0);
        m_pMapTreeLabel_9 = new QLabel(MapSettings);
        m_pMapTreeLabel_9->setObjectName(QString::fromUtf8("m_pMapTreeLabel_9"));
        m_pMapTreeLabel_9->setGeometry(QRect(10, 170, 151, 21));
        m_pMapTreeLabel_9->setFont(font);
        QWidget::setTabOrder(m_pID, m_pScriptName);
        QWidget::setTabOrder(m_pScriptName, m_pFileName);
        QWidget::setTabOrder(m_pFileName, m_pName);
        QWidget::setTabOrder(m_pName, m_pSizeX);
        QWidget::setTabOrder(m_pSizeX, m_pSizeY);
        QWidget::setTabOrder(m_pSizeY, m_pButtons);

        retranslateUi(MapSettings);
        QObject::connect(m_pButtons, SIGNAL(accepted()), MapSettings, SLOT(accept()));
        QObject::connect(m_pButtons, SIGNAL(rejected()), MapSettings, SLOT(reject()));

        QMetaObject::connectSlotsByName(MapSettings);
    } // setupUi

    void retranslateUi(QDialog *MapSettings)
    {
        MapSettings->setWindowTitle(QApplication::translate("MapSettings", "Dialog", 0, QApplication::UnicodeUTF8));
        m_pMapTreeLabel->setText(QApplication::translate("MapSettings", "file name:", 0, QApplication::UnicodeUTF8));
        m_pName->setText(QString());
        m_pMapTreeLabel_2->setText(QApplication::translate("MapSettings", "name:", 0, QApplication::UnicodeUTF8));
        m_pScriptName->setText(QString());
        m_pMapTreeLabel_3->setText(QApplication::translate("MapSettings", "script name:", 0, QApplication::UnicodeUTF8));
        m_pMapTreeLabel_4->setText(QApplication::translate("MapSettings", "size X:", 0, QApplication::UnicodeUTF8));
        m_pMapTreeLabel_5->setText(QApplication::translate("MapSettings", "x", 0, QApplication::UnicodeUTF8));
        m_pMapTreeLabel_6->setText(QApplication::translate("MapSettings", "size Y:", 0, QApplication::UnicodeUTF8));
        m_pMapTreeLabel_7->setText(QApplication::translate("MapSettings", "ID:", 0, QApplication::UnicodeUTF8));
        m_pMapTreeLabel_8->setText(QApplication::translate("MapSettings", "background layer:", 0, QApplication::UnicodeUTF8));
        m_pMapTreeLabel_9->setText(QApplication::translate("MapSettings", "foreground layer:", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MapSettings: public Ui_MapSettings {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAPSETTINGS_H
