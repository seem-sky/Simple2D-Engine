#include "TileTextureSource.h"

TileTextureSource::TileTextureSource(void) : TextureSource()
{
    m_Passable  = PASSABLE_NONE;
}

TileTextureSource::~TileTextureSource(void)
{
}

void TileTextureSource::SetPassabilityFlag(unsigned short newFlag)
{
    unsigned short oldval = m_Passable;
    unsigned short newval = oldval | newFlag;

    if(oldval != newval)
        m_Passable = newval;
}

bool TileTextureSource::HasPassabilityFlag(unsigned short Flag)
{
    return (m_Passable & Flag) != 0;
}

void TileTextureSource::RemovePassabilityFlag(unsigned short oldFlag)
{
    unsigned short oldval = m_Passable;
    unsigned short newval = oldval & ~oldFlag;

    if(oldval != newval)
        m_Passable = newval;
}