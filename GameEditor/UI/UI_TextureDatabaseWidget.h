/********************************************************************************
** Form generated from reading UI file 'TextureDatabaseWidget.ui'
**
** Created: Wed 12. Jun 18:46:23 2013
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TEXTUREDATABASEWIDGET_H
#define UI_TEXTUREDATABASEWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGraphicsView>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TextureDatabaseWidget
{
public:
    QLabel *label_18;
    QLineEdit *m_pFileName;
    QLineEdit *m_pTransparencyColor;
    QLabel *label_43;
    QPushButton *m_pChoseFile;
    QGraphicsView *m_pView;
    QPushButton *m_pChoseColor;
    QLineEdit *m_pPath;

    void setupUi(QWidget *TextureDatabaseWidget)
    {
        if (TextureDatabaseWidget->objectName().isEmpty())
            TextureDatabaseWidget->setObjectName(QString::fromUtf8("TextureDatabaseWidget"));
        TextureDatabaseWidget->resize(380, 440);
        label_18 = new QLabel(TextureDatabaseWidget);
        label_18->setObjectName(QString::fromUtf8("label_18"));
        label_18->setGeometry(QRect(210, 80, 91, 21));
        QFont font;
        font.setPointSize(12);
        font.setBold(true);
        font.setWeight(75);
        label_18->setFont(font);
        m_pFileName = new QLineEdit(TextureDatabaseWidget);
        m_pFileName->setObjectName(QString::fromUtf8("m_pFileName"));
        m_pFileName->setGeometry(QRect(210, 160, 161, 20));
        m_pFileName->setReadOnly(true);
        m_pTransparencyColor = new QLineEdit(TextureDatabaseWidget);
        m_pTransparencyColor->setObjectName(QString::fromUtf8("m_pTransparencyColor"));
        m_pTransparencyColor->setGeometry(QRect(210, 410, 161, 20));
        label_43 = new QLabel(TextureDatabaseWidget);
        label_43->setObjectName(QString::fromUtf8("label_43"));
        label_43->setGeometry(QRect(210, 360, 131, 21));
        label_43->setFont(font);
        m_pChoseFile = new QPushButton(TextureDatabaseWidget);
        m_pChoseFile->setObjectName(QString::fromUtf8("m_pChoseFile"));
        m_pChoseFile->setGeometry(QRect(210, 100, 111, 23));
        m_pView = new QGraphicsView(TextureDatabaseWidget);
        m_pView->setObjectName(QString::fromUtf8("m_pView"));
        m_pView->setGeometry(QRect(210, 190, 161, 161));
        m_pView->setDragMode(QGraphicsView::ScrollHandDrag);
        m_pChoseColor = new QPushButton(TextureDatabaseWidget);
        m_pChoseColor->setObjectName(QString::fromUtf8("m_pChoseColor"));
        m_pChoseColor->setGeometry(QRect(210, 380, 111, 23));
        m_pPath = new QLineEdit(TextureDatabaseWidget);
        m_pPath->setObjectName(QString::fromUtf8("m_pPath"));
        m_pPath->setGeometry(QRect(210, 130, 161, 20));
        m_pPath->setReadOnly(true);

        retranslateUi(TextureDatabaseWidget);

        QMetaObject::connectSlotsByName(TextureDatabaseWidget);
    } // setupUi

    void retranslateUi(QWidget *TextureDatabaseWidget)
    {
        TextureDatabaseWidget->setWindowTitle(QApplication::translate("TextureDatabaseWidget", "Form", 0, QApplication::UnicodeUTF8));
        label_18->setText(QApplication::translate("TextureDatabaseWidget", "file name:", 0, QApplication::UnicodeUTF8));
        label_43->setText(QApplication::translate("TextureDatabaseWidget", "transp. color:", 0, QApplication::UnicodeUTF8));
        m_pChoseFile->setText(QApplication::translate("TextureDatabaseWidget", "chose file", 0, QApplication::UnicodeUTF8));
        m_pChoseColor->setText(QApplication::translate("TextureDatabaseWidget", "chose color", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class TextureDatabaseWidget: public Ui_TextureDatabaseWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TEXTUREDATABASEWIDGET_H
