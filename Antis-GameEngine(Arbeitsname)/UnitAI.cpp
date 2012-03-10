#include "UnitAI.h"
#include "Unit.h"

UnitAI::UnitAI() : ObjectAI()
{
}

UnitAI::~UnitAI(void)
{
}

void UnitAI::OnPathComplete()
{
    Unit *pOwner = (Unit*)GetOwner();
    if (!pOwner)
        return;

    Point<int> pos = pOwner->GetPosition();
}

void UnitAI::SetOwner(Unit *pOwner)
{
    ObjectAI::SetOwner(pOwner);
}