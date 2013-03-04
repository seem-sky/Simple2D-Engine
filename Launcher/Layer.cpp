#include "Layer.h"

Layer::Layer(void) : m_LayerType(LAYER_TYPE_LAYER)
{
    m_sLogLocationName  = LOGFILE_ENGINE_LOG_NAME + "Layer : ";
}

Layer::~Layer(void)
{
}

void Layer::DrawLayer()
{
    Draw();
}