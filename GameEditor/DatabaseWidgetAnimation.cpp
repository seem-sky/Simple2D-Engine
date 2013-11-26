#include "DatabaseWidgetAnimation.h"

DatabaseWidgetAnimation::DatabaseWidgetAnimation(QWidget *pParent) : DatabaseWidgetBase(pParent), m_pModuleAnimation(new DatabaseModuleAnimation(this)),
    m_pModuleSpriteList(new DatabaseModuleTextureDragList(this))
{
    if (auto pLayout = dynamic_cast<QGridLayout*>(layout()))
    {
        pLayout->addWidget(m_pModuleAnimation, 0, 1);

        // add tile list module
        QVBoxLayout *pBoxLayout(new QVBoxLayout());
        pBoxLayout->setSpacing(6);

        // setup "tiles:" label
        QLabel *pLabel(new QLabel("sprites:"));
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

void DatabaseWidgetAnimation::setSpriteDatabaseModel(SpriteDatabaseModel *pDBModel)
{
    m_pModuleSpriteList->setModel(pDBModel);
    m_pModuleAnimation->setSpriteDatabase(dynamic_cast<DATABASE::SpriteDatabase*>(pDBModel->getDatabase()));
}

void DatabaseWidgetAnimation::setupWidgetsFromPrototype(const DATABASE::Prototype *pPrototype)
{
    if (auto pProto = dynamic_cast<const DATABASE::ANIMATION::AnimationPrototype*>(pPrototype))
    {
        m_pModuleAnimation->setAnimation(pProto->getAnimation());
    }
    DatabaseWidgetBase::setupWidgetsFromPrototype(pPrototype);
}

void DatabaseWidgetAnimation::setupPrototypeFromWidgets(DATABASE::Prototype *pPrototype)
{
    if (auto pProto = dynamic_cast<DATABASE::ANIMATION::AnimationPrototype*>(pPrototype))
    {
        auto animation = m_pModuleAnimation->getAnimation();
        // remove empty sprites at end
        if (!animation.empty())
        {
            uint32 sizeWithoutEmptyFrames = animation.size();
            for (uint32 i = animation.size()-1; i >= 0; --i)
            {
                if (!animation.at(i).isEmpty())
                {
                    animation.resize(sizeWithoutEmptyFrames);
                    break;
                }
                else
                    sizeWithoutEmptyFrames = i;
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
