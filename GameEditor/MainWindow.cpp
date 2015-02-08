#include "MainWindow.h"
#include "moc_MainWindow.h"
#include "DatabaseWindow.h"
#include <QtWidgets/QFileDialog>
#include <QtCore/QDir>
#include <QtCore/QTimer>
#include <QtWidgets/QErrorMessage>
#include <log/Log.h>
#include <log/FileOutput.h>
#include <ConsoleOutput.h>
#include <QtCore/QTime>

using namespace DATABASE;

MainWindow::MainWindow(QMainWindow* pParent) : QMainWindow(pParent), Ui_MainWindow(), m_pMapEditor(new MapEditorWidgetEditor(m_Project.getDatabaseMgr(), this))
{
    LOG::GlobalLog::get()->setOutputCount(2);
    LOG::GlobalLog::get()->setOutput(0, new LOG::FileOutput("Logfile"));
    LOG::GlobalLog::get()->setOutput(1, new ConsoleOutput());
    setupUi(this);

    // setup map editor widget
	setCentralWidget(m_pMapEditor);

    connect(this, SIGNAL(projectLoadDone()), m_pMapEditor, SLOT(_projectOpened()));
    connect(openDatabase, SIGNAL(triggered()), this, SLOT(_openDatabase()));
    connect(this, SIGNAL(saveProject()), m_pMapEditor, SLOT(onSaveChanges()));
    connect(actionNew, SIGNAL(triggered()), this, SLOT(_newProject()));
    connect(actionLoad, SIGNAL(triggered()), this, SLOT(_loadProject()));
    connect(actionSave, SIGNAL(triggered()), this, SLOT(_saveProject()));
    connect(actionClose, SIGNAL(triggered()), this, SLOT(_closeProject()));
    connect(actionMapScreenshot, SIGNAL(triggered()), this, SLOT(_mapScreenshot()));
    actionSave->setShortcut(QKeySequence(tr("Ctrl+S", "File|Save")));
    actionLoad->setShortcut(QKeySequence(tr("Ctrl+O", "File|Open")));
    actionNew->setShortcut(QKeySequence(tr("Ctrl+N", "File|New")));

    // setup caches
    //GTileCache::get()->setDBMgr(&m_Project.getDatabaseMgr());
    //GAutoTileCache::get()->setDBMgr(&m_Project.getDatabaseMgr());

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
            STANDARD_MESSAGE("Successfully create project at " + dir.toStdString());
            _loadProject(dir);
        }
        else
            ERROR_MESSAGE("Unable to create project at " + dir.toStdString());
    }
    else
        ERROR_MESSAGE("Unable to create project at " + dir.toStdString() + ". It already exists!");
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
    STANDARD_MESSAGE("Begin project load: " + dir.toStdString());
    if (m_Project.load(dir))
    {
        _setDBs();
        Config::get()->setProjectDirectory(dir);
        STANDARD_MESSAGE("Project load successfully ends after " + std::to_string(time.elapsed()) + "msec.");
        m_pMapEditor->projectOpened();
        emit projectLoadDone();
        return true;
    }
    QErrorMessage* pMsg = new QErrorMessage(this);
    pMsg->showMessage(dir + " is no valid project directory.");
    ERROR_MESSAGE("Unable to load " + dir.toStdString() + ". Corrupt project or no such directory.");
    Config::get()->clear();
    return false;
}

void MainWindow::_closeProject()
{
    if (m_Project.isOpen())
    {
        STANDARD_MESSAGE("Close project " + m_Project.getPath().toStdString());
        //m_pMapEditor->clearWidgets();
        m_Project.close();
        Config::get()->clear();
    }
}