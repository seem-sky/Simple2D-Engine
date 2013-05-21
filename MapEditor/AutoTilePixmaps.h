#ifndef AUTO_TILE_PIXMAPS_H
#define AUTO_TILE_PIXMAPS_H

#include "MapBrush.h"
#include "QtTypedefs.h"

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

#endif