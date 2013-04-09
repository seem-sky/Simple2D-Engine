#include "DatabaseWindow.h"
#include "moc_DatabaseWindow.h"
#include <QTGui/QFileDialog>
#include <StringAdditions.h>
#include <QtCore/QTimer>
#include <QtGui/QPainter>
#include <QtGui/QGraphicsPixmapItem>
#include "Config.h"
#include "MainWindow.h"
#include "DatabaseIO.h"

using namespace DATABASE;

DatabaseWindow::DatabaseWindow(DatabaseMgrPtr pDBMgr, QWidget *p_pParent) : QDialog(p_pParent), Ui_Database(),
m_pDBMgr(pDBMgr)
{
    setupUi(this);

    // setup db for specific widgets
    m_pTiles->setDB(m_pDBMgr->getTileDatabase());
    // 2 dbs for AutoTile widget
    m_pAutoTiles->setTileDB(m_pTiles->getDBChanger());
    m_pAutoTiles->setDB(m_pDBMgr->getAutoTileDatabase());
    m_pSprites->setDB(m_pDBMgr->getSpriteDatabase());
    m_pTexts->setDB(m_pDBMgr->getTextDatabase());

    // setup move and resize widgets
    m_ModifyObj.setWidget(m_pSections, MODIFY_RESIZE, QPoint(10, ButtonCancel->height()+15));
    m_ModifyObj.setWidget(m_pTextureTabs, MODIFY_RESIZE, QPoint(50, ButtonCancel->height()+60));
    m_ModifyObj.setWidget(m_pTextTabs, MODIFY_RESIZE, QPoint(50, ButtonCancel->height()+60));
    m_ModifyObj.setWidget(ButtonApply, MODIFY_MOVE, QPoint(10, 10));
    m_ModifyObj.setWidget(ButtonCancel, MODIFY_MOVE, QPoint(ButtonApply->width()+10, 10));
    m_ModifyObj.setWidget(ButtonOK, MODIFY_MOVE, QPoint(ButtonApply->width()+ButtonOK->width()+10, 10));

    m_sLogLocationName = LOGFILE_ENGINE_LOG_NAME + "DatabaseWindow : ";

    connect(ButtonOK, SIGNAL(clicked()), this, SLOT(clickButtonOK()));
    connect(ButtonApply, SIGNAL(clicked()), this, SLOT(clickButtonApply()));

    // focus connections
    connect(m_pTextureTabs, SIGNAL(currentChanged (int)), this, SLOT(_textureWidgetChanged(int)));
    m_pSections->setCurrentIndex(0);
}

void DatabaseWindow::_textureWidgetChanged(int index)
{
    DatabaseWidgetObject *pTab = dynamic_cast<DatabaseWidgetObject*>(m_pTextureTabs->widget(index));
    if (pTab)
        pTab->setFocus();
}

void DatabaseWindow::saveDatabase()
{
    // TileDB
    if (m_pTiles->hasChanged())
    {
        m_pTiles->storeDBChanges();
        TileDatabaseXMLWriter writer(m_pDBMgr->getTileDatabase());
        writer.writeFile(Config::Get()->getProjectDirectory() + TILE_DATABASE_PATH);
    }

    // AutoTileDB
    if (m_pAutoTiles->hasChanged())
    {
        m_pAutoTiles->storeDBChanges();
        AutoTileDatabaseXMLWriter writer(m_pDBMgr->getAutoTileDatabase());
        writer.writeFile(Config::Get()->getProjectDirectory() + AUTO_TILE_DATABASE_PATH);
    }

    // SpriteDB
    if (m_pSprites->hasChanged())
    {
        m_pSprites->storeDBChanges();
        SpriteDatabaseXMLWriter writer(m_pDBMgr->getSpriteDatabase());
        writer.writeFile(Config::Get()->getProjectDirectory() + SPRITE_DATABASE_PATH);
    }

    // TextDB
    if (m_pTexts->hasChanged())
    {
        m_pTexts->storeDBChanges();
        TextDatabaseXMLWriter writer(m_pDBMgr->getTextDatabase());
        writer.writeFile(Config::Get()->getProjectDirectory() + TEXT_DATABASE_PATH);
    }
    // ToDo: save changes
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