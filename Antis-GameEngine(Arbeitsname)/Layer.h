#ifndef LAYER_H
#define LAYER_H

#include "Direct3D.h"
#include "WorldObject.h"

enum LAYER_TYPE
{
    LAYER_TYPE_LAYER,
    LAYER_TYPE_OBJECT_LAYER,
    LAYER_TYPE_MAP,
};

class Layer;
typedef std::vector<Layer*> LayerList;
typedef std::vector<WorldObject*> ObjectList;

class Layer
{
public:
    Layer(void);
    virtual ~Layer(void);

    LAYER_TYPE GetLayerType() { return m_LayerType; }

    void DrawLayer();
    virtual void UpdateLayer(const ULONGLONG uiCurTime, const uint32 uiDiff) = 0;

protected:
    virtual void Draw() = 0;

    LAYER_TYPE m_LayerType;
    std::string m_sLogLocationName;  

    ObjectList m_lObjects;

private:
};
#endif;
