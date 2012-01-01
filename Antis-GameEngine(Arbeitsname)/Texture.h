#ifndef TEXTURE_SOURCE_H
#define TEXTURE_SOURCE_H

#include "GameDatabase.h"
#include <d3d9.h>

struct TextureSource
{
    TextureSource(void) : m_pTexture(NULL) { }

    void GetTextureSize(UINT &XSize, UINT &YSize);
    void GetShownSpriteSize(UINT &XSize, UINT &YSize);
    inline const SpritePrototype* GetTextureInfo() { return &m_TextureInfo; }

    LPDIRECT3DTEXTURE9 m_pTexture;
    SpritePrototype m_TextureInfo;
};

typedef std::map<UINT /*ID*/, TextureSource*> TextureList;

class TextureMgr : public TSingleton<TextureMgr>
{
public:
    TextureMgr();
    ~TextureMgr();

    inline TextureSource* GetTextureSource(UINT uiID)
    {
        TextureList::iterator itr = m_TextureList.find(uiID);
        if (itr != m_TextureList.end())
            return (*itr).second;

        return AddTextureSource(uiID);
    }

private:
    TextureList m_TextureList;

    void ClearTextureList();
    TextureSource* AddTextureSource(UINT uiID);
};
#endif;