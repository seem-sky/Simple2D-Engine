#ifndef QT_GLOBAL_H
#define QT_GLOBAL_H

#include <QtGui/QPixmap>
#include <QtGui/QBitmap>
#include <QtCore/QString>
#include <QtGui/QPixmapCache>
#include "DatabasePrototypes.h"
#include "Container.h"

static bool createPixmap(const QString &path, const QString &fileNamePath, const Color &color, QPixmap &result)
{
    QPixmap pixmap(path + "/Textures/" + fileNamePath);
    if (!pixmap.isNull())
    {
        // set transparency color
        if (color.hasValidColor())
            pixmap.setMask(pixmap.createMaskFromColor(QColor(color.getRed(), color.getGreen(), color.getBlue())));
        result = pixmap;
        return true;
    }
    return false;
}

static bool createPixmapFromTexturePrototype(const QString &path, const DATABASE::TexturePrototype *pTexture, QPixmap &result)
{
    if (pTexture)
    {
        // use pixmap cache
        QString pixmapKey = pTexture->getPathName() + QString::fromStdString(pTexture->getTransparencyColor().getColorString());
        if (QPixmapCache::find(pixmapKey, &result))
            return true;
        else if (createPixmap(path, pTexture->getPathName(), pTexture->getTransparencyColor(), result))
        {
            QPixmapCache::insert(pixmapKey, result);
            return true;
        }
    }
    return false;
}

#endif