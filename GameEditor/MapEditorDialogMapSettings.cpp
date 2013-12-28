#include "MapEditorDialogMapSettings.h"
#include "moc_MapEditorDialogMapSettings.h"


MapEditorDialogMapSettings::MapEditorDialogMapSettings(DATABASE::MAP_STRUCTURE::MapPrototype* pPrototype, QWidget* pParent) : QDialog(pParent),
    Ui_MapEditorDialogMapSettings(), m_pPrototype(pPrototype)
{
    setupUi(this);
    _setup();

    connect(this, SIGNAL(accepted()), this, SLOT(_onAccepted()));
}

void MapEditorDialogMapSettings::_setup()
{
    if (!m_pPrototype)
        return;

    m_pID->setValue(m_pPrototype->getID());
    m_pName->setText(m_pPrototype->getName());
    m_pScriptName->setText(m_pPrototype->getScriptName());
    m_pFileName->setText(m_pPrototype->getFileName());
    m_pSizeX->setValue(m_pPrototype->getSize().x);
    m_pSizeY->setValue(m_pPrototype->getSize().y);
    m_pBackgroundLayer->setValue(m_pPrototype->getLayerSize(MAP::Layer::LAYER_BACKGROUND));
    m_pForegroundLayer->setValue(m_pPrototype->getLayerSize(MAP::Layer::LAYER_FOREGROUND));
}

void MapEditorDialogMapSettings::_save()
{
    if (!m_pPrototype)
        return;

    m_pPrototype->setName(m_pName->text());
    m_pPrototype->setScriptName(m_pScriptName->text());
    m_pPrototype->setSizeX(m_pSizeX->value());
    m_pPrototype->setSizeY(m_pSizeY->value());
    m_pPrototype->setLayerSize(m_pBackgroundLayer->value(), MAP::Layer::LAYER_BACKGROUND);
    m_pPrototype->setLayerSize(m_pForegroundLayer->value(), MAP::Layer::LAYER_FOREGROUND);
}

void MapEditorDialogMapSettings::_onAccepted()
{
    _save();
}
