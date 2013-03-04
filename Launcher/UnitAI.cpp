#include "UnitAI.h"
#include "Unit.h"
#include "Player.h"

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

void UnitAI::OnUse(Player *pWho)
{
    // set this facing to pWho
    if (Unit *pThis = (Unit*)GetOwner())
    {
        if (Unit *pUnit = pWho->GetControledUnit())
            pThis->FacingObject(pUnit);
    }

    ObjectAI::OnUse(pWho);
}