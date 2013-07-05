#ifndef QT_GLOBAL_H
#define QT_GLOBAL_H

#include <QtGui/QPixmap>
#include <QtGui/QBitmap>
#include <QtCore/QString>
#include "DatabasePrototypes.h"
#include "Container.h"

// QPixmap typedefs
typedef Container<QPixmap> QPixmapCotainer;
typedef std::shared_ptr<QPixmap> QPixmapPtr;
typedef std::shared_ptr<const QPixmap> ConstQPixmapPtr;

static bool createPixmapFromTexturePrototype(const QString &path, DATABASE::ConstTexturePrototypePtr proto, QPixmap &result)
{
    if (!proto)
        return false;

    QPixmap pixmap(path + "/Textures/" + proto->getPathName());
    if (!pixmap.isNull())
    {
        // set transparency color
        if (proto->getTransparencyColor().hasValidColor())
            pixmap.setMask(pixmap.createMaskFromColor(QColor(proto->getTransparencyColor().getRed(),
            proto->getTransparencyColor().getGreen(), proto->getTransparencyColor().getBlue())));
        result = pixmap;
        return true;
    }
    return false;
}

#endif