#include "MapEditorModuleTileSets.h"
#include "QtGlobal.h"
#include "Config.h"
#include <QtGui/QMouseEvent>
#include <QtGui/QPainter>
#include "moc_MapEditorModuleTileSets.h"

MapEditorModuleTileSets::MapEditorModuleTileSets(QWidget *pParent) : DatabaseModuleDragList(pParent), m_pTileDB(nullptr)
{
    setDragEnabled(false);
    setToolTipPosition(TOOLTIP_RIGHT);
    setShowTooltip(true);
    setFrameShape(QFrame::NoFrame);
}

QWidget* MapEditorModuleTileSets::_setupTooltipWidget(uint32 uiPrototypeID)
{
    auto pLabel = new QLabel(this);
    pLabel->resize(TILE_SIZE, TILE_SIZE);
    if (auto pModel = dynamic_cast<IDatabaseModel*>(model()))
    {
        if (auto pPrototype = dynamic_cast<DATABASE::TILE_SET::TileSetPrototype*>(pModel->getDatabase()->getPrototype(uiPrototypeID)))
        {
            QPixmap pixmap(pPrototype->getTileCount().x*TILE_SIZE, pPrototype->getTileCount().y*TILE_SIZE);
            pixmap.fill();
            QPainter painter(&pixmap);
            if (m_pTileDB)
            {
                for (uint32 x = 0; x < pPrototype->getTileCount().x; ++x)
                {
                    for (uint32 y = 0; y < pPrototype->getTileCount().y; ++y)
                    {
                        auto pTilePrototype = m_pTileDB->getOriginalPrototype(pPrototype->getTileID(UInt32Point(x, y)));
                        QPixmap tilePixmap;
                        if (createPixmapFromTexturePrototype(Config::get()->getProjectDirectory(), pTilePrototype, tilePixmap))
                            painter.drawTiledPixmap(x*TILE_SIZE, y*TILE_SIZE, TILE_SIZE, TILE_SIZE, tilePixmap);
                    }
                }
            }
            pLabel->setPixmap(pixmap);
            pLabel->resize(pixmap.size());
        }
    }
    return pLabel;
}

void MapEditorModuleTileSets::setTileDatabase(const DATABASE::TileDatabase *pDB)
{
    m_pTileDB = pDB;
}

void MapEditorModuleTileSets::setModel(IDatabaseModel *pModel)
{
    // remove old database, otherwise it will be deleted
    if (auto pOldModel = dynamic_cast<IDatabaseModel*>(model()))
        pOldModel->takeDatabase();
    DatabaseModuleDragList::setModel(pModel);
}

void MapEditorModuleTileSets::mousePressEvent(QMouseEvent *pEvent)
{
    DatabaseModuleDragList::mousePressEvent(pEvent);

    auto item = indexAt(pEvent->pos());
    if (item.isValid() && (pEvent->button() == Qt::RightButton || pEvent->button() == Qt::LeftButton))
    {
        auto brush = BRUSH::BrushIndex::BRUSH_LEFT;
        if (pEvent->button() == Qt::RightButton)
            brush = BRUSH::BrushIndex::BRUSH_RIGHT;

        // ToDo: hacky ID; better solution?
        emit selectionChanged(brush, MAP::BRUSH::SelectionType::TILE_SETS, item.row()+1);
        emit itemClicked(brush);
    }
}
