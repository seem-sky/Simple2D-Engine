#include "WorldObjectDatabaseWidget.h"
#include "moc_WorldObjectDatabaseWidget.h"

using namespace DATABASE;

/*#####
# WorldObjectDatabaseWidget
#####*/
WorldObjectDatabaseWidget::WorldObjectDatabaseWidget(QWidget *pParent) : DatabaseWidget(pParent), Ui_WorldObjectDatabaseWidget()
{
    Ui_WorldObjectDatabaseWidget::setupUi(this);
    connectWidgets();
    m_ModObj.setWidget(m_pAnimationList, MODIFY_RESIZE, QPoint(5, 5), MODIFY_DIRECTION_HEIGHT);
    m_ModObj.setWidget(m_pAnimationLabel, MODIFY_MOVE, QPoint(m_pAnimationList->width() - m_pAnimationLabel->width() + 5, 5), MODIFY_DIRECTION_WIDTH);
    m_ModObj.setWidget(m_pAnimationList, MODIFY_MOVE, QPoint(5, 5), MODIFY_DIRECTION_WIDTH);
    m_ModObj.setWidget(m_pAnimationTable, MODIFY_RESIZE, QPoint(m_pAnimationList->width() + 10, m_pAddAnimationButton->height() + 10));
    m_ModObj.setWidget(m_pAddAnimationButton, MODIFY_MOVE, QPoint(5,5), MODIFY_DIRECTION_HEIGHT);
    m_ModObj.setWidget(m_pRemoveAnimationButton, MODIFY_MOVE, QPoint(5,5), MODIFY_DIRECTION_HEIGHT);

    connect(m_pAddAnimationButton, SIGNAL(clicked()), this, SLOT(_onAddAnimationButtonClicked()));
    connect(m_pRemoveAnimationButton, SIGNAL(clicked()), this, SLOT(_onRemoveAnimationButtonClicked()));
    connect(m_pSpeedModifyer, SIGNAL(editingFinished()), this, SLOT(_updateAnimationSpeed()));
    connect(m_pScript, SIGNAL(editingFinished()), this, SLOT(_onItemChanged()));
    connect(m_pAnimationTable, SIGNAL(onItemAdd(AnimationViewerWidget*)), this, SLOT(_onItemAdded(AnimationViewerWidget*)));
    connect(m_pAnimationTable, SIGNAL(onItemRemove(AnimationViewerWidget*)), this, SLOT(_onItemRemoved(AnimationViewerWidget*)));
    m_pAddAnimationButton->setToolTip("Add new animation slot at the end.");
    m_pRemoveAnimationButton->setToolTip("Removes the last animation slot.");
}

void WorldObjectDatabaseWidget::setFocus()
{
    m_pAnimationList->fillWithPrototypes();
}

void WorldObjectDatabaseWidget::setAdditionalDB(ConstSpriteDatabaseChangerPtr pSpriteDB, ConstAnimationDatabaseChangerPtr pAnimationDB,
                                                ConstObjectAnimationTypeDatabaseChangerPtr pObjAniTypeDB)
{
    m_pAnimationDB = pAnimationDB;
    m_pSpriteDB = pSpriteDB;

    m_pAnimationList->setSpriteDB(m_pSpriteDB);
    m_pAnimationList->setDB(m_pAnimationDB);
    m_pAnimationTable->setAdditionalDBs(m_pSpriteDB, m_pAnimationDB, pObjAniTypeDB);
}

void WorldObjectDatabaseWidget::setupAnimationViewer(AnimationView* pViewer, uint32 uiAnimationID)
{
    if (!pViewer)
        return;
    pViewer->setCurrentAnimation(uiAnimationID);
    pViewer->setContinuesAnimation();
    pViewer->playCurrentAnimation();
}

void WorldObjectDatabaseWidget::_onDrop(AnimationView *pTarget, uint32 uiID, Point<int32> pos)
{
    setupAnimationViewer(pTarget, uiID);
    updateItem();
}

bool WorldObjectDatabaseWidget::getItemFromWidgets(WorldObjectPrototypePtr &proto)
{
    if (!DatabaseWidget::getItemFromWidgets(proto))
        return false;

    proto->setAnimationSpeed(m_pSpeedModifyer->value());
    proto->setBoundingRect(UInt32Rect(Point<uint32>(m_pBoundingLeft->value(), m_pBoundingUp->value()),
        Point<uint32>(m_pBoundingRight->value(), m_pBoundingBottom->value())));
    for (uint32 i = 0; i < m_pAnimationTable->getCellCount(); ++i)
        proto->setAnimationInfo(i, m_pAnimationTable->getCellAnimationInfo(i));
    return true;
}

bool WorldObjectDatabaseWidget::setWidgetsFromPrototype(const WorldObjectPrototypePtr &proto)
{
    if (!DatabaseWidget::setWidgetsFromPrototype(proto))
        return false;

    disconnect(m_pBoundingLeft, SIGNAL(valueChanged(int)), this, SLOT(_dataChanged(int)));
    disconnect(m_pBoundingUp, SIGNAL(valueChanged(int)), this, SLOT(_dataChanged(int)));
    disconnect(m_pBoundingRight, SIGNAL(valueChanged(int)), this, SLOT(_dataChanged(int)));
    disconnect(m_pBoundingBottom, SIGNAL(valueChanged(int)), this, SLOT(_dataChanged(int)));
    // add animations first
    for (uint32 i = 0; i < proto->getAnimationCount(); ++i)
        m_pAnimationTable->setCellAnimationInfo(i, proto->getAnimationInfo(i));
    QRect boundingRect(proto->getBoundingRect().getPositionX(), proto->getBoundingRect().getPositionY(), proto->getBoundingRect().getWidth(), proto->getBoundingRect().getHeight());
    m_pBoundingLeft->setValue(boundingRect.x());
    m_pBoundingUp->setValue(boundingRect.y());
    m_pBoundingRight->setValue(boundingRect.width());
    m_pBoundingBottom->setValue(boundingRect.height());
    m_pAnimationTable->setSceneBoundingRect(boundingRect);
    m_pSpeedModifyer->setValue(proto->getAnimationSpeed());
    _updateAnimationSpeed();
    connect(m_pBoundingLeft, SIGNAL(valueChanged(int)), this, SLOT(_dataChanged(int)));
    connect(m_pBoundingUp, SIGNAL(valueChanged(int)), this, SLOT(_dataChanged(int)));
    connect(m_pBoundingRight, SIGNAL(valueChanged(int)), this, SLOT(_dataChanged(int)));
    connect(m_pBoundingBottom, SIGNAL(valueChanged(int)), this, SLOT(_dataChanged(int)));
    return true;
}

void WorldObjectDatabaseWidget::_dataChanged(int value)
{
    updateItem();
    m_pAnimationTable->setSceneBoundingRect(getBoundingRect());
}

void WorldObjectDatabaseWidget::_updateAnimationSpeed()
{
    updateItem();
    m_pAnimationTable->setSpeedModifyer(m_pSpeedModifyer->value());
}

void WorldObjectDatabaseWidget::_onItemChanged()
{
    updateItem();
}

QRect WorldObjectDatabaseWidget::getBoundingRect()
{
    return QRect(m_pBoundingLeft->value(), m_pBoundingUp->value(), m_pBoundingRight->value(), m_pBoundingBottom->value());
}

void WorldObjectDatabaseWidget::_onAddAnimationButtonClicked()
{
    AnimationViewerWidget *pWidget = m_pAnimationTable->getNewAnimationViewerWidget();
    m_pAnimationTable->setAnimationWidget(m_pAnimationTable->getCellCount(), pWidget);
    if (ObjectAnimationViewScene *pScene = dynamic_cast<ObjectAnimationViewScene*>(pWidget->m_pAnimationView->scene()))
        pScene->setBoundingRect(getBoundingRect());
    updateItem();
}

void WorldObjectDatabaseWidget::_onRemoveAnimationButtonClicked()
{
    if (m_pAnimationTable->getCellCount() <= 4)
        return;
    m_pAnimationTable->removeAnimationWidget(m_pAnimationTable->getCellCount()-1);
    updateItem();
}

void WorldObjectDatabaseWidget::_onItemAdded(AnimationViewerWidget *pWidget)
{
    connect(pWidget, SIGNAL(onItemChanged()), this, SLOT(_onItemChanged()));
}

void WorldObjectDatabaseWidget::_onItemRemoved(AnimationViewerWidget *pWidget)
{
    disconnect(pWidget, SIGNAL(onItemChanged()), this, SLOT(_onItemChanged()));
}