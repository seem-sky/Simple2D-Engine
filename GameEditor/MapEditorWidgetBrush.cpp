#include "MapEditorWidgetBrush.h"
#include <QtWidgets/QHBoxLayout>
#include <QtCore/QEvent>
#include "moc_MapEditorWidgetBrush.h"
#include <BrushFactory.h>
#include "MapEditorModuleBrush.h"

using namespace BRUSH;
using namespace MAP::BRUSH;

const qreal INACTIVE_OPACITY = 0.5;

MapEditorWidgetBrush::MapEditorWidgetBrush(const DATABASE::DatabaseMgr& DBMgr, QWidget* pParent) : QWidget(pParent), m_DBMgr(DBMgr)
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

void MapEditorWidgetBrush::changeEvent(QEvent* pEvent)
{
    if (pEvent->type() == QEvent::ActivationChange)
        _updateOpacity();
}

void MapEditorWidgetBrush::_updateOpacity()
{
    setWindowOpacity(isActiveWindow() ? 1 : INACTIVE_OPACITY); 
}

const MAP::BRUSH::BrushInfo& MapEditorWidgetBrush::getBrushInfo(BrushIndex brush) const
{
    return m_pBrushes.at(static_cast<uint32>(brush))->getBrushInfo();
}

void MapEditorWidgetBrush::onSelectionChanged(BrushIndex brush, MAP::BRUSH::BrushInfo::Type type, uint32 ID)
{
    if (auto pBrush = m_pBrushes.at(static_cast<uint32>(brush)))
    {
        auto brushInfo = pBrush->getBrushInfo();
        brushInfo.setID(ID);
        brushInfo.setType(type);
        pBrush->setBrushInfo(brushInfo);
    }
}

void MapEditorWidgetBrush::setBrushInfo(BrushIndex brush, const MAP::BRUSH::BrushInfo& brushInfo)
{
    m_pBrushes.at(static_cast<uint32>(brush))->setBrushInfo(brushInfo);
}

void MapEditorWidgetBrush::onBrushInfoRequested(BRUSH::BrushIndex brush, MAP::BRUSH::BrushInfo& brushInfo)
{
    brushInfo = m_pBrushes.at(static_cast<uint32>(brush))->getBrushInfo();
}

MAP::BRUSH::BrushPtr MapEditorWidgetBrush::createBrush(BrushIndex brushIndex, MAP::Layer& layer) const
{
    return MAP::BRUSH::BrushFactory::createBrush(getBrushInfo(brushIndex), getDatabaseMgr(), layer);
}
