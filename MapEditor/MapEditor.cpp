#include "MapEditor.h"
#include "DatabaseOutput.h"
#include "Config.h"
#include <QtGui/QBitmap>
#include "moc_MapEditor.h"

using namespace DATABASE;
using namespace MAP;

MapEditor::MapEditor(QWidget *pParent) : QWidget(pParent), Ui_MapEditor()
{
    setupUi(this);
    m_ModifyObj.setWidget(m_pTileTab, MODIFY_RESIZE, QPoint(0, m_pMapTree->height() + 35), MODIFY_DIRECTION_HEIGHT);
    m_ModifyObj.setWidget(m_pMapEditor, MODIFY_RESIZE);
    m_ModifyObj.setWidget(m_pMapTree, MODIFY_MOVE, QPoint(0, 0), MODIFY_DIRECTION_HEIGHT);
    m_ModifyObj.setWidget(m_pMapTreeLabel, MODIFY_MOVE, QPoint(0, m_pMapTree->height()), MODIFY_DIRECTION_HEIGHT);

    connect(m_pMapTree, SIGNAL(mapUpdated(const MAP::MapPrototypePtr&)), m_pMapEditor, SLOT(_updateMap(const MAP::MapPrototypePtr&)));
    connect(m_pMapTree, SIGNAL(mapDeleted(const MAP::MapPrototypePtr&)), m_pMapEditor, SLOT(_closeMap(const MAP::MapPrototypePtr&)));
    connect(m_pMapTree, SIGNAL(mapOpened(const MAP::MapPrototypePtr&)), m_pMapEditor, SLOT(_addMapTab(const MAP::MapPrototypePtr&)));

    connect(m_pTiles, SIGNAL(changeTile(uint32, uint32)), this, SLOT(_currentTileChanged(uint32, uint32)));
    connect(window(), SIGNAL(projectLoadDone()), this, SLOT(_loadOpenMapsFromConfig()));

    connect(m_pLeftBrushPen, SIGNAL(toggled(bool)), this, SLOT(_setLeftPen(bool)));
    connect(m_pLeftBrushRect, SIGNAL(toggled(bool)), this, SLOT(_setLeftRect(bool)));
    connect(m_pLeftBrushFill, SIGNAL(toggled(bool)), this, SLOT(_setLeftFill(bool)));
    connect(m_pRightBrushPen, SIGNAL(toggled(bool)), this, SLOT(_setRightPen(bool)));
    connect(m_pRightBrushRect, SIGNAL(toggled(bool)), this, SLOT(_setRightRect(bool)));
    connect(m_pRightBrushFill, SIGNAL(toggled(bool)), this, SLOT(_setRightFill(bool)));
}

void MapEditor::_loadOpenMapsFromConfig()
{
    const UInt32UnorderedSet openMaps = Config::Get()->getOpenMaps();
    for (UInt32UnorderedSet::const_iterator itr = openMaps.begin(); itr != openMaps.end(); ++itr)
    {
        MapPrototypePtr map;
        if (MapDatabase::Get()->getMap(*itr, map))
            m_pMapEditor->_addMapTab(map);
    }
}

void MapEditor::clearWidgets()
{
    for (int i = m_pMapEditor->count()-1; i >= 0; --i)
        m_pMapEditor->_closeTab(i);

    m_pTileTab->clearTiles();
    m_pMapTree->clear();
    m_pLeftCurrentTile->clear();
    m_pRightCurrentTile->clear();
}

bool MapEditor::getTilePixmap(uint32 uiTileID, QPixmapPtr &result)
{
    if (uiTileID < m_TileCache.size())
    {
        result = m_TileCache.at(uiTileID);
        return true;
    }
    return false;
}

void MapEditor::_setLeftPen(bool checked)
{
    if (checked)
        m_Brush.setBrushType(MapBrush::BRUSH_PEN, MapBrush::MOUSE_LEFT);
}

void MapEditor::_setLeftRect(bool checked)
{
    if (checked)
        m_Brush.setBrushType(MapBrush::BRUSH_RECT, MapBrush::MOUSE_LEFT);
}

void MapEditor::_setLeftFill(bool checked)
{
    if (checked)
        m_Brush.setBrushType(MapBrush::BRUSH_FILL, MapBrush::MOUSE_LEFT);
}

void MapEditor::_setRightPen(bool checked)
{
    if (checked)
        m_Brush.setBrushType(MapBrush::BRUSH_PEN, MapBrush::MOUSE_RIGHT);
}

void MapEditor::_setRightRect(bool checked)
{
    if (checked)
        m_Brush.setBrushType(MapBrush::BRUSH_RECT, MapBrush::MOUSE_RIGHT);
}

void MapEditor::_setRightFill(bool checked)
{
    if (checked)
        m_Brush.setBrushType(MapBrush::BRUSH_FILL, MapBrush::MOUSE_RIGHT);
}

void MapEditor::_updateTiles()
{
    m_pTileTab->fillTiles(m_TileCache);
}

void MapEditor::_currentTileChanged(uint32 uiSpriteIndex, uint32 uiButton)
{
    if (uiSpriteIndex == m_Brush.getTile((MapBrush::MouseKey)uiButton) || uiSpriteIndex >= m_TileCache.size() || !m_TileCache.at(uiSpriteIndex))
        return;
    m_Brush.setTile(uiSpriteIndex, (MapBrush::MouseKey)uiButton);
    switch (uiButton)
    {
        case Qt::LeftButton: m_pLeftCurrentTile->setPixmap(*m_TileCache.at(uiSpriteIndex).get()); break;
        case Qt::RightButton: m_pRightCurrentTile->setPixmap(*m_TileCache.at(uiSpriteIndex).get()); break;
    }
}

void MapEditor::_pressBrush(MapViewWidget *pWidget, Point3D<uint32> point, uint32 uiButton)
{
    if (!pWidget)
        return;

    m_Brush.brushPress(pWidget, point, (MapBrush::MouseKey)uiButton);
}

void MapEditor::_releaseBrush(MapViewWidget *pWidget, Point3D<uint32> point, uint32 uiButton)
{
    if (!pWidget)
        return;

    m_Brush.brushRelease(pWidget, point, (MapBrush::MouseKey)uiButton);
}

void MapEditor::_moveBrush(MapViewWidget *pWidget, Point3D<uint32> point)
{
    if (!pWidget)
        return;

    m_Brush.brushMove(pWidget, point);
}

void MapEditor::_saveMaps()
{
    for (int i = 0; i < m_pMapEditor->count(); ++i)
    {
        if (MapViewWidget *pWidget = ((MapViewWidget*)m_pMapEditor->widget(i)))
        {
            if (pWidget->hasChanged())
                pWidget->saveCurrentMap();
        }
    }
}
