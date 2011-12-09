#include "RessourceManager.h"
#include "GameDatabase.h"

//directory for charsets
const std::string DIRECTORY_TEXTURES_OBJECTS    = "Charset/";

CRessourceManager::CRessourceManager(void) : TSingleton()
{
    m_sLogLocationName = LOGFILE_ENGINE_LOG_NAME + "CRessourceManager : ";
}

CRessourceManager::~CRessourceManager(void)
{
    ClearTextures();
}

LPDIRECT3DTEXTURE9 CRessourceManager::GetTexture(std::string sLocation, const SpritePrototype *proto)
{
    if (!proto)
        return NULL;

    std::map<std::string, LPDIRECT3DTEXTURE9>::iterator itr = m_TextureList.find(sLocation+proto->m_sFileName);
    if (itr != m_TextureList.end())
        return (*itr).second;

    // if texture does not exist, add it
    return AddTexture(sLocation, proto);
}

LPDIRECT3DTEXTURE9 CRessourceManager::AddTexture(std::string sLocation, const SpritePrototype *proto)
{
    if (!proto)
        return NULL;

    HRESULT hr;

    CDirect3D *pDirect3D = CDirect3D::Get();

    // load infos from file
    D3DXIMAGE_INFO ImageInfo;
    D3DXGetImageInfoFromFile((sLocation+proto->m_sFileName).c_str(), &ImageInfo);

    // load file
    LPDIRECT3DTEXTURE9 pTexture = NULL;
    hr = D3DXCreateTextureFromFileEx(pDirect3D->GetDevice(), (sLocation+proto->m_sFileName).c_str(), ImageInfo.Width, ImageInfo.Height, 1, 0,
        D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_FILTER_NONE, proto->m_transparentColor, NULL, NULL, &pTexture);

    // return NULL if unable to load texture from file
    if (FAILED(hr))
    {
        if (!proto->m_sFileName.empty())
            ERROR_LOG(m_sLogLocationName + "Unable to load file " + sLocation+proto->m_sFileName + ". No such file or directory.");
    }
    else
        BASIC_LOG(m_sLogLocationName + "Succesfully load file " + sLocation+proto->m_sFileName + ".");

    // add texture to list
    m_TextureList.insert(std::make_pair<std::string, LPDIRECT3DTEXTURE9>((sLocation+proto->m_sFileName).c_str(), pTexture));

    return pTexture;
}

void CRessourceManager::ClearTextures()
{
    for (std::map<std::string, LPDIRECT3DTEXTURE9>::iterator itr = m_TextureList.begin(); itr != m_TextureList.end(); ++itr)
    {
        if ((*itr).second)
            (*itr).second->Release();
    }

    m_TextureList.clear();
    BASIC_LOG(m_sLogLocationName + "Remove stored textures from memory.");
}