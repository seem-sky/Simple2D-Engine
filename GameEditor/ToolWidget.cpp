#include "ToolWidget.h"
#include <QtCore/QEvent>

const qreal INACTIVE_OPACITY = 0.5;

ToolWidget::ToolWidget(QWidget* pParent) : QWidget(pParent)
{
    setWindowFlags(Qt::Tool | Qt::CustomizeWindowHint | Qt::WindowTitleHint);
    setFocusPolicy(Qt::ClickFocus);
    setWindowOpacity(INACTIVE_OPACITY);
}

void ToolWidget::_updateOpacity()
{
    setWindowOpacity(isActiveWindow() ? 1 : INACTIVE_OPACITY);
}

void ToolWidget::changeEvent(QEvent* pEvent)
{
    switch (pEvent->type())
    {
    case QEvent::ActivationChange:
        _updateOpacity();
        break;
    }
    QWidget::changeEvent(pEvent);
}

void ToolWidget::hideEvent(QHideEvent* pEvent)
{
    _updateOpacity();
    QWidget::hideEvent(pEvent);
}
