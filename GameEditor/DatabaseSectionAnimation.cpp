#include "DatabaseSectionAnimation.h"
#include "DatabaseModulePrototypeAnimation.h"

using namespace database;
using namespace ui::section;

Animation::Animation(const helper::CreatorInterface& creator, database::Model* pAdditionTypeModel, AnimationDatabase& DB, QWidget* pParent)
    : Base(DB, new module::PrototypeAnimation(creator, pAdditionTypeModel), pParent)
{
    if (auto pLayout = dynamic_cast<QGridLayout*>(layout()))
    {
        pLayout->setColumnStretch(1, 1);
        pLayout->setColumnStretch(2, 0);
    }
}
