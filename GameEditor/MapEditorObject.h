#ifndef MAP_EDITOR_OBJECT_H
#define MAP_EDITOR_OBJECT_H

#include "SharedMapEditorData.h"

class MapEditorObject
{
public:
    virtual void setSharedData(SharedMapEditorDataPtr pData) { m_pSharedData = pData; }
    virtual void updateObject() = 0;

protected:
    SharedMapEditorDataPtr m_pSharedData;
};
#endif