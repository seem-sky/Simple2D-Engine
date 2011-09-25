#ifndef TILE_TEXTURE_SOURCE_H
#define TILE_TEXTURE_SOURCE_H

#include "Texture.h"

enum PassabilityFlag
{
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

    void SetPassabilityFlag(UINT newFlag);
    void RemovePassabilityFlag(UINT Flag);
    bool HasPassabilityFlag(UINT Flag);

protected:
    // passability flag
    UINT m_Passable;
};
#endif;
