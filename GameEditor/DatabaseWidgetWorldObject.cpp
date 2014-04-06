#include "DatabaseWidgetWorldObject.h"
#include "moc_DatabaseWidgetWorldObject.h"
#include <QtWidgets/QCheckBox>

using namespace DATABASE;

ModuleTab::ModuleTab(QWidget* pParent) : QTabWidget(pParent)
{
}

void ModuleTab::tabInserted(int index)
{
    QTabWidget::tabInserted(index);
    auto pTabBar = getTabBar();
    pTabBar->setTabButton(index, QTabBar::RightSide, new QCheckBox(this));
}

DatabaseWidgetWorldObject::DatabaseWidgetWorldObject(QWidget* pParent) : DatabaseWidgetBase(pParent),
    m_pModuleTab(new ModuleTab(this)),
    m_pModuleAnimation(new DatabaseModuleWorldObjectAnimation(this))
{
    // setup tabs
    m_pModuleTab->addTab(m_pModuleAnimation, "animations");

    // setup layout
    if (auto pLayout = dynamic_cast<QGridLayout*>(layout()))
        pLayout->addWidget(m_pModuleTab, 0, 1, -1, -1);
}

void DatabaseWidgetWorldObject::clear()
{
    DatabaseWidgetBase::clear();
    m_pModuleAnimation->clear();
}

void DatabaseWidgetWorldObject::setupPrototypeFromWidgets(DATABASE::Prototype* pPrototype)
{
    if (auto pWorldObject = dynamic_cast<WORLD_OBJECT::WorldObjectPrototype*>(pPrototype))
    {
        auto count = m_pModuleAnimation->getAnimationCount();
        pWorldObject->setAnimationCount(count);
        for (uint32 i = 0; i < count; ++i)
        {
            if (auto viewer = m_pModuleAnimation->getVisualViewer(i))
            {
                WORLD_OBJECT::AnimationInfo info;
                info.m_ID = viewer->getAnimationID();
                info.m_VisualType = viewer->getVisualType();
                info.m_AnimationTypeID = viewer->getAnimationTypeID();
                pWorldObject->setAnimationInfo(i, info);
            }
        }
    }
    DatabaseWidgetBase::setupPrototypeFromWidgets(pPrototype);
}

void DatabaseWidgetWorldObject::setupWidgetsFromPrototype(const DATABASE::Prototype* pPrototype)
{
    if (auto pWorldObject = dynamic_cast<const WORLD_OBJECT::WorldObjectPrototype*>(pPrototype))
    {
        auto count = pWorldObject->getAnimationCount();
        m_pModuleAnimation->setAniamtionCount(count);
        for (uint32 i = 0; i < count; ++i)
        {
            if (auto pViewer = m_pModuleAnimation->getVisualViewer(i))
            {
                auto &info = pWorldObject->getAnimationInfo(i);
                pViewer->setAnimation(info.m_ID, info.m_VisualType);
                pViewer->setAnimationType(info.m_AnimationTypeID);
            }
        }
    }
    DatabaseWidgetBase::setupWidgetsFromPrototype(pPrototype);
}

void DatabaseWidgetWorldObject::setDatabaseMgr(DatabaseMgr& DBMgr)
{
    m_pModuleList->setDatabaseModel(new DatabaseModel(DBMgr, DatabaseType::WORLD_OBJECT_DATABASE));
}

void DatabaseWidgetWorldObject::setSpriteDatabaseModel(DATABASE::ConstDatabaseModel* pModel)
{
    m_pModuleAnimation->setSpriteDatabaseModel(pModel);
}

void DatabaseWidgetWorldObject::setAnimationDatabaseModel(DATABASE::ConstDatabaseModel* pModel)
{
    m_pModuleAnimation->setAnimationDatabaseModel(pModel);
}
