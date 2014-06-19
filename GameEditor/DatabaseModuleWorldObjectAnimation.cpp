#include "DatabaseModuleWorldObjectAnimation.h"
#include "moc_DatabaseModuleWorldObjectAnimation.h"
#include <QtWidgets/QGridLayout>

using namespace DATABASE::PROTOTYPE::WORLD_OBJECT;
using namespace DATABASE::PROTOTYPE::MODULE::ANIMATION;

DatabaseModuleWorldObjectAnimation::DatabaseModuleWorldObjectAnimation(QWidget* pParent) : DeactivatedWidget(pParent),
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

void DatabaseModuleWorldObjectAnimation::setupModule(const AnimationModule& module, bool activated)
{
	setActivated(activated);
	if (activated)
	{
		auto count = module.getAnimationCount();
		setAniamtionCount(count);
		for (uint32 i = 0; i < count; ++i)
		{
			if (auto pViewer = getVisualViewer(i))
			{
				auto &info = module.getAnimationInfo(i);
				pViewer->setAnimation(info.m_ID, info.m_VisualType);
				pViewer->setAnimationType(info.m_AnimationTypeID);
			}
		}
	}
}

bool DatabaseModuleWorldObjectAnimation::setupPrototype(AnimationModule& module) const
{
	if (!isActivated())
		return false;

	auto count = getAnimationCount();
	module.setAnimationCount(count);
	for (uint32 i = 0; i < count; ++i)
	{
		if (auto viewer = getVisualViewer(i))
		{
			AnimationInfo info;
			info.m_ID = viewer->getAnimationID();
			info.m_VisualType = viewer->getVisualType();
			info.m_AnimationTypeID = viewer->getAnimationTypeID();
			module.setAnimationInfo(i, info);
		}
	}

	return true;
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

    switch (static_cast<VisualType>(type))
    {
    case VisualType::SPRITE:
        m_pModuleSpriteList->show();
        break;
    case VisualType::ANIMATION:
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
