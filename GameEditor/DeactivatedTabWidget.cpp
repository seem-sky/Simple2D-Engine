#include "DeactivatedTabWidget.h"
#include <QtWidgets/QTabBar>
#include "moc_DeactivatedTabWidget.h"

DeactivatedWidget::DeactivatedWidget(QWidget* pParent) : QWidget(pParent)
{
}

void DeactivatedWidget::_onCheckBoxChanged(int state)
{
    setEnabled(state != Qt::Unchecked);
}

DeactivatedTabWidget::DeactivatedTabWidget(QWidget* pParent) : QTabWidget(pParent)
{
}

void DeactivatedTabWidget::tabInserted(int index)
{
    QTabWidget::tabInserted(index);
    auto pTabBar = getTabBar();
    auto pWidget = widget(index);
    auto pCheckBox = new QCheckBox(pWidget);
    connect(pCheckBox, SIGNAL(stateChanged(int)), pWidget, SLOT(_onCheckBoxChanged(int)));
    pTabBar->setTabButton(index, QTabBar::RightSide, pCheckBox);
    pCheckBox->setChecked(pWidget->isEnabled());
}