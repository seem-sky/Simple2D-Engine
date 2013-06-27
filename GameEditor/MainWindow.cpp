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

MainWindow::MainWindow(QMainWindow *pParent) : QMainWindow(pParent)
{
    Logfile::Get();
    setupUi(this);

    // setup move/resize widgets
    m_ModifyObj.setWidget(m_pMapEditor, MODIFY_RESIZE, QPoint(0, 20));

    connect(this, SIGNAL(projectLoadDone()), m_pMapEditor, SLOT(_projectOpened()));
    connect(menuDatabase, SIGNAL(aboutToShow()), this, SLOT(_openDatabase()));
    connect(this, SIGNAL(saveProject()), m_pMapEditor, SLOT(_saveChanges()));
    connect(actionNew, SIGNAL(triggered()), this, SLOT(_newProject()));
    connect(actionLoad, SIGNAL(triggered()), this, SLOT(_loadProject()));
    connect(actionSave, SIGNAL(triggered()), this, SLOT(_saveProject()));
    connect(actionClose, SIGNAL(triggered()), this, SLOT(_closeProject()));
    connect(actionMapScreenshot, SIGNAL(triggered()), this, SLOT(_mapScreenshot()));
    actionSave->setShortcut(QKeySequence(tr("Ctrl+S", "File|Save")));
    actionLoad->setShortcut(QKeySequence(tr("Ctrl+O", "File|Open")));
    actionNew->setShortcut(QKeySequence(tr("Ctrl+N", "File|New")));

    // load old config data and open last project
    Config::Get()->loadConfig();
    //move(Config::Get()->getMainWindowPos().x, Config::Get()->getMainWindowPos().y);
    //QSize newSize = QSize(Config::Get()->getMainWindowSize().x, Config::Get()->getMainWindowSize().y);
    //if (newSize.width() >= minimumWidth() && newSize.height() >= minimumHeight())
    //    resize(newSize);
    //if (!Config::Get()->getProjectDirectory().isEmpty())
    //    _loadProject(Config::Get()->getProjectDirectory());
}

void MainWindow::resizeEvent(QResizeEvent *pEvent)
{
    m_ModifyObj.resizeEvent(this);
    Config::Get()->setMainWindowSize(UInt32Point(pEvent->size().width(), pEvent->size().height()));
}

void MainWindow::_setDBs()
{
    // share DBMgr pointer
    m_pMapEditor->setDBMgr(m_project.getDatabaseMgr());
}

MainWindow::~MainWindow(void)
{
    Config::Get()->Del();
    if (Logfile *pLog = Logfile::Get())
        pLog->Del();
}

void MainWindow::_mapScreenshot()
{
    m_pMapEditor->storeCurrentMapInClipboard();
}

void MainWindow::_openDatabase()
{
    DatabaseWindow pDB(m_project.getDatabaseMgr(), this);
    pDB.exec();
    m_pMapEditor->updateMapEditor();
}

void MainWindow::_saveProject()
{
    emit saveProject();
    m_project.saveCurrentProject();
}

void MainWindow::_newProject()
{
    QString dir = QFileDialog::getSaveFileName(this, tr("New Project"), "projects", tr("Project File"));
    QDir fileDir(dir);
    if (!fileDir.exists())
    {
        if (m_project.createNewProject(dir))
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

bool MainWindow::_loadProject(const QString &dir)
{
    QTime time;
    time.start();
    BASIC_LOG("Begin project load: " + dir);
    if (m_project.loadProject(dir))
    {
        _setDBs();
        Config::Get()->setProjectDirectory(dir);
        BASIC_LOG("Project load successfully ends after " + QString::number(time.elapsed()) + "msec.");
        m_pMapEditor->updateMapEditor();
        emit projectLoadDone();
        return true;
    }
    QErrorMessage *pMsg = new QErrorMessage(this);
    pMsg->showMessage(dir + " is no valid project directory.");
    ERROR_LOG("Unable to load " + dir + ". Corrupt project or no such directory.");
    Config::Get()->clear();
    return false;
}

void MainWindow::_closeProject()
{
    if (m_project.isOpenProject())
    {
        BASIC_LOG("Close project " + m_project.getProjectPath());
        m_pMapEditor->clearWidgets();
        m_project.closeCurrentProject();
        Config::Get()->clear();
    }
}