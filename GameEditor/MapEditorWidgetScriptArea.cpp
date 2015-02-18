#include "MapEditorWidgetScriptArea.h"
#include "moc_MapEditorWidgetScriptArea.h"
#include <QtWidgets/QGridLayout>

MapEditorWidgetScriptArea::MapEditorWidgetScriptArea(QWidget* pParent)
    : ToolWidget(pParent), Ui_MapEditorWidgetScriptAreaMapping()
{
    setupUi(this);

    resize(0, 0);

    // layout
    auto pLayout = new QGridLayout(this);
    pLayout->setContentsMargins(0, 0, 0, 0);
    pLayout->addWidget(m_pTriggerTypeGroup, 0, 0);
    pLayout->addWidget(m_pRepeatTypeGroup, 1, 0);
    pLayout->addWidget(m_pScriptGroup, 2, 0);
    setLayout(pLayout);

    // script layout
    auto pScriptLayout = new QGridLayout(m_pScriptGroup);
    pScriptLayout->addWidget(m_pScriptType, 0, 0);
    pScriptLayout->addWidget(m_pCustomScriptWidget, 1, 0);
    pScriptLayout->addWidget(m_pTeleportWidget, 1, 0);
    m_pScriptGroup->setLayout(pScriptLayout);

    // connections
    connect(m_pScriptType, SIGNAL(currentIndexChanged(int)), this, SLOT(_onScriptTypeChanged(int)));

    setup();
}

void MapEditorWidgetScriptArea::setup()
{
    m_pCustomScriptWidget->hide();
    m_pTeleportWidget->hide();
    switch (static_cast<MAP::SCRIPT_AREA::ACTION::Type>(m_pScriptType->currentIndex() + 1))
    {
    case MAP::SCRIPT_AREA::ACTION::Type::teleport:
        m_pTeleportWidget->show();
        break;
    case MAP::SCRIPT_AREA::ACTION::Type::custom_script:
        m_pCustomScriptWidget->show();
        break;
    }
}

void MapEditorWidgetScriptArea::_onScriptTypeChanged(int index)
{
    setup();
}
