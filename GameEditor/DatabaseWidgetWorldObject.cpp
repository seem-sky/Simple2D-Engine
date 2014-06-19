#include "DatabaseWidgetWorldObject.h"
#include "moc_DatabaseWidgetWorldObject.h"

using namespace DATABASE;
using namespace PROTOTYPE;

DatabaseWidgetWorldObject::DatabaseWidgetWorldObject(QWidget* pParent) : DatabaseWidgetBase(pParent),
    m_pModuleTab(new DeactivatedTabWidget(this)),
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

void DatabaseWidgetWorldObject::setupPrototypeFromWidgets(Prototype* pPrototype)
{
    if (auto pWorldObject = dynamic_cast<WORLD_OBJECT::WorldObjectPrototype*>(pPrototype))
    {
		auto& flagMgr = pWorldObject->getFlagManager();
		flagMgr.setFlag(WORLD_OBJECT::Flags::FLAG_ANIMATION, m_pModuleAnimation->setupPrototype(pWorldObject->getAnimationModule()));
    }
    DatabaseWidgetBase::setupPrototypeFromWidgets(pPrototype);
}

void DatabaseWidgetWorldObject::setupWidgetsFromPrototype(const Prototype* pPrototype)
{
    if (auto pWorldObject = dynamic_cast<const WORLD_OBJECT::WorldObjectPrototype*>(pPrototype))
    {
		auto& flagMgr = pWorldObject->getFlagManager();
		m_pModuleAnimation->setupModule(pWorldObject->getAnimationModule(), flagMgr.hasFlag(WORLD_OBJECT::Flags::FLAG_ANIMATION));
    }
    DatabaseWidgetBase::setupWidgetsFromPrototype(pPrototype);
}

void DatabaseWidgetWorldObject::setDatabaseMgr(DatabaseMgr& DBMgr)
{
    m_pModuleList->setDatabaseModel(new DatabaseModel(DBMgr, DatabaseType::WORLD_OBJECT_DATABASE));
}

void DatabaseWidgetWorldObject::setSpriteDatabaseModel(ConstDatabaseModel* pModel)
{
    m_pModuleAnimation->setSpriteDatabaseModel(pModel);
}

void DatabaseWidgetWorldObject::setAnimationDatabaseModel(ConstDatabaseModel* pModel)
{
    m_pModuleAnimation->setAnimationDatabaseModel(pModel);
}
