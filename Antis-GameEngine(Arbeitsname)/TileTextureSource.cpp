#include "TileTextureSource.h"

TileTextureSource::TileTextureSource(void) : TextureSource()
{
    m_Passable  = 0;
}

TileTextureSource::~TileTextureSource(void)
{
}

void TileTextureSource::SetPassabilityFlag(UINT newFlag)
{
    UINT oldval = m_Passable;
    UINT newval = oldval | newFlag;

    if(oldval != newval)
        m_Passable = newval;
}

bool TileTextureSource::HasPassabilityFlag(UINT Flag)
{
    return (m_Passable & Flag) != 0;
}

void TileTextureSource::RemovePassabilityFlag(UINT oldFlag)
{
    UINT oldval = m_Passable;
    UINT newval = oldval & ~oldFlag;

    if(oldval != newval)
        m_Passable = newval;
}