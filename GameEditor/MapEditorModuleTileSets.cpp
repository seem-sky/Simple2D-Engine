#include "MapEditorModuleTileSets.h"
#include "QtGlobal.h"
#include "Config.h"
#include <QtGui/QMouseEvent>
#include <QtGui/QPainter>
#include "moc_MapEditorModuleTileSets.h"

MapEditorModuleTileSets::MapEditorModuleTileSets(QWidget* pParent) : DatabaseModuleTooltipList(pParent)
{
    setDragEnabled(false);
    setToolTipPosition(ToolTipPosition::TOOLTIP_RIGHT);
    setShowTooltip(true);
    setFrameShape(QFrame::NoFrame);
}

QWidget* MapEditorModuleTileSets::_setupTooltipWidget(uint32 uiPrototypeID)
{
    auto pLabel = new QLabel(this);
    pLabel->resize(TILE_SIZE, TILE_SIZE);
    if (auto pModel = dynamic_cast<DATABASE::DatabaseModel*>(model()))
    {
        if (auto pPrototype = dynamic_cast<DATABASE::TILE_SET::TileSetPrototype*>(pModel->getDatabase()->getPrototype(uiPrototypeID)))
        {
            auto pixmap = DATABASE::TILE_SET::createPixmap(*pPrototype);
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
        emit selectionChanged(brush, MAP::BRUSH::SelectionType::TILE_SETS, item.row()+1);
        emit itemClicked(brush);
    }
}
