#ifndef TILE_TAB_H
#define TILE_TAB_H

#include <QtGui/QTabWidget>
#include "ModifyObject.h"
#include "TileView.h"
#include "MapEditorObject.h"

class TileTab : public QTabWidget, public MapEditorObject
{
    Q_OBJECT
protected:
    void tabInserted(int index);
    void tabRemoved(int index);

public:
    TileTab(QWidget* pParent = NULL);

    void fillTiles(QPixmapCotainer &pixmapCache);
    void clearTiles();
    inline void setTileDB(DATABASE::ConstTileDatabasePtr pTileDB) { m_pTileDB = pTileDB; }
    inline void setAutoTileDB(DATABASE::ConstAutoTileDatabasePtr pAutoTileDB) { m_pAutoTileDB = pAutoTileDB; }

private slots:
    void _changeCurrentTile(const QPoint &pos, const QSize &tileSize, uint32 uiButton);

private:
    ModifyObject m_ModifyObj;
    DATABASE::ConstTileDatabasePtr m_pTileDB;
    DATABASE::ConstAutoTileDatabasePtr m_pAutoTileDB;
};
#endif