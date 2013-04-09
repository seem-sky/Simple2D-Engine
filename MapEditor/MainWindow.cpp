#include "MainWindow.h"
#include "moc_MainWindow.h"
#include "DatabaseWindow.h"
#include <QtGui/QFileDialog>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QTextstream>
#include <QtCore/QTimer>
#include <QtGui/QErrorMessage>
#include <QtGui/QBitmap>
#include "Logfile.h"

using namespace DATABASE;

MainWindow::MainWindow(QMainWindow *pParent) : QMainWindow(pParent), m_WindowAction(WINDOW_DO), m_pDBMgr(new DatabaseMgr())
{
    setupUi(this);

    // setup move/resize widgets
    m_ModifyObj.setWidget(m_pMapEditor, MODIFY_RESIZE, QPoint(0, 20));

    connect(this, SIGNAL(projectSave()), m_pMapEditor, SLOT(_saveMaps()));
    connect(this, SIGNAL(projectLoadDone()), m_pMapEditor, SLOT(_updateTiles()));
    connect(menuDatabase, SIGNAL(aboutToShow()), this, SLOT(_openDatabase()));
    connect(actionNew, SIGNAL(triggered()), this, SLOT(_newProject()));
    connect(actionLoad, SIGNAL(triggered()), this, SLOT(_loadProject()));
    connect(actionSave, SIGNAL(triggered()), this, SLOT(_saveProject()));
    connect(actionClose, SIGNAL(triggered()), this, SLOT(_closeProject()));
    actionSave->setShortcut(QKeySequence(tr("Ctrl+S", "File|Save")));
    actionLoad->setShortcut(QKeySequence(tr("Ctrl+O", "File|Open")));
    actionNew->setShortcut(QKeySequence(tr("Ctrl+N", "File|New")));

    // load old config data and open last project
    Config::Get()->loadConfig();
    move(Config::Get()->getMainWindowPos().x, Config::Get()->getMainWindowPos().y);
    QSize newSize = QSize(Config::Get()->getMainWindowSize().x, Config::Get()->getMainWindowSize().y);
    if (newSize.width() >= minimumWidth() && newSize.height() >= minimumHeight())
        resize(newSize);
    if (!Config::Get()->getProjectDirectory().empty())
        _loadProject(Config::Get()->getProjectDirectory());
}

void MainWindow::resizeEvent(QResizeEvent *pEvent)
{
    m_ModifyObj.resizeEvent(this);
    Config::Get()->setMainWindowSize(Point<uint32>(pEvent->size().width(), pEvent->size().height()));
}

void MainWindow::_setDBs()
{
    // share DBMgr pointer
    m_pMapEditor->setDBMgr(m_pDBMgr);
}

MainWindow::~MainWindow(void)
{
    Config::Get()->Del();
    if (Logfile *pLog = Logfile::Get())
        pLog->Del();
}

void MainWindow::_openDatabase()
{
    DatabaseWindow pDB(m_pDBMgr, this);
    pDB.exec();
    m_pMapEditor->updateTiles();
}

void MainWindow::_saveProject()
{
    Config *pConf = Config::Get();
    if (!pConf)
        return;

    // save map database
    if (DATABASE::MapDatabasePtr pMapDB = m_pDBMgr->getMapDatabase())
    {
        pMapDB->deleteRemovedMaps(Config::Get()->getProjectDirectory());
        MapDatabaseXMLWriter writer(pMapDB);
        writer.writeFile(Config::Get()->getProjectDirectory() + MAP_DATABASE_PATH);
    }

    emit projectSave();
}

void MainWindow::_newProject()
{
    QString sFileName = QFileDialog::getSaveFileName(this, tr("New Project"), "projects/untitled", tr("Project File"));
    QDir fileDir(sFileName);
    if (!fileDir.exists())
    {
        fileDir.mkpath(sFileName);
        fileDir.mkpath(sFileName + "/Game");
        fileDir.mkpath(sFileName + "/Maps");
        fileDir.mkpath(sFileName + "/Textures");
        // create game database
        QFile newFile(sFileName + "/Game/GameDatabase.xml");
        if (newFile.open(QIODevice::WriteOnly))
        {
            QTextStream stream(&newFile);
            stream << "<Database></Database>";
            newFile.close();
        }
        // create map database
        newFile.setFileName(sFileName + QString::fromStdString(MAP_DATABASE_PATH.c_str()));
        if (newFile.open(QIODevice::WriteOnly))
        {
            QTextStream stream(&newFile);
            stream << "<MapDatabase></MapDatabase>";
            newFile.close();
        }
        BASIC_LOG("Successfully create project at " + sFileName.toStdString());
        _loadProject(sFileName.toStdString());
    }
    else
        ERROR_LOG("Unable to create project at " + sFileName.toStdString() + ". It already exists!");
}

bool MainWindow::_loadDB(const std::string &projectPath)
{
    if (!m_pDBMgr)
        return false;
    // tile database
    TileDatabasePtr tileDB(new TileDatabase());
    TileDatabaseXMLReader tileReader(tileDB);
    tileReader.readFile(projectPath + TILE_DATABASE_PATH);
    m_pDBMgr->setTileDatabase(tileDB);

    // autotile database
    AutoTileDatabasePtr autoTileDB(new AutoTileDatabase());
    AutoTileDatabaseXMLReader autoTileReader(autoTileDB);
    autoTileReader.readFile(projectPath + AUTO_TILE_DATABASE_PATH);
    m_pDBMgr->setAutoTileDatabase(autoTileDB);

    // sprite database
    SpriteDatabasePtr spriteDB(new SpriteDatabase());
    SpriteDatabaseXMLReader spriteReader(spriteDB);
    spriteReader.readFile(projectPath + SPRITE_DATABASE_PATH);
    m_pDBMgr->setSpriteDatabase(spriteDB);

    // load map database
    MapDatabasePtr mapDB(new MAP::MapDatabase());
    MapDatabaseXMLReader mapReader(mapDB);
    mapReader.readFile(projectPath + MAP_DATABASE_PATH);
    m_pDBMgr->setMapDatabase(mapDB);

    // load text database
    TextDatabasePtr textDB(new TextDatabase());
    TextDatabaseXMLReader textReader(textDB);
    textReader.readFile(projectPath + TEXT_DATABASE_PATH);
    m_pDBMgr->setTextDatabase(textDB);

    // ToDo: load complete databases
    return true;
}

bool MainWindow::_loadProject(const std::string &sDir)
{
    if (_loadDB(sDir))
    {
        _setDBs();
        Config::Get()->setProjectDirectory(sDir);
        BASIC_LOG("Successfully load " + sDir + ".");
        emit projectLoadDone();
        return true;
    }
    QErrorMessage *pMsg = new QErrorMessage(this);
    pMsg->showMessage(QString::fromStdString(sDir + " is no valid project directory."));
    BASIC_LOG("Unable to load " + sDir + ". Corrupt project or no such directory.");
    Config::Get()->clear();
    return false;
}

void MainWindow::_loadProject()
{
    QString sPath = QFileDialog::getExistingDirectory(this, tr("Open Project"), QDir::current().path()+"/projects", QFileDialog::ShowDirsOnly);
    _closeProject();
    _loadProject(sPath.toStdString());
}

void MainWindow::_closeProject()
{
    if (Config::Get()->getProjectDirectory() != "")
    {
        BASIC_LOG("Close project " + Config::Get()->getProjectDirectory());
        m_pMapEditor->clearWidgets();
        if (m_pDBMgr)
            m_pDBMgr->clear();
        Config::Get()->clear();
    }
}