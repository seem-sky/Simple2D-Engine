#ifndef TOOL_WIDGET_H
#define TOOL_WIDGET_H

#include <QtWidgets/QWidget>

class ToolWidget : public QWidget
{
private:
    void _updateOpacity();

protected:
    void changeEvent(QEvent* pEvent);
    void hideEvent(QHideEvent* pEvent);

public:
    ToolWidget(QWidget* pParent = nullptr);
};
#endif
