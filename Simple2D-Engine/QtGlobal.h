#pragma once

#include <QtGui/QPixmap>
#include <QtCore/QString>
#include <QtWidgets/QGraphicsItem>
#include <QtWidgets/QStyleOptionGraphicsItem>
#include <QtGui/QPainter>
#include <QtCore/QDebug>

namespace database
{
    namespace prototype
    {
        class Texture;
    }
}

class Color;

bool createPixmap(const QString& absolutePathName, const Color& color, QPixmap& result);
bool createPixmapFromTexturePrototype(const QString& path, const database::prototype::Texture* pTexture, QPixmap& result);

void highlightSelection(const QGraphicsItem& item, QPainter* pPainter, const QStyleOptionGraphicsItem* pOption);

/*#####
# DiagonalCross
#####*/
class DiagonalCross
{
public:
    void paint(QPainter* pPainter);
    QRectF boundingRect() const;

    const QPen& getPen() const;
    void setPen(const QPen& pen);

    void setCrossSize(const QSize& size);
    QSize getCrossSize() const;

private:
    QPen m_Pen;
    QSize m_CrossSize;
};
