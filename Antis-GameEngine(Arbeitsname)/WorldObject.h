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

    void SetTexture(std::string sTextureName);
    D3DXVECTOR3 GetPosition() { return m_v3Position; }
    sTextureSource* GetTexture() { return m_pTexture; }
    void UpdateObject(const UINT uiCurTime, const UINT uiDiff);

protected:
    virtual void Update(const UINT uiCurTime, const UINT uiDiff) { }

    sTextureSource* m_pTexture;
    D3DXVECTOR3 m_v3Position;
    UNIT_TYPE m_UnitType;
};
#endif;
