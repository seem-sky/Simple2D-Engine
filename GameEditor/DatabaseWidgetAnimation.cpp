#include "DatabaseWidgetAnimation.h"

using namespace DATABASE;

DatabaseWidgetAnimation::DatabaseWidgetAnimation(QWidget* pParent) : DatabaseWidgetBase(pParent), m_pModuleAnimation(new DatabaseModuleAnimation(this)),
    m_pModuleSpriteList(new DatabaseModuleTextureDragList(this))
{
    if (auto pLayout = dynamic_cast<QGridLayout*>(layout()))
    {
        pLayout->addWidget(m_pModuleAnimation, 0, 1);

        // add tile list module
        QVBoxLayout* pBoxLayout(new QVBoxLayout());
        pBoxLayout->setSpacing(6);

        // setup "tiles:" label
        QLabel* pLabel(new QLabel("sprites:"));
        QFont font;
        font.setPointSize(8);
        font.setBold(true);
        font.setWeight(75);
        pLabel->setFont(font);
        pBoxLayout->addWidget(pLabel);

        pBoxLayout->addWidget(m_pModuleSpriteList);
        pLayout->addLayout(pBoxLayout, 0, 2, -1, 1);
    }
}

void DatabaseWidgetAnimation::setDatabaseMgr(DatabaseMgr& DBMgr)
{
    m_pModuleAnimation->setSpriteDatabase(DBMgr.getSpriteDatabase());
    m_pModuleList->setDatabaseModel(new DatabaseModel(DBMgr, DatabaseType::ANIMATION_DATABASE));
}

void DatabaseWidgetAnimation::setSpriteDatabaseModel(ConstDatabaseModel* pModel)
{
    m_pModuleSpriteList->setModel(pModel);
}

void DatabaseWidgetAnimation::setupWidgetsFromPrototype(const Prototype* pPrototype)
{
    if (auto pProto = dynamic_cast<const ANIMATION::AnimationPrototype*>(pPrototype))
    {
        m_pModuleAnimation->setAnimation(pProto->getAnimation());
    }
    DatabaseWidgetBase::setupWidgetsFromPrototype(pPrototype);
}

void DatabaseWidgetAnimation::setupPrototypeFromWidgets(Prototype* pPrototype)
{
    if (auto pProto = dynamic_cast<ANIMATION::AnimationPrototype*>(pPrototype))
    {
        auto animation = m_pModuleAnimation->getAnimation();
        // remove empty sprites at end
        if (!animation.empty())
        {
            auto sizeWithoutEmptyFrames = static_cast<uint32>(animation.size());
            for (auto i = static_cast<uint32>(animation.size()); i > 0; --i)
            {
                if (!animation.at(i-1).isEmpty())
                {
                    animation.resize(sizeWithoutEmptyFrames);
                    break;
                }
                else
                    sizeWithoutEmptyFrames = i-1;
            }
        }

        pProto->setAnimation(m_pModuleAnimation->getAnimation());
    }
    DatabaseWidgetBase::setupPrototypeFromWidgets(pPrototype);
}

void DatabaseWidgetAnimation::clear()
{
    m_pModuleAnimation->clear();
    DatabaseWidgetBase::clear();
}
