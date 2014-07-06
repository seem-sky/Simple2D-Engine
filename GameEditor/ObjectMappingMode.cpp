#include "ObjectMappingMode.h"
#include "MapViewer.h"
#include "MapEditorModuleWorldObjects.h"
#include <MapViewItem.h>

namespace MAPPING_MODE
{
    Object::Object(const MapEditorModuleWorldObjects& objectWidget) : m_ObjectWidget(objectWidget)
    {}

    void Object::press(MapViewer* pViewer, const QMouseEvent* pEvent)
    {
        //_deselectItem();
        //if (!pViewer || !pEvent)
        //    return;

        //auto pScene = pViewer->getScene();
        //if (!pScene)
        //    return;

        //auto scenePos = pViewer->mapToScene(pEvent->pos());
        //if (auto pItem = dynamic_cast<MapViewItem*>(pScene->itemAt(scenePos, QTransform())))
        //{
        //    m_pSelectedItem = pItem;
        //    m_pSelectedItem->setSelected(true);
        //    m_pSelectedItem->setFocus(Qt::MouseFocusReason);
        //}
    }

    void Object::release(MapViewer* pViewer, const QMouseEvent* pEvent)
    {
        if (!pViewer || !pEvent)
            return;

        auto selectedItems = pViewer->getScene()->selectedItems();
        if (selectedItems.isEmpty())
            pViewer->addWorldObject(pViewer->mapToScene(pEvent->pos()).toPoint(), m_ObjectWidget.getSelectedID());
    }

    void Object::move(MapViewer* pViewer, const QMouseEvent* pEvent)
    {
    }
}
