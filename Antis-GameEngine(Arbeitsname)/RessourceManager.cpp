#include "RessourceManager.h"

//directory for charsets
const std::string DIRECTORY_TEXTURES_OBJECTS    = "Charset/";

CRessourceManager::CRessourceManager(void) : TSingleton()
{
    m_sLogLocationName = LOGFILE_ENGINE_LOG_NAME + "CRessourceManager : ";
    m_pLogfile = CLogfile::Get();
}

CRessourceManager::~CRessourceManager(void)
{
    ClearCharsetTextures();
    ClearTileTextures();
}


// Charset Textures
TextureSource* CRessourceManager::AddCharsetTexture(std::string sTextureName)
{
    HRESULT hr;

    CDirect3D *pDirect3D = CDirect3D::Get();

    // create new texture source and store file name
    TextureSource* Texture  = new TextureSource();
    Texture->SetTextureName(sTextureName);

    // load infos from file
    D3DXIMAGE_INFO ImageInfo;
    D3DXGetImageInfoFromFile(sTextureName.c_str(), &ImageInfo);

    // load file
    LPDIRECT3DTEXTURE9 pTemp = NULL;
    hr = D3DXCreateTextureFromFileEx(pDirect3D->GetDevice(), sTextureName.c_str(), ImageInfo.Width, ImageInfo.Height, 1, 0,
        D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_FILTER_NONE, D3DCOLOR_XRGB(0, 0, 0), NULL, NULL, &pTemp);

    // return NULL if unable to load texture from file
    if(FAILED(hr))
    {
        if (!sTextureName.empty())
            ERROR_LOG(m_sLogLocationName + "Unable to load file " + sTextureName + ". No such file or directory.");
        delete pTemp;
        pTemp = NULL;
    }
    Texture->SetTexture(pTemp);
    m_lTextures.insert(std::make_pair<std::string, TextureSource*>(sTextureName, Texture));
    return Texture;
}

// searches charset texture list for, if not found, load it
TextureSource* CRessourceManager::GetCharsetTexture(std::string sTextureName)
{
    std::map<std::string, TextureSource*>::iterator itr = m_lTextures.find(sTextureName);
    if (itr != m_lTextures.end())
        return (*itr).second;

    // if texture does not exist, add it
    return AddCharsetTexture(sTextureName);
}

// searches map texture list for, if not found, load it
TileTextureSource* CRessourceManager::GetMapTexture(std::string sTextureName)
{
    std::map<std::string, TileTextureSource*>::iterator itr = m_lTileTextures.find(sTextureName);
    if (itr != m_lTileTextures.end())
        return (*itr).second;

    // if texture does not exist, add it
    return AddTileTexture(sTextureName);
}

void CRessourceManager::ClearCharsetTextures()
{
    for (std::map<std::string, TextureSource*>::iterator itr = m_lTextures.begin(); itr != m_lTextures.end(); ++itr)
    {
        if ((*itr).second->GetTexture())
            (*itr).second->GetTexture()->Release();

        delete (*itr).second;
    }

    m_lTextures.clear();
}

TileTextureSource* CRessourceManager::AddTileTexture(std::string sTextureName)
{
    CDirect3D *pDirect3D = CDirect3D::Get();

    HRESULT hr;

    // create new texture source and store file name
    TileTextureSource* Texture  = new TileTextureSource();
    Texture->SetTextureName(sTextureName);

    // load infos from file
    D3DXIMAGE_INFO ImageInfo;
    D3DXGetImageInfoFromFile(sTextureName.c_str(), &ImageInfo);

    // load file
    LPDIRECT3DTEXTURE9 pTemp = NULL;
    hr = D3DXCreateTextureFromFileEx(pDirect3D->GetDevice(), sTextureName.c_str(), ImageInfo.Width, ImageInfo.Height, 1, 0,
        D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_FILTER_NONE, D3DCOLOR_XRGB(0, 0, 0), NULL, NULL, &pTemp);
    if (FAILED(hr))
    {
        if (!sTextureName.empty())
            ERROR_LOG(m_sLogLocationName + "Unable to load file " + sTextureName + ". No such file or directory.");
        delete pTemp;
        pTemp = NULL;
    }
    else
        BASIC_LOG(m_sLogLocationName + "Succesfully load file " + sTextureName + ".");

    Texture->SetTexture(pTemp);

    m_lTileTextures.insert(std::make_pair<std::string, TileTextureSource*>(sTextureName, Texture));
    return Texture;
}

void CRessourceManager::ClearTileTextures()
{
    for (std::map<std::string, TileTextureSource*>::iterator itr = m_lTileTextures.begin(); itr != m_lTileTextures.end(); ++itr)
    {
        if ((*itr).second->GetTexture())
            (*itr).second->GetTexture()->Release();

        delete (*itr).second;
    }

    m_lTileTextures.clear();
}