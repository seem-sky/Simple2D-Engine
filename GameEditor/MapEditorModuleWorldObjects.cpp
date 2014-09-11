#include "MapEditorModuleWorldObjects.h"
#include "VisualViewer.h"
#include "DatabaseMgr.h"
#include "MapEditorWidgetObjectMapping.h"

using namespace DATABASE::PROTOTYPE::WORLD_OBJECT;

MapEditorModuleWorldObjects::MapEditorModuleWorldObjects(const DATABASE::DatabaseMgr& DBMgr, QWidget* pParent) :
DatabaseModuleTooltipList(pParent), m_DBMgr(DBMgr), m_pObjectWidget(new MapEditorWidgetObjectMapping(DBMgr, this))
{
    setMaximumWidth(200);
    setDragEnabled(false);
    setToolTipPosition(ToolTipPosition::TOOLTIP_RIGHT);
    setShowTooltip(true);
    setFrameShape(QFrame::NoFrame);

    connect(this, SIGNAL(changeIndex(int32)), m_pObjectWidget, SLOT(onIndexChanged(int32)));

    m_pObjectWidget->show();
}

QWidget* MapEditorModuleWorldObjects::_setupTooltipWidget(uint32 uiPrototypeID)
{
	auto pViewer = new VisualViewer(this);
	pViewer->setDatabaseMgr(&m_DBMgr);
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

void MapEditorModuleWorldObjects::hideEvent(QHideEvent* pEvent)
{
    m_pObjectWidget->hide();
    QWidget::hideEvent(pEvent);
}

void MapEditorModuleWorldObjects::showEvent(QShowEvent* pEvent)
{
    m_pObjectWidget->show();
    QWidget::showEvent(pEvent);
}
