#include "WorldObject.h"
#include "ObjectLayer.h"
#include "Map.h"

WorldObject::WorldObject(D3DXVECTOR3 pos) : m_pTexture(NULL), m_v3Position(pos), m_Color(1,1,1,1),
    m_ModRed(0), m_ModGreen(0), m_ModBlue(0), m_ModAlpha(0), m_ColorModTime(0), m_pOwnerLayer(NULL)
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

void WorldObject::Update(const ULONGLONG uiCurTime, const UINT uiDiff)
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

    m_pTexture->GetShownSpriteSize(Xsize, Ysize);
}

void WorldObject::DrawObject(LPD3DXSPRITE pSprite)
{
    if (m_pTexture && m_pTexture->m_pTexture && pSprite)
        pSprite->Draw(m_pTexture->m_pTexture, NULL, NULL, &GetPosition(), GetColor());
}

UINT WorldObject::GetMapPosX()
{
    if (m_pOwnerLayer)
    {
        if (Map *pMap = m_pOwnerLayer->GetOwnerMap())
        {
            return (UINT)(GetPositionX() - pMap->GetPositionX());
        }
    }
    return 0;
}

UINT WorldObject::GetMapPosY()
{
    if (m_pOwnerLayer)
    {
        if (Map *pMap = m_pOwnerLayer->GetOwnerMap())
        {
            return (UINT)(GetPositionY() - pMap->GetPositionY());
        }
    }
    return 0;
}

void WorldObject::GetBoundingRect(RECT &bound)
{
    if (!m_pTexture)
        return;

    if (const SpritePrototype* pInfo = m_pTexture->GetTextureInfo())
    {
        switch (pInfo->m_uiSpriteType)
        {
        case SPRITE_TYPE_OBJECT:
        case SPRITE_TYPE_ANIMATED_OBJECT:
            bound.left = pInfo->Type.Object.m_uiBoundingXBegin;
            bound.right = bound.left + pInfo->Type.Object.m_uiBoundingXRange;
            bound.top = pInfo->Type.Object.m_uiBoundingYBegin;
            bound.bottom = bound.top + pInfo->Type.Object.m_uiBoundingYRange;
            break;
        default:
            break;
        }
    }
}