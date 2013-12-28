#include "MainWindow.h"
#include "moc_MainWindow.h"
#include "DatabaseWindow.h"
#include <QtWidgets/QFileDialog>
#include <QtCore/QDir>
#include <QtCore/QTimer>
#include <QtWidgets/QErrorMessage>
#include "Logfile.h"
#include <QtCore/QTime>

using namespace DATABASE;

MainWindow::MainWindow(QMainWindow* pParent) : QMainWindow(pParent), Ui_MainWindow(), m_pMapEditor(new MapEditorWidgetEditor(m_Project.getDatabaseMgr(), this))
{
    Logfile::get();
    setupUi(this);

    // setup map editor widget
    auto pLayout = new QGridLayout(this);
    centralwidget->setLayout(pLayout);
    pLayout->addWidget(m_pMapEditor, 0, 0, -1, -1);

    connect(this, SIGNAL(projectLoadDone()), m_pMapEditor, SLOT(_projectOpened()));
    connect(openDatabase, SIGNAL(triggered()), this, SLOT(_openDatabase()));
    connect(this, SIGNAL(saveProject()), m_pMapEditor, SLOT(_saveChanges()));
    connect(actionNew, SIGNAL(triggered()), this, SLOT(_newProject()));
    connect(actionLoad, SIGNAL(triggered()), this, SLOT(_loadProject()));
    connect(actionSave, SIGNAL(triggered()), this, SLOT(_saveProject()));
    connect(actionClose, SIGNAL(triggered()), this, SLOT(_closeProject()));
    connect(actionMapScreenshot, SIGNAL(triggered()), this, SLOT(_mapScreenshot()));
    actionSave->setShortcut(QKeySequence(tr("Ctrl+S", "File|Save")));
    actionLoad->setShortcut(QKeySequence(tr("Ctrl+O", "File|Open")));
    actionNew->setShortcut(QKeySequence(tr("Ctrl+N", "File|New")));

    // setup caches
    GTileCache::get()->setDBMgr(&m_Project.getDatabaseMgr());
    GAutoTileCache::get()->setDBMgr(&m_Project.getDatabaseMgr());

    // load old config data and open last project
    Config::get()->loadConfig();
    //move(Config::get()->getMainWindowPos().x, Config::get()->getMainWindowPos().y);
    //QSize newSize = QSize(Config::get()->getMainWindowSize().x, Config::get()->getMainWindowSize().y);
    //if (newSize.width() >= minimumWidth() && newSize.height() >= minimumHeight())
    //    resize(newSize);
    if (!Config::get()->getProjectDirectory().isEmpty())
        _loadProject(Config::get()->getProjectDirectory());
}

void MainWindow::_setDBs()
{
    // share DBMgr pointer
    //m_pMapEditor->setDBMgr(m_project.getDatabaseMgr());
}

MainWindow::~MainWindow(void)
{
    Config::get()->del();
    Logfile::get()->del();
}

void MainWindow::_mapScreenshot()
{
    //m_pMapEditor->storeCurrentMapInClipboard();
}

void MainWindow::_openDatabase()
{
    DatabaseWindow pDB(m_Project.getDatabaseMgr(), this);
    pDB.exec();

    // clear map editor and caches
    m_pMapEditor->setup();
}

void MainWindow::_saveProject()
{
    emit saveProject();
    m_Project.save();
}

void MainWindow::_newProject()
{
    QString dir = QFileDialog::getSaveFileName(this, tr("New Project"), "projects", tr("Project File"));
    QDir fileDir(dir);
    if (!fileDir.exists())
    {
        if (m_Project.createNew(dir))
        {
            BASIC_LOG("Successfully create project at " + dir);
            _loadProject(dir);
        }
        else
            ERROR_LOG("Unable to create project at " + dir);
    }
    else
        ERROR_LOG("Unable to create project at " + dir + ". It already exists!");
}

void MainWindow::_loadProject()
{
    QString path = QFileDialog::getExistingDirectory(this, tr("Open Project"), QDir::current().path()+"/projects", QFileDialog::ShowDirsOnly);
    if (!path.isEmpty())
    {
        _closeProject();
        _loadProject(path);
    }
}

bool MainWindow::_loadProject(const QString& dir)
{
    QTime time;
    time.start();
    BASIC_LOG("Begin project load: " + dir);
    if (m_Project.load(dir))
    {
        _setDBs();
        Config::get()->setProjectDirectory(dir);
        BASIC_LOG("Project load successfully ends after " + QString::number(time.elapsed()) + "msec.");
        m_pMapEditor->projectOpened();
        emit projectLoadDone();
        return true;
    }
    QErrorMessage* pMsg = new QErrorMessage(this);
    pMsg->showMessage(dir + " is no valid project directory.");
    ERROR_LOG("Unable to load " + dir + ". Corrupt project or no such directory.");
    Config::get()->clear();
    return false;
}

void MainWindow::_closeProject()
{
    if (m_Project.isOpen())
    {
        BASIC_LOG("Close project " + m_Project.getPath());
        //m_pMapEditor->clearWidgets();
        m_Project.close();
        Config::get()->clear();
    }
}