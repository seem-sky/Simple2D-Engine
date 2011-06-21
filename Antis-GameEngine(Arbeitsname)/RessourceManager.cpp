#include "RessourceManager.h"

#ifndef RESSOURCEDIRECTORYS
#define RESSOURCEDIRECTORYS
//directory for charsets
const std::string DIRECTORY_TEXTURES_OBJECTS    = "Charset/";

#endif;

CRessourceManager::CRessourceManager(void) : TSingleton()
{
    m_sLogLocationName = LOGFILE_ENGINE_LOG_NAME + "CRessourceManager : ";
    m_pLogfile = CLogfile::Get();
}

CRessourceManager::~CRessourceManager(void)
{
    ClearCharsetTextures();
}


// Charset Textures
TextureSource* CRessourceManager::AddCharsetTexture(std::string sTextureName)
{
    CDirect3D *pDirect3D = CDirect3D::Get();

    // create new texture source and store file name
    TextureSource* Texture  = new TextureSource();
    Texture->SetTextureName(sTextureName);

    // add directory to filename
    sTextureName = DIRECTORY_TEXTURES_OBJECTS + sTextureName;

    // load infos from file
    Texture->SetImageInfo(new D3DXIMAGE_INFO());
    D3DXGetImageInfoFromFile(sTextureName.c_str(), Texture->GetImageInfo());

    // load file
    LPDIRECT3DTEXTURE9 pTemp;
    D3DXCreateTextureFromFileEx(pDirect3D->GetDevice(), sTextureName.c_str(), Texture->GetImageInfo()->Width, Texture->GetImageInfo()->Height, 1, 0,
        D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_FILTER_NONE, D3DCOLOR_XRGB(0, 0, 0), NULL, NULL, &pTemp);

    Texture->SetTexture(pTemp);

    // return NULL if unable to load texture from file
    if(!Texture->GetTexture())
    {
        ERROR_LOG(m_sLogLocationName + "Unable to load file " + sTextureName + ". No such file or directory.");
        return NULL;
    }

    m_lTextures.push_front(Texture);
    return *m_lTextures.begin();
}

TextureSource* CRessourceManager::GetCharsetTexture(std::string sTextureName)
{
    for (std::list<TextureSource*>::iterator itr = m_lTextures.begin(); itr != m_lTextures.end(); ++itr)
    {
        if ((*itr)->GetTextureName() == sTextureName)
            return (*itr);
    }

    // if texture does not exist, add it
    return AddCharsetTexture(sTextureName);
}

void CRessourceManager::ClearCharsetTextures()
{
    for (std::list<TextureSource*>::iterator itr = m_lTextures.begin(); itr != m_lTextures.end(); ++itr)
    {
        (*itr)->GetTexture()->Release();
        delete *itr;
    }
    m_lTextures.clear();
}

void CRessourceManager::ChangeTextureWith(TextureSource* pTexture, std::string sTextureName)
{
    TextureSource *pNewTexture = GetCharsetTexture(sTextureName);
    // if new or cur Texture = NULL return
    if (!pNewTexture || !pTexture)
        return;

    // change TextureSource with new Texture
    for (std::list<TextureSource*>::iterator itr = m_lTextures.begin(); itr != m_lTextures.end(); ++itr)
    {
        if ((*itr) == pTexture)
        {
            TextureSource *pTemp = new TextureSource();
            *pTemp      = *pTexture;
            **itr       = *pNewTexture;
            *pNewTexture= *pTemp;
            delete pTemp;
            return;
        }
    }
}