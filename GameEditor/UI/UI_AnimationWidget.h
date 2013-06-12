/********************************************************************************
** Form generated from reading UI file 'AnimationWidget.ui'
**
** Created: Wed 12. Jun 18:46:19 2013
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ANIMATIONWIDGET_H
#define UI_ANIMATIONWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpinBox>
#include <QtGui/QWidget>
#include "animationviewwidget.h"
#include "dragtreewidget.h"

QT_BEGIN_NAMESPACE

class Ui_AnimationWidget
{
public:
    QLabel *label_18;
    AnimationViewDBChanger *m_pAniViewer;
    QLabel *m_pCurrentFrameLabel;
    QSpinBox *m_pCurrentFrame;
    QLabel *m_pFrameTimeLabel;
    QSpinBox *m_pFrameTime;
    QPushButton *m_pAniPlayerButton;
    QLabel *m_pSpriteLabel;
    SpritePrototypeDragTreeWidget *m_pSpriteList;
    QFrame *m_pSpriteFrame;
    QLabel *label_26;
    QLabel *label_24;
    QSpinBox *m_pSpriteSpriteID;
    QLabel *label_23;
    QSpinBox *m_pSpritePosY;
    QLabel *label_25;
    QSpinBox *m_pSpritePosX;
    QSpinBox *m_pSpriteScale;
    QLabel *label_27;
    QLabel *label_29;
    QSpinBox *m_pSpriteRotation;
    QLabel *label_28;
    QSpinBox *m_pSpriteOpacity;
    QLabel *label_30;
    QLabel *label_31;

    void setupUi(QWidget *AnimationWidget)
    {
        if (AnimationWidget->objectName().isEmpty())
            AnimationWidget->setObjectName(QString::fromUtf8("AnimationWidget"));
        AnimationWidget->resize(703, 472);
        label_18 = new QLabel(AnimationWidget);
        label_18->setObjectName(QString::fromUtf8("label_18"));
        label_18->setGeometry(QRect(210, 80, 101, 21));
        QFont font;
        font.setPointSize(12);
        font.setBold(true);
        font.setWeight(75);
        label_18->setFont(font);
        m_pAniViewer = new AnimationViewDBChanger(AnimationWidget);
        m_pAniViewer->setObjectName(QString::fromUtf8("m_pAniViewer"));
        m_pAniViewer->setGeometry(QRect(210, 110, 250, 250));
        m_pAniViewer->setAlignment(Qt::AlignCenter);
        m_pCurrentFrameLabel = new QLabel(AnimationWidget);
        m_pCurrentFrameLabel->setObjectName(QString::fromUtf8("m_pCurrentFrameLabel"));
        m_pCurrentFrameLabel->setGeometry(QRect(210, 410, 131, 21));
        m_pCurrentFrameLabel->setFont(font);
        m_pCurrentFrame = new QSpinBox(AnimationWidget);
        m_pCurrentFrame->setObjectName(QString::fromUtf8("m_pCurrentFrame"));
        m_pCurrentFrame->setGeometry(QRect(370, 410, 91, 22));
        m_pCurrentFrame->setMaximum(1000);
        m_pFrameTimeLabel = new QLabel(AnimationWidget);
        m_pFrameTimeLabel->setObjectName(QString::fromUtf8("m_pFrameTimeLabel"));
        m_pFrameTimeLabel->setGeometry(QRect(210, 440, 161, 21));
        m_pFrameTimeLabel->setFont(font);
        m_pFrameTime = new QSpinBox(AnimationWidget);
        m_pFrameTime->setObjectName(QString::fromUtf8("m_pFrameTime"));
        m_pFrameTime->setGeometry(QRect(370, 440, 91, 22));
        m_pFrameTime->setMaximum(999999);
        m_pAniPlayerButton = new QPushButton(AnimationWidget);
        m_pAniPlayerButton->setObjectName(QString::fromUtf8("m_pAniPlayerButton"));
        m_pAniPlayerButton->setGeometry(QRect(300, 370, 71, 31));
        m_pSpriteLabel = new QLabel(AnimationWidget);
        m_pSpriteLabel->setObjectName(QString::fromUtf8("m_pSpriteLabel"));
        m_pSpriteLabel->setGeometry(QRect(470, 0, 91, 21));
        m_pSpriteLabel->setFont(font);
        m_pSpriteList = new SpritePrototypeDragTreeWidget(AnimationWidget);
        m_pSpriteList->setObjectName(QString::fromUtf8("m_pSpriteList"));
        m_pSpriteList->setGeometry(QRect(470, 20, 221, 261));
        m_pSpriteList->setEditTriggers(QAbstractItemView::NoEditTriggers);
        m_pSpriteList->setDragEnabled(true);
        m_pSpriteList->setDragDropMode(QAbstractItemView::DragOnly);
        m_pSpriteList->setIndentation(0);
        m_pSpriteList->setRootIsDecorated(false);
        m_pSpriteList->setSortingEnabled(true);
        m_pSpriteList->setHeaderHidden(false);
        m_pSpriteList->setExpandsOnDoubleClick(false);
        m_pSpriteList->setColumnCount(2);
        m_pSpriteList->header()->setVisible(true);
        m_pSpriteList->header()->setCascadingSectionResizes(false);
        m_pSpriteList->header()->setDefaultSectionSize(50);
        m_pSpriteList->header()->setHighlightSections(false);
        m_pSpriteList->header()->setProperty("showSortIndicator", QVariant(true));
        m_pSpriteFrame = new QFrame(AnimationWidget);
        m_pSpriteFrame->setObjectName(QString::fromUtf8("m_pSpriteFrame"));
        m_pSpriteFrame->setGeometry(QRect(470, 290, 221, 171));
        m_pSpriteFrame->setFrameShape(QFrame::NoFrame);
        m_pSpriteFrame->setFrameShadow(QFrame::Sunken);
        m_pSpriteFrame->setLineWidth(1);
        label_26 = new QLabel(m_pSpriteFrame);
        label_26->setObjectName(QString::fromUtf8("label_26"));
        label_26->setGeometry(QRect(0, 150, 81, 21));
        label_26->setFont(font);
        label_24 = new QLabel(m_pSpriteFrame);
        label_24->setObjectName(QString::fromUtf8("label_24"));
        label_24->setGeometry(QRect(0, 30, 51, 21));
        label_24->setFont(font);
        m_pSpriteSpriteID = new QSpinBox(m_pSpriteFrame);
        m_pSpriteSpriteID->setObjectName(QString::fromUtf8("m_pSpriteSpriteID"));
        m_pSpriteSpriteID->setGeometry(QRect(120, 150, 101, 22));
        m_pSpriteSpriteID->setFrame(true);
        m_pSpriteSpriteID->setReadOnly(true);
        m_pSpriteSpriteID->setButtonSymbols(QAbstractSpinBox::NoButtons);
        m_pSpriteSpriteID->setMaximum(999999999);
        label_23 = new QLabel(m_pSpriteFrame);
        label_23->setObjectName(QString::fromUtf8("label_23"));
        label_23->setGeometry(QRect(0, 0, 131, 21));
        label_23->setFont(font);
        m_pSpritePosY = new QSpinBox(m_pSpriteFrame);
        m_pSpritePosY->setObjectName(QString::fromUtf8("m_pSpritePosY"));
        m_pSpritePosY->setGeometry(QRect(170, 30, 51, 22));
        m_pSpritePosY->setMinimum(-999999999);
        m_pSpritePosY->setMaximum(999999999);
        label_25 = new QLabel(m_pSpriteFrame);
        label_25->setObjectName(QString::fromUtf8("label_25"));
        label_25->setGeometry(QRect(120, 30, 51, 21));
        label_25->setFont(font);
        m_pSpritePosX = new QSpinBox(m_pSpriteFrame);
        m_pSpritePosX->setObjectName(QString::fromUtf8("m_pSpritePosX"));
        m_pSpritePosX->setGeometry(QRect(60, 30, 51, 22));
        m_pSpritePosX->setMinimum(-999999999);
        m_pSpritePosX->setMaximum(999999999);
        m_pSpriteScale = new QSpinBox(m_pSpriteFrame);
        m_pSpriteScale->setObjectName(QString::fromUtf8("m_pSpriteScale"));
        m_pSpriteScale->setGeometry(QRect(120, 60, 81, 22));
        m_pSpriteScale->setMinimum(0);
        m_pSpriteScale->setMaximum(999999999);
        m_pSpriteScale->setValue(100);
        label_27 = new QLabel(m_pSpriteFrame);
        label_27->setObjectName(QString::fromUtf8("label_27"));
        label_27->setGeometry(QRect(0, 60, 61, 21));
        label_27->setFont(font);
        label_29 = new QLabel(m_pSpriteFrame);
        label_29->setObjectName(QString::fromUtf8("label_29"));
        label_29->setGeometry(QRect(0, 120, 81, 21));
        label_29->setFont(font);
        m_pSpriteRotation = new QSpinBox(m_pSpriteFrame);
        m_pSpriteRotation->setObjectName(QString::fromUtf8("m_pSpriteRotation"));
        m_pSpriteRotation->setGeometry(QRect(120, 120, 101, 22));
        m_pSpriteRotation->setMinimum(0);
        m_pSpriteRotation->setMaximum(359);
        label_28 = new QLabel(m_pSpriteFrame);
        label_28->setObjectName(QString::fromUtf8("label_28"));
        label_28->setGeometry(QRect(200, 60, 21, 21));
        label_28->setFont(font);
        m_pSpriteOpacity = new QSpinBox(m_pSpriteFrame);
        m_pSpriteOpacity->setObjectName(QString::fromUtf8("m_pSpriteOpacity"));
        m_pSpriteOpacity->setGeometry(QRect(120, 90, 81, 22));
        m_pSpriteOpacity->setMinimum(0);
        m_pSpriteOpacity->setMaximum(100);
        label_30 = new QLabel(m_pSpriteFrame);
        label_30->setObjectName(QString::fromUtf8("label_30"));
        label_30->setGeometry(QRect(0, 90, 81, 21));
        label_30->setFont(font);
        label_31 = new QLabel(m_pSpriteFrame);
        label_31->setObjectName(QString::fromUtf8("label_31"));
        label_31->setGeometry(QRect(200, 90, 21, 21));
        label_31->setFont(font);
        label_18->raise();
        m_pAniViewer->raise();
        m_pCurrentFrameLabel->raise();
        m_pFrameTimeLabel->raise();
        m_pFrameTime->raise();
        m_pAniPlayerButton->raise();
        m_pSpriteLabel->raise();
        m_pSpriteList->raise();
        m_pCurrentFrame->raise();
        m_pSpriteFrame->raise();
        QWidget::setTabOrder(m_pAniViewer, m_pAniPlayerButton);
        QWidget::setTabOrder(m_pAniPlayerButton, m_pCurrentFrame);
        QWidget::setTabOrder(m_pCurrentFrame, m_pFrameTime);
        QWidget::setTabOrder(m_pFrameTime, m_pSpriteList);
        QWidget::setTabOrder(m_pSpriteList, m_pSpritePosX);
        QWidget::setTabOrder(m_pSpritePosX, m_pSpritePosY);
        QWidget::setTabOrder(m_pSpritePosY, m_pSpriteScale);
        QWidget::setTabOrder(m_pSpriteScale, m_pSpriteOpacity);
        QWidget::setTabOrder(m_pSpriteOpacity, m_pSpriteRotation);
        QWidget::setTabOrder(m_pSpriteRotation, m_pSpriteSpriteID);

        retranslateUi(AnimationWidget);

        QMetaObject::connectSlotsByName(AnimationWidget);
    } // setupUi

    void retranslateUi(QWidget *AnimationWidget)
    {
        AnimationWidget->setWindowTitle(QApplication::translate("AnimationWidget", "Form", 0, QApplication::UnicodeUTF8));
        label_18->setText(QApplication::translate("AnimationWidget", "animation:", 0, QApplication::UnicodeUTF8));
        m_pCurrentFrameLabel->setText(QApplication::translate("AnimationWidget", "current frame:", 0, QApplication::UnicodeUTF8));
        m_pFrameTimeLabel->setText(QApplication::translate("AnimationWidget", "frame time (msec):", 0, QApplication::UnicodeUTF8));
        m_pAniPlayerButton->setText(QApplication::translate("AnimationWidget", "Play", 0, QApplication::UnicodeUTF8));
        m_pSpriteLabel->setText(QApplication::translate("AnimationWidget", "sprites:", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem = m_pSpriteList->headerItem();
        ___qtreewidgetitem->setText(1, QApplication::translate("AnimationWidget", "Name", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem->setText(0, QApplication::translate("AnimationWidget", "ID", 0, QApplication::UnicodeUTF8));
        label_26->setText(QApplication::translate("AnimationWidget", "spriteID:", 0, QApplication::UnicodeUTF8));
        label_24->setText(QApplication::translate("AnimationWidget", "posX:", 0, QApplication::UnicodeUTF8));
        label_23->setText(QApplication::translate("AnimationWidget", "current sprite:", 0, QApplication::UnicodeUTF8));
        label_25->setText(QApplication::translate("AnimationWidget", "posY:", 0, QApplication::UnicodeUTF8));
        label_27->setText(QApplication::translate("AnimationWidget", "scale:", 0, QApplication::UnicodeUTF8));
        label_29->setText(QApplication::translate("AnimationWidget", "rotation:", 0, QApplication::UnicodeUTF8));
        label_28->setText(QApplication::translate("AnimationWidget", "%", 0, QApplication::UnicodeUTF8));
        label_30->setText(QApplication::translate("AnimationWidget", "opacity:", 0, QApplication::UnicodeUTF8));
        label_31->setText(QApplication::translate("AnimationWidget", "%", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class AnimationWidget: public Ui_AnimationWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ANIMATIONWIDGET_H
