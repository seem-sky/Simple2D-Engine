#include "MapEditorModuleContent.h"
#include "moc_MapEditorModuleContent.h"
#include "Project.h"
#include <QtWidgets/QMessageBox>
#include "MapEditor.h"
#include "WorldObjectItem.h"
#include <DatabaseMgr.h>
#include <QtWidgets/QGraphicsItem>
#include <Core/Cache/Manager.h>

MapEditorModuleContent::MapEditorModuleContent(CACHE::Manager& cacheMgr, const MappingObject& mappingObject, DATABASE::DatabaseMgr& databaseMgr, QWidget* pWidget) :
    QWidget(pWidget), m_CacheMgr(cacheMgr), m_DBMgr(databaseMgr), m_MappingObject(mappingObject), Ui_MapEditorModuleContent()
{
    setupUi(this);

    connect(m_pMapTabs, SIGNAL(currentChanged(int)), this, SLOT(_onCurrentChanged(int)));
    connect(m_pZoom, SIGNAL(valueChanged(int)), this, SLOT(_onZoomChanged(int)));
    connect(m_pCurLayer, SIGNAL(valueChanged(int)), this, SLOT(_onLayerChanged(int)));
    connect(m_pLayerBackground, SIGNAL(clicked()), this, SLOT(_onLayerTypeChanged()));
    connect(m_pLayerForeground, SIGNAL(clicked()), this, SLOT(_onLayerTypeChanged()));
    connect(m_pShowGrid, SIGNAL(stateChanged(int)), this, SLOT(_onGridShowChanged(int)));
    connect(m_pRevert, SIGNAL(clicked()), this, SLOT(_onRevertPressed()));
    connect(m_pMapTabs, (SIGNAL(tabCloseRequested(int))), this, SLOT(_onTabCloseRequested(int)));

    _onZoomChanged(m_pZoom->value());

    m_pRevert->setShortcut(QKeySequence(tr("Ctrl+Z", "Action|Revert")));
}

void MapEditorModuleContent::_onTabCloseRequested(int index)
{
    if (auto pTab = dynamic_cast<MapEditor*>(m_pMapTabs->widget(index)))
    {
        // ask for saving before closing tab
        if (pTab->hasChanged())
        {
            if (auto pMap = m_DBMgr.getMapDatabase()->getOriginalPrototype(pTab->getMapID()))
            {
                QString mapName = pMap->getName();
                switch (QMessageBox::question(this, "close " + mapName, mapName + " has been changed. Do you want to save changes?",
                    QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, QMessageBox::Yes))
                {
                case QMessageBox::Yes: pTab->saveMap(); break;  // save map
                case QMessageBox::Cancel: return;               // do nothing
                }
            }
        }
        m_pMapTabs->removeTab(index);
        pTab->deleteLater();
    }
}

void MapEditorModuleContent::_onRevertPressed()
{
    if (auto pTab = dynamic_cast<MapEditor*>(m_pMapTabs->currentWidget()))
        pTab->revertLast();
}

void MapEditorModuleContent::_onCurrentChanged(int index)
{
    if (auto pTab = dynamic_cast<MapEditor*>(m_pMapTabs->widget(index)))
    {
        // setup zoom
        m_pZoom->setValue(pTab->getZoom());
        m_pZoomLabel->setText("zoom: " + QString::number(pTab->getZoom()) + "%");

        m_pRevert->setEnabled(pTab->hasChanged());

        m_pShowGrid->setCheckState(pTab->isGridActive() ? Qt::Checked : Qt::Unchecked);
        pTab->getLayerType() == MAP::LayerType::LAYER_BACKGROUND ? m_pLayerBackground->setChecked(true) : m_pLayerForeground->setChecked(true);
        _onLayerTypeChanged();
    }
}

void MapEditorModuleContent::_onZoomChanged(int value)
{
    if (auto pTab = dynamic_cast<MapEditor*>(m_pMapTabs->currentWidget()))
        pTab->setZoom(value);
    m_pZoomLabel->setText("zoom: " + QString::number(value) + "%");
}

void MapEditorModuleContent::_onLayerChanged(int value)
{
    if (auto pTab = dynamic_cast<MapEditor*>(m_pMapTabs->currentWidget()))
        pTab->setLayerIndex(value);
}

void MapEditorModuleContent::_onLayerTypeChanged()
{
    if (auto pTab = dynamic_cast<MapEditor*>(m_pMapTabs->currentWidget()))
    {
        auto layerType = m_pLayerBackground->isChecked() ? MAP::LayerType::LAYER_BACKGROUND : MAP::LayerType::LAYER_FOREGROUND;
        pTab->setLayerType(layerType);
        auto maxSize = pTab->getMaximumLayerIndex(layerType);
        m_pCurLayer->setMinimum(maxSize == 0 ? 0 : 1);
        m_pCurLayer->setMaximum(maxSize);
        m_pCurLayer->setValue(pTab->getLayerIndex());
        pTab->setLayerIndex(m_pCurLayer->value());
    }
}

void MapEditorModuleContent::_onGridShowChanged(int state)
{
    if (auto pTab = dynamic_cast<MapEditor*>(m_pMapTabs->currentWidget()))
        pTab->showGrid(state == Qt::Checked);
}

void MapEditorModuleContent::onMapOpened(uint32 mapID)
{
    if (auto pTab = getTab(mapID))
        return;

    auto pEditor = new MapEditor(mapID, m_CacheMgr, m_MappingObject, m_DBMgr, this);
    try
    {
        m_pMapTabs->addTab(pEditor, m_DBMgr.getMapDatabase()->getOriginalPrototype(mapID)->getName());
        m_pMapTabs->setCurrentWidget(pEditor);
        connect(this, SIGNAL(save()), pEditor, SLOT(_onSave()));
        connect(this, SIGNAL(changeMappingMode(MAPPING_MODE::Type)), pEditor, SLOT(onMappingModeChanged(MAPPING_MODE::Type)));
        connect(pEditor, SIGNAL(actionMousePress(MapEditor&, QMouseEvent*)), &m_MappingObject,
            SLOT(press(MapEditor&, QMouseEvent*)));
        connect(pEditor, SIGNAL(actionMouseRelease(MapEditor&, QMouseEvent*)), &m_MappingObject,
            SLOT(release(MapEditor&, QMouseEvent*)));
        connect(pEditor, SIGNAL(actionMouseMove(MapEditor&, QMouseEvent*)), &m_MappingObject, SLOT(move(MapEditor&, QMouseEvent*)));
        connect(pEditor, SIGNAL(actionKeyPress(MapEditor&, const QPoint&, QKeyEvent*)),
            &m_MappingObject, SLOT(onActionKeyPressed(MapEditor&, const QPoint&, QKeyEvent*)));
        connect(pEditor, SIGNAL(actionKeyRelease(MapEditor&, const QPoint&, QKeyEvent*)),
            &m_MappingObject, SLOT(onActionKeyReleased(MapEditor&, const QPoint&, QKeyEvent*)));

        connect(pEditor, SIGNAL(actionCopy(const MapEditor&)), &m_MappingObject, SLOT(onActionCopy(const MapEditor&)));
        connect(pEditor, SIGNAL(actionCut(const MapEditor&)), &m_MappingObject, SLOT(onActionCut(const MapEditor&)));
        connect(pEditor, SIGNAL(actionDelete(MapEditor&)), &m_MappingObject, SLOT(onActionDelete(MapEditor&)));
        connect(pEditor, SIGNAL(actionPaste(MapEditor&, const QPoint&)), &m_MappingObject, SLOT(onActionPaste(MapEditor&, const QPoint&)));

        connect(pEditor, SIGNAL(changed(uint32)), this, SLOT(_onMapChanged(uint32)));
    }
    catch (const std::bad_alloc& e)
    {
        pEditor->deleteLater();
        QMessageBox::information(0, "Memory Error.", QString("It looks as if the map is too large.\n") +
            QString("The Error-Message is: ") + QString(e.what()), QMessageBox::Ok, QMessageBox::Ok);
    }
}

void MapEditorModuleContent::onMapClosed(uint32 mapID)
{
    if (auto pTab = getTab(mapID))
    {
        m_pMapTabs->removeTab(m_pMapTabs->indexOf(pTab));
        pTab->deleteLater();
    }
}

void MapEditorModuleContent::onMapEdited(uint32 mapID)
{
    if (auto pTab = getTab(mapID))
    {
        pTab->reloadMap();
        // get new maximum layer size
        _onLayerTypeChanged();
    }
}

void MapEditorModuleContent::onMappingModeChanged(MAPPING_MODE::Type mode)
{
    emit changeMappingMode(mode);
    m_pLayer->setEnabled(mode == MAPPING_MODE::Type::TILE_MAPPING);
    if (auto pTab = dynamic_cast<MapEditor*>(m_pMapTabs->currentWidget()))
        pTab->scene()->update();
}

MapEditor* MapEditorModuleContent::getTab(uint32 mapID)
{
    for (int32 i = 0; i < m_pMapTabs->count(); ++i)
    {
        if (auto pTab = dynamic_cast<MapEditor*>(m_pMapTabs->widget(i)))
        {
            if (pTab->getMapID() == mapID)
                return pTab;
        }
    }
    return nullptr;
}

void MapEditorModuleContent::_onMapChanged(uint32 mapID)
{
    if (auto pViewer = getTab(mapID))
    {
        m_pMapTabs->setTabText(m_pMapTabs->indexOf(pViewer), m_DBMgr.getMapDatabase()->getOriginalPrototype(pViewer->getMapID())->getName()
            + (pViewer->hasChanged() ? "*" : ""));
        m_pRevert->setEnabled(pViewer->hasChanged());
    }
}

void MapEditorModuleContent::saveMaps()
{
    emit save();
}
