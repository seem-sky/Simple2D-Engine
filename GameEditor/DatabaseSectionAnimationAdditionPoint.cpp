#include "DatabaseSectionAnimationAdditionPoint.h"
#include "DatabaseModulePrototypeAnimationAdditionPoint.h"

using namespace database;
using namespace ui::section;

AnimationAdditionPoint::AnimationAdditionPoint(const helper::CreatorInterface& creator, database::Model* pAnimationAdditionTypeModel,
    AnimationAdditionPointDatabase& DB, QWidget* pParent)
    : Base(DB, new module::PrototypeAnimationAdditionPoint(creator, pAnimationAdditionTypeModel), pParent)
{
    if (auto pLayout = dynamic_cast<QGridLayout*>(layout()))
    {
        pLayout->setColumnStretch(1, 1);
        pLayout->setColumnStretch(2, 0);
    }
}
