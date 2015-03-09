#include "MapEditorModuleTileSets.h"
#include "moc_MapEditorModuleTileSets.h"
#include "QtGlobal.h"
#include "Config.h"
#include <QtGui/QMouseEvent>
#include <QtGui/QPainter>
#include <Core/Cache/Manager.h>
#include "DatabaseModel.h"
#include "EditorGlobal.h"

MapEditorModuleTileSets::MapEditorModuleTileSets(const database::Manager& DBMgr, CACHE::Manager& cacheMgr, QWidget* pParent)
    : TooltipList(pParent), m_CacheMgr(cacheMgr), m_DBMgr(DBMgr)
{
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setDragEnabled(false);
    setToolTipPosition(ToolTipPosition::Right);
    setShowTooltip(true);
    setFrameShape(QFrame::NoFrame);
    setModel(new database::Model(DBMgr.getTileSetDatabase(), this));
    resizeColumnToContents(0);
}

QWidget* MapEditorModuleTileSets::_setupTooltipWidget(uint32 uiPrototypeID)
{
    auto pLabel = new QLabel(this);
    pLabel->resize(MAP::TILE_SIZE, MAP::TILE_SIZE);
    if (auto pPrototype = m_DBMgr.getTileSetDatabase().getPrototype(uiPrototypeID))
    {
        auto pixmap = database::prototype::createPixmap(pPrototype, m_CacheMgr.getTileCache());
        pLabel->setPixmap(pixmap);
        pLabel->resize(pixmap.size());
    }
    return pLabel;
}

void MapEditorModuleTileSets::mousePressEvent(QMouseEvent* pEvent)
{
    TooltipList::mousePressEvent(pEvent);

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
