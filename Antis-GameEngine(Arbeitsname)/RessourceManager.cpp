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
sTextureSource* CRessourceManager::AddCharsetTexture(std::string sTextureName)
{
    CDirect3D *pDirect3D = CDirect3D::Get();

    // create new texture source and store file name
    sTextureSource* Texture = new sTextureSource();
    Texture->m_sFileName = sTextureName;

    // add directory to filename
    sTextureName = DIRECTORY_TEXTURES_OBJECTS + sTextureName;

    // load infos from file
    Texture->m_pImageInfo = new D3DXIMAGE_INFO();
    D3DXGetImageInfoFromFile(sTextureName.c_str(), Texture->m_pImageInfo);

    // load file
    D3DXCreateTextureFromFileEx(pDirect3D->GetDevice(), sTextureName.c_str(), Texture->m_pImageInfo->Width, Texture->m_pImageInfo->Height, 1, 0,
        D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_FILTER_NONE, D3DCOLOR_XRGB(0, 0, 0), NULL, NULL, &Texture->m_pTexture);

    // return NULL if unable to load texture from file
    if(!Texture->m_pTexture)
    {
        ERROR_LOG(m_sLogLocationName + "Unable to load file" + sTextureName + ". No such file or directory.");
        return NULL;
    }

    m_lTextures.push_front(Texture);
    return *m_lTextures.begin();
}

sTextureSource* CRessourceManager::GetCharsetTexture(std::string sTextureName)
{
    for (std::list<sTextureSource*>::iterator itr = m_lTextures.begin(); itr != m_lTextures.end(); ++itr)
    {
        if ((*itr)->m_sFileName == sTextureName)
            return (*itr);
    }

    // if texture does not exist, add it
    return AddCharsetTexture(sTextureName);
}

void CRessourceManager::ClearCharsetTextures()
{
    for (std::list<sTextureSource*>::iterator itr = m_lTextures.begin(); itr != m_lTextures.end(); ++itr)
    {
        (*itr)->m_pTexture->Release();
        delete *itr;
    }
    m_lTextures.clear();
}