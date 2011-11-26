#ifndef TILE_TEXTURE_SOURCE_H
#define TILE_TEXTURE_SOURCE_H

#include "Texture.h"

enum PassabilityFlag
{
    PASSABLE_NONE   = 0x00,
    PASSABLE_UP     = 0x01,
    PASSABLE_DOWN   = 0x02,
    PASSABLE_RIGHT  = 0x04,
    PASSABLE_LEFT   = 0x08,
};

class TileTextureSource : public TextureSource
{
public:
    TileTextureSource(void);
    ~TileTextureSource(void);

    void SetPassabilityFlag(unsigned short newFlag);
    void RemovePassabilityFlag(unsigned short Flag);
    bool HasPassabilityFlag(unsigned short Flag);


protected:
    // passability flag
    unsigned short m_Passable;
};
#endif;