#ifndef TILE_TAB_H
#define TILE_TAB_H

#include <QtGui/QTabWidget>
#include "ModifyObject.h"
#include "TileView.h"

class TileTab : public QTabWidget
{
    Q_OBJECT
protected:
    void tabInserted(int index);
    void tabRemoved(int index);

public:
    TileTab(QWidget* pParent = NULL);

    void fillTiles(QPixmapPtrVector &pixmapCache);
    void clearTiles();

private slots:
    void _changeCurrentTile(const QPoint &pos, const QSize &tileSize, uint32 uiButton);

private:
    ModifyObject m_ModifyObj;
};
#endif