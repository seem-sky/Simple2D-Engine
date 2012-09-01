#ifndef MAPLAYER_H
#define MAPLAYER_H

#include "layer.h"
#include "Map.h"

class MapLayer : public Layer
{
public:
    MapLayer(void);
    ~MapLayer(void);
    void UpdateLayer(const uint32 uiCurTime, const uint32 uiDiff);

protected:
};
#endif