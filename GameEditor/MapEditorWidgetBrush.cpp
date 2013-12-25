#include "MapEditorWidgetBrush.h"
#include <QtWidgets/QHBoxLayout>
#include <QtCore/QEvent>
#include "moc_MapEditorWidgetBrush.h"

using namespace BRUSH;

const qreal INACTIVE_OPACITY = 0.5;

MapEditorWidgetBrush::MapEditorWidgetBrush(const DATABASE::DatabaseMgr &DBMgr, QWidget *pParent) : QWidget(pParent), m_DBMgr(DBMgr)
{
    setWindowFlags(Qt::Tool | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::MSWindowsFixedSizeDialogHint);
    setWindowTitle("map editor brush");

    setFocusPolicy(Qt::ClickFocus);
    setWindowOpacity(INACTIVE_OPACITY);

    auto pLayout = new QHBoxLayout(this);
    setLayout(pLayout);
    pLayout->setContentsMargins(0, 0, 0, 0);
    for (uint32 i = 0; i < 2; ++i)
    {
        m_pBrushes.at(i) = new MapEditorModuleBrush(m_DBMgr, this);
        pLayout->addWidget(m_pBrushes.at(i));
    }

    m_pBrushes.at(static_cast<uint32>(BrushIndex::BRUSH_LEFT))->setText("left brush");
    m_pBrushes.at(static_cast<uint32>(BrushIndex::BRUSH_RIGHT))->setText("right brush");
}

void MapEditorWidgetBrush::changeEvent(QEvent *pEvent)
{
    if (pEvent->type() == QEvent::ActivationChange)
        setWindowOpacity(isActiveWindow() ? 1 : INACTIVE_OPACITY); 
}

void MapEditorWidgetBrush::setBrushSelection(BrushIndex brush, SelectionType type, uint32 ID)
{
}

BrushType MapEditorWidgetBrush::getBrushType(BrushIndex brush) const
{
    return m_pBrushes.at(static_cast<uint32>(brush))->getBrushType();
}

void MapEditorWidgetBrush::_onSelectionChanged(BrushIndex brush, SelectionType selectionType, uint32 ID)
{
    m_pBrushes.at(static_cast<uint32>(brush))->setSelection(selectionType, ID);

}
