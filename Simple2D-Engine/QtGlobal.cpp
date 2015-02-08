#include "QtGlobal.h"
#include "DatabasePrototypes.h"
#include "Container.h"

#include <QtGui/QBitmap>
#include <QtGui/QPixmapCache>

bool createPixmap(const QString& path, const QString& fileNamePath, const Color& color, QPixmap& result)
{
    result = QPixmap(path + "/Textures/" + fileNamePath);
    if (!result.isNull())
    {
        // set transparency color
        if (color.isValid())
            result.setMask(result.createMaskFromColor(QColor(color.getRed(), color.getGreen(), color.getBlue())));
        return true;
    }
    return false;
}

bool createPixmapFromTexturePrototype(const QString& path, const DATABASE::PROTOTYPE::TexturePrototype* pTexture, QPixmap& result)
{
    if (pTexture)
    {
        // use pixmap cache
        QString pixmapKey = pTexture->getTextureString();
        if (QPixmapCache::find(pixmapKey, result))
            return true;
        else if (createPixmap(path, pTexture->getPathName(), pTexture->getTransparencyColor(), result))
        {
            QPixmapCache::insert(pixmapKey, result);
            return true;
        }
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
}
