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
    void UpdateLayer(const UINT uiCurTime, const UINT uiDiff);

    void SetOwnerMap(Map *pMap) { m_pOwnerMap = pMap; }

protected:
    void Draw();
    bool IsObjectVisible(WorldObject *pObject);

    WorldObjectList m_lObjects;
    std::map<int, std::vector<WorldObject*>> m_v2VisibleObjects;
    Map *m_pOwnerMap;
};
#endif;