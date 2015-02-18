#include "MapEditorModuleScriptArea.h"

MapEditorModuleScriptArea::MapEditorModuleScriptArea(QWidget* pParent)
    : QWidget(pParent), m_pWidgetScriptArea(new MapEditorWidgetScriptArea(this))
{}

void MapEditorModuleScriptArea::hideEvent(QHideEvent* pEvent)
{
    m_pWidgetScriptArea->hide();
    QWidget::hideEvent(pEvent);
}

void MapEditorModuleScriptArea::showEvent(QShowEvent* pEvent)
{
    m_pWidgetScriptArea->show();
    QWidget::showEvent(pEvent);
}
