#ifndef TILE_VIEW_H
#define TILE_VIEW_H

#include <QtGui/QGraphicsView>
#include <map>
#include "Global.h"
#include "Database.h"
#include <boost/smart_ptr.hpp>

typedef boost::shared_ptr<QPixmap> QPixmapPtr;
typedef std::vector<QPixmapPtr> QPixmapPtrVector;
typedef std::map<uint32, QPixmapPtr> QPixmapPtrMap;

class TileView : public QGraphicsView
{
    Q_OBJECT
protected:
    void mousePressEvent(QMouseEvent *pEvent);

public:
    TileView(QWidget* pParent = NULL);

    void updateTiles(const QPixmapPtrMap &pixmaps, const QSize &tileSize, const QSize &pixmapSize);
    int getTile(QPoint at) const;

signals:
    void changeTile(uint32 uiID, uint32 uiButton);
    void changeTile(const QPoint &pos, const QSize &tileSize, uint32 uiButton);

private:
    UInt32Vector2D m_uiIDVector;
    QSize m_TileSize;
};
#endif