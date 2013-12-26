#include "MapEditorModuleContent.h"
#include "moc_MapEditorModuleContent.h"
#include "Project.h"

MapEditorModuleContent::MapEditorModuleContent(DATABASE::DatabaseMgr& databaseMgr, QWidget* pWidget) : QWidget(pWidget), Ui_MapEditorModuleContent(),
    m_DBMgr(databaseMgr)
{
    setupUi(this);

    connect(m_pMapTabs, SIGNAL(currentChanged(int)), this, SLOT(_onCurrentChanged(int)));
    connect(m_pZoom, SIGNAL(valueChanged(int)), this, SLOT(_onZoomChanged(int)));
    connect(m_pCurLayer, SIGNAL(valueChanged(int)), this, SLOT(_onLayerChanged(int)));
    connect(m_pLayerBackground, SIGNAL(clicked()), this, SLOT(_onLayerTypeChanged()));
    connect(m_pLayerForeground, SIGNAL(clicked()), this, SLOT(_onLayerTypeChanged()));
    connect(m_pShowGrid, SIGNAL(stateChanged(int)), this, SLOT(_onGridShowChanged(int)));
    connect(m_pRevert, SIGNAL(clicked()), this, SLOT(_onRevertPressed()));

    _onZoomChanged(m_pZoom->value());
}

void MapEditorModuleContent::_onRevertPressed()
{
    if (auto pTab = dynamic_cast<MapViewer*>(m_pMapTabs->currentWidget()))
        pTab->revertLast();
}

void MapEditorModuleContent::_onCurrentChanged(int index)
{
    if (auto pTab = dynamic_cast<MapViewer*>(m_pMapTabs->widget(index)))
    {
        // setup zoom
        m_pZoom->setValue(pTab->getZoom());
        m_pZoomLabel->setText("zoom: " + QString::number(pTab->getZoom()) + "%");

        m_pShowGrid->setCheckState(pTab->isGridActive() ? Qt::Checked : Qt::Unchecked);
        pTab->getLayerType() == MAP::LAYER_BACKGROUND ? m_pLayerBackground->setChecked(true) : m_pLayerForeground->setChecked(true);
        _onLayerTypeChanged();
    }
}

void MapEditorModuleContent::_onZoomChanged(int value)
{
    if (auto pTab = dynamic_cast<MapViewer*>(m_pMapTabs->currentWidget()))
        pTab->setZoom(value);
    m_pZoomLabel->setText("zoom: " + QString::number(value) + "%");
}

void MapEditorModuleContent::_onLayerChanged(int value)
{
    if (auto pTab = dynamic_cast<MapViewer*>(m_pMapTabs->currentWidget()))
        pTab->setLayerIndex(value);
}

void MapEditorModuleContent::_onLayerTypeChanged()
{
    if (auto pTab = dynamic_cast<MapViewer*>(m_pMapTabs->currentWidget()))
    {
        auto layerType = m_pLayerBackground->isChecked() ? MAP::LAYER_BACKGROUND : MAP::LAYER_FOREGROUND;
        pTab->setLayerType(layerType);
        auto maxSize = pTab->getMaximumLayerIndex(layerType);
        m_pCurLayer->setMinimum(maxSize == 0 ? 0 : 1);
        m_pCurLayer->setMaximum(maxSize);
        m_pCurLayer->setValue(pTab->getLayerIndex());
    }
}

void MapEditorModuleContent::_onGridShowChanged(int state)
{
    if (auto pTab = dynamic_cast<MapViewer*>(m_pMapTabs->currentWidget()))
        pTab->showGrid(state == Qt::Checked);
}

void MapEditorModuleContent::onMapOpened(uint32 mapID)
{
    if (auto pTab = getTab(mapID))
        return;
    auto pNewMapViewer = new MapViewer(mapID, m_DBMgr, this);
    pNewMapViewer->loadMap();
    m_pMapTabs->addTab(pNewMapViewer, pNewMapViewer->getMapName());
    m_pMapTabs->setCurrentWidget(pNewMapViewer);
    emit registerTab(pNewMapViewer);

    connect(pNewMapViewer, SIGNAL(changed(MapViewer*)), this, SLOT(_onMapChanged(MapViewer*)));
}

void MapEditorModuleContent::onMapClosed(uint32 mapID)
{
    if (auto pTab = getTab(mapID))
        m_pMapTabs->removeTab(m_pMapTabs->indexOf(pTab));
}

void MapEditorModuleContent::onMapEdited(uint32 mapID)
{
    if (auto pTab = getTab(mapID))
    {
        pTab->updateMap();
        // get new maximum layer size
        _onLayerTypeChanged();
    }
}

MapViewer* MapEditorModuleContent::getTab(uint32 mapID)
{
    for (int32 i = 0; i < m_pMapTabs->count(); ++i)
    {
        if (auto pTab = dynamic_cast<MapViewer*>(m_pMapTabs->widget(i)))
        {
            if (pTab->getMapID() == mapID)
                return pTab;
        }
    }
    return nullptr;
}

void MapEditorModuleContent::_onMapChanged(MapViewer* pMapViewer)
{
    m_pMapTabs->setTabText(m_pMapTabs->indexOf(pMapViewer), pMapViewer->getMapName() + (pMapViewer->hasChanges() ? "*" : ""));
}
