#include "MapEditorWidgetBrush.h"
#include <QtWidgets/QHBoxLayout>
#include "moc_MapEditorWidgetBrush.h"
#include <BrushFactory.h>
#include "MapEditorModuleBrush.h"

using namespace BRUSH;
using namespace MAP::BRUSH;

MapEditorWidgetBrush::MapEditorWidgetBrush(const DATABASE::DatabaseMgr& DBMgr, QWidget* pParent) : ToolWidget(pParent), m_DBMgr(DBMgr)
{
    setWindowFlags(windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
    setWindowTitle("map editor brush");

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
