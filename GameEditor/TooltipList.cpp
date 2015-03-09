#include "TooltipList.h"
#include "moc_TooltipList.h"
#include <QtGui/QDrag>
#include <QtCore/QMimeData>
#include <QtGui/QDragMoveEvent>
#include <QtGui/QPainter>
#include "QtGlobal.h"
#include "Config.h"
#include "AnimationViewer.h"

TooltipList::TooltipList(QWidget* pParent)
    : QTreeView(pParent), m_pToolTip(nullptr), m_ShowTooltip(false),
    m_ToolTipPos(ToolTipPosition::Left), m_MaximumTooltipSize(-1, -1)
{
    setDragEnabled(true);
    setDragDropMode(QAbstractItemView::DragOnly);

    setIndentation(0);

    m_Timer.setSingleShot(true);
}

void TooltipList::setShowTooltip(bool show)
{
    if (m_ShowTooltip == show)
        return;
    // do not show
    if (m_ShowTooltip && !show)
    {
        this->removeEventFilter(this);
        setMouseTracking(false);
        disconnect(&m_Timer, SIGNAL(timeout()), this, SLOT(_showTooltip()));
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

void TooltipList::_endTracking()
{
    m_Timer.stop();
    if (m_pToolTip)
    {
        m_pToolTip->deleteLater();
        m_pToolTip = nullptr;
    }
    m_CurrentRow = MATH::maximum<uint32>();
}

void TooltipList::_onItemEntered(const QModelIndex& index)
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

uint32 TooltipList::getSelectedID() const
{
    auto pModel = currentIndex();
    if (pModel.isValid())
        return pModel.row() + 1;
    return 0;
}

void TooltipList::_showTooltip()
{
    if (m_CurrentRow != MATH::maximum<uint32>())
    {
        if (m_pToolTip = _setupTooltipWidget(m_CurrentRow + 1))
        {
            m_pToolTip->setMouseTracking(false);
            m_pToolTip->setWindowFlags(Qt::ToolTip);
            if (m_MaximumTooltipSize.isValid())
            {
                QSize newSize(m_pToolTip->size());
                if (m_pToolTip->width() > m_MaximumTooltipSize.width())
                    newSize.setWidth(m_MaximumTooltipSize.width());
                if (m_pToolTip->height() > m_MaximumTooltipSize.height())
                    newSize.setHeight(m_MaximumTooltipSize.height());
                m_pToolTip->resize(newSize);
            }
            QPoint widgetPos = mapToGlobal(QPoint(m_ToolTipPos == ToolTipPosition::Left ? -m_pToolTip->width() : width(), 0));
            widgetPos.setY(QCursor::pos().y());
            m_pToolTip->move(widgetPos);
            m_pToolTip->show();
        }
    }
}

void TooltipList::leaveEvent(QEvent* pEvent)
{
    _endTracking();
    pEvent->accept();
}

void TooltipList::currentChanged(const QModelIndex& current, const QModelIndex& previous)
{
    QTreeView::currentChanged(current, previous);
    if (!current.isValid())
        emit changeIndex(-1);
    emit changeIndex(current.row());
}
