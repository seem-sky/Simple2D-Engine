#include "QtGlobal.h"
#include "Database/Prototype/Derived.h"
#include "Container.h"

#include <QtGui/QBitmap>
#include <QtGui/QPixmapCache>

bool createPixmap(const QString& absolutePathName, const Color& color, QPixmap& result)
{
    result = QPixmap(absolutePathName);
    if (!result.isNull())
    {
        // set transparency color
        if (!color.isEmpty())
            result.setMask(result.createMaskFromColor(QColor(color.getRed(), color.getGreen(), color.getBlue())));
        return true;
    }
    return false;
}

bool createPixmapFromTexturePrototype(const QString& path, const database::prototype::Texture* pTexture, QPixmap& result)
{
    if (pTexture)
    {
        // ToDo:
        //// use pixmap cache
        //QString pixmapKey = pTexture->getTextureString();
        //if (QPixmapCache::find(pixmapKey, result))
        //    return true;
        //else if (createPixmap(path, pTexture->getPathName(), pTexture->getTransparencyColor(), result))
        //{
        //    QPixmapCache::insert(pixmapKey, result);
        //    return true;
        //}
    }
    return false;
}

void highlightSelection(const QGraphicsItem& item, QPainter* pPainter, const QStyleOptionGraphicsItem* pOption)
{
    // from QGraphicsItem.cpp
    const QRectF murect = pPainter->transform().mapRect(QRectF(0, 0, 1, 1));
    if (qFuzzyCompare(qMax(murect.width(), murect.height()) + 1, 1))
        return;

    const QRectF mbrect = pPainter->transform().mapRect(item.boundingRect());
    if (qMin(mbrect.width(), mbrect.height()) < qreal(1.0))
        return;

    qreal itemPenWidth = 1.0;
    const qreal pad = itemPenWidth / 2;

    const qreal penWidth = 0; // cosmetic pen

    const QColor fgcolor = pOption->palette.windowText().color();
    const QColor bgcolor( // ensure good contrast against fgcolor
        fgcolor.red() > 127 ? 0 : 255,
        fgcolor.green() > 127 ? 0 : 255,
        fgcolor.blue() > 127 ? 0 : 255);

    pPainter->setPen(QPen(bgcolor, penWidth, Qt::SolidLine));
    pPainter->setBrush(Qt::NoBrush);
    pPainter->drawRect(item.boundingRect().adjusted(pad, pad, -pad, -pad));

    pPainter->setPen(QPen(pOption->palette.windowText(), 0, Qt::DashLine));
    pPainter->setBrush(Qt::NoBrush);
    pPainter->drawRect(item.boundingRect().adjusted(pad, pad, -pad, -pad));
}

/*#####
# DiagonalCross
#####*/
void DiagonalCross::paint(QPainter* pPainter)
{
    pPainter->drawLine(-(m_CrossSize.width() / 2), -(m_CrossSize.height() / 2), m_CrossSize.width() / 2, m_CrossSize.height() / 2);
    pPainter->drawLine(m_CrossSize.width() / 2, -(m_CrossSize.height() / 2), -(m_CrossSize.width() / 2), m_CrossSize.height() / 2);
}

QRectF DiagonalCross::boundingRect() const
{
    return QRectF(-m_CrossSize.width() / 2 - qreal(m_Pen.width()), -m_CrossSize.height() / 2 - qreal(m_Pen.width()),
        m_CrossSize.width() + m_Pen.width() * 2, m_CrossSize.height() + m_Pen.width() * 2);
}

void DiagonalCross::setCrossSize(const QSize& size)
{
    m_CrossSize = size;
}

QSize DiagonalCross::getCrossSize() const
{
    return m_CrossSize;
}

void DiagonalCross::setPen(const QPen& pen)
{
    m_Pen = pen;
}

const QPen& DiagonalCross::getPen() const
{
    return m_Pen;
}
