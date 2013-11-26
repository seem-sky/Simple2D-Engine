#include "DatabaseWindow.h"
#include "moc_DatabaseWindow.h"
#include <QtWidgets/QFileDialog>
#include <QtCore/QTimer>
#include <QtGui/QPainter>
#include <QtWidgets/QGraphicsPixmapItem>
#include "Config.h"

using namespace DATABASE;

DatabaseWindow::DatabaseWindow(DatabaseMgr &pDBMgr, QWidget *p_pParent) : QDialog(p_pParent), Ui_Database(), m_pDBMgr(pDBMgr)
{
    setupUi(this);
    setWindowFlags(Qt::Window);

    m_pTiles->setDatabaseModel(new TileDatabaseModel(std::unique_ptr<TileDatabase>(new TileDatabase(*m_pDBMgr.getTileDatabase()))));
    m_pTileSets->setDatabaseModel(new TileSetDatabaseModel(std::unique_ptr<TileSetDatabase>(new TileSetDatabase(*m_pDBMgr.getTileSetDatabase()))));
    m_pTileSets->setTileDatabaseModel(dynamic_cast<TileDatabaseModel*>(m_pTiles->getDatabaseModel()));

    // 3 dbs for AutoTile widget
    m_pAutoTiles->setDatabaseModel(new AutoTileDatabaseModel(std::unique_ptr<AutoTileDatabase>(new AutoTileDatabase(*m_pDBMgr.getAutoTileDatabase()))));
    m_pAutoTiles->setTileDatabaseModel(dynamic_cast<TileDatabaseModel*>(m_pTiles->getDatabaseModel()));

    m_pSprites->setDatabaseModel(new SpriteDatabaseModel(std::unique_ptr<SpriteDatabase>(new SpriteDatabase(*m_pDBMgr.getSpriteDatabase()))));

    // set 2 dbs for animation widget
    m_pAnimations->setSpriteDatabaseModel(dynamic_cast<SpriteDatabaseModel*>(m_pSprites->getDatabaseModel()));
    m_pAnimations->setDatabaseModel(new AnimationDatabaseModel(std::unique_ptr<AnimationDatabase>(new AnimationDatabase(*m_pDBMgr.getAnimationDatabase()))));

    m_pAnimationTypes->setDatabaseModel(new AnimationTypeDatabaseModel(
        std::unique_ptr<AnimationTypeDatabase>(new AnimationTypeDatabase(*m_pDBMgr.getAnimationTypeDatabase()))));


    //// object section
    //m_pObjectAnimationTypes->setDB(m_pDBMgr->getObjectAnimationTypeDatabase());
    //m_pWorldObjects->setAdditionalDB(m_pSprites->getDBChanger(), m_pAnimations->getDBChanger(), m_pObjectAnimationTypes->getDBChanger());
    //m_pWorldObjects->setDB(m_pDBMgr->getWorldObjectDatabase());

    //m_pDynamicObjects->setAdditionalDB(m_pSprites->getDBChanger(), m_pAnimations->getDBChanger(), m_pObjectAnimationTypes->getDBChanger());
    //m_pDynamicObjects->setDB(m_pDBMgr->getDynamicObjectDatabase());

    // text section
    m_pLocalisation->setDatabaseModel(new LocalisationDatabaseModel(std::unique_ptr<LocalisationDatabase>(new LocalisationDatabase(*m_pDBMgr.getLocalisationDatabase()))));

    connect(ButtonOK, SIGNAL(clicked()), this, SLOT(clickButtonOK()));
    connect(ButtonApply, SIGNAL(clicked()), this, SLOT(clickButtonApply()));
    connect(ButtonCancel, SIGNAL(clicked()), this, SLOT(clickButtonCancel()));

    /* #### save connections ####*/
    connect(this, SIGNAL(storeChanges()), m_pTiles, SLOT(saveCurrent()));
    connect(this, SIGNAL(storeChanges()), m_pTileSets, SLOT(saveCurrent()));
    connect(this, SIGNAL(storeChanges()), m_pAutoTiles, SLOT(saveCurrent()));
    connect(this, SIGNAL(storeChanges()), m_pSprites, SLOT(saveCurrent()));
    connect(this, SIGNAL(storeChanges()), m_pAnimations, SLOT(saveCurrent()));
    connect(this, SIGNAL(storeChanges()), m_pAnimationTypes, SLOT(saveCurrent()));
    connect(this, SIGNAL(storeChanges()), m_pLocalisation, SLOT(saveCurrent()));
}

void DatabaseWindow::saveDatabase()
{
    // request store of current prototype
    emit storeChanges();

    m_pDBMgr.setTileDatabase(dynamic_cast<TileDatabase*>(m_pTiles->takeDatabase()));
    m_pDBMgr.setTileSetDatabase(dynamic_cast<TileSetDatabase*>(m_pTileSets->takeDatabase()));
    m_pDBMgr.setAutoTileDatabase(dynamic_cast<AutoTileDatabase*>(m_pAutoTiles->takeDatabase()));
    m_pDBMgr.setSpriteDatabase(dynamic_cast<SpriteDatabase*>(m_pSprites->takeDatabase()));
    m_pDBMgr.setAnimationDatabase(dynamic_cast<AnimationDatabase*>(m_pAnimations->takeDatabase()));
    m_pDBMgr.setAnimationTypeDatabase(dynamic_cast<AnimationTypeDatabase*>(m_pAnimationTypes->takeDatabase()));
    m_pDBMgr.setLocalisationDatabase(dynamic_cast<LocalisationDatabase*>(m_pLocalisation->takeDatabase()));
}

void DatabaseWindow::clickButtonApply()
{
    saveDatabase();
}

void DatabaseWindow::clickButtonOK()
{
    clickButtonApply();
    close();
}

void DatabaseWindow::clickButtonCancel()
{
    close();
}