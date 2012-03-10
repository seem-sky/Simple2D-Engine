#include "WorldObject.h"
#include "ObjectLayer.h"
#include "Map.h"

WorldObject::WorldObject(UINT uiGUID, Point<int> pos, ObjectAI* pAI) : m_pTexture(NULL), m_Position(pos), m_Color(1,1,1,1),
    m_ModRed(0), m_ModGreen(0), m_ModBlue(0), m_ModAlpha(0), m_ColorModTime(0), m_pOwnerLayer(NULL), m_uiGUID(uiGUID), m_pAI(pAI)
{
    m_sLogLocationName  = LOGFILE_ENGINE_LOG_NAME + "WorldObject : ";
    m_UnitType          = UNIT_TYPE_WORLDOBJECT;
    m_pAI->SetOwner(this);
}

WorldObject::~WorldObject(void)
{
    if (m_pAI)
    {
        delete m_pAI;
        m_pAI = NULL;
    }
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
        pSprite->Draw(m_pTexture->m_pTexture, NULL, NULL, &D3DXVECTOR3((float)GetScreenPosX(), (float)GetScreenPosY(), 0), GetColor());
}

int WorldObject::GetScreenPosX()
{
    if (m_pOwnerLayer)
    {
        if (Map *pMap = m_pOwnerLayer->GetOwnerMap())
        {
            return GetPositionX() + (int)pMap->GetPositionX();
        }
    }
    return 0;
}

int WorldObject::GetScreenPosY()
{
    if (m_pOwnerLayer)
    {
        if (Map *pMap = m_pOwnerLayer->GetOwnerMap())
        {
            return GetPositionY() + (int)pMap->GetPositionY();
        }
    }
    return 0;
}

void WorldObject::GetBoundingRect(RECT &bound)
{
    if (!m_pTexture)
        return;

    switch (m_pTexture->m_TextureInfo.m_uiSpriteType)
    {
    case SPRITE_TYPE_OBJECT:
    case SPRITE_TYPE_ANIMATED_OBJECT:
        bound.left = m_pTexture->m_TextureInfo.Type.Object.m_uiBoundingXBegin;
        bound.right = bound.left + m_pTexture->m_TextureInfo.Type.Object.m_uiBoundingXRange;
        bound.top = m_pTexture->m_TextureInfo.Type.Object.m_uiBoundingYBegin;
        bound.bottom = bound.top + m_pTexture->m_TextureInfo.Type.Object.m_uiBoundingYRange;
        break;
    default:
        break;
    }
}

Map* WorldObject::GetMap()
{
    if (m_pOwnerLayer)
        return m_pOwnerLayer->GetOwnerMap();

    return NULL;
}