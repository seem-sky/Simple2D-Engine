#include "DatabaseModuleDragDrop.h"
#include <QtGui/QDrag>
#include <QtCore/QMimeData>
#include <QtGui/QDragMoveEvent>
#include <QtGui/QPainter>
#include <math_extensions.h>
#include <Global.h>
#include "moc_DatabaseModuleDragDrop.h"
#include "QtGlobal.h"
#include "Config.h"
#include "AnimationViewer.h"

DatabaseModuleTooltipList::DatabaseModuleTooltipList(QWidget* pParent) : QTreeView(pParent), m_CurrentRow(MATH::maximum<uint32>()), m_pToolTip(nullptr), m_ShowTooltip(false),
    m_ToolTipPos(ToolTipPosition::TOOLTIP_LEFT), m_MaximumTooltipSize(100, 100)
{
    setDragEnabled(true);
    setDragDropMode(QAbstractItemView::DragOnly);

    setIndentation(0);

    m_Timer.setSingleShot(true);
}

void DatabaseModuleTooltipList::setShowTooltip(bool show)
{
    if (m_ShowTooltip == show)
        return;
    // do not show
    if (m_ShowTooltip && !show)
    {
        this->removeEventFilter(this);
        setMouseTracking(false);
        disconnect(this, SIGNAL(itemEntered(QTreeWidgetItem*, int)), this, SLOT(_onItemEnter(QTreeWidgetItem*, int)));
        disconnect(&m_Timer, SIGNAL(timeout()), this, SLOT(_showPixmapTooltip()));
        disconnect(this, SIGNAL(entered(const QModelIndex&)), this, SLOT(_onItemEntered(const QModelIndex&)));
    }
    // show
    else if (!m_ShowTooltip && show)
    {
        this->installEventFilter(this);
        setMouseTracking(true);
        connect(&m_Timer, SIGNAL(timeout()), this, SLOT(_showTooltip()));
        connect(this, SIGNAL(entered(const QModelIndex&)), this, SLOT(_onItemEntered(const QModelIndex&)));
    }
    m_ShowTooltip = show;
}

void DatabaseModuleTooltipList::setModel(DATABASE::ConstDatabaseModel* pModel)
{
    if (pModel)
    {
        QTreeView::setModel(pModel);
        resizeColumnToContents(0);
    }
}

void DatabaseModuleTooltipList::_endTracking()
{
    m_Timer.stop();
    if (m_pToolTip)
    {
        m_pToolTip->deleteLater();
        m_pToolTip = nullptr;
    }
    m_CurrentRow = MATH::maximum<uint32>();
}

void DatabaseModuleTooltipList::_onItemEntered(const QModelIndex& index)
{
    if (!showTooltip())
        return;

    if (m_pToolTip)
    {
        _endTracking();
        m_CurrentRow = index.row();
        _showTooltip();
    }
    else
    {
        m_CurrentRow = index.row();
        m_Timer.start(250);
    }
}

uint32 DatabaseModuleTooltipList::getSelectedID() const
{
    auto pModel = currentIndex();
    if (pModel.isValid())
        return pModel.row() + 1;
    return 0;
}

void DatabaseModuleTooltipList::_showTooltip()
{
    if (m_CurrentRow != MATH::maximum<uint32>())
    {
        if (m_pToolTip = _setupTooltipWidget(m_CurrentRow + 1))
        {
            m_pToolTip->setMouseTracking(false);
            m_pToolTip->setWindowFlags(Qt::ToolTip);
            QSize newSize(m_pToolTip->size());
            if (m_pToolTip->width() > m_MaximumTooltipSize.width())
                newSize.setWidth(m_MaximumTooltipSize.width());
            if (m_pToolTip->height() > m_MaximumTooltipSize.height())
                newSize.setHeight(m_MaximumTooltipSize.height());
            m_pToolTip->resize(newSize);
            QPoint widgetPos = mapToGlobal(QPoint(m_ToolTipPos == ToolTipPosition::TOOLTIP_LEFT ? -m_pToolTip->width() : width(), 0));
            widgetPos.setY(QCursor::pos().y());
            m_pToolTip->move(widgetPos);
            m_pToolTip->show();
        }
    }
}

void DatabaseModuleTooltipList::leaveEvent(QEvent* pEvent)
{
    _endTracking();
    pEvent->accept();
}

void DatabaseModuleTooltipList::currentChanged(const QModelIndex& current, const QModelIndex& previous)
{
    QTreeView::currentChanged(current, previous);
    if (!current.isValid())
        emit changeIndex(-1);
    emit changeIndex(current.row());
}


/*#####
# Tile drag&drop
#####*/
DatabaseModuleTextureDragList::DatabaseModuleTextureDragList(QWidget* pParent) : DatabaseModuleTooltipList(pParent)
{
    setShowTooltip(true);
}

QWidget* DatabaseModuleTextureDragList::_setupTooltipWidget(uint32 uiPrototypeID)
{
    auto pLabel = new QLabel(this);
    pLabel->resize(10, 10);
    if (auto pModel = dynamic_cast<DATABASE::ConstDatabaseModel*>(model()))
    {
        if (auto pPrototype = dynamic_cast<const DATABASE::PROTOTYPE::TexturePrototype*>(pModel->getDatabase()->getPrototype(uiPrototypeID)))
        {
            QPixmap pixmap;
            if (createPixmapFromTexturePrototype(Config::get()->getProjectDirectory(), pPrototype, pixmap))
            {
                pLabel->setPixmap(pixmap);
                pLabel->resize(pixmap.size());
            }
        }
    }
    return pLabel;
}

void DatabaseModuleTextureDragList::startDrag(Qt::DropActions supportedActions)
{
    auto pItem = currentIndex();
    if (pItem.isValid())
    {
        QDrag* pDrag = new QDrag(this);
        auto mimeData = new QMimeData();
        if (auto pModel = dynamic_cast<DATABASE::ConstDatabaseModel*>(model()))
        {
            if (auto pPrototype = dynamic_cast<const DATABASE::PROTOTYPE::TexturePrototype*>(pModel->getDatabase()->getPrototype(pItem.row()+1)))
            {
                QPixmap pixmap;
                if (createPixmapFromTexturePrototype(Config::get()->getProjectDirectory(), pPrototype, pixmap))
                    pDrag->setPixmap(pixmap);

                mimeData->setText(QString::number(pPrototype->getID()) + "," + QString::number(static_cast<uint32>(pModel->getDatabaseType())));
            }
        }
        pDrag->setMimeData(mimeData);
        pDrag->exec(supportedActions);
        return;
    }
    QTreeView::startDrag(supportedActions);
}

TileDropLabel::TileDropLabel(QWidget* pParent) : QLabel(pParent), m_uiCurrentTileID(0), m_pTileDB(nullptr)
{
    setAcceptDrops(true);
}

void TileDropLabel::dragMoveEvent(QDragMoveEvent* pEvent)
{
    pEvent->accept();
}

void TileDropLabel::dragEnterEvent(QDragEnterEvent* pEvent)
{
    pEvent->acceptProposedAction();
}

void TileDropLabel::dropEvent(QDropEvent* pEvent)
{
    auto texts = pEvent->mimeData()->text().split(",");
    if (texts.count() >= 2)
    {
        uint32 ID = texts.at(0).toUInt();
        auto DBtype = static_cast<DATABASE::DatabaseType>(texts.at(1).toUInt());
        if (ID && DBtype == DATABASE::DatabaseType::TILE_DATABASE)
        {
            setCurrentTileID(ID);
            emit onDrop(getCurrentTileID(), GEOMETRY::Point<int32>(pEvent->pos().x(), pEvent->pos().y()));
        }
    }
}

void TileDropLabel::paintEvent(QPaintEvent* pEvent)
{
    QLabel::paintEvent(pEvent);

    if (!m_pTileDB)
        return;
    if (auto pProto = m_pTileDB->getOriginalPrototype(m_uiCurrentTileID))
    {
        QPainter painter(this);
        QPixmap pixmap;
        if (createPixmapFromTexturePrototype(Config::get()->getProjectDirectory(), pProto, pixmap))
            painter.drawPixmap(0, 0, pixmap);
    }
}

/*#####
# Animation drag&drop
#####*/
DatabaseModuleAnimationDragList::DatabaseModuleAnimationDragList(QWidget* pParent) : DatabaseModuleTooltipList(pParent), m_pSpriteDB(nullptr)
{
    setShowTooltip(true);
}

void DatabaseModuleAnimationDragList::setSpriteDatabase(const DATABASE::SpriteDatabase* pSpriteDB)
{
    m_pSpriteDB = pSpriteDB;
}

QWidget* DatabaseModuleAnimationDragList::_setupTooltipWidget(uint32 uiPrototypeID)
{
    if (!m_pSpriteDB)
        throw std::runtime_error("You have to set DatabaseManager first.");
    auto pViewer = new AnimationViewer(this);
    pViewer->resize(100, 100);
    pViewer->setSpriteDatabase(m_pSpriteDB);
    pViewer->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    pViewer->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    if (auto pModel = dynamic_cast<DATABASE::ConstDatabaseModel*>(model()))
    {
        if (auto pPrototype = dynamic_cast<const DATABASE::PROTOTYPE::ANIMATION::AnimationPrototype*>(pModel->getDatabase()->getPrototype(uiPrototypeID)))
        {
            pViewer->setAnimation(&pPrototype->getAnimation());
            pViewer->startAnimation();
        }
    }
    return pViewer;
}

void DatabaseModuleAnimationDragList::startDrag(Qt::DropActions supportedActions)
{
    auto pItem = currentIndex();
    if (pItem.isValid())
    {
        QDrag* pDrag = new QDrag(this);
        auto mimeData = new QMimeData();
        if (auto pModel = dynamic_cast<DATABASE::ConstDatabaseModel*>(model()))
        {
            if (auto pPrototype = dynamic_cast<const DATABASE::PROTOTYPE::ANIMATION::AnimationPrototype*>(pModel->getDatabase()->getPrototype(pItem.row()+1)))
            {
                AnimationViewer viewer(this);
                viewer.resize(100, 100);
                viewer.setSpriteDatabase(m_pSpriteDB);
                viewer.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
                viewer.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
                viewer.setAnimation(&pPrototype->getAnimation());
                pDrag->setPixmap(viewer.grab());

                mimeData->setText(QString::number(pPrototype->getID()) + "," + QString::number(static_cast<uint32>(pModel->getDatabaseType())));
            }
        }
        pDrag->setMimeData(mimeData);
        pDrag->exec(supportedActions);
        return;
    }
    QTreeView::startDrag(supportedActions);
}
