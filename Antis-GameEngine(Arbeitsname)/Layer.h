#ifndef LAYER_H
#define LAYER_H

#include "Direct3D.h"
#include "Logfile.h"
#include <list>

enum LAYER_TYPE
{
    LAYER_TYPE_LAYER,
    LAYER_TYPE_OBJECT_LAYER,
};

class Layer
{
public:
    Layer(void);
    ~Layer(void);

    LAYER_TYPE GetLayerType() { return m_LayerType; }

    void DrawLayer();
    virtual void UpdateLayer(const UINT uiCurTime, const UINT uiDiff) { }

protected:
    virtual void Draw() { }

    LPD3DXSPRITE m_pSprite;
    CDirect3D *m_pDirect3D;
    CLogfile *m_pLogfile;
    LAYER_TYPE m_LayerType;
    std::string m_sLogLocationName;

private:
    void Layer::BeginDraw(UINT DrawFlag = D3DXSPRITE_ALPHABLEND) { m_pSprite->Begin(DrawFlag); }
    void Layer::EndDraw() { m_pSprite->End(); }
};
#endif;
