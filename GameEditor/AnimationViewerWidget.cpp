#include "AnimationViewerWidget.h"
#include "WorldObjectDatabaseWidget.h"
#include "moc_AnimationViewerWidget.h"

using namespace DATABASE;

const uint32 WIDGETS_PER_ROW = 3;

/*#####
# ObjectAnimationViewScene
#####*/
ObjectAnimationViewScene::ObjectAnimationViewScene(AnimationView *pParent) : AnimationViewScene(pParent)
{
    setGridDraw(false);
    setPreviousFrameDraw(false);
}

void ObjectAnimationViewScene::drawForeground(QPainter *painter, const QRectF &rect)
{
    if (!m_BoundingRect.isNull())
    {
        painter->setPen(QColor(255, 0, 0));
        painter->drawRect(m_BoundingRect);
    }
}

/*#####
# AnimationViewerWidget
#####*/
AnimationViewerWidget::AnimationViewerWidget(AnimationViewerTableWidget *pParent) : QWidget(pParent), Ui_AnimationViewerWidget(), m_uiObjectAnimationType(0)
{
    setupUi(this);
    m_pAnimationView->setGridDraw(false);
    m_pAnimationView->setPreviousFrameDraw(false);
    connect(m_pObjectAnimationType, SIGNAL(currentIndexChanged(int)), this, SLOT(_comboBoxIndexChanged(int)));
}

void AnimationViewerWidget::setCurrentObjectAnimationType(uint32 uiID)
{
    m_uiObjectAnimationType = uiID;
    m_pObjectAnimationType->setCurrentIndex(m_uiObjectAnimationType-1);
}

void AnimationViewerWidget::_comboBoxIndexChanged(int index)
{
    m_uiObjectAnimationType = ++index;
    emit onItemChanged();
}

/*#####
# AnimationViewerTableWidget
#####*/
AnimationViewerTableWidget::AnimationViewerTableWidget(QWidget *pParent) : QTableWidget(pParent), m_uiSize(0)
{
    setRowCount(0);
    setColumnCount(0);
    m_ObjectAnimationTypeTreeWidget.setShowTooltip(false);
    setSelectionMode(QAbstractItemView::NoSelection);
}

ObjectAnimationTypeTreeWidget* AnimationViewerTableWidget::_getNewObjectAnimationTypeTreeWidget()
{
    ObjectAnimationTypeTreeWidget *pTreeWidget = new ObjectAnimationTypeTreeWidget(this);
    pTreeWidget->setDB(m_pObjAniTypeDB);
    pTreeWidget->setShowTooltip(false);
    pTreeWidget->setColumnWidth(0, 40);
    pTreeWidget->setColumnWidth(1, 90);
    return pTreeWidget;
}

void AnimationViewerTableWidget::_setupAnimationViewerComboBox(AnimationViewerWidget *pWidget)
{
    if (!pWidget)
        return;
    ObjectAnimationTypeTreeWidget *pTreeWidget = _getNewObjectAnimationTypeTreeWidget();
    pWidget->m_pObjectAnimationType->setModel(pTreeWidget->model());
    pWidget->m_pObjectAnimationType->setView(pTreeWidget);
    pWidget->m_pObjectAnimationType->setModelColumn(1);
    pWidget->setCurrentObjectAnimationType(pWidget->getCurrentObjectAnimationType());
}

AnimationViewerWidget* AnimationViewerTableWidget::getNewAnimationViewerWidget()
{
    AnimationViewerWidget *pWidget = new AnimationViewerWidget(this);
    pWidget->m_pAnimationView->setScene(new ObjectAnimationViewScene(pWidget->m_pAnimationView));

    if (WorldObjectDatabaseWidget *pParent = dynamic_cast<WorldObjectDatabaseWidget*>(parent()))
    {
        connect(pWidget->m_pAnimationView, SIGNAL(onDrop(AnimationView*, uint32, Point<int32>)), pParent, SLOT(_onDrop(AnimationView*, uint32, Point<int32>)));
        pWidget->m_pAnimationView->setAnimationDB(pParent->getAnimationDB());
        pWidget->m_pAnimationView->setSpriteDB(pParent->getSpriteDB());
        pWidget->m_pAnimationView->setEditable(false);
        _setupAnimationViewerComboBox(pWidget);
    }
    return pWidget;
}

void AnimationViewerTableWidget::setAdditionalDBs(ConstSpriteDatabaseChangerPtr pSpriteDB, ConstAnimationDatabaseChangerPtr pAnimationDB,
                                                  ConstObjectAnimationTypeDatabaseChangerPtr pObjAniTypeDB)
{
    m_pObjAniTypeDB = pObjAniTypeDB;
    m_pSpriteDB = pSpriteDB;
    m_pAnimationDB = pAnimationDB;
    m_ObjectAnimationTypeTreeWidget;
    m_ObjectAnimationTypeTreeWidget.setDB(m_pObjAniTypeDB);
    // update cell widgets
    for (int32 i = 0; i < rowCount()*columnCount(); ++i)
    {
        if (AnimationViewerWidget *pWidget = dynamic_cast<AnimationViewerWidget*>(getAnimationWidget(i)))
        {
            pWidget->m_pAnimationView->setSpriteDB(m_pSpriteDB);
            pWidget->m_pAnimationView->setAnimationDB(m_pAnimationDB);
            _setupAnimationViewerComboBox(pWidget);
        }
    }
}

void AnimationViewerTableWidget::setCellAnimationInfo(uint32 uiIndex, WorldObjectPrototype::AnimationInfo animationInfo)
{
    _resizeIfNeeded(uiIndex);
    if (AnimationViewerWidget *pWidget = getAnimationWidget(uiIndex))
    {
        pWidget->setCurrentObjectAnimationType(animationInfo.m_uiObjectAnimationTypeID);
        pWidget->m_pAnimationView->setCurrentAnimation(animationInfo.m_uiAnimationID);
        pWidget->m_pAnimationView->setContinuesAnimation(true);
        pWidget->m_pAnimationView->playCurrentAnimation();
    }
}

WorldObjectPrototype::AnimationInfo AnimationViewerTableWidget::getCellAnimationInfo(uint32 uiIndex)
{
    if (AnimationViewerWidget *pWidget = getAnimationWidget(uiIndex))
        return  WorldObjectPrototype::AnimationInfo(pWidget->m_pAnimationView->getCurrentAnimationID(), pWidget->getCurrentObjectAnimationType());
    return  WorldObjectPrototype::AnimationInfo();
}

void AnimationViewerTableWidget::setSceneBoundingRect(const QRect &boundingRect)
{
    for (uint32 i = 0; i < rowCount()*columnCount()*WIDGETS_PER_ROW; ++i)
    {
        if (AnimationViewerWidget *pWidget = getAnimationWidget(i))
        {
            if (ObjectAnimationViewScene *pScene = dynamic_cast<ObjectAnimationViewScene*>(pWidget->m_pAnimationView->scene()))
            {
                pScene->setBoundingRect(boundingRect);
                pWidget->m_pAnimationView->viewport()->update();
            }
        }
    }
}

void AnimationViewerTableWidget::setSpeedModifyer(uint32 uiSpeedModifyer)
{
    for (uint32 i = 0; i < rowCount()*columnCount()*WIDGETS_PER_ROW; ++i)
    {
        if (AnimationViewerWidget *pWidget = getAnimationWidget(i))
        {
            pWidget->m_pAnimationView->setSpeedModifyer(uiSpeedModifyer);
            pWidget->m_pAnimationView->playCurrentAnimation();
        }
    }
}

Point<int32> AnimationViewerTableWidget::getCellFromIndex(uint32 uiIndex)
{
    return Point<int32>(uiIndex / WIDGETS_PER_ROW, uiIndex % WIDGETS_PER_ROW);
}

void AnimationViewerTableWidget::_resizeIfNeeded(uint32 uiIndex)
{
    if (uiIndex < m_uiSize)
        return;
    Point<int32> oldSize(columnCount(), rowCount());
    Point<int32> cell(getCellFromIndex(uiIndex));
    if (oldSize.x < cell.x+1)
        setColumnCount(cell.x+1);
    if (oldSize.y < WIDGETS_PER_ROW)
        setRowCount(WIDGETS_PER_ROW);
    for (; m_uiSize <= uiIndex; ++m_uiSize)
        _setAnimationWidget(m_uiSize, getNewAnimationViewerWidget());
}

void AnimationViewerTableWidget::_setAnimationWidget(uint32 uiIndex, AnimationViewerWidget *pWidget)
{
    Point<int32> cell(getCellFromIndex(uiIndex));
    if (pWidget && columnWidth(cell.x) < pWidget->width())
        setColumnWidth(cell.x, pWidget->width());
    if (pWidget && rowHeight(cell.y) < pWidget->height())
        setRowHeight(cell.y, pWidget->height());
    setCellWidget(cell.y, cell.x, pWidget);
    emit onItemAdd(pWidget);
    if (uiIndex < MIN_WORLD_OBJECT_POSE)
        pWidget->m_pObjectAnimationType->setDisabled(true);
}

void AnimationViewerTableWidget::setAnimationWidget(uint32 uiIndex, AnimationViewerWidget *pWidget)
{
    _resizeIfNeeded(uiIndex);
    _setAnimationWidget(uiIndex, pWidget);
}

AnimationViewerWidget* AnimationViewerTableWidget::getAnimationWidget(uint32 uiIndex)
{
    Point<int32> cell(getCellFromIndex(uiIndex));
    if (columnCount() <= cell.x || rowCount() <= cell.y)
        return NULL;
    return dynamic_cast<AnimationViewerWidget*>(cellWidget(cell.y, cell.x));
}

void AnimationViewerTableWidget::removeAnimationWidget(uint32 uiIndex)
{
    Point<int32> cell(getCellFromIndex(uiIndex));
    emit onItemRemove(dynamic_cast<AnimationViewerWidget*>(cellWidget(cell.y, cell.x)));
    removeCellWidget(cell.y, cell.x);
    m_uiSize = uiIndex;
    _resizeToShortestColumn(m_uiSize -(m_uiSize >= 0 ? 1 : 0));
}

void AnimationViewerTableWidget::_resizeToShortestColumn(uint32 lastIndex)
{
    Point<int32> lastCell = getCellFromIndex(lastIndex);
    setColumnCount(lastCell.x+1);
}