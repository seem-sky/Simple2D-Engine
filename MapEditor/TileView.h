#ifndef TILE_VIEW_H
#define TILE_VIEW_H

#include <QtGui/QGraphicsView>
#include <map>
#include "Global.h"
#include <boost/smart_ptr.hpp>
#include "DatabaseMgr.h"
#include "QtTypedefs.h"

class TileTabView : public QGraphicsView
{
    Q_OBJECT
public:
    TileTabView(QWidget* pParent = NULL);

    void updateTiles(const QPixmapPtrMap &pixmaps, const QSize &tileSize, const QSize &pixmapSize);
    int getTile(QPoint at) const;
    Point<uint32> getTileCount() const;
    inline QSize getTileSize() const { return m_TileSize; }

signals:
    void changeTile(uint32 uiID, uint32 uiButton, bool autoTile);
    void changeTile(const QPoint &pos, const QSize &tileSize, uint32 uiButton);

private:
    UInt32Vector2D m_uiIDVector;
    QSize m_TileSize;
};

class TileView : public TileTabView
{
protected:
    void mousePressEvent(QMouseEvent *pEvent);

public:
    TileView(QWidget* pParent = NULL) : TileTabView(pParent), m_isAutoTileView(false) {}
    inline bool isAutoTileView() const { return m_isAutoTileView; }
    inline void setAutoTileView(bool isAutoTile = true) { m_isAutoTileView = isAutoTile; }

private:
    bool m_isAutoTileView;
    
};

class TileViewScene : public QGraphicsScene
{
protected:
    void drawForeground(QPainter *painter, const QRectF &rect);

public:
    TileViewScene(TileTabView *pParent) : QGraphicsScene(pParent) {}
};
#endif