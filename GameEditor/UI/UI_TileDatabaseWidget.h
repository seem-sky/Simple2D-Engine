/********************************************************************************
** Form generated from reading UI file 'TileDatabaseWidget.ui'
**
** Created: Wed 12. Jun 18:46:24 2013
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TILEDATABASEWIDGET_H
#define UI_TILEDATABASEWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGraphicsView>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TileDatabaseWidget
{
public:
    QGraphicsView *m_pPassability;
    QPushButton *m_pUp;
    QPushButton *m_pRight;
    QPushButton *m_pDown;
    QPushButton *m_pLeft;

    void setupUi(QWidget *TileDatabaseWidget)
    {
        if (TileDatabaseWidget->objectName().isEmpty())
            TileDatabaseWidget->setObjectName(QString::fromUtf8("TileDatabaseWidget"));
        TileDatabaseWidget->resize(594, 430);
        m_pPassability = new QGraphicsView(TileDatabaseWidget);
        m_pPassability->setObjectName(QString::fromUtf8("m_pPassability"));
        m_pPassability->setGeometry(QRect(385, 0, 161, 161));
        m_pUp = new QPushButton(TileDatabaseWidget);
        m_pUp->setObjectName(QString::fromUtf8("m_pUp"));
        m_pUp->setGeometry(QRect(450, 10, 32, 32));
        m_pUp->setFocusPolicy(Qt::StrongFocus);
        m_pUp->setIconSize(QSize(32, 32));
        m_pUp->setCheckable(true);
        m_pUp->setChecked(true);
        m_pRight = new QPushButton(TileDatabaseWidget);
        m_pRight->setObjectName(QString::fromUtf8("m_pRight"));
        m_pRight->setGeometry(QRect(500, 60, 32, 32));
        m_pRight->setFocusPolicy(Qt::StrongFocus);
        m_pRight->setIconSize(QSize(32, 32));
        m_pRight->setCheckable(true);
        m_pRight->setChecked(true);
        m_pDown = new QPushButton(TileDatabaseWidget);
        m_pDown->setObjectName(QString::fromUtf8("m_pDown"));
        m_pDown->setGeometry(QRect(450, 120, 32, 32));
        m_pDown->setFocusPolicy(Qt::StrongFocus);
        m_pDown->setIconSize(QSize(32, 32));
        m_pDown->setCheckable(true);
        m_pDown->setChecked(true);
        m_pLeft = new QPushButton(TileDatabaseWidget);
        m_pLeft->setObjectName(QString::fromUtf8("m_pLeft"));
        m_pLeft->setGeometry(QRect(400, 60, 32, 32));
        m_pLeft->setFocusPolicy(Qt::StrongFocus);
        m_pLeft->setIconSize(QSize(32, 32));
        m_pLeft->setCheckable(true);
        m_pLeft->setChecked(true);

        retranslateUi(TileDatabaseWidget);

        QMetaObject::connectSlotsByName(TileDatabaseWidget);
    } // setupUi

    void retranslateUi(QWidget *TileDatabaseWidget)
    {
        TileDatabaseWidget->setWindowTitle(QApplication::translate("TileDatabaseWidget", "Form", 0, QApplication::UnicodeUTF8));
        m_pUp->setText(QString());
        m_pRight->setText(QString());
        m_pDown->setText(QString());
        m_pLeft->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class TileDatabaseWidget: public Ui_TileDatabaseWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TILEDATABASEWIDGET_H
