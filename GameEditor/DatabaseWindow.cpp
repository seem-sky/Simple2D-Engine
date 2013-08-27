#include "DatabaseWindow.h"
#include "moc_DatabaseWindow.h"
#include <QtWidgets/QFileDialog>
#include <QtCore/QTimer>
#include <QtGui/QPainter>
#include <QtWidgets/QGraphicsPixmapItem>
#include "Config.h"

using namespace DATABASE;

DatabaseWindow::DatabaseWindow(DatabaseMgrPtr pDBMgr, QWidget *p_pParent) : QDialog(p_pParent), Ui_Database(), m_pDBMgr(pDBMgr)
{
    setupUi(this);
    setWindowFlags(Qt::Window);

    // setup db for specific widgets
    // texture section
    m_pTiles->setDB(m_pDBMgr->getTileDatabase());
    // 2 dbs for tileset widget
    m_pTileSets->setTileDB(m_pTiles->getDBChanger());
    m_pTileSets->setDB(m_pDBMgr->getTileSetDatabase());
    // 2 dbs for AutoTile widget
    m_pAutoTiles->setTileDB(m_pTiles->getDBChanger());
    m_pAutoTiles->setDB(m_pDBMgr->getAutoTileDatabase());
    m_pSprites->setDB(m_pDBMgr->getSpriteDatabase());
    // set 2dbs for animation widget
    m_pAnimations->setSpriteDB(m_pSprites->getDBChanger());
    m_pAnimations->setDB(m_pDBMgr->getAnimationDatabase());

    // object section
    m_pObjectAnimationTypes->setDB(m_pDBMgr->getObjectAnimationTypeDatabase());
    m_pWorldObjects->setAdditionalDB(m_pSprites->getDBChanger(), m_pAnimations->getDBChanger(), m_pObjectAnimationTypes->getDBChanger());
    m_pWorldObjects->setDB(m_pDBMgr->getWorldObjectDatabase());

    m_pDynamicObjects->setAdditionalDB(m_pSprites->getDBChanger(), m_pAnimations->getDBChanger(), m_pObjectAnimationTypes->getDBChanger());
    m_pDynamicObjects->setDB(m_pDBMgr->getDynamicObjectDatabase());

    // text section
    m_pTexts->setDB(m_pDBMgr->getLocalsDatabase());

    // setup move and resize widgets
    m_ModifyObj.setWidget(m_pSections, MODIFY_RESIZE, QPoint(10, ButtonCancel->height()+15));
    m_ModifyObj.setWidget(m_pTextureTabs, MODIFY_RESIZE, QPoint(50, ButtonCancel->height()+60));
    m_ModifyObj.setWidget(m_pTextTabs, MODIFY_RESIZE, QPoint(50, ButtonCancel->height()+60));
    m_ModifyObj.setWidget(m_pObjectTabs, MODIFY_RESIZE, QPoint(50, ButtonCancel->height()+60));
    m_ModifyObj.setWidget(ButtonApply, MODIFY_MOVE, QPoint(10, 10));
    m_ModifyObj.setWidget(ButtonCancel, MODIFY_MOVE, QPoint(ButtonApply->width()+10, 10));
    m_ModifyObj.setWidget(ButtonOK, MODIFY_MOVE, QPoint(ButtonApply->width()+ButtonOK->width()+10, 10));

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
    QString parentTabName = m_pSections->tabText(m_pSections->currentIndex());
    QTabWidget *pTabWidget = NULL;
    if (parentTabName == "Textures")
        pTabWidget = m_pTextureTabs;
    else if (parentTabName == "Objects")
        pTabWidget = m_pObjectTabs;
    else if (parentTabName == "Texts")
        pTabWidget = m_pTextTabs;
    else
        return;

    if (DatabaseWidgetObject *pTab = dynamic_cast<DatabaseWidgetObject*>(pTabWidget->currentWidget()))
        pTab->setFocus();
}

void DatabaseWindow::saveDatabase()
{
    // TileDB
    if (m_pTiles->hasChanged())
        m_pTiles->storeDBChanges();

    // TileSetDB
    if (m_pTileSets->hasChanged())
        m_pTileSets->storeDBChanges();

    // AutoTileDB
    if (m_pAutoTiles->hasChanged())
        m_pAutoTiles->storeDBChanges();

    // SpriteDB
    if (m_pSprites->hasChanged())
        m_pSprites->storeDBChanges();

    // AnimationDB
    if (m_pAnimations->hasChanged())
        m_pAnimations->storeDBChanges();

    // TextDB
    if (m_pTexts->hasChanged())
        m_pTexts->storeDBChanges();

    // WorldObjectDB
    if (m_pWorldObjects->hasChanged())
        m_pWorldObjects->storeDBChanges();

    // ObjectAnimationDB
    if (m_pObjectAnimationTypes->hasChanged())
        m_pObjectAnimationTypes->storeDBChanges();
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