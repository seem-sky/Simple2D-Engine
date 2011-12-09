#ifndef RESSOURCE_MANAGER_H
#define RESSOURCE_MANAGER_H

#include "Direct3D.h"
#include <map>

struct SpritePrototype;

class CRessourceManager : public TSingleton<CRessourceManager>
{
public:
    CRessourceManager(void);
    ~CRessourceManager(void);

    LPDIRECT3DTEXTURE9 GetTexture(std::string sLocation, const SpritePrototype *proto);

private:
    LPDIRECT3DTEXTURE9 AddTexture(std::string sLocation, const SpritePrototype *proto);
    void ClearTextures();

    // store all textures
    std::map<std::string, LPDIRECT3DTEXTURE9> m_TextureList;
};
#endif;