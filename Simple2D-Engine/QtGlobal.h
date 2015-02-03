#ifndef QT_GLOBAL_H
#define QT_GLOBAL_H

#include <QtGui/QPixmap>
#include <QtCore/QString>
#include <QtWidgets/QGraphicsItem>
#include <QtWidgets/QStyleOptionGraphicsItem>
#include <QtGui/QPainter>
#include <QtCore/QDebug>

namespace DATABASE
{
    namespace PROTOTYPE
    {
        class TexturePrototype;
    }
}

class Color;

bool createPixmap(const QString& path, const QString& fileNamePath, const Color& color, QPixmap& result);
bool createPixmapFromTexturePrototype(const QString& path, const DATABASE::PROTOTYPE::TexturePrototype* pTexture, QPixmap& result);

void highlightSelection(const QGraphicsItem& item, QPainter* pPainter, const QStyleOptionGraphicsItem* pOption);

#endif
