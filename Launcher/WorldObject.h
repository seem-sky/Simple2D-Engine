#ifndef WORLDOBJECT_H
#define WORLDOBJECT_H

#include "Texture.h"
#include <d3dx9.h>
#include "Point.h"
#include "WrapperFunctions.h"
#include "ObjectAI.h"
#include "Database.h"

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
    WorldObject(uint32 m_uiGUID, Point<int> pos, ObjectAI* pAI = new ObjectAI());
    virtual ~WorldObject(void);

    // object info
    inline uint32 GetGUID() { return m_uiGUID; }
    inline DATABASE::ObjectPrototype* GetObjectInfo() { return &m_ObjectInfo; }
    virtual void SetObjectInfo(const DATABASE::ObjectPrototype* pInfo) { if (pInfo) m_ObjectInfo = *pInfo; }

    UNIT_TYPE GetObjectType() { return m_UnitType; }

    inline void SetOwnerLayer(ObjectLayer *pOwner)
    {
        if (!pOwner)
            return;

        m_pOwnerLayer = pOwner;
    }
    inline ObjectLayer* GetOwnerLayer() { return m_pOwnerLayer; }

    void GetBoundingRect(RECT &bound);

    // position
    inline void ChangePosition(int XPos, int32 YPos) { m_Position += Point<int>(XPos, YPos); }
    inline Point<int> GetPosition() { return m_Position; }
    inline Point<int>* GetPositionPtr() { return &m_Position; }
    inline int32 GetPositionX() { return m_Position.x; }
    inline int32 GetPositionY() { return m_Position.y; }
    inline int32 GetBottomPosY()
    {
        uint32 x = 0, y = 0;
        GetObjectSize(x, y);
        return GetPositionY() + y;
    }
    int32 GetScreenPosX();
    int32 GetScreenPosY();

    void GetObjectSize(uint32 &Xsize, uint32 &Ysize);
    Map* GetMap();

    // Color
    inline D3DXCOLOR GetColor() { return m_Color; }
    inline void SetColor(float red, float green, float blue, float alpha) { m_Color = D3DXCOLOR(red, green, blue, alpha); }
    void ModifyColorTo(float red, float green, float blue, float alpha, uint32 time = 0);

    // texture
    virtual void SetTextureSource(const DATABASE::SpritePrototype *proto);
    inline TextureSource* GetTextureSource() { return m_pTexture; }

    // Update
    virtual void Update(const ULONGLONG uiCurTime, const uint32 uiDiff);

    virtual void DrawObject(LPD3DXSPRITE pSprite);

    // AI
    virtual ObjectAI* GetAI() { return m_pAI; }
    virtual void SetAI(ObjectAI* AI)
    {
        if (AI)
            m_pAI = AI;
    }

protected:
    // object type
    UNIT_TYPE m_UnitType;
    std::string m_sLogLocationName;

private:
    void UpdateColor(const uint32 uiDiff);

    ObjectLayer *m_pOwnerLayer;

    // modify color variables
    float m_ModRed, m_ModGreen, m_ModBlue, m_ModAlpha;
    uint32 m_ColorModTime;
    TextureSource* m_pTexture;
    DATABASE::ObjectPrototype m_ObjectInfo;
    Point<int> m_Position;
    D3DXCOLOR m_Color;

    uint32 m_uiGUID;

    ObjectAI *m_pAI;
};
#endif;
