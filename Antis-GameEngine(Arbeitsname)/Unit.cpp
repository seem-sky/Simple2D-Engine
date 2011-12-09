#include "Unit.h"
#include "RessourceManager.h"

Unit::Unit(void)
{
    m_sLogLocationName  = LOGFILE_ENGINE_LOG_NAME + "Unit : ";
    m_UnitType          = UNIT_TYPE_UNIT;

    m_pMovement         = new MovementGenerator((D3DXVECTOR2*)GetPositionPtr());

    m_uiDirection       = DIRECTION_DOWN;
    m_uiSpriteSector    = 0;
    
}

Unit::~Unit(void)
{
    if (m_pMovement)
        delete m_pMovement;
}

void Unit::Update(const UINT uiCurTime, const UINT uiDiff)
{
    // WorldObject update
    WorldObject::Update(uiCurTime, uiDiff);

    // Update movement
    if (m_pMovement)
        m_pMovement->UpdateMovement(uiCurTime, uiDiff);
    //if (!m_pMovement->IsMoveCommandListEmpty())
    //    m_v3Position += m_pMovement->UpdateMovement(uiCurTime, uiDiff);
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