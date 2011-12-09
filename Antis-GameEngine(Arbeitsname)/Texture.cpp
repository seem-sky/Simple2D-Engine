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
    if (CRessourceManager *pResMgr = CRessourceManager::Get())
    {
        if (GameDatabase *pDatabase = GameDatabase::Get())
        {
            if (const SpritePrototype *pProto = pDatabase->GetSpriteFile(uiID))
            {
                TextureSource *pTexture = new TextureSource();
                pTexture->m_TextureInfo = *pProto;
                pTexture->m_pTexture    = pResMgr->GetTexture(pDatabase->GetFileLocation((SpriteType)pProto->m_uiSpriteType), pProto);
                m_TextureList.insert(std::make_pair<UINT, TextureSource*>(pTexture->m_TextureInfo.m_uiID, pTexture));
                return pTexture;
            }
        }
    }

    return NULL;
}