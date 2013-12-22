#include "MapEditorModuleContent.h"
#include "moc_MapEditorModuleContent.h"

MapEditorModuleContent::MapEditorModuleContent(QWidget *pWidget) : QWidget(pWidget), Ui_MapEditorModuleContent()
{
    setupUi(this);

    connect(this, SIGNAL(currentChanged(int)), this, SLOT(_onCurrentChanged(int)));
    connect(m_pZoom, SIGNAL(valueChanged(int)), this, SLOT(_onZoomChanged(int)));
    connect(m_pLayer, SIGNAL(valueChanged(int)), this, SLOT(_onLayerChanged(int)));
    connect(m_pLayerBackground, SIGNAL(clicked()), this, SLOT(_onLayerTypeChanged()));
    connect(m_pLayerBackground, SIGNAL(clicked()), this, SLOT(_onLayerTypeChanged()));
}

void MapEditorModuleContent::_onCurrentChanged(int index)
{
}

void MapEditorModuleContent::_onZoomChanged(int value)
{
}

void MapEditorModuleContent::_onLayerChanged(int value)
{
}

void MapEditorModuleContent::_onLayerTypeChanged()
{
}
