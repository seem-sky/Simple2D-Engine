#include "MapEditorModuleTileSets.h"
#include "QtGlobal.h"
#include "Config.h"
#include <QtGui/QMouseEvent>
#include <QtGui/QPainter>
#include <Core/Cache/Manager.h>
#include "moc_MapEditorModuleTileSets.h"

using namespace DATABASE;
using namespace PROTOTYPE::TILE_SET;

MapEditorModuleTileSets::MapEditorModuleTileSets(CACHE::Manager& cacheMgr, QWidget* pParent)
    : DatabaseModuleTooltipList(pParent), m_CacheMgr(cacheMgr)
{
    setDragEnabled(false);
    setToolTipPosition(ToolTipPosition::TOOLTIP_RIGHT);
    setShowTooltip(true);
    setFrameShape(QFrame::NoFrame);
}

QWidget* MapEditorModuleTileSets::_setupTooltipWidget(uint32 uiPrototypeID)
{
    auto pLabel = new QLabel(this);
    pLabel->resize(MAP::TILE_SIZE, MAP::TILE_SIZE);
    if (auto pModel = dynamic_cast<ConstDatabaseModel*>(model()))
    {
        if (auto pPrototype = dynamic_cast<const TileSetPrototype*>(pModel->getDatabase()->getPrototype(uiPrototypeID)))
        {
            auto pixmap = createPixmap(*pPrototype, m_CacheMgr.getTileCache());
            pLabel->setPixmap(pixmap);
            pLabel->resize(pixmap.size());
        }
    }
    return pLabel;
}

void MapEditorModuleTileSets::mousePressEvent(QMouseEvent* pEvent)
{
    DatabaseModuleTooltipList::mousePressEvent(pEvent);

    auto item = indexAt(pEvent->pos());
    if (item.isValid() && (pEvent->button() == Qt::RightButton || pEvent->button() == Qt::LeftButton))
    {
        auto brush = BRUSH::BrushIndex::BRUSH_LEFT;
        if (pEvent->button() == Qt::RightButton)
            brush = BRUSH::BrushIndex::BRUSH_RIGHT;

        // ToDo: hacky ID; better solution?
        emit selectionChanged(brush, MAP::BRUSH::BrushInfo::Type::TILE_SET, item.row()+1);
        emit itemClicked(brush);
    }
}
