/********************************************************************************
** Form generated from reading UI file 'Database.ui'
**
** Created: Tue 24. Jul 17:41:53 2012
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
#include <QtGui/QComboBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QPushButton>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Database
{
public:
    QWidget *centralwidget;
    QTabWidget *tabWidget;
    QWidget *TabTextures;
    QTabWidget *TextureTabs;
    QWidget *TabTiles;
    QWidget *TabTextureObjects;
    QWidget *TabTextureUnit;
    QLabel *TextureView;
    QComboBox *TextureBox;
    QLineEdit *TextureName;
    QLabel *label;
    QLineEdit *TextureIDLabel;
    QLineEdit *TextureTypeLabel;
    QLabel *label_2;
    QPushButton *NewTexture;
    QPushButton *DeleteTexture;
    QWidget *TabMapObjects;
    QPushButton *ButtonApply;
    QPushButton *ButtonCancel;
    QPushButton *ButtonOK;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *Database)
    {
        if (Database->objectName().isEmpty())
            Database->setObjectName(QString::fromUtf8("Database"));
        Database->setWindowModality(Qt::NonModal);
        Database->resize(802, 600);
        Database->setContextMenuPolicy(Qt::DefaultContextMenu);
        centralwidget = new QWidget(Database);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setGeometry(QRect(0, 0, 801, 551));
        TabTextures = new QWidget();
        TabTextures->setObjectName(QString::fromUtf8("TabTextures"));
        TextureTabs = new QTabWidget(TabTextures);
        TextureTabs->setObjectName(QString::fromUtf8("TextureTabs"));
        TextureTabs->setGeometry(QRect(150, 0, 641, 531));
        TextureTabs->setTabPosition(QTabWidget::East);
        TabTiles = new QWidget();
        TabTiles->setObjectName(QString::fromUtf8("TabTiles"));
        TextureTabs->addTab(TabTiles, QString());
        TabTextureObjects = new QWidget();
        TabTextureObjects->setObjectName(QString::fromUtf8("TabTextureObjects"));
        TextureTabs->addTab(TabTextureObjects, QString());
        TabTextureUnit = new QWidget();
        TabTextureUnit->setObjectName(QString::fromUtf8("TabTextureUnit"));
        TextureTabs->addTab(TabTextureUnit, QString());
        TextureView = new QLabel(TabTextures);
        TextureView->setObjectName(QString::fromUtf8("TextureView"));
        TextureView->setGeometry(QRect(10, 160, 131, 121));
        TextureView->setAutoFillBackground(true);
        TextureView->setFrameShape(QFrame::Box);
        TextureView->setFrameShadow(QFrame::Plain);
        TextureView->setAlignment(Qt::AlignCenter);
        TextureBox = new QComboBox(TabTextures);
        TextureBox->setObjectName(QString::fromUtf8("TextureBox"));
        TextureBox->setGeometry(QRect(10, 40, 131, 21));
        TextureName = new QLineEdit(TabTextures);
        TextureName->setObjectName(QString::fromUtf8("TextureName"));
        TextureName->setGeometry(QRect(10, 70, 131, 20));
        TextureName->setReadOnly(true);
        label = new QLabel(TabTextures);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 100, 31, 21));
        QFont font;
        font.setPointSize(12);
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);
        TextureIDLabel = new QLineEdit(TabTextures);
        TextureIDLabel->setObjectName(QString::fromUtf8("IDLabel"));
        TextureIDLabel->setGeometry(QRect(40, 100, 101, 20));
        TextureIDLabel->setReadOnly(true);
        TextureTypeLabel = new QLineEdit(TabTextures);
        TextureTypeLabel->setObjectName(QString::fromUtf8("TypeLabel"));
        TextureTypeLabel->setGeometry(QRect(80, 130, 61, 20));
        TextureTypeLabel->setReadOnly(true);
        label_2 = new QLabel(TabTextures);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(10, 130, 71, 21));
        label_2->setFont(font);
        NewTexture = new QPushButton(TabTextures);
        NewTexture->setObjectName(QString::fromUtf8("NewTexture"));
        NewTexture->setGeometry(QRect(10, 10, 61, 23));
        DeleteTexture = new QPushButton(TabTextures);
        DeleteTexture->setObjectName(QString::fromUtf8("DeleteTexture"));
        DeleteTexture->setGeometry(QRect(80, 10, 61, 23));
        tabWidget->addTab(TabTextures, QString());
        TabMapObjects = new QWidget();
        TabMapObjects->setObjectName(QString::fromUtf8("TabMapObjects"));
        tabWidget->addTab(TabMapObjects, QString());
        ButtonApply = new QPushButton(centralwidget);
        ButtonApply->setObjectName(QString::fromUtf8("ButtonApply"));
        ButtonApply->setGeometry(QRect(670, 550, 121, 31));
        ButtonCancel = new QPushButton(centralwidget);
        ButtonCancel->setObjectName(QString::fromUtf8("ButtonCancel"));
        ButtonCancel->setGeometry(QRect(550, 550, 121, 31));
        ButtonOK = new QPushButton(centralwidget);
        ButtonOK->setObjectName(QString::fromUtf8("ButtonOK"));
        ButtonOK->setGeometry(QRect(430, 550, 121, 31));
        Database->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(Database);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        Database->setStatusBar(statusbar);

        retranslateUi(Database);
        QObject::connect(ButtonCancel, SIGNAL(clicked()), Database, SLOT(close()));

        tabWidget->setCurrentIndex(0);
        TextureTabs->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(Database);
    } // setupUi

    void retranslateUi(QMainWindow *Database)
    {
        Database->setWindowTitle(QApplication::translate("Database", "Database", 0, QApplication::UnicodeUTF8));
        TextureTabs->setTabText(TextureTabs->indexOf(TabTiles), QApplication::translate("Database", "Tiles", 0, QApplication::UnicodeUTF8));
        TextureTabs->setTabText(TextureTabs->indexOf(TabTextureObjects), QApplication::translate("Database", "Objects", 0, QApplication::UnicodeUTF8));
        TextureTabs->setTabText(TextureTabs->indexOf(TabTextureUnit), QApplication::translate("Database", "NPCs", 0, QApplication::UnicodeUTF8));
        TextureView->setText(QString());
        label->setText(QApplication::translate("Database", "ID:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("Database", "TypeID:", 0, QApplication::UnicodeUTF8));
        NewTexture->setText(QApplication::translate("Database", "New", 0, QApplication::UnicodeUTF8));
        DeleteTexture->setText(QApplication::translate("Database", "Delete", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(TabTextures), QApplication::translate("Database", "Textures", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(TabMapObjects), QApplication::translate("Database", "Objects", 0, QApplication::UnicodeUTF8));
        ButtonApply->setText(QApplication::translate("Database", "Apply", 0, QApplication::UnicodeUTF8));
        ButtonCancel->setText(QApplication::translate("Database", "Cancel", 0, QApplication::UnicodeUTF8));
        ButtonOK->setText(QApplication::translate("Database", "OK", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Database: public Ui_Database {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DATABASE_H
