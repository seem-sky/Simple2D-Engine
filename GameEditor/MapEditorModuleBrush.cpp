#include "MapEditorModuleBrush.h"
#include <QtGui/QPainter>
#include <QtGui/QTextOption>
#include <Core/Cache/Manager.h>
#include "moc_MapEditorModuleBrush.h"

using namespace BRUSH;
using namespace MAP::BRUSH;
using namespace DATABASE::PROTOTYPE;

MapEditorModuleBrush::MapEditorModuleBrush(CACHE::Manager& cacheMgr, const DATABASE::DatabaseMgr& DBMgr, QWidget* pParent)
    : QWidget(), Ui_MapEditorModuleBrush(), m_DBMgr(DBMgr), m_CacheMgr(cacheMgr)
{
    setupUi(this);
    _update();

    connect(m_pBrushType, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(_onBrushTypeChanged(const QString&)));
}

void MapEditorModuleBrush::clear()
{
    m_BrushInfo = MAP::BRUSH::BrushInfo();
    _update();
    emit changeBrush(this);
}

void MapEditorModuleBrush::setText(const QString& text)
{
    m_pText->setText(text);
}

void MapEditorModuleBrush::_update()
{
    auto labelSize = m_pCurrentTile->size();
    QPixmap newPixmap(labelSize);
    newPixmap.fill(Qt::transparent);
    QPainter painter(&newPixmap);

    QPixmap pixmap(MAP::TILE_SIZE, MAP::TILE_SIZE);
    pixmap.fill(Qt::transparent);
    QPainter pixPainter(&pixmap);
    QString text;
    switch (m_BrushInfo.getType())
    {
    case MAP::BRUSH::BrushInfo::Type::TILE:
    {
        text = "tile";
        auto info = m_CacheMgr.getTileCache().get(m_BrushInfo.getID());
        if (info.isValid())
            pixPainter.drawPixmap(0, 0, *info.getPixmap(), info.getPosition().getX(), info.getPosition().getY(), MAP::TILE_SIZE, MAP::TILE_SIZE);
        break;
    }
    case MAP::BRUSH::BrushInfo::Type::AUTO_TILE:
    {
        text = "auto tile";
        auto info = m_CacheMgr.getAutoTileCache().get(m_BrushInfo.getID(), AUTO_TILE::INDEX_INNER_CENTER);
        if (info.isValid())
            pixPainter.drawPixmap(0, 0, *info.getPixmap(), info.getPosition().getX(), info.getPosition().getY(), MAP::TILE_SIZE, MAP::TILE_SIZE);
        break;
    }
    case MAP::BRUSH::BrushInfo::Type::TILE_SET:
        text = "tile set";
        if (auto pTileSet = m_DBMgr.getTileSetDatabase()->getOriginalPrototype(m_BrushInfo.getID()))
            pixmap = TILE_SET::createPixmap(*pTileSet, m_CacheMgr.getTileCache());
        break;
    }

    // draw pixmap
    if (!pixmap.isNull())
        painter.drawPixmap((labelSize.width() - pixmap.size().width()) / 2, (labelSize.height() - pixmap.size().height()) / 2,
            pixmap.size().width(), pixmap.size().height(), pixmap);

    // draw text
    QTextOption textOption(Qt::AlignHCenter | Qt::AlignVCenter);
    textOption.setWrapMode(QTextOption::WordWrap);
    auto font = painter.font();
    font.setPointSize(16);
    painter.setFont(font);
    painter.setPen(QPen(QColor(Qt::blue)));
    painter.setOpacity(0.7);
    painter.drawText(newPixmap.rect(), text, textOption);

    m_pCurrentTile->setPixmap(newPixmap);
}

void MapEditorModuleBrush::setBrushInfo(const BrushInfo& brushInfo)
{
    m_BrushInfo = brushInfo;
    _update();
    emit changeBrush(this);
}

void MapEditorModuleBrush::_onBrushTypeChanged(const QString& text)
{
    if (text == "fill")
        m_BrushInfo.setMode(MAP::BRUSH::BrushInfo::Mode::FILL);
    else
        m_BrushInfo.setMode(MAP::BRUSH::BrushInfo::Mode::PEN);
    emit changeBrush(this);
}
