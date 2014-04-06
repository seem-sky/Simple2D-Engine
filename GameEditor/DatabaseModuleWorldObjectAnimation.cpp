#include "DatabaseModuleWorldObjectAnimation.h"
#include "moc_DatabaseModuleWorldObjectAnimation.h"
#include <QtWidgets/QGridLayout>

using namespace DATABASE::WORLD_OBJECT;

DatabaseModuleWorldObjectAnimation::DatabaseModuleWorldObjectAnimation(QWidget* pParent) : QWidget(pParent),
    // modules
    m_pModuleSpriteList(new DatabaseModuleTextureDragList(this)),
    m_pModuleAnimationList(new DatabaseModuleAnimationDragList(this)),
    m_pModuleVisualViewer(new DatabaseModuleVisualViewer(this)),
    m_pVisualTypeBox(new QComboBox(this))
{
    // setup layout and add modules
    auto pAnimationLayout = new QGridLayout(this);
    setLayout(pAnimationLayout);
    pAnimationLayout->addWidget(m_pModuleVisualViewer, 0, 0, -1, 1);
    pAnimationLayout->addWidget(m_pVisualTypeBox, 0, 1);
    pAnimationLayout->addWidget(m_pModuleSpriteList, 1, 1, -1, 1);
    pAnimationLayout->addWidget(m_pModuleAnimationList, 1, 1, -1, 1);

    // stretches
    pAnimationLayout->setRowStretch(0, 0);
    pAnimationLayout->setRowStretch(1, 1);
    pAnimationLayout->setColumnStretch(0, 1);
    pAnimationLayout->setColumnStretch(1, 0);
    pAnimationLayout->setColumnMinimumWidth(0, 200);
    pAnimationLayout->setColumnMinimumWidth(1, 150);

    // setup visual type box
    m_pVisualTypeBox->addItem("sprites");
    m_pVisualTypeBox->addItem("animations");
    m_pVisualTypeBox->setCurrentIndex(0);

    m_pVisualTypeBox->setMaximumWidth(200);
    m_pModuleSpriteList->setMaximumWidth(200);
    m_pModuleAnimationList->setMaximumWidth(200);

    _onVisualTypeChanged(m_pVisualTypeBox->currentIndex());

    // setup connections
    connect(m_pVisualTypeBox, SIGNAL(currentIndexChanged(int)), this, SLOT(_onVisualTypeChanged(int)));
}

uint32 DatabaseModuleWorldObjectAnimation::getAnimationCount() const
{
    return m_pModuleVisualViewer->getVisualViewerCount();
}

void DatabaseModuleWorldObjectAnimation::setAniamtionCount(uint32 count)
{
    m_pModuleVisualViewer->setVisualViewerCount(count);
}

void DatabaseModuleWorldObjectAnimation::_onVisualTypeChanged(int type)
{
    // hide all first
    m_pModuleSpriteList->hide();
    m_pModuleAnimationList->hide();

    switch (static_cast<AnimationInfo::VisualType>(type))
    {
    case AnimationInfo::VisualType::SPRITE:
        m_pModuleSpriteList->show();
        break;
    case AnimationInfo::VisualType::ANIMATION:
        m_pModuleAnimationList->show();
        break;
    }
}

void DatabaseModuleWorldObjectAnimation::setSpriteDatabaseModel(DATABASE::ConstDatabaseModel* pModel)
{
    m_pModuleVisualViewer->setDatabaseMgr(&pModel->getDatabaseMgr());
    m_pModuleSpriteList->setModel(pModel);
    m_pModuleAnimationList->setSpriteDatabase(pModel->getDatabaseMgr().getSpriteDatabase());
}

void DatabaseModuleWorldObjectAnimation::setAnimationDatabaseModel(DATABASE::ConstDatabaseModel* pModel)
{
    m_pModuleAnimationList->setModel(pModel);
}

void DatabaseModuleWorldObjectAnimation::clear()
{
    m_pModuleVisualViewer->clear();
}
