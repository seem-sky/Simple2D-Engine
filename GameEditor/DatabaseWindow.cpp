#include "DatabaseWindow.h"
#include "moc_DatabaseWindow.h"
#include <QtWidgets/QFileDialog>
#include <QtCore/QTimer>
#include <QtGui/QPainter>
#include <QtWidgets/QGraphicsPixmapItem>
#include "Config.h"

using namespace DATABASE;

DatabaseWindow::DatabaseWindow(DatabaseMgr& DBMgr, QWidget* p_pParent) : QDialog(p_pParent), Ui_Database(), m_OwnDBMgr(DATABASE::DatabaseMgr(DBMgr)), m_DBMgr(DBMgr)
{
    setupUi(this);
    setWindowFlags(Qt::Window);

    m_pTiles->setDatabaseModel(new DATABASE::DatabaseModel(m_OwnDBMgr, DatabaseType::TILE_DATABASE));
    m_pTileSets->setDatabaseModel(new DATABASE::DatabaseModel(m_OwnDBMgr, DatabaseType::TILE_SET_DATABASE));
    m_pTileSets->setTileDatabaseModel(new DATABASE::DatabaseModel(m_OwnDBMgr, DatabaseType::TILE_DATABASE));

    // 3 dbs for AutoTile widget
    m_pAutoTiles->setDatabaseModel(new DATABASE::DatabaseModel(m_OwnDBMgr, DatabaseType::AUTO_TILE_DATABASE));
    m_pAutoTiles->setTileDatabaseModel(new DATABASE::DatabaseModel(m_OwnDBMgr, DatabaseType::TILE_DATABASE));

    m_pSprites->setDatabaseModel(new DATABASE::DatabaseModel(m_OwnDBMgr, DatabaseType::SPRITE_DATABASE));

    // set 2 dbs for animation widget
    m_pAnimations->setSpriteDatabaseModel(new DATABASE::DatabaseModel(m_OwnDBMgr, DatabaseType::SPRITE_DATABASE));
    m_pAnimations->setDatabaseModel(new DATABASE::DatabaseModel(m_OwnDBMgr, DatabaseType::ANIMATION_DATABASE));

    m_pAnimationTypes->setDatabaseModel(new DATABASE::DatabaseModel(m_OwnDBMgr, DatabaseType::ANIMATION_TYPE_DATABASE));


    //// object section
    //m_pObjectAnimationTypes->setDB(m_pDBMgr->getObjectAnimationTypeDatabase());
    //m_pWorldObjects->setAdditionalDB(m_pSprites->getDBChanger(), m_pAnimations->getDBChanger(), m_pObjectAnimationTypes->getDBChanger());
    //m_pWorldObjects->setDB(m_pDBMgr->getWorldObjectDatabase());

    //m_pDynamicObjects->setAdditionalDB(m_pSprites->getDBChanger(), m_pAnimations->getDBChanger(), m_pObjectAnimationTypes->getDBChanger());
    //m_pDynamicObjects->setDB(m_pDBMgr->getDynamicObjectDatabase());

    // text section
    m_pLocalisation->setDatabaseModel(new DATABASE::DatabaseModel(m_OwnDBMgr, DatabaseType::LOCALISATION_DATABASE));

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

void DatabaseWindow::clickButtonApply()
{
    // request store of current prototype
    emit storeChanges();
    m_DBMgr.copyFrom(m_OwnDBMgr);
}

void DatabaseWindow::clickButtonOK()
{
    // request store of current prototype
    emit storeChanges();
    m_DBMgr.takeFrom(m_OwnDBMgr);

    close();
}

void DatabaseWindow::clickButtonCancel()
{
    close();
}