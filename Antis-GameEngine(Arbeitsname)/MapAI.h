#ifndef MAP_AI_H
#define MAP_AI_H

#include "WorldObject.h"
#include "AI_Manager.h"

class MapAI
{
public:
    MapAI() {};
    virtual ~MapAI() {};

    virtual void OnLoad() {}
    virtual void OnClose() {}
    virtual void OnObjectCreate(WorldObject* p_pWho) {}
    virtual void OnObjectDelete(WorldObject* p_pWho) {}
};
#endif