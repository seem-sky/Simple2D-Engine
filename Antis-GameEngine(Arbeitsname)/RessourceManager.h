#ifndef RESSOURCE_MANAGER_H
#define RESSOURCE_MANAGER_H

#include "Direct3D.h"
#include <d3d9.h>
#include <string>
#include <map>
#include "TileTextureSource.h"

class CRessourceManager : public TSingleton<CRessourceManager>
{
public:
    CRessourceManager(void);
    ~CRessourceManager(void);

    // map texture
    TileTextureSource* GetMapTexture(std::string sTextureName);
    void ClearTileTextures();

    // charset texture
    TextureSource* GetCharsetTexture(std::string sTextureName);
    void ClearCharsetTextures();

protected:
    CLogfile *m_pLogfile;

private:
    TextureSource* AddCharsetTexture(std::string sTextureName);
    TileTextureSource* AddTileTexture(std::string sTexttureName);

    std::map<std::string, TextureSource*> m_lTextures;
    std::map<std::string, TileTextureSource*> m_lTileTextures;
};
#endif;