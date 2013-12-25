#include "MapEditorModuleBrush.h"
#include "AutoTileCache.h"
#include <QtGui/QPainter>
#include <QtGui/QTextOption>

using namespace BRUSH;

MapEditorModuleBrush::MapEditorModuleBrush(const DATABASE::DatabaseMgr &DBMgr, QWidget *pParent) : QWidget(), Ui_MapEditorModuleBrush(), m_DBMgr(DBMgr)
{
    setupUi(this);
    _update();
}

void MapEditorModuleBrush::setText(const QString &text)
{
    m_pText->setText(text);
}

void MapEditorModuleBrush::_update()
{
    auto labelSize = m_pCurrentTile->size();
    QPixmap newPixmap(labelSize);
    newPixmap.fill(Qt::transparent);
    QPainter painter(&newPixmap);

    const QPixmap *pPixmap = nullptr;
    QString text;
    switch (m_BrushInfo.m_SelectionType)
    {
    case MAP::BRUSH::SelectionType::TILES:
        text = "tile";
        pPixmap = GTileCache::get()->getItem(m_BrushInfo.m_ID);
        break;
    case MAP::BRUSH::SelectionType::AUTO_TILES:
        text = "auto tile";
        if (auto pAutoTile = GAutoTileCache::get()->getItem(m_BrushInfo.m_ID))
            pPixmap = pAutoTile->getPixmap(DATABASE::AUTO_TILE::INDEX_INNER_CENTER);
        break;
    case MAP::BRUSH::SelectionType::TILE_SETS:
        text = "tile set";
        //pPixmap = GTileCache::get()->getItem(ID);
        break;
    }

    // draw pixmap
    if (pPixmap)
        painter.drawPixmap((labelSize.width() - pPixmap->size().width()) / 2, (labelSize.height() - pPixmap->size().height()) / 2,
        pPixmap->size().width(), pPixmap->size().height(), *pPixmap);

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

void MapEditorModuleBrush::setBrushInfo(const MAP::BRUSH::BrushInfo& brushInfo)
{
    m_BrushInfo = brushInfo;
    _update();
}
