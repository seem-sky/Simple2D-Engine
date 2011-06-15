#include "Unit.h"

Unit::Unit(void)
{
    m_UnitType      = UNIT_TYPE_UNIT;
    m_pMovement     = new MovementGenerator();
    m_pMovement->Init(static_cast<D3DXVECTOR2>(m_v3Position));
    m_pMovement->Move2DWithoutCollision(5789549, 0, 1000);
}

Unit::~Unit(void)
{
}

void Unit::Update(const UINT uiCurTime, const UINT uiDiff)
{
    // Update Movement
    m_v3Position += m_pMovement->UpdateMovement(uiCurTime, uiDiff);
}
