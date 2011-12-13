#include "Unit.h"
#include "RessourceManager.h"

Unit::Unit(void) : m_uiDirection(DIRECTION_DOWN), m_uiSpriteSector(0), m_bIsPlayer(false), m_bAnimationDirection(false), m_bAllTimeAnimation(false),
    m_uiAnimationTime(ANIMATION_TIME_NORMAL), m_uiAnimation_Timer(m_uiAnimationTime), m_pMovement(new MovementGenerator((D3DXVECTOR2*)GetPositionPtr())),
    WorldObject()
{
    m_sLogLocationName      = LOGFILE_ENGINE_LOG_NAME + "Unit : ";
    m_UnitType              = UNIT_TYPE_UNIT;
}

Unit::~Unit(void)
{
    if (m_pMovement)
    {
        m_pMovement->ClearMovement();
        delete m_pMovement;
    }
}

void Unit::Update(const UINT uiCurTime, const UINT uiDiff)
{
    // WorldObject update
    WorldObject::Update(uiCurTime, uiDiff);

    // Update movement
    if (m_pMovement && !m_pMovement->IsMoveCommandListEmpty())
        m_pMovement->UpdateMovement(uiCurTime, uiDiff);
    else
        m_uiSpriteSector = m_uiDirection * GetTextureSource()->m_TextureInfo.Type.AnimatedObject.m_uiSpritesX + 1;

    // update animation
    if ((m_pMovement && !m_pMovement->IsMoveCommandListEmpty()) || m_bAllTimeAnimation)
        UpdateAnimation(uiCurTime, uiDiff);
}

void Unit::DrawObject(LPD3DXSPRITE pSprite)
{
    if (!GetTextureSource() || !GetTextureSource()->m_pTexture || !pSprite)
        return;

    // get sprite size and calc sprite sector
    UINT uiSizeX = 0, uiSizeY = 0;
    GetTextureSource()->GetTextureSize(uiSizeX, uiSizeY);
    RECT srcRect = {0,0,uiSizeX,uiSizeY};
    if (GetTextureSource()->m_TextureInfo.m_uiSpriteType == (UINT)SPRITE_TYPE_AIMATED_OBJECT)
    {
        UINT uiSpritesX = GetTextureSource()->m_TextureInfo.Type.AnimatedObject.m_uiSpritesX;
        UINT uiSpritesY = GetTextureSource()->m_TextureInfo.Type.AnimatedObject.m_uiSpritesY;

        if (uiSpritesX)
        {
            srcRect.left = (m_uiSpriteSector % uiSpritesX)*(uiSizeX / uiSpritesX);
            srcRect.right = srcRect.left + uiSizeX / uiSpritesX;
        }

        if (uiSpritesY)
        {
            srcRect.top = (m_uiSpriteSector / uiSpritesX)*(uiSizeY / uiSpritesY);
            srcRect.bottom = srcRect.top + uiSizeY / uiSpritesY;
        }
    }

    pSprite->Draw(GetTextureSource()->m_pTexture, &srcRect, NULL, &GetPosition(), GetColor());
}

void Unit::SetTextureSource(const SpritePrototype *proto)
{
    WorldObject::SetTextureSource(proto);

    // set some special information for unit
    if (GetTextureSource())
    {
        if (GetTextureSource()->m_TextureInfo.m_uiSpriteType == (UINT)SPRITE_TYPE_AIMATED_OBJECT)
            m_uiSpriteSector = m_uiDirection * GetTextureSource()->m_TextureInfo.Type.AnimatedObject.m_uiSpritesX + 1;

        else
            ERROR_LOG(m_sLogLocationName + "Set Texture " + GetTextureSource()->m_TextureInfo.m_sFileName + " is not type SPRITE_TYPE_AIMATED_OBJECT, so "+
                        "walkanimation are not going to be shown.");
    }
}

void Unit::MovePosition(int XMove, int YMove, UINT time)
{
    if (m_pMovement)
        m_pMovement->Move2DWithoutCollision(XMove, YMove, time);
}

void Unit::UpdateAnimation(const UINT uiCurTime, const UINT uiDiff)
{
    // check animation timer
    if (m_uiAnimation_Timer < uiDiff)
    {
        TextureSource *pTexture = GetTextureSource();
        if (!pTexture)
        {
            ERROR_LOG(m_sLogLocationName + "Animation failed. No valid texture set.");
            return;
        }

        UINT uiSector = GetTextureSrcRct();
        UINT uiSpritesX = pTexture->m_TextureInfo.Type.AnimatedObject.m_uiSpritesX;

        // check src rect space
        if (m_bAnimationDirection)
        {
            if ((uiSector+1) % uiSpritesX == 0)
                m_bAnimationDirection = false;
        }
        else
        {
            if ((uiSector) % uiSpritesX == 0)
                m_bAnimationDirection = true;
        }

        // animation update
        if (m_bAnimationDirection)
            uiSector++;
        else
            uiSector--;

        SetTextureSrcRct(uiSector);

        m_uiAnimation_Timer = m_uiAnimationTime;
    }
    else
        m_uiAnimation_Timer -= uiDiff;
}