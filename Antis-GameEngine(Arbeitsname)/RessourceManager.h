#ifndef RESSOURCE_MANAGER_H
#define RESSOURCE_MANAGER_H

#include "Direct3D.h"
#include <d3d9.h>
#include <string>
#include <list>
#include "TileTextureSource.h"

class CRessourceManager : public TSingleton<CRessourceManager>
{
public:
    CRessourceManager(void);
    ~CRessourceManager(void);

    TextureSource* GetCharsetTexture(std::string sTextureName);
    TileTextureSource* GetMapTexture(std::string sTextureName);

    void ClearCharsetTextures();
    void ClearTileTextures();

    void ChangeCharsetTextureWith(TextureSource* pTexture, std::string sTextureName);

protected:
    CLogfile *m_pLogfile;

private:
    TextureSource* AddCharsetTexture(std::string sTextureName);
    TileTextureSource* AddTileTexture(std::string sTexttureName);

    std::list<TextureSource*> m_lTextures;
    std::list<TileTextureSource*> m_lTileTextures;
};
#endif;