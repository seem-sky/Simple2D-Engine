#include "TexturePrototypeFrame.h"
#include <QtGui/QPainter>
#include "Config.h"
#include "QtGlobal.h"

TexturePrototypeFrame::TexturePrototypeFrame(QWidget *pParent) : QFrame(pParent)
{}

void TexturePrototypeFrame::paintEvent(QPaintEvent *pEvent)
{
    QFrame::paintEvent(pEvent);

    if (auto pProtoype = getTexturePrototype())
    {
        QPainter painter(this);
        QPixmap pixmap;
        createPixmapFromTexturePrototype(Config::get()->getProjectDirectory(), pProtoype, pixmap);
        painter.drawPixmap(0, 0, pixmap);
    }
}
