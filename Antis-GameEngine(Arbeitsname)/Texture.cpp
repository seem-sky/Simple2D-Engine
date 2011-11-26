#include "Texture.h"

TextureSource::TextureSource(void)
{
    m_pTexture      = NULL;
}

TextureSource::~TextureSource(void)
{
}

void TextureSource::ChangeTexture(std::string sFileName)
{

}

void *TextureSource::GetPixels()
{
    if (!m_pTexture)
        return NULL;

    /*m_pTexture->LockRect(*/
    return NULL;
}

void TextureSource::GetTextureSize(UINT &XSize, UINT &YSize)
{
    D3DSURFACE_DESC desc;
    m_pTexture->GetLevelDesc(0, &desc);
    XSize = desc.Width;
    YSize = desc.Height;
}