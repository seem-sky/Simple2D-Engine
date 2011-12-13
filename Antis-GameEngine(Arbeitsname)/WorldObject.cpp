#include "WorldObject.h"

WorldObject::WorldObject(void) : m_pTexture(NULL), m_v3Position(0,0,0), m_Color(1,1,1,1),
    m_ModRed(0), m_ModGreen(0), m_ModBlue(0), m_ModAlpha(0), m_ColorModTime(0)
{
    m_sLogLocationName  = LOGFILE_ENGINE_LOG_NAME + "WorldObject : ";
    m_UnitType          = UNIT_TYPE_WORLDOBJECT;
}

WorldObject::~WorldObject(void)
{
}

void WorldObject::SetTextureSource(const SpritePrototype *proto)
{
    if (!proto)
        return;

    if (TextureMgr *pTextureMgr = TextureMgr::Get())
        m_pTexture = pTextureMgr->GetTextureSource(proto->m_uiID);
}

void WorldObject::Update(const UINT uiCurTime, const UINT uiDiff)
{
    // Update color
    if (m_ColorModTime)
        UpdateColor(uiDiff);
}

void WorldObject::ModifyColorTo(float red, float green, float blue, float alpha, UINT time)
{
    m_ModRed            = (red - m_Color.r) / time;
    m_ModGreen          = (green - m_Color.g) / time;
    m_ModBlue           = (blue - m_Color.b) / time;
    m_ModAlpha          = (alpha - m_Color.a) / time;
    m_ColorModTime      = time;
}

void WorldObject::UpdateColor(const UINT uiDiff)
{
    float red, green, blue, alpha;
    if (m_ColorModTime > uiDiff)
    {
        red     = m_ModRed * uiDiff;
        green   = m_ModGreen * uiDiff;
        blue    = m_ModBlue * uiDiff;
        alpha   = m_ModAlpha * uiDiff;
        m_ColorModTime -= uiDiff;
    }
    else
    {
        red     = m_ModRed * m_ColorModTime;
        green   = m_ModGreen * m_ColorModTime;
        blue    = m_ModBlue * m_ColorModTime;
        alpha   = m_ModAlpha * m_ColorModTime;
        m_ColorModTime = 0;
    }

    m_Color.r   += red;
    if (m_Color.r > 1)
        m_Color.r = 1;
    else if (m_Color.r < 0)
        m_Color.r = 0;

    m_Color.g   += green;
    if (m_Color.g > 1)
        m_Color.g = 1;
    else if (m_Color.g < 0)
        m_Color.g = 0;

    m_Color.b   += blue;
    if (m_Color.b > 1)
        m_Color.b = 1;
    else if (m_Color.b < 0)
        m_Color.b = 0;

    m_Color.a   += alpha;
    if (m_Color.a > 1)
        m_Color.a = 1;
    else if (m_Color.a < 0)
        m_Color.a = 0;
}

void WorldObject::GetObjectSize(UINT &Xsize, UINT &Ysize)
{
    if (!m_pTexture)
        return;

    m_pTexture->GetTextureSize(Xsize, Ysize);
}

void WorldObject::DrawObject(LPD3DXSPRITE pSprite)
{
    if (m_pTexture && m_pTexture->m_pTexture && pSprite)
        pSprite->Draw(m_pTexture->m_pTexture, NULL, NULL, &GetPosition(), GetColor());
}