#ifndef TEXTURE_SOURCE_H
#define TEXTURE_SOURCE_H

#include <string>
#include <d3d9.h>
#include <d3dx9.h>

class TextureSource
{
public:
    TextureSource(void);
    ~TextureSource(void);

    LPDIRECT3DTEXTURE9 GetTexture() { return m_pTexture; }
    void SetTexture(LPDIRECT3DTEXTURE9 pTexture) { m_pTexture = pTexture; }

    void SetTextureName(std::string sTextureName) { m_sFileName = sTextureName; }
    std::string GetTextureName() { return m_sFileName; }

    void ChangeTexture(std::string sFileName);

protected:
    std::string m_sFileName;
    LPDIRECT3DTEXTURE9 m_pTexture;
    D3DXIMAGE_INFO* m_pImageInfo;
};

#endif;