#ifndef MAPLAYER_H
#define MAPLAYER_H

#include "layer.h"
#include "Map.h"

class MapLayer : public Layer
{
public:
    MapLayer(void);
    ~MapLayer(void);
    void UpdateLayer(const UINT uiCurTime, const UINT uiDiff);

protected:
};
#endif