#include "Texture.h"
#include "RessourceManager.h"

/*#####
# TextureSource
#####*/
void TextureSource::GetTextureSize(UINT &XSize, UINT &YSize)
{
    if (!m_pTexture)
        return;

    D3DSURFACE_DESC desc;
    m_pTexture->GetLevelDesc(0, &desc);
    XSize = desc.Width;
    YSize = desc.Height;
}

void TextureSource::GetShownSpriteSize(UINT &XSize, UINT &YSize)
{
    if (!m_pTexture)
        return;

    D3DSURFACE_DESC desc;
    m_pTexture->GetLevelDesc(0, &desc);
    XSize = desc.Width;
    YSize = desc.Height;

    // if texture is split up into more sprites
    if (m_TextureInfo.m_uiSpriteType == 3)
    {
        XSize /= m_TextureInfo.Type.AnimatedObject.m_uiSpritesX;
        YSize /= m_TextureInfo.Type.AnimatedObject.m_uiSpritesY;
    }
}

/*#####
# TextureMgr
#####*/
TextureMgr::TextureMgr() : TSingleton()
{
}

TextureMgr::~TextureMgr()
{
    ClearTextureList();
}

void TextureMgr::ClearTextureList()
{
    for (TextureList::iterator itr = m_TextureList.begin(); itr != m_TextureList.end(); ++itr)
    {
        if ((*itr).second)
            delete (*itr).second;
    }

    m_TextureList.clear();
    BASIC_LOG(m_sLogLocationName + "Clear Texture Manager.");
}

TextureSource* TextureMgr::AddTextureSource(UINT uiID)
{
    if (CRessourceManager *t_pResMgr = CRessourceManager::Get())
    {
        if (DATABASE::Database *t_pDB = DATABASE::Database::Get())
        {
            if (const DATABASE::SpritePrototype *t_pProto = t_pDB->GetSpritePrototype(uiID))
            {
                TextureSource *t_pTexture   = new TextureSource();
                t_pTexture->m_TextureInfo   = *t_pProto;
                t_pTexture->m_pTexture      = t_pResMgr->GetTexture(t_pDB->GetSpritePath(t_pProto->m_uiSpriteType), t_pProto);
                m_TextureList.insert(std::make_pair<UINT, TextureSource*>(t_pTexture->m_TextureInfo.m_uiID, t_pTexture));
                return t_pTexture;
            }
        }
    }

    return NULL;
}