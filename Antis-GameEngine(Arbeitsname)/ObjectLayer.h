#ifndef OBJECTLAYER_H
#define OBJECTLAYER_H

#include "Layer.h"
#include "Unit.h"

typedef std::list<WorldObject*> WorldObjectList;

class Map;

class ObjectLayer : public Layer
{
public:
    ObjectLayer(void);
    ~ObjectLayer(void);
    void ClearObjectList();
    void AddWorldObject(WorldObject* pObject);
    void UpdateLayer(const ULONGLONG uiCurTime, const UINT uiDiff);

    inline void SetOwnerMap(Map *pMap)
    {
        if (!pMap)
            return;

        m_pOwnerMap = pMap;
    }
    inline Map* GetOwnerMap() { return m_pOwnerMap; }

    void ModObjectPosition(D3DXVECTOR2 oldPos, D3DXVECTOR2 newPos, WorldObject *pObj);

protected:
    void Draw();
    bool IsObjectVisible(WorldObject *pObject);

    WorldObjectList m_lObjects;
    std::map<int, std::vector<WorldObject*>> m_v2VisibleObjects;
    Map *m_pOwnerMap;
};
#endif;