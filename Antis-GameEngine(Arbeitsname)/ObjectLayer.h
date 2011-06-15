#ifndef OBJECTLAYER_H
#define OBJECTLAYER_H

#include "Layer.h"
#include "Unit.h"

typedef std::list<WorldObject*> WorldObjectList;

class ObjectLayer : public Layer
{
public:
    ObjectLayer(void);
    ~ObjectLayer(void);
    void ClearObjectList();
    void AddWorldObject();
    void UpdateLayer(const UINT uiCurTime, const UINT uiDiff);

protected:
    void Draw();

    WorldObjectList m_lObjects;
};

#endif;
