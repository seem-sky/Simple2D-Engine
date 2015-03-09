#include "MapEditorModuleBrush.h"
#include "moc_MapEditorModuleBrush.h"
#include <QtGui/QPainter>
#include <QtGui/QTextOption>
#include <Core/Cache/Manager.h>
#include <QtWidgets/QGraphicsItem>
#include "EditorGlobal.h"

using namespace BRUSH;
using namespace MAP::BRUSH;

MapEditorModuleBrush::MapEditorModuleBrush(CACHE::Manager& cacheMgr, const database::Manager& DBMgr, QWidget* pParent)
    : QWidget(), Ui_MapEditorModuleBrush(), m_DBMgr(DBMgr), m_CacheMgr(cacheMgr)
{
    setupUi(this);
    m_pCurrentTile->setScene(new QGraphicsScene(m_pCurrentTile));
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
    m_pCurrentTile->scene()->clear();

    // draw text
    QTextOption textOption(Qt::AlignHCenter | Qt::AlignVCenter);
    textOption.setWrapMode(QTextOption::WordWrap);
    QFont font;
    font.setPointSize(16);

    switch (m_BrushInfo.getType())
    {
    case MAP::BRUSH::BrushInfo::Type::TILE:
    {
        auto info = m_CacheMgr.getTileCache().get(m_BrushInfo.getID());
        if (info.isValid())
        {
            QPixmap pixmap(MAP::TILE_SIZE, MAP::TILE_SIZE);
            pixmap.fill(Qt::transparent);
            QPainter pixPainter(&pixmap);
            pixPainter.drawPixmap(0, 0, *info.getPixmap(), info.getPosition().getX(), info.getPosition().getY(), MAP::TILE_SIZE, MAP::TILE_SIZE);
            m_pCurrentTile->scene()->addPixmap(pixmap);
        }
        m_pCurrentTile->scene()->addText("tile", font);
        break;
    }
    case MAP::BRUSH::BrushInfo::Type::AUTO_TILE:
    {
        auto info = m_CacheMgr.getAutoTileCache().get(m_BrushInfo.getID(), database::prototype::AutoTile::Index::InnerCenter);
        if (info.isValid())
        {
            QPixmap pixmap(MAP::TILE_SIZE, MAP::TILE_SIZE);
            pixmap.fill(Qt::transparent);
            QPainter pixPainter(&pixmap);
            pixPainter.drawPixmap(0, 0, *info.getPixmap(), info.getPosition().getX(), info.getPosition().getY(), MAP::TILE_SIZE, MAP::TILE_SIZE);
            m_pCurrentTile->scene()->addPixmap(pixmap);
        }
        m_pCurrentTile->scene()->addText("auto tile", font);
        break;
    }
    case MAP::BRUSH::BrushInfo::Type::TILE_SET:
        if (auto pTileSet = m_DBMgr.getTileSetDatabase().getPrototype(m_BrushInfo.getID()))
        {
            auto pixmap = database::prototype::createPixmap(pTileSet, m_CacheMgr.getTileCache());
            m_pCurrentTile->scene()->addPixmap(pixmap);
        }
        m_pCurrentTile->scene()->addText("tile set", font);
        break;
    }

    // center items
    for (auto pItem : m_pCurrentTile->scene()->items())
    {
        auto rect = pItem->boundingRect();
        auto center = rect.center();
        pItem->moveBy(-center.x(), -center.y());
    }
    auto rect = m_pCurrentTile->scene()->itemsBoundingRect();
    m_pCurrentTile->centerOn(rect.center());
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
