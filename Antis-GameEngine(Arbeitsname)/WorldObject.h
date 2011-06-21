#ifndef WORLDOBJECT_H
#define WORLDOBJECT_H

#include "RessourceManager.h"

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
    void SetPosition(D3DXVECTOR2 v2NewPos);

    // Color
    D3DXCOLOR GetColor() { return m_Color; }
    void SetColor(float red, float green, float blue, float alpha) { m_Color = D3DXCOLOR(red, green, blue, alpha); }
    void MoveColorTo(float red, float green, float blue, float alpha, UINT time);

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

private:
    void UpdateColor(const UINT uiDiff);

    // modify color variables
    float m_ModRed, m_ModGreen, m_ModBlue, m_ModAlpha;
    UINT m_ColorModTime;
};
#endif;
