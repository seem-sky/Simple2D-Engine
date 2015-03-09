#include "TexturePrototypeFrame.h"
#include <QtGui/QPainter>
#include "Config.h"
#include "QtGlobal.h"
#include <Database/Prototype/Derived.h>

using namespace database::prototype;

TexturePrototypeFrame::TexturePrototypeFrame(QWidget* pParent)
    : QFrame(pParent), m_pPrototype(nullptr)
{}

void TexturePrototypeFrame::paintEvent(QPaintEvent* pEvent)
{
    QFrame::paintEvent(pEvent);

    QPixmap pixmap;
    if (createPixmapFromTexturePrototype(Config::get()->getProjectDirectory(), m_pPrototype, pixmap))
    {
        QPainter painter(this);
        painter.drawPixmap(0, 0, pixmap);
    }
}

void TexturePrototypeFrame::setPrototype(const Texture* pPrototype)
{
    m_pPrototype = pPrototype;
    repaint();
}

uint32 TexturePrototypeFrame::getID() const
{
    if (m_pPrototype)
        return m_pPrototype->getID();
    return 0;
}
