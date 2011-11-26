#include "Layer.h"

Layer::Layer(void)
{

    m_LayerType         = LAYER_TYPE_LAYER;
    m_sLogLocationName  = LOGFILE_ENGINE_LOG_NAME + "Layer : ";
}

Layer::~Layer(void)
{
    // delete all world objects
    for (ObjectList::iterator itr = m_lObjects.begin(); itr != m_lObjects.end(); ++itr)
    {
        if (*itr)
            delete *itr;
    }

    m_lObjects.clear();
}

void Layer::DrawLayer()
{
    Draw();
}