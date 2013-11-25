#ifndef SHARED_MAP_EDITOR_DATA_H
#define SHARED_MAP_EDITOR_DATA_H

#include "DatabaseMgr.h"
#include "AutoTileCache.h"
//#include "MapDatabaseChanger.h"
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
    void _setDatabaseMgr(DATABASE::DatabaseMgr &DBMgr);
    void _updateData();

public:
    SharedMapEditorData();

    inline MappingMode getMappingMode() const { return m_MappingMode; }
    inline MAP::Layer getCurrentLayer() const { return m_CurrentLayer; }

    const DATABASE::TileDatabase* getTileDatabase() const;
    const DATABASE::TileSetDatabase* getTileSetDatabase() const;
    const DATABASE::AutoTileDatabase* getAutoTileDatabase() const;
    const DATABASE::SpriteDatabase* getSpriteDatabase() const;
    const DATABASE::AnimationDatabase* getAnimationDatabase() const;
    const DATABASE::WorldObjectDatabase* getWorldObjectDatabase() const;
    const DATABASE::DynamicObjectDatabase* getDynamicObjectDatabase() const;

    //DATABASE::MAP_STRUCTURE::MapDatabaseChangerPtr getMapDatabase();

    inline const TileCache& getTileCache() const { return m_TileCache; }
    inline const AutoTileCache* getAutoTileCache() const { return m_pAutoTileCache; }

private:
    DATABASE::DatabaseMgr &m_DatabaseMgr;
    //DATABASE::MAP_STRUCTURE::MapDatabaseChangerPtr m_pMapDBChanger;
    TileCache m_TileCache;
    const AutoTileCache* m_pAutoTileCache;

    MappingMode m_MappingMode;
    MAP::Layer m_CurrentLayer;
};
typedef std::shared_ptr<SharedMapEditorData> SharedMapEditorDataPtr;
typedef std::shared_ptr<const SharedMapEditorData> ConstSharedMapEditorDataPtr;

#endif
