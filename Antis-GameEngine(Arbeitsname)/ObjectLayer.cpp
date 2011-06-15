#include "ObjectLayer.h"

ObjectLayer::ObjectLayer(void) : Layer()
{
    m_sLogLocationName  = LOGFILE_ENGINE_LOG_NAME + "ObjectLayer : ";
    m_LayerType         = LAYER_TYPE_OBJECT_LAYER;
}

ObjectLayer::~ObjectLayer(void)
{
    ClearObjectList();
    if (m_pSprite)
        m_pSprite->Release();
}

void ObjectLayer::Draw()
{
    for (WorldObjectList::const_iterator itr = m_lObjects.begin(); itr != m_lObjects.end(); ++itr)
    {
        m_pSprite->Draw((*itr)->GetTexture()->m_pTexture, NULL, NULL, &(*itr)->GetPosition(), D3DXCOLOR(0, 125, 125, 125));
    }
}

void ObjectLayer::ClearObjectList()
{
    for (WorldObjectList::const_iterator itr = m_lObjects.begin(); itr != m_lObjects.end(); ++itr)
    {
        delete *itr;
    }
}

void ObjectLayer::AddWorldObject()
{
    Unit* pObject = new Unit();
    pObject->SetTexture("asymptome.png");
    m_lObjects.push_back(pObject);
}

void ObjectLayer::UpdateLayer(const UINT uiCurTime, const UINT uiDiff)
{
    for (WorldObjectList::const_iterator itr = m_lObjects.begin(); itr != m_lObjects.end(); ++itr)
        (*itr)->UpdateObject(uiCurTime, uiDiff);
}
