#ifndef WORLDOBJECT_H
#define WORLDOBJECT_H

#include "MovementGenerator.h"
#include "Texture.h"

enum UNIT_TYPE
{
    UNIT_TYPE_WORLDOBJECT,
    UNIT_TYPE_UNIT,
};

class WorldObject
{
public:
    WorldObject(void);
    virtual ~WorldObject(void);

    // object info
    inline ObjectPrototype* GetObjectInfo() { return &m_ObjectInfo; }
    inline void SetObjectInfo(const ObjectPrototype* pInfo) { if (pInfo) m_ObjectInfo = *pInfo; }

    // position
    inline D3DXVECTOR3 GetPosition() { return m_v3Position; }
    inline D3DXVECTOR3* GetPositionPtr() { return &m_v3Position; }
    inline int GetPositionX() { return (int)m_v3Position.x; }
    inline int GetPositionY() { return (int)m_v3Position.y; }
    void SetPosition(D3DXVECTOR2 v2NewPos)
    {
        m_v3Position.x = v2NewPos.x;
        m_v3Position.y = v2NewPos.y;
    }
    inline void SetPositionX(int newPos) { m_v3Position.x = (float)newPos; }
    inline void SetPositionY(int newPos) { m_v3Position.y = (float)newPos; }
    void GetObjectSize(UINT &Xsize, UINT &Ysize);

    // Color
    inline D3DXCOLOR GetColor() { return m_Color; }
    inline void SetColor(float red, float green, float blue, float alpha) { m_Color = D3DXCOLOR(red, green, blue, alpha); }
    void ModifyColorTo(float red, float green, float blue, float alpha, UINT time = 0);

    // texture
    virtual void SetTextureSource(const SpritePrototype *proto);
    inline TextureSource* GetTextureSource() { return m_pTexture; }

    // Update
    virtual void Update(const UINT uiCurTime, const UINT uiDiff);

    virtual void DrawObject(LPD3DXSPRITE pSprite);

protected:
    // object type
    UNIT_TYPE m_UnitType;
    std::string m_sLogLocationName;

private:
    void UpdateColor(const UINT uiDiff);

    // modify color variables
    float m_ModRed, m_ModGreen, m_ModBlue, m_ModAlpha;
    UINT m_ColorModTime;
    TextureSource* m_pTexture;
    ObjectPrototype m_ObjectInfo;
    D3DXVECTOR3 m_v3Position;
    D3DXCOLOR m_Color;
};
#endif;
