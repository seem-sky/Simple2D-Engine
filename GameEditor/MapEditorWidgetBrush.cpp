#include "MapEditorWidgetBrush.h"
#include <QtWidgets/QHBoxLayout>
#include "moc_MapEditorWidgetBrush.h"
#include <BrushFactory.h>
#include "MapEditorModuleBrush.h"

using namespace BRUSH;
using namespace MAP::BRUSH;

MapEditorWidgetBrush::MapEditorWidgetBrush(CACHE::Manager& cacheMgr, const database::Manager& DBMgr, QWidget* pParent)
    : ToolWidget(pParent), m_DBMgr(DBMgr)
{
    setWindowFlags(windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
    setWindowTitle("map editor brush");

    auto pLayout = new QHBoxLayout(this);
    setLayout(pLayout);
    pLayout->setContentsMargins(0, 0, 0, 0);
    for (uint32 i = 0; i < 2; ++i)
    {
        m_pBrushes.at(i) = new MapEditorModuleBrush(cacheMgr, m_DBMgr, this);
        pLayout->addWidget(m_pBrushes.at(i));
    }

    m_pBrushes.at(static_cast<uint32>(BrushIndex::BRUSH_LEFT))->setText("left brush");
    m_pBrushes.at(static_cast<uint32>(BrushIndex::BRUSH_RIGHT))->setText("right brush");

    for (auto pModule : m_pBrushes)
        connect(pModule, SIGNAL(changeBrush(const MapEditorModuleBrush*)), this, SLOT(onBrushChanged(const MapEditorModuleBrush*)));
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

void MapEditorWidgetBrush::onBrushChanged(const MapEditorModuleBrush* pModule)
{
    for (uint8 i = 0; i < m_pBrushes.size(); ++i)
    {
        if (m_pBrushes.at(i) == pModule)
            emit changeBrushInfo(static_cast<BRUSH::BrushIndex>(i), m_pBrushes.at(i)->getBrushInfo());
    }
}

void MapEditorWidgetBrush::clear()
{
    for (auto pModule : m_pBrushes)
        pModule->clear();
}
