#include "MapEditorModuleBrush.h"
#include "AutoTileCache.h"
#include <QtGui/QPainter>
#include <QtGui/QTextOption>
#include "moc_MapEditorModuleBrush.h"

using namespace BRUSH;
using namespace MAP::BRUSH;

MapEditorModuleBrush::MapEditorModuleBrush(const DATABASE::DatabaseMgr& DBMgr, QWidget* pParent) : QWidget(), Ui_MapEditorModuleBrush(), m_DBMgr(DBMgr)
{
    setupUi(this);
    _update();

    connect(m_pBrushType, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(_onBrushTypeChanged(const QString&)));
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

    QPixmap pixmap;
    QString text;
    switch (m_BrushInfo.m_SelectionType)
    {
    case MAP::BRUSH::SelectionType::TILES:
        text = "tile";
        if (auto pPixmap = GTileCache::get()->getItem(m_BrushInfo.m_ID))
            pixmap =* pPixmap;
        break;
    case MAP::BRUSH::SelectionType::AUTO_TILES:
        text = "auto tile";
        if (auto pAutoTile = GAutoTileCache::get()->getItem(m_BrushInfo.m_ID))
        {
            if (auto pPixmap = pAutoTile->getPixmap(DATABASE::AUTO_TILE::INDEX_INNER_CENTER))
                pixmap =* pPixmap;
        }
        break;
    case MAP::BRUSH::SelectionType::TILE_SETS:
        text = "tile set";
        if (auto pTileSet = m_DBMgr.getTileSetDatabase()->getOriginalPrototype(m_BrushInfo.m_ID))
            pixmap = DATABASE::TILE_SET::createPixmap(*pTileSet);
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
}

void MapEditorModuleBrush::_onBrushTypeChanged(const QString& text)
{
    if (text == "fill")
        m_BrushInfo.m_BrushType = BrushType::FILL;
    else
        m_BrushInfo.m_BrushType = BrushType::PEN;
}
