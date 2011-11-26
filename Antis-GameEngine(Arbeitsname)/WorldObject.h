#ifndef WORLDOBJECT_H
#define WORLDOBJECT_H

#include "RessourceManager.h"
#include "MovementGenerator.h"

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

    // position
    D3DXVECTOR3 GetPosition() { return m_v3Position; }
    int GetPositionX() { return (int)m_v3Position.x; }
    int GetPositionY() { return (int)m_v3Position.y; }
    void SetPosition(D3DXVECTOR2 v2NewPos);
    void MovePosition(int XMove, int YMove, UINT time = 0);
    void SetPositionX(int newPos) { m_v3Position.x = (float)newPos; }
    void SetPositionY(int newPos) { m_v3Position.y = (float)newPos; }
    void GetObjectSize(UINT &Xsize, UINT &Ysize);

    // Color
    D3DXCOLOR GetColor() { return m_Color; }
    void SetColor(float red, float green, float blue, float alpha) { m_Color = D3DXCOLOR(red, green, blue, alpha); }
    void ModifyColorTo(float red, float green, float blue, float alpha, UINT time = 0);

    // texture
    void SetTextureSource(std::string sTextureName);
    TextureSource* GetTextureSource() { return m_pTexture; }

    // Update
    virtual void Update(const UINT uiCurTime, const UINT uiDiff);

protected:
    TextureSource* m_pTexture;
    D3DXVECTOR3 m_v3Position;
    UNIT_TYPE m_UnitType;
    std::string m_sLogLocationName;
    D3DXCOLOR m_Color;

    MovementGenerator *m_pMovement;

private:
    void UpdateColor(const UINT uiDiff);

    // modify color variables
    float m_ModRed, m_ModGreen, m_ModBlue, m_ModAlpha;
    UINT m_ColorModTime;
};
#endif;
