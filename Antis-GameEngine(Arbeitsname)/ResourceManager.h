#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include "Direct3D.h"
#include "Database.h"
#include <map>

struct SpritePrototype;

class ResourceMgr : public TSingleton<ResourceMgr>
{
public:
    ResourceMgr(void);
    ~ResourceMgr(void);

    LPDIRECT3DTEXTURE9 GetTexture(std::string sLocation, const DATABASE::SpritePrototype *proto);

private:
    LPDIRECT3DTEXTURE9 AddTexture(std::string sLocation, const DATABASE::SpritePrototype *proto);
    void ClearTextures();

    // store all textures
    std::map<std::string, LPDIRECT3DTEXTURE9> m_TextureList;
};
#endif;