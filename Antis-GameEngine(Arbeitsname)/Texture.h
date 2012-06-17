#ifndef TEXTURE_SOURCE_H
#define TEXTURE_SOURCE_H

#include <d3d9.h>
#include "Database.h"

struct TextureSource
{
    TextureSource(void) : m_pTexture(NULL) { }

    void GetTextureSize(UINT &XSize, UINT &YSize);
    void GetShownSpriteSize(UINT &XSize, UINT &YSize);

    LPDIRECT3DTEXTURE9 m_pTexture;
    DATABASE::SpritePrototype m_TextureInfo;
};

typedef std::map<UINT /*ID*/, TextureSource*> TextureList;

class TextureMgr : public TSingleton<TextureMgr>
{
public:
    TextureMgr();
    ~TextureMgr();

    inline TextureSource* GetTextureSource(UINT uiID)
    {
        if (uiID == 0)
            return NULL;

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