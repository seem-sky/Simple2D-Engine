#include "DatabaseModuleDragDrop.h"
#include <QtGui/QDrag>
#include <QtCore/QMimeData>
#include <QtGui/QDragMoveEvent>
#include <QtGui/QPainter>
#include "moc_DatabaseModuleDragDrop.h"
#include "QtGlobal.h"
#include "Config.h"

DatabaseModuleDragList::DatabaseModuleDragList(QWidget* pParent) : QTreeView(pParent), m_CurrentRow(MATH::maximum<uint32>()), m_pToolTip(nullptr), m_ShowTooltip(false),
    m_ToolTipPos(TOOLTIP_LEFT), m_MaximumTooltipSize(100, 100)
{
    setDragEnabled(true);
    setDragDropMode(QAbstractItemView::DragOnly);

    setIndentation(0);

    m_Timer.setSingleShot(true);
}

void DatabaseModuleDragList::setShowTooltip(bool show)
{
    if (m_ShowTooltip == show)
        return;
    // do not show
    if (m_ShowTooltip& & !show)
    {
        this->removeEventFilter(this);
        setMouseTracking(false);
        disconnect(this, SIGNAL(itemEntered(QTreeWidgetItem*, int)), this, SLOT(_onItemEnter(QTreeWidgetItem*, int)));
        disconnect(&m_Timer, SIGNAL(timeout()), this, SLOT(_showPixmapTooltip()));
        disconnect(this, SIGNAL(entered(const QModelIndex&)), this, SLOT(_onItemEntered(const QModelIndex&)));
    }
    // show
    else if (!m_ShowTooltip& & show)
    {
        this->installEventFilter(this);
        setMouseTracking(true);
        connect(&m_Timer, SIGNAL(timeout()), this, SLOT(_showTooltip()));
        connect(this, SIGNAL(entered(const QModelIndex&)), this, SLOT(_onItemEntered(const QModelIndex&)));
    }
    m_ShowTooltip = show;
}

void DatabaseModuleDragList::setModel(IDatabaseModel* pModel)
{
    if (pModel)
    {
        QTreeView::setModel(pModel);
        resizeColumnToContents(0);
    }
}

void DatabaseModuleDragList::_endTracking()
{
    m_Timer.stop();
    if (m_pToolTip)
    {
        m_pToolTip->deleteLater();
        m_pToolTip = nullptr;
    }
    m_CurrentRow = MATH::maximum<uint32>();
}

void DatabaseModuleDragList::_onItemEntered(const QModelIndex& index)
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

void DatabaseModuleDragList::_showTooltip()
{
    if (m_CurrentRow != MATH::maximum<uint32>())
    {
        if (m_pToolTip = _setupTooltipWidget(m_CurrentRow+1))
        {
            m_pToolTip->setMouseTracking(false);
            m_pToolTip->setWindowFlags(Qt::ToolTip);
            QSize newSize(m_pToolTip->size());
            if (m_pToolTip->width() > m_MaximumTooltipSize.width())
                newSize.setWidth(m_MaximumTooltipSize.width());
            if (m_pToolTip->height() > m_MaximumTooltipSize.height())
                newSize.setHeight(m_MaximumTooltipSize.height());
            m_pToolTip->resize(newSize);
            QPoint widgetPos = mapToGlobal(QPoint(m_ToolTipPos == TOOLTIP_LEFT ? -m_pToolTip->width() : width(), 0));
            widgetPos.setY(QCursor::pos().y());
            m_pToolTip->move(widgetPos);
            m_pToolTip->show();
        }
    }
}

void DatabaseModuleDragList::leaveEvent(QEvent* pEvent)
{
    _endTracking();
    pEvent->accept();
}


/*#####
# Tile drag&drop
#####*/
DatabaseModuleTextureDragList::DatabaseModuleTextureDragList(QWidget* pParent) : DatabaseModuleDragList(pParent)
{
    setShowTooltip(true);
}

QWidget* DatabaseModuleTextureDragList::_setupTooltipWidget(uint32 uiPrototypeID)
{
    auto pLabel = new QLabel(this);
    pLabel->resize(10, 10);
    if (auto pModel = dynamic_cast<IDatabaseModel*>(model()))
    {
        if (auto pPrototype = dynamic_cast<DATABASE::TexturePrototype*>(pModel->getDatabase()->getPrototype(uiPrototypeID)))
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
        if (auto pModel = dynamic_cast<IDatabaseModel*>(model()))
        {
            if (auto pPrototype = dynamic_cast<DATABASE::TexturePrototype*>(pModel->getDatabase()->getPrototype(pItem.row()+1)))
            {
                QPixmap pixmap;
                if (createPixmapFromTexturePrototype(Config::get()->getProjectDirectory(), pPrototype, pixmap))
                    pDrag->setPixmap(pixmap);
            }
        }
        mimeData->setText(QString::number(pItem.row()+1));
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
    if (uint32 ID = pEvent->mimeData()->text().toUInt())
    {
        setCurrentTileID(ID);
        emit onDrop(getCurrentTileID(), Int32Point(pEvent->pos().x(), pEvent->pos().y()));
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