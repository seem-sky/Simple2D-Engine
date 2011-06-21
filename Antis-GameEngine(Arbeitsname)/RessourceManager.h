#ifndef RESSOURCE_MANAGER_H
#define RESSOURCE_MANAGER_H

#include "Direct3D.h"
#include <d3d9.h>
#include <string>
#include <list>
#include "Texture.h"

class CRessourceManager : public TSingleton<CRessourceManager>
{
public:
    CRessourceManager(void);
    ~CRessourceManager(void);

    TextureSource* GetCharsetTexture(std::string sTextureName);

    void ClearCharsetTextures();

    void ChangeTextureWith(TextureSource* pTexture, std::string sTextureName);

protected:
    CLogfile *m_pLogfile;

private:
    TextureSource* AddCharsetTexture(std::string sTextureName);

    std::list<TextureSource*> m_lTextures;
};
#endif;