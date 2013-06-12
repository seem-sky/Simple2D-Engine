#include "MapSettings.h"
#include "moc_MapSettings.h"
#include "MapTree.h"

MapSettings::MapSettings(const MAP::MapPrototypePtr &map, QWidget *pParent) : m_pMap(map), QDialog(pParent), Ui_MapSettings()
{
    setupUi(this);
    connect(m_pButtons, SIGNAL(accepted()), this, SLOT(_accepted()));
    updateWidgets();
}

void MapSettings::updateWidgets()
{
    if (!m_pMap)
        return;

    m_pID->setValue(m_pMap->getID());
    m_pFileName->setText(m_pMap->getFileName());
    m_pName->setText(m_pMap->getName());
    m_pScriptName->setText(m_pMap->getScriptName());
    m_pSizeX->setValue(m_pMap->getSize().x);
    m_pSizeY->setValue(m_pMap->getSize().y);
    m_pBackgroundLayer->setValue(m_pMap->getLayerSize(MAP::LAYER_BACKGROUND));
    m_pForegroundLayer->setValue(m_pMap->getLayerSize(MAP::LAYER_FOREGROUND));
}

void MapSettings::_accepted()
{
    if (!m_pMap)
        return;
    m_pMap->setName(m_pName->text());
    m_pMap->setScriptName(m_pScriptName->text());
    m_pMap->setSize(Point3D<uint32>(m_pSizeX->value(), m_pSizeY->value()), m_pForegroundLayer->value(), m_pBackgroundLayer->value());
}