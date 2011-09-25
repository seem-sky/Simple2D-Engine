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
        if ((*itr)->GetTextureSource() && (*itr)->GetTextureSource()->GetTexture())
            m_pSprite->Draw((*itr)->GetTextureSource()->GetTexture(), NULL, NULL, &(*itr)->GetPosition(), (*itr)->GetColor());
    }
}

void ObjectLayer::ClearObjectList()
{
    for (WorldObjectList::const_iterator itr = m_lObjects.begin(); itr != m_lObjects.end(); ++itr)
        delete (*itr);
}

Unit* ObjectLayer::AddWorldObject(std::string sTextureName, D3DXVECTOR2 v2Position)
{
    Unit* pObject = new Unit();
    pObject->SetPosition(v2Position);
    pObject->SetTextureSource(sTextureName);
    m_lObjects.push_back(pObject);
	return pObject;
}

void ObjectLayer::UpdateLayer(const UINT uiCurTime, const UINT uiDiff)
{
    for (WorldObjectList::const_iterator itr = m_lObjects.begin(); itr != m_lObjects.end(); ++itr)
        (*itr)->Update(uiCurTime, uiDiff);
}
