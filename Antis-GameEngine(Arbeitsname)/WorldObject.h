#ifndef WORLDOBJECT_H
#define WORLDOBJECT_H

#include "Texture.h"
#include <d3dx9.h>
#include "Point.h"

enum UNIT_TYPE
{
    UNIT_TYPE_WORLDOBJECT,
    UNIT_TYPE_UNIT,
};

class ObjectLayer;
class Map;

class WorldObject
{
public:
    WorldObject(UINT m_uiGUID, Point<int> pos);
    virtual ~WorldObject(void);

    // object info
    inline UINT GetGUID() { return m_uiGUID; }
    inline ObjectPrototype* GetObjectInfo() { return &m_ObjectInfo; }
    virtual void SetObjectInfo(const ObjectPrototype* pInfo) { if (pInfo) m_ObjectInfo = *pInfo; }

    inline void SetOwnerLayer(ObjectLayer *pOwner)
    {
        if (!pOwner)
            return;

        m_pOwnerLayer = pOwner;
    }
    inline ObjectLayer* GetOwnerLayer() { return m_pOwnerLayer; }

    void GetBoundingRect(RECT &bound);

    // position
    inline void ChangePosition(int XPos, int YPos) { m_Position += Point<int>(XPos, YPos); }
    inline Point<int> GetPosition() { return m_Position; }
    inline Point<int>* GetPositionPtr() { return &m_Position; }
    inline int GetPositionX() { return m_Position.x; }
    inline int GetPositionY() { return m_Position.y; }
    inline int GetBottomPosY()
    {
        UINT x = 0, y = 0;
        GetObjectSize(x, y);
        return GetPositionY() + y;
    }
    int GetScreenPosX();
    int GetScreenPosY();

    void GetObjectSize(UINT &Xsize, UINT &Ysize);
    Map* GetMap();

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
    Point<int> m_Position;
    D3DXCOLOR m_Color;

    UINT m_uiGUID;
};
#endif;
