#include "DatabaseSectionAnimationAdditionType.h"
#include "DatabaseModulePrototypeAnimationAdditionType.h"

using namespace database;
using namespace ui::section;

AnimationAdditionType::AnimationAdditionType(AnimationAdditionTypeDatabase& DB, QWidget* pParent)
    : Base(DB, new module::PrototypeAnimationAdditionType(), pParent)
{}
