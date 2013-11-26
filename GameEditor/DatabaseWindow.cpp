#include "DatabaseWindow.h"
#include "moc_DatabaseWindow.h"
#include <QtWidgets/QFileDialog>
#include <QtCore/QTimer>
#include <QtGui/QPainter>
#include <QtWidgets/QGraphicsPixmapItem>
#include "Config.h"

using namespace DATABASE;

DatabaseWindow::DatabaseWindow(const DatabaseMgr &pDBMgr, QWidget *p_pParent) : QDialog(p_pParent), Ui_Database(), m_pDBMgr(pDBMgr)
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

    // setup move and resize widgets
    //m_ModifyObj.setWidget(m_pSections, MODIFY_RESIZE, QPoint(10, ButtonCancel->height()+15));
    //m_ModifyObj.setWidget(m_pTextureTabs, MODIFY_RESIZE, QPoint(50, ButtonCancel->height()+60));
    //m_ModifyObj.setWidget(m_pTextTabs, MODIFY_RESIZE, QPoint(50, ButtonCancel->height()+60));
    //m_ModifyObj.setWidget(m_pObjectTabs, MODIFY_RESIZE, QPoint(50, ButtonCancel->height()+60));
    //m_ModifyObj.setWidget(ButtonApply, MODIFY_MOVE, QPoint(10, 10));
    //m_ModifyObj.setWidget(ButtonCancel, MODIFY_MOVE, QPoint(ButtonApply->width()+10, 10));
    //m_ModifyObj.setWidget(ButtonOK, MODIFY_MOVE, QPoint(ButtonApply->width()+ButtonOK->width()+10, 10));

    connect(ButtonOK, SIGNAL(clicked()), this, SLOT(clickButtonOK()));
    connect(ButtonApply, SIGNAL(clicked()), this, SLOT(clickButtonApply()));
    connect(ButtonCancel, SIGNAL(clicked()), this, SLOT(clickButtonCancel()));

    /* ####focus connections####*/
    connect(m_pSections, SIGNAL(currentChanged (int)), this, SLOT(_focusChanged(int)));
    connect(m_pTextureTabs, SIGNAL(currentChanged (int)), this, SLOT(_focusChanged(int)));
    connect(m_pObjectTabs, SIGNAL(currentChanged (int)), this, SLOT(_focusChanged(int)));
    connect(m_pTextTabs, SIGNAL(currentChanged (int)), this, SLOT(_focusChanged(int)));
}

void DatabaseWindow::_focusChanged(int index)
{
    //QString parentTabName = m_pSections->tabText(m_pSections->currentIndex());
    //QTabWidget *pTabWidget = NULL;
    //if (parentTabName == "Textures")
    //    pTabWidget = m_pTextureTabs;
    //else if (parentTabName == "Objects")
    //    pTabWidget = m_pObjectTabs;
    //else if (parentTabName == "Texts")
    //    pTabWidget = m_pTextTabs;
    //else
    //    return;

    //if (DatabaseBase *pTab = dynamic_cast<DatabaseBase*>(pTabWidget->currentWidget()))
    //    pTab->setFocus();
}

void DatabaseWindow::saveDatabase()
{
    //// TileDB
    //if (m_pTiles->hasChanged())
    //    m_pTiles->storeDBChanges();

    //// TileSetDB
    //if (m_pTileSets->hasChanged())
    //    m_pTileSets->storeDBChanges();

    //// AutoTileDB
    //if (m_pAutoTiles->hasChanged())
    //    m_pAutoTiles->storeDBChanges();

    //// SpriteDB
    //if (m_pSprites->hasChanged())
    //    m_pSprites->storeDBChanges();

    //// AnimationDB
    //if (m_pAnimations->hasChanged())
    //    m_pAnimations->storeDBChanges();

    //// TextDB
    //if (m_pTexts->hasChanged())
    //    m_pTexts->storeDBChanges();

    //// WorldObjectDB
    //if (m_pWorldObjects->hasChanged())
    //    m_pWorldObjects->storeDBChanges();

    //// DynamicObjectDB
    //if (m_pDynamicObjects->hasChanged())
    //    m_pDynamicObjects->storeDBChanges();

    //// ObjectAnimationDB
    //if (m_pObjectAnimationTypes->hasChanged())
    //    m_pObjectAnimationTypes->storeDBChanges();
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