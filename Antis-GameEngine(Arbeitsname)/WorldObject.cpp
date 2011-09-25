#include "WorldObject.h"

WorldObject::WorldObject(void)
{
    m_sLogLocationName  = LOGFILE_ENGINE_LOG_NAME + "WorldObject : ";
    m_pTexture          = NULL;
    m_v3Position.x      = 0;
    m_v3Position.y      = 0;
    m_v3Position.z      = 0;
    m_Color             = D3DXCOLOR(1, 1, 1, 1);

    m_UnitType          = UNIT_TYPE_WORLDOBJECT;

    m_ModRed            = 0;
    m_ModGreen          = 0;
    m_ModBlue           = 0;
    m_ModAlpha          = 0;
    m_ColorModTime      = 0;
}

WorldObject::~WorldObject(void)
{
}

void WorldObject::SetTextureSource(std::string sTextureName)
{
    CRessourceManager *pRManager = CRessourceManager::Get();
    if (!pRManager)
        return;

    m_pTexture = pRManager->GetCharsetTexture(sTextureName);
}

void WorldObject::SetPosition(D3DXVECTOR2 v2NewPos)
{
    m_v3Position.x = v2NewPos.x;
    m_v3Position.y = v2NewPos.y;
}

void WorldObject::Update(const UINT uiCurTime, const UINT uiDiff)
{
    // Update Color
    if (m_ColorModTime)
        UpdateColor(uiDiff);
}

void WorldObject::MoveColorTo(float red, float green, float blue, float alpha, UINT time)
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