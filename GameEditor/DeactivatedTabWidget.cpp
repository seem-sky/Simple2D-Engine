#include "DeactivatedTabWidget.h"
#include "moc_DeactivatedTabWidget.h"
#include <QtWidgets/QTabBar>

DeactivatedWidget::DeactivatedWidget(QWidget* pParent) : QWidget(pParent)
{
}

void DeactivatedWidget::_onCheckBoxChanged(int state)
{
    setEnabled(state != Qt::Unchecked);
	setActivated(state == Qt::Checked);
}

void DeactivatedWidget::setActivated(bool activated)
{
	m_IsActivated = activated;
	emit activationChanged(m_IsActivated);
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
	connect(pWidget, SIGNAL(activationChanged(bool)), pCheckBox, SLOT(setChecked(bool)));
    pTabBar->setTabButton(index, QTabBar::RightSide, pCheckBox);
    pCheckBox->setChecked(pWidget->isEnabled());
}
