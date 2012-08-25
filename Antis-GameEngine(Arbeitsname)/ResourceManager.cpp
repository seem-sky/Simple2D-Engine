#include "ResourceManager.h"

//directory for charsets
const std::string DIRECTORY_TEXTURES_OBJECTS    = "Charset/";

ResourceMgr::ResourceMgr(void) : TSingleton()
{
    m_sLogLocationName = LOGFILE_ENGINE_LOG_NAME + "ResourceManager : ";
}

ResourceMgr::~ResourceMgr(void)
{
    ClearTextures();
}

LPDIRECT3DTEXTURE9 ResourceMgr::GetTexture(std::string sLocation, const DATABASE::SpritePrototype *proto)
{
    if (!proto)
        return NULL;

    std::map<std::string, LPDIRECT3DTEXTURE9>::iterator itr = m_TextureList.find(sLocation+proto->m_sFileName);
    if (itr != m_TextureList.end())
        return (*itr).second;

    // if texture does not exist, add it
    return AddTexture(sLocation, proto);
}

LPDIRECT3DTEXTURE9 ResourceMgr::AddTexture(std::string p_sLocation, const DATABASE::SpritePrototype *p_pProto)
{
    if (!p_pProto)
        return NULL;

    HRESULT hr;

    CDirect3D *pDirect3D = CDirect3D::Get();

    // load infos from file
    D3DXIMAGE_INFO ImageInfo;
    D3DXGetImageInfoFromFile((p_sLocation+p_pProto->m_sFileName).c_str(), &ImageInfo);

    // load file
    LPDIRECT3DTEXTURE9 pTexture = NULL;

    // parse transparency color
    std::string t_sColorString = p_pProto->m_sTransparentColor;
    UINT t_uiColorRed = atoi(t_sColorString.substr((t_sColorString.find("R"))).c_str());
    UINT t_uiColorBlue = atoi(t_sColorString.substr((t_sColorString.find("B"))).c_str());
    UINT t_uiColorGreen = atoi(t_sColorString.substr((t_sColorString.find("G"))).c_str());
    hr = D3DXCreateTextureFromFileEx(pDirect3D->GetDevice(), (p_sLocation+p_pProto->m_sFileName).c_str(), ImageInfo.Width, ImageInfo.Height, 1, 0,
        D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_FILTER_NONE, D3DCOLOR_XRGB(t_uiColorRed, t_uiColorGreen, t_uiColorBlue), NULL, NULL, &pTexture);

    // return NULL if unable to load texture from file
    if (FAILED(hr))
    {
        if (!p_pProto->m_sFileName.empty())
            ERROR_LOG(m_sLogLocationName + "Unable to load file " + p_sLocation+p_pProto->m_sFileName + ". No such file or directory.");
    }
    else
        BASIC_LOG(m_sLogLocationName + "Succesfully load file " + p_sLocation+p_pProto->m_sFileName + ".");

    // add texture to list
    m_TextureList.insert(std::make_pair<std::string, LPDIRECT3DTEXTURE9>((p_sLocation+p_pProto->m_sFileName).c_str(), pTexture));

    return pTexture;
}

void ResourceMgr::ClearTextures()
{
    for (std::map<std::string, LPDIRECT3DTEXTURE9>::iterator itr = m_TextureList.begin(); itr != m_TextureList.end(); ++itr)
    {
        if ((*itr).second)
            (*itr).second->Release();
    }

    m_TextureList.clear();
    BASIC_LOG(m_sLogLocationName + "Remove stored textures from memory.");
}