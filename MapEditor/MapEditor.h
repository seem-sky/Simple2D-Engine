#ifndef MAP_EDITOR_H
#define MAP_EDITOR_H

#include "ui/UI_MapEditor.h"
#include "ModifyObject.h"
#include "MapBrush.h"

class MapEditor;

class AutoTilePixmaps
{
private:
    inline void _setPixmap(DATABASE::AutoTilePrototype::AUTO_TILE_INDEX index, QPixmapPtr pixmap)
    {
        if (index < DATABASE::AutoTilePrototype::INDEX_NONE)
            m_pPixmaps[index] = pixmap;
    }

    void _createPixmaps();
    void _clearPixmaps()
    {
        for (uint32 i = 0; i < DATABASE::AutoTilePrototype::INDEX_NONE; ++i)
            m_pPixmaps[i] = QPixmapPtr();
    }

public:
    AutoTilePixmaps() : m_pEditor(NULL) {}

    inline bool getPixmap(DATABASE::AutoTilePrototype::AUTO_TILE_INDEX index, QPixmapPtr &result)
    {
        if (index >= DATABASE::AutoTilePrototype::INDEX_NONE || !m_pPixmaps[index])
            return false;
        result = m_pPixmaps[index];
        return true;
    }

    inline void setPrototype(DATABASE::ConstAutoTilePrototypePtr proto) { m_pProto = proto; _createPixmaps(); }
    inline void setMapEditorWidget(MapEditor *pEditor) { m_pEditor = pEditor; }

private:
    QPixmapPtr m_pPixmaps[DATABASE::AutoTilePrototype::INDEX_NONE];
    DATABASE::ConstAutoTilePrototypePtr m_pProto;
    MapEditor *m_pEditor;
};
typedef boost::shared_ptr<AutoTilePixmaps> AutoTilePixmapsPtr;
typedef std::vector<AutoTilePixmapsPtr> AutoTilePixmapsPtrVector;

class MapEditor : public QWidget, Ui_MapEditor
{
    Q_OBJECT
private:
    MAP::MapBrushPtr& _updateNewPoint(const MAP::MapBrushPtr &pOldBrush, MAP::MapBrushPtr &pNewBrush);

protected:
    void resizeEvent(QResizeEvent *pEvent) { m_ModifyObj.resizeEvent(this); }

public:
    MapEditor(QWidget *pParent = NULL);

    bool getTilePixmap(uint32 uiTileID, QPixmapPtr &result) const;
    bool createAutoTilePixmaps(uint32 uiID, AutoTilePixmapsPtr &result);
    bool getAutoTilePixmaps(uint32 uiID, AutoTilePixmapsPtr &result) const;
    void clearWidgets();
    
    inline DATABASE::DatabaseMgrPtr getDBMgr() const { return m_pDBMgr; }
    void setDBMgr(const DATABASE::DatabaseMgrPtr &pDBMgr);

    void updateTiles();

private slots:
    void _currentTileChanged(uint32 uiSpriteIndex, uint32 uiButton, bool autoTile);
    void _updateTiles();
    void _saveMaps();
    void _loadOpenMapsFromConfig();

    void _pressBrush(MapViewWidget *pWidget, Point3D<uint32> point, uint32 uiButton);
    void _releaseBrush(MapViewWidget *pWidget, Point3D<uint32> point, uint32 uiButton);
    void _moveBrush(MapViewWidget *pWidget, Point3D<uint32> point);
    void _setLeftPen(bool checked);
    void _setLeftRect(bool checked);
    void _setLeftFill(bool checked);
    void _setRightPen(bool checked);
    void _setRightRect(bool checked);
    void _setRightFill(bool checked);

private:
    AutoTilePixmapsPtrVector m_AutoTilePixmaps;
    QPixmapPtrVector m_TileCache;
    ModifyObject m_ModifyObj;
    MAP::MapBrushPtr m_pLHBrush;
    MAP::MapBrushPtr m_pRHBrush;
    DATABASE::DatabaseMgrPtr m_pDBMgr;
};
#endif