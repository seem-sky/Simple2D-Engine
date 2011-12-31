#ifndef WORLDOBJECT_H
#define WORLDOBJECT_H

#include "MovementGenerator.h"
#include "Texture.h"

enum UNIT_TYPE
{
    UNIT_TYPE_WORLDOBJECT,
    UNIT_TYPE_UNIT,
};

class ObjectLayer;

class WorldObject
{
public:
    WorldObject(D3DXVECTOR3 pos);
    virtual ~WorldObject(void);

    // object info
    inline ObjectPrototype* GetObjectInfo() { return &m_ObjectInfo; }
    virtual void SetObjectInfo(const ObjectPrototype* pInfo) { if (pInfo) m_ObjectInfo = *pInfo; }

    inline void SetOwnerLayer(ObjectLayer *pOwner)
    {
        if (!pOwner)
            return;

        m_pOwnerLayer = pOwner;
    }
    inline ObjectLayer* GetOwnerLayer() { return m_pOwnerLayer; }

    // position
    inline void ChangePosition(int XPos, int YPos) { m_v3Position += D3DXVECTOR3((float)XPos, (float)YPos, 0); }
    inline D3DXVECTOR3 GetPosition() { return m_v3Position; }
    inline D3DXVECTOR3* GetPositionPtr() { return &m_v3Position; }
    inline int GetPositionX() { return (int)m_v3Position.x; }
    inline int GetPositionY() { return (int)m_v3Position.y; }
    inline int GetBottomPosY()
    {
        UINT x = 0, y = 0;
        GetObjectSize(x, y);
        return GetPositionY() + y;
    }
    UINT GetMapPosX();
    UINT GetMapPosY();

    void GetObjectSize(UINT &Xsize, UINT &Ysize);

    // Color
    inline D3DXCOLOR GetColor() { return m_Color; }
    inline void SetColor(float red, float green, float blue, float alpha) { m_Color = D3DXCOLOR(red, green, blue, alpha); }
    void ModifyColorTo(float red, float green, float blue, float alpha, UINT time = 0);

    // texture
    virtual void SetTextureSource(const SpritePrototype *proto);
    inline TextureSource* GetTextureSource() { return m_pTexture; }

    // Update
    virtual void Update(const ULONGLONG uiCurTime, const UINT uiDiff);

    virtual void DrawObject(LPD3DXSPRITE pSprite);

protected:
    // object type
    UNIT_TYPE m_UnitType;
    std::string m_sLogLocationName;

private:
    void UpdateColor(const UINT uiDiff);

    ObjectLayer *m_pOwnerLayer;

    // modify color variables
    float m_ModRed, m_ModGreen, m_ModBlue, m_ModAlpha;
    UINT m_ColorModTime;
    TextureSource* m_pTexture;
    ObjectPrototype m_ObjectInfo;
    D3DXVECTOR3 m_v3Position;
    D3DXCOLOR m_Color;
};
#endif;
