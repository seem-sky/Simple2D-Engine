#ifndef RESSOURCE_MANAGER_H
#define RESSOURCE_MANAGER_H

#include "Direct3D.h"
#include <d3d9.h>
#include <string>
#include <list>

struct sTextureSource 
{
    std::string m_sFileName;
    LPDIRECT3DTEXTURE9 m_pTexture;
    D3DXIMAGE_INFO* m_pImageInfo;

    sTextureSource()
    {
        m_pTexture      = NULL;
        m_pImageInfo    = NULL;
    }
};

class CRessourceManager : public TSingleton<CRessourceManager>
{
public:
    CRessourceManager(void);
    ~CRessourceManager(void);

    sTextureSource* GetCharsetTexture(std::string sTextureName);
    void ClearCharsetTextures();

protected:
    CLogfile *m_pLogfile;

private:
    sTextureSource* AddCharsetTexture(std::string sTextureName);

    std::list<sTextureSource*> m_lTextures;
};
#endif;