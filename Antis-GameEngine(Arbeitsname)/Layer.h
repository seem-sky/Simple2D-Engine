#ifndef LAYER_H
#define LAYER_H

#include "Direct3D.h"
#include "Logfile.h"
#include <list>
#include "RessourceManager.h"

enum LAYER_TYPE
{
    LAYER_TYPE_LAYER,
    LAYER_TYPE_OBJECT_LAYER,
    LAYER_TYPE_MAP,
};

class Layer
{
public:
    Layer(void);
    virtual ~Layer(void);

    LAYER_TYPE GetLayerType() { return m_LayerType; }

    void DrawLayer();
    virtual void UpdateLayer(const UINT uiCurTime, const UINT uiDiff) { }

    HRESULT CreateSprite();

protected:
    virtual void Draw() { }

    LPD3DXSPRITE m_pSprite;
    CLogfile *m_pLogfile;
    CRessourceManager *m_pRessourceManager;
    LAYER_TYPE m_LayerType;
    std::string m_sLogLocationName;  

private:
    void BeginDraw(UINT DrawFlag = D3DXSPRITE_ALPHABLEND);
    void EndDraw();

    CDirect3D *m_pDirect3D;
};
#endif;
