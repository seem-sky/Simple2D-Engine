/********************************************************************************
** Form generated from reading UI file 'Database.ui'
**
** Created: Wed 12. Jun 18:46:23 2013
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DATABASE_H
#define UI_DATABASE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QTabWidget>
#include <QtGui/QWidget>
#include "animationdatabasewidget.h"
#include "autotiledatabasewidget.h"
#include "objectanimationtypedatabasewidget.h"
#include "spritedatabasewidget.h"
#include "textdatabasewidget.h"
#include "tiledatabasewidget.h"
#include "worldobjectdatabasewidget.h"

QT_BEGIN_NAMESPACE

class Ui_Database
{
public:
    QPushButton *ButtonCancel;
    QTabWidget *m_pSections;
    QWidget *widget;
    QTabWidget *m_pTextureTabs;
    TileDatabaseWidget *m_pTiles;
    AutoTileDatabaseWidget *m_pAutoTiles;
    SpriteDatabaseWidget *m_pSprites;
    AnimationDatabaseWidget *m_pAnimations;
    QWidget *Objects;
    QTabWidget *m_pObjectTabs;
    WorldObjectDatabaseWidget *m_pWorldObjects;
    ObjectAnimationTypeDatabaseWidget *m_pObjectAnimationTypes;
    QWidget *Text;
    QTabWidget *m_pTextTabs;
    TextDatabaseWidget *m_pTexts;
    QPushButton *ButtonOK;
    QPushButton *ButtonApply;

    void setupUi(QDialog *Database)
    {
        if (Database->objectName().isEmpty())
            Database->setObjectName(QString::fromUtf8("Database"));
        Database->resize(798, 587);
        Database->setMinimumSize(QSize(798, 587));
        ButtonCancel = new QPushButton(Database);
        ButtonCancel->setObjectName(QString::fromUtf8("ButtonCancel"));
        ButtonCancel->setGeometry(QRect(550, 550, 121, 31));
        m_pSections = new QTabWidget(Database);
        m_pSections->setObjectName(QString::fromUtf8("m_pSections"));
        m_pSections->setGeometry(QRect(10, 10, 781, 531));
        m_pSections->setMinimumSize(QSize(0, 0));
        widget = new QWidget();
        widget->setObjectName(QString::fromUtf8("widget"));
        m_pTextureTabs = new QTabWidget(widget);
        m_pTextureTabs->setObjectName(QString::fromUtf8("m_pTextureTabs"));
        m_pTextureTabs->setGeometry(QRect(10, 10, 761, 491));
        m_pTiles = new TileDatabaseWidget();
        m_pTiles->setObjectName(QString::fromUtf8("m_pTiles"));
        m_pTextureTabs->addTab(m_pTiles, QString());
        m_pAutoTiles = new AutoTileDatabaseWidget();
        m_pAutoTiles->setObjectName(QString::fromUtf8("m_pAutoTiles"));
        m_pTextureTabs->addTab(m_pAutoTiles, QString());
        m_pSprites = new SpriteDatabaseWidget();
        m_pSprites->setObjectName(QString::fromUtf8("m_pSprites"));
        m_pTextureTabs->addTab(m_pSprites, QString());
        m_pAnimations = new AnimationDatabaseWidget();
        m_pAnimations->setObjectName(QString::fromUtf8("m_pAnimations"));
        m_pTextureTabs->addTab(m_pAnimations, QString());
        m_pSections->addTab(widget, QString());
        Objects = new QWidget();
        Objects->setObjectName(QString::fromUtf8("Objects"));
        m_pObjectTabs = new QTabWidget(Objects);
        m_pObjectTabs->setObjectName(QString::fromUtf8("m_pObjectTabs"));
        m_pObjectTabs->setGeometry(QRect(10, 10, 761, 491));
        m_pWorldObjects = new WorldObjectDatabaseWidget();
        m_pWorldObjects->setObjectName(QString::fromUtf8("m_pWorldObjects"));
        m_pObjectTabs->addTab(m_pWorldObjects, QString());
        m_pObjectAnimationTypes = new ObjectAnimationTypeDatabaseWidget();
        m_pObjectAnimationTypes->setObjectName(QString::fromUtf8("m_pObjectAnimationTypes"));
        m_pObjectTabs->addTab(m_pObjectAnimationTypes, QString());
        m_pSections->addTab(Objects, QString());
        Text = new QWidget();
        Text->setObjectName(QString::fromUtf8("Text"));
        m_pTextTabs = new QTabWidget(Text);
        m_pTextTabs->setObjectName(QString::fromUtf8("m_pTextTabs"));
        m_pTextTabs->setGeometry(QRect(10, 10, 761, 491));
        m_pTexts = new TextDatabaseWidget();
        m_pTexts->setObjectName(QString::fromUtf8("m_pTexts"));
        m_pTextTabs->addTab(m_pTexts, QString());
        m_pSections->addTab(Text, QString());
        ButtonOK = new QPushButton(Database);
        ButtonOK->setObjectName(QString::fromUtf8("ButtonOK"));
        ButtonOK->setGeometry(QRect(430, 550, 121, 31));
        ButtonApply = new QPushButton(Database);
        ButtonApply->setObjectName(QString::fromUtf8("ButtonApply"));
        ButtonApply->setEnabled(true);
        ButtonApply->setGeometry(QRect(670, 550, 121, 31));
        QWidget::setTabOrder(m_pSections, m_pTextureTabs);
        QWidget::setTabOrder(m_pTextureTabs, ButtonOK);
        QWidget::setTabOrder(ButtonOK, ButtonCancel);
        QWidget::setTabOrder(ButtonCancel, ButtonApply);
        QWidget::setTabOrder(ButtonApply, m_pTextTabs);

        retranslateUi(Database);

        m_pSections->setCurrentIndex(2);
        m_pTextureTabs->setCurrentIndex(0);
        m_pObjectTabs->setCurrentIndex(0);
        m_pTextTabs->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(Database);
    } // setupUi

    void retranslateUi(QDialog *Database)
    {
        Database->setWindowTitle(QApplication::translate("Database", "Game Database", 0, QApplication::UnicodeUTF8));
        ButtonCancel->setText(QApplication::translate("Database", "Cancel", 0, QApplication::UnicodeUTF8));
        m_pTextureTabs->setTabText(m_pTextureTabs->indexOf(m_pTiles), QApplication::translate("Database", "Tiles", 0, QApplication::UnicodeUTF8));
        m_pTextureTabs->setTabText(m_pTextureTabs->indexOf(m_pAutoTiles), QApplication::translate("Database", "AutoTiles", 0, QApplication::UnicodeUTF8));
        m_pTextureTabs->setTabText(m_pTextureTabs->indexOf(m_pSprites), QApplication::translate("Database", "Sprites", 0, QApplication::UnicodeUTF8));
        m_pTextureTabs->setTabText(m_pTextureTabs->indexOf(m_pAnimations), QApplication::translate("Database", "Animations", 0, QApplication::UnicodeUTF8));
        m_pSections->setTabText(m_pSections->indexOf(widget), QApplication::translate("Database", "Textures", 0, QApplication::UnicodeUTF8));
        m_pObjectTabs->setTabText(m_pObjectTabs->indexOf(m_pWorldObjects), QApplication::translate("Database", "WorldObjects", 0, QApplication::UnicodeUTF8));
        m_pObjectTabs->setTabText(m_pObjectTabs->indexOf(m_pObjectAnimationTypes), QApplication::translate("Database", "ObjectAnimationTypes", 0, QApplication::UnicodeUTF8));
        m_pSections->setTabText(m_pSections->indexOf(Objects), QApplication::translate("Database", "Objects", 0, QApplication::UnicodeUTF8));
        m_pTextTabs->setTabText(m_pTextTabs->indexOf(m_pTexts), QApplication::translate("Database", "Locals", 0, QApplication::UnicodeUTF8));
        m_pSections->setTabText(m_pSections->indexOf(Text), QApplication::translate("Database", "Text", 0, QApplication::UnicodeUTF8));
        ButtonOK->setText(QApplication::translate("Database", "OK", 0, QApplication::UnicodeUTF8));
        ButtonApply->setText(QApplication::translate("Database", "Apply", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Database: public Ui_Database {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DATABASE_H
