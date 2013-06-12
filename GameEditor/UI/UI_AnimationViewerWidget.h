/********************************************************************************
** Form generated from reading UI file 'AnimationViewerWidget.ui'
**
** Created: Wed 12. Jun 18:46:19 2013
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ANIMATIONVIEWERWIDGET_H
#define UI_ANIMATIONVIEWERWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>
#include "animationviewwidget.h"

QT_BEGIN_NAMESPACE

class Ui_AnimationViewerWidget
{
public:
    AnimationViewDBChanger *m_pAnimationView;
    QComboBox *m_pObjectAnimationType;

    void setupUi(QWidget *AnimationViewerWidget)
    {
        if (AnimationViewerWidget->objectName().isEmpty())
            AnimationViewerWidget->setObjectName(QString::fromUtf8("AnimationViewerWidget"));
        AnimationViewerWidget->resize(151, 171);
        m_pAnimationView = new AnimationViewDBChanger(AnimationViewerWidget);
        m_pAnimationView->setObjectName(QString::fromUtf8("m_pAnimationView"));
        m_pAnimationView->setGeometry(QRect(0, 20, 150, 150));
        m_pObjectAnimationType = new QComboBox(AnimationViewerWidget);
        m_pObjectAnimationType->setObjectName(QString::fromUtf8("m_pObjectAnimationType"));
        m_pObjectAnimationType->setGeometry(QRect(0, 0, 151, 22));

        retranslateUi(AnimationViewerWidget);

        QMetaObject::connectSlotsByName(AnimationViewerWidget);
    } // setupUi

    void retranslateUi(QWidget *AnimationViewerWidget)
    {
        AnimationViewerWidget->setWindowTitle(QApplication::translate("AnimationViewerWidget", "Form", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class AnimationViewerWidget: public Ui_AnimationViewerWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ANIMATIONVIEWERWIDGET_H
