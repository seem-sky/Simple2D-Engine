#include "DatabaseDialog.h"
#include "moc_DatabaseDialog.h"
#include <Database/Manager.h>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QGridLayout>
#include "DatabaseSectionTile.h"
#include "DatabaseSectionAutoTile.h"
#include "DatabaseSectionTileSet.h"
#include "DatabaseSectionSprite.h"
#include "DatabaseSectionAnimation.h"
#include "DatabaseSectionAnimationAdditionPoint.h"
#include "DatabaseSectionAnimationAdditionType.h"
#include <QtWidgets/QPushButton>

using namespace database;
using namespace ui;

MainDialog::MainDialog(Manager& DBMgr, QWidget* pParent)
    : QDialog(pParent), m_DBMgr(DBMgr)
{
    setWindowFlags(Qt::Window);

    auto pLayout = new QGridLayout(this);
    setLayout(pLayout);
    pLayout->setColumnStretch(0, 1);
    auto pTabWidget = new QTabWidget(this);
    pLayout->addWidget(pTabWidget, 0, 0, 1, -1);

    // setup buttons
    auto pButtonOK = new QPushButton("OK", this);
    connect(pButtonOK, SIGNAL(clicked()), this, SLOT(_onOKButtonClicked()));
    pLayout->addWidget(pButtonOK, 1, 1, Qt::AlignRight);
    auto pButtonCancel = new QPushButton("cancel", this);
    connect(pButtonCancel, SIGNAL(clicked()), this, SLOT(_onCancelButtonClicked()));
    pLayout->addWidget(pButtonCancel, 1, 2, Qt::AlignRight);
    auto pButtonApply = new QPushButton("apply", this);
    connect(pButtonApply, SIGNAL(clicked()), this, SLOT(_onApplyButtonClicked()));
    pLayout->addWidget(pButtonApply, 1, 3, Qt::AlignRight);

    // section tile
    auto pTileTabWidget = new QTabWidget(pTabWidget);
    pTabWidget->addTab(pTileTabWidget, "Tile Section");
    // tiles
    auto pTileTab = new section::Tile(m_DBMgr.getTileDatabase(), this);
    pTileTabWidget->addTab(pTileTab, "Tiles");
    m_pTileCreator = std::unique_ptr<helper::TileCreator>(new helper::TileCreator(pTileTab->getDatabaseMerger()));
    connect(this, SIGNAL(storeChanges()), pTileTab, SLOT(onSaveChanges()));
    // AutoTiles
    auto pAutoTileTab = new section::AutoTile(*m_pTileCreator, m_DBMgr.getAutoTileDatabase(), this);
    pTileTabWidget->addTab(pAutoTileTab, "AutoTiles");
    connect(this, SIGNAL(storeChanges()), pAutoTileTab, SLOT(onSaveChanges()));
    // TileSets
    auto pTileSetTab = new section::TileSet(*m_pTileCreator, m_DBMgr.getTileSetDatabase(), this);
    pTileTabWidget->addTab(pTileSetTab, "TileSets");
    connect(this, SIGNAL(storeChanges()), pTileSetTab, SLOT(onSaveChanges()));

    // section animation
    auto pAnimationTabWidget = new QTabWidget(pTabWidget);
    pTabWidget->addTab(pAnimationTabWidget, "Animation Section");
    // sprite
    auto pSpriteTab = new section::Sprite(m_DBMgr.getSpriteDatabase(), this);
    pAnimationTabWidget->addTab(pSpriteTab, "Sprites");
    connect(this, SIGNAL(storeChanges()), pSpriteTab, SLOT(onSaveChanges()));
    m_pSpriteCreator = std::unique_ptr<helper::SpriteCreator>(new helper::SpriteCreator(pSpriteTab->getDatabaseMerger()));
    // AnimationAdditionType
    auto pAnimationAdditionTypeTab = new section::AnimationAdditionType(m_DBMgr.getAnimationAdditionTypeDatabase(), this);
    pAnimationTabWidget->addTab(pAnimationAdditionTypeTab, "AnimationAdditionType");
    connect(this, SIGNAL(storeChanges()), pAnimationAdditionTypeTab, SLOT(onSaveChanges()));
    // AnimationAdditionPoint
    auto pAnimationAdditionPointTab = new section::AnimationAdditionPoint(*m_pSpriteCreator, pAnimationAdditionTypeTab->getModel(),
        m_DBMgr.getAnimationAdditionPointDatabase(), this);
    pAnimationTabWidget->addTab(pAnimationAdditionPointTab, "AnimationAdditionPoint");
    connect(this, SIGNAL(storeChanges()), pAnimationAdditionPointTab, SLOT(onSaveChanges()));
    // Animation
    auto pAnimationTab = new section::Animation(*m_pSpriteCreator, pAnimationAdditionTypeTab->getModel(), m_DBMgr.getAnimationDatabase(), this);
    pAnimationTabWidget->addTab(pAnimationTab, "Animation");
    connect(this, SIGNAL(storeChanges()), pAnimationTab, SLOT(onSaveChanges()));

    // sound section
    auto pSoundTabWidget = new QTabWidget(pTabWidget);
    pTabWidget->addTab(pSoundTabWidget, "Sound Section");
    //m_pTiles->setDatabaseMgr(m_OwnDBMgr);
    //m_pTileSets->setDatabaseMgr(m_OwnDBMgr);
    //m_pTileSets->setTileDatabaseModel(m_pTiles->getDatabaseModel());
    //m_pAutoTiles->setDatabaseMgr(m_OwnDBMgr);
    //m_pAutoTiles->setTileDatabaseModel(m_pTiles->getDatabaseModel());
    //m_pSprites->setDatabaseMgr(m_OwnDBMgr);
    //m_pAnimations->setDatabaseMgr(m_OwnDBMgr);
    //m_pAnimations->setSpriteDatabaseModel(m_pSprites->getDatabaseModel());
    //m_pAnimationTypes->setDatabaseMgr(m_OwnDBMgr);

    //// object section
    //m_pWorldObjects->setDatabaseMgr(m_OwnDBMgr);
    //m_pWorldObjects->setSpriteDatabaseModel(m_pSprites->getDatabaseModel());
    //m_pWorldObjects->setAnimationDatabaseModel(m_pAnimations->getDatabaseModel());

    // text section
    // m_pLocalisation->setDatabaseMgr(m_OwnDBMgr);
}

void MainDialog::_onOKButtonClicked()
{
    _onApplyButtonClicked();
    _onCancelButtonClicked();
}

void MainDialog::_onCancelButtonClicked()
{
    close();
}

void MainDialog::_onApplyButtonClicked()
{
    emit storeChanges();
}
