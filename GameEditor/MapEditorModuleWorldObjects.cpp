#include "MapEditorModuleWorldObjects.h"
#include "VisualViewer.h"
#include "DatabaseMgr.h"

using namespace DATABASE::PROTOTYPE::WORLD_OBJECT;

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
	auto pViewer = new VisualViewer(this);
	pViewer->setDatabaseManager(&m_DBMgr);
	pViewer->showGrid(false);
	pViewer->resize(150, 150);
	if (auto pModel = dynamic_cast<DATABASE::ConstDatabaseModel*>(model()))
	{
		if (auto pPrototype = dynamic_cast<const WorldObjectPrototype*>(pModel->getDatabase()->getPrototype(uiPrototypeID)))
		{
			if (pPrototype->getFlagManager().hasFlag(Flags::FLAG_ANIMATION))
				pViewer->setAnimation(pPrototype->getAnimationModule().getAnimationInfo(1));
		}
	}
	return pViewer;
}

