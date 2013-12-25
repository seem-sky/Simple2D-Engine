#ifndef QT_GLOBAL_H
#define QT_GLOBAL_H

#include <QtGui/QPixmap>
#include <QtGui/QBitmap>
#include <QtCore/QString>
#include <QtGui/QPixmapCache>
#include "DatabasePrototypes.h"
#include "Container.h"
#include <QtCore/QDebug>

static bool createPixmap(const QString& path, const QString& fileNamePath, const Color& color, QPixmap& result)
{
    result = QPixmap(path + "/Textures/" + fileNamePath);
    if (!result.isNull())
    {
        // set transparency color
        if (color.hasValidColor())
            result.setMask(result.createMaskFromColor(QColor(color.getRed(), color.getGreen(), color.getBlue())));
        return true;
    }
    return false;
}

static bool createPixmapFromTexturePrototype(const QString& path, const DATABASE::TexturePrototype* pTexture, QPixmap& result)
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

#endif
