#ifndef QT_GLOBAL_H
#define QT_GLOBAL_H

#include <QtWidgets/QTreeWidgetItem>
#include <QtGui/QPixmap>
#include <QtGui/QBitmap>
#include "DatabasePrototypes.h"
#include "Config.h"
#include "Container.h"

// QPixmap typedefs
typedef Container<QPixmap> QPixmapCotainer;
typedef std::shared_ptr<QPixmap> QPixmapPtr;
typedef std::shared_ptr<const QPixmap> ConstQPixmapPtr;

class PrototypeTreeWidgetItem : public QTreeWidgetItem
{
public:
    PrototypeTreeWidgetItem(const QStringList &strings) : QTreeWidgetItem(strings)
    {}

    bool operator <(const QTreeWidgetItem &other) const
    {
        switch (treeWidget()->sortColumn())
        {
        case 0: return text(0).toUInt() < other.text(0).toUInt();
        default: return QTreeWidgetItem::operator <(other);
        }
    }
};

static bool createPixmapFromTexturePrototype(DATABASE::ConstTexturePrototypePtr proto, QPixmap &result)
{
    if (!proto)
        return false;

    QPixmap pixmap(Config::Get()->getProjectDirectory() + "/Textures/" + proto->getPathName());
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