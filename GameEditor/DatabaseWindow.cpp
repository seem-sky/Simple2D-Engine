#include "DatabaseWindow.h"
#include "moc_DatabaseWindow.h"
#include <QtWidgets/QFileDialog>
#include <QtCore/QTimer>
#include <QtGui/QPainter>
#include <QtWidgets/QGraphicsPixmapItem>
#include "Config.h"

using namespace DATABASE;

DatabaseWindow::DatabaseWindow(DatabaseMgr& DBMgr, QWidget* p_pParent) : QDialog(p_pParent), Ui_Database(), m_OwnDBMgr(DatabaseMgr(DBMgr)), m_DBMgr(DBMgr)
{
    setupUi(this);
    setWindowFlags(Qt::Window);

    m_pTiles->setDatabaseMgr(m_OwnDBMgr);
    m_pTileSets->setDatabaseMgr(m_OwnDBMgr);
    m_pTileSets->setTileDatabaseModel(m_pTiles->getDatabaseModel());
    m_pAutoTiles->setDatabaseMgr(m_OwnDBMgr);
    m_pAutoTiles->setTileDatabaseModel(m_pTiles->getDatabaseModel());
    m_pSprites->setDatabaseMgr(m_OwnDBMgr);
    m_pAnimations->setDatabaseMgr(m_OwnDBMgr);
    m_pAnimations->setSpriteDatabaseModel(m_pSprites->getDatabaseModel());
    m_pAnimationTypes->setDatabaseMgr(m_OwnDBMgr);

    // object section
    m_pWorldObjects->setDatabaseMgr(m_OwnDBMgr);
    m_pWorldObjects->setSpriteDatabaseModel(m_pSprites->getDatabaseModel());
    m_pWorldObjects->setAnimationDatabaseModel(m_pAnimations->getDatabaseModel());

    // text section
    m_pLocalisation->setDatabaseMgr(m_OwnDBMgr);

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