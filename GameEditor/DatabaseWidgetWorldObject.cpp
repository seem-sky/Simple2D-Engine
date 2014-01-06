#include "DatabaseWidgetWorldObject.h"
#include "moc_DatabaseWidgetWorldObject.h"

using namespace DATABASE;

DatabaseWidgetWorldObject::DatabaseWidgetWorldObject(QWidget* pParent) : DatabaseWidgetBase(pParent),
    // modules
    m_pModuleSpriteList(new DatabaseModuleTextureDragList(this)),
    m_pModuleVisualViewer(new DatabaseModuleVisualViewer(this)),

    // others
    m_pVisualTypeBox(new QComboBox(this))
{
    // setup visual type box
    m_pVisualTypeBox->addItem("sprites");
    m_pVisualTypeBox->addItem("animations");
    m_pVisualTypeBox->setCurrentIndex(0);

    m_pModuleSpriteList->setMaximumWidth(200);
    m_pModuleVisualViewer->setMaximumWidth(200);

    connect(m_pVisualTypeBox, SIGNAL(currentIndexChanged(int)), this, SLOT(_onVisualTypeChanged(int)));

    // setup layout
    if (auto pLayout = dynamic_cast<QGridLayout*>(layout()))
    {
        pLayout->setColumnMinimumWidth(1, 200);
        pLayout->setColumnMinimumWidth(2, 150);

        // stretches
        pLayout->setColumnStretch(0, 0);
        pLayout->setColumnStretch(1, 1);
        pLayout->setColumnStretch(2, 0);
        pLayout->setColumnStretch(3, 1);
        pLayout->setRowStretch(0, 0);
        pLayout->setRowStretch(1, 1);

        pLayout->addWidget(m_pModuleVisualViewer, 0, 1, -1, 1);
        pLayout->addWidget(m_pVisualTypeBox, 0, 2);
        pLayout->addWidget(m_pModuleSpriteList, 1, 2, -1, 1);
    }
}

void DatabaseWidgetWorldObject::clear()
{
    DatabaseWidgetBase::clear();
}

void DatabaseWidgetWorldObject::setupPrototypeFromWidgets(DATABASE::Prototype* pPrototype)
{
    if (auto pWorldObject = dynamic_cast<WORLD_OBJECT::WorldObjectPrototype*>(pPrototype))
    {
        pWorldObject->setAnimationCount(m_pModuleVisualViewer->getVisualViewerCount());
    }
    DatabaseWidgetBase::setupPrototypeFromWidgets(pPrototype);
}

void DatabaseWidgetWorldObject::setupWidgetsFromPrototype(const DATABASE::Prototype* pPrototype)
{
    if (auto pWorldObject = dynamic_cast<const WORLD_OBJECT::WorldObjectPrototype*>(pPrototype))
    {
        m_pModuleVisualViewer->setVisualViewerCount(pWorldObject->getAnimationCount());
    }
    DatabaseWidgetBase::setupWidgetsFromPrototype(pPrototype);
}

void DatabaseWidgetWorldObject::_onVisualTypeChanged(int type)
{
    // hide all first
    m_pModuleSpriteList->hide();

    switch (static_cast<WORLD_OBJECT::AnimationInfo::VisualType>(type))
    {
    case WORLD_OBJECT::AnimationInfo::VisualType::SPRITE:
        m_pModuleSpriteList->show();
        break;
    case WORLD_OBJECT::AnimationInfo::VisualType::ANIMATION:
        break;
    }

}

void DatabaseWidgetWorldObject::setDatabaseMgr(DatabaseMgr& DBMgr)
{
    m_pModuleList->setDatabaseModel(new DatabaseModel(DBMgr, DatabaseType::WORLD_OBJECT_DATABASE));
}

void DatabaseWidgetWorldObject::setSpriteDatabaseModel(DATABASE::ConstDatabaseModel* pModel)
{
    m_pModuleSpriteList->setModel(pModel);
}

void DatabaseWidgetWorldObject::setAnimationDatabaseModel(DATABASE::ConstDatabaseModel* pModel)
{
    // m_pModuleSpriteList->setModel(pModel);
}
