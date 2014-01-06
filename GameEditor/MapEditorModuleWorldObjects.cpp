#include "MapEditorModuleWorldObjects.h"

MapEditorModuleWorldObjects::MapEditorModuleWorldObjects(const DATABASE::DatabaseMgr& DBMgr, QWidget* pParent) : DatabaseModuleTooltipList(pParent), m_DBMgr(DBMgr)
{
    setMaximumWidth(200);
    setDragEnabled(false);
    setToolTipPosition(ToolTipPosition::TOOLTIP_RIGHT);
    setShowTooltip(true);
    setFrameShape(QFrame::NoFrame);
}

QWidget* MapEditorModuleWorldObjects::_setupTooltipWidget(uint32 uiPrototypeID)
{
    auto pLabel = new QLabel(this);
    //pLabel->resize(TILE_SIZE, TILE_SIZE);
    //if (auto pModel = dynamic_cast<DATABASE::DatabaseModel*>(model()))
    //{
    //    if (auto pPrototype = dynamic_cast<DATABASE::TILE_SET::TileSetPrototype*>(pModel->getDatabase()->getPrototype(uiPrototypeID)))
    //    {
    //        auto pixmap = DATABASE::TILE_SET::createPixmap(*pPrototype);
    //        pLabel->setPixmap(pixmap);
    //        pLabel->resize(pixmap.size());
    //    }
    //}
    return pLabel;
}

