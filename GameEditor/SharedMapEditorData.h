#ifndef SHARED_MAP_EDITOR_DATA_H
#define SHARED_MAP_EDITOR_DATA_H

#include "DatabaseMgr.h"
#include "AutoTileCache.h"
#include "MapDatabaseChanger.h"
#include "MapDatabase.h"

enum MappingMode
{
    MODE_TILE,
    MODE_OBJECT,
    MODE_VIEW
};

class MapEditorWidget;

class SharedMapEditorData
{
    friend MapEditorWidget;
private:
    void _setDatabaseMgr(DATABASE::DatabaseMgrPtr pDBMgr);
    void _updateData();

public:
    SharedMapEditorData();

    inline MappingMode getMappingMode() const { return m_MappingMode; }
    inline DATABASE::MAP::Layer getCurrentLayer() const { return m_CurrentLayer; }

    DATABASE::ConstTileDatabasePtr getTileDatabase() const;
    DATABASE::ConstTileSetDatabasePtr getTileSetDatabase() const;
    DATABASE::ConstAutoTileDatabasePtr getAutoTileDatabase() const;
    DATABASE::ConstSpriteDatabasePtr getSpriteDatabase() const;
    DATABASE::ConstAnimationDatabasePtr getAnimationDatabase() const;
    DATABASE::ConstWorldObjectDatabasePtr getWorldObjectDatabase() const;

    DATABASE::MAP::MapDatabaseChangerPtr getMapDatabase();

    inline ConstTileCachePtr getTileCache() const { return m_pTileCache; }
    inline ConstAutoTileCachePtr getAutoTileCache() const { return m_pAutoTileCache; }

private:
    DATABASE::DatabaseMgrPtr m_pDatabaseMgr;
    DATABASE::MAP::MapDatabaseChangerPtr m_pMapDBChanger;
    TileCachePtr m_pTileCache;
    AutoTileCachePtr m_pAutoTileCache;

    MappingMode m_MappingMode;
    DATABASE::MAP::Layer m_CurrentLayer;
};
typedef std::shared_ptr<SharedMapEditorData> SharedMapEditorDataPtr;
typedef std::shared_ptr<const SharedMapEditorData> ConstSharedMapEditorDataPtr;

#endif