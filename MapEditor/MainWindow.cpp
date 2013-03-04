#include "MainWindow.h"
#include "moc_MainWindow.h"
#include "DatabaseWindow.h"
#include <QtGui/QFileDialog>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QTextstream>
#include <QtCore/QTimer>
#include <QtGui/QErrorMessage>
#include "DatabaseOutput.h"
#include <QtGui/QBitmap>

using namespace DATABASE;
using namespace XML;

MainWindow::MainWindow(QMainWindow *pParent) : QMainWindow(pParent), m_WindowAction(WINDOW_DO), m_pTimer(NULL)
{
    setupUi(this);

    Database::Get();

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

MainWindow::~MainWindow(void)
{
    if (Database *pDB = Database::Get())
        pDB->Del();

    if (DatabaseOutput *pDBOut = DatabaseOutput::Get())
        pDBOut->Del();

    if (Logfile *pLog = Logfile::Get())
        pLog->Del();
}

void MainWindow::_openDatabase()
{
    DatabaseWindow *pDB = new DatabaseWindow(this);
    pDB->exec();
}

void MainWindow::_saveProject()
{
    Config *pConf = Config::Get();
    if (!pConf)
        return;

    bool mapDBResult = false;
    if (MAP::MapDatabase *pMapDB = MAP::MapDatabase::Get())
        pMapDB->saveMapDatabase(mapDBResult, pConf->getProjectDirectory());

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
        newFile.setFileName(sFileName + "/Game/MapDatabase.xml");
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

bool MainWindow::_loadDB()
{
    if (Database *pDB = Database::Get())
    {
        bool mapDBResult = false;
        if (Config *pConf = Config::Get())
        {
            pDB->LoadDB(pConf->getProjectDirectory()+"/Game/GameDatabase.xml");
            MAP::MapDatabase::Get()->loadMapDatabase(mapDBResult, pConf->getProjectDirectory());
        }

        if (!mapDBResult)
            return false;

        if (!m_pTimer)
            m_pTimer = new QTimer(this);
        connect(m_pTimer,SIGNAL(timeout()),SLOT(_customUpdate()));
        m_pTimer->start(100);
        m_WindowAction = WINDOW_LOAD_DB;
        setEnabled(false);
        return true;
    }

    return false;
}

bool MainWindow::_loadProject(const std::string &sDir)
{
    QDir t_dir(QString::fromStdString(sDir + "/Game"));

    if (t_dir.exists() && _loadDB())
    {
        Config::Get()->setProjectDirectory(sDir);
        emit projectLoadDone();
        Config::Get()->saveConfig();
        BASIC_LOG("Successfully load " + sDir + ".");
        return true;
    }
    else
    {
        QErrorMessage *pMsg = new QErrorMessage(this);
        pMsg->showMessage(QString::fromStdString(sDir + " is no valid project directory."));
        BASIC_LOG("Unable to load " + sDir + ". Corrupt project or no such directory.");
        Config::Get()->clear();
    }

    return false;
}

void MainWindow::_loadProject()
{
    QDir sAppDir = QDir::current();
    QString sFileName = QFileDialog::getExistingDirectory(this, tr("Open Project"), sAppDir.path()+"/projects", QFileDialog::ShowDirsOnly);
    if (sFileName.isEmpty())
        return;

    _closeProject();
    _loadProject(sFileName.toStdString());
}

void MainWindow::_closeProject()
{
    if (Config::Get()->getProjectDirectory() != "")
    {
        BASIC_LOG("Close project " + Config::Get()->getProjectDirectory());
        m_pMapEditor->clearWidgets();
        MAP::MapDatabase::Get()->unloadMapDatabase();
        DatabaseOutput::Get()->ClearOutput();
        // Database::Get()->ClearDB();
        Config::Get()->clear();
    }
}

void MainWindow::_customUpdate()
{
    switch(m_WindowAction)
    {
    case WINDOW_SAVE_DB:
        if (DatabaseOutput *pDB = DatabaseOutput::Get())
        {
            switch(pDB->GetDBState())
            {
            case THREAD_IN_PROGRESS:
                if (m_pTimer)
                    m_pTimer->start(100);
                return;
            //case XML_CORRUPT_FILE:
            case THREAD_FAILED:
            //case XML_NO_FILE:
                {
                    QErrorMessage *pMsg = new QErrorMessage(this);
                    pMsg->showMessage("Unable to read GameDatabase.");
                }
            case THREAD_DONE:
            default:
                if (DatabaseOutput *pDBOut = DatabaseOutput::Get())
                    pDBOut->ClearOutput();

                // reload db, so new files are stored
                _loadDB();
                break;
            }
        }
        break;
    case WINDOW_LOAD_DB:
        if (Database *pDB = Database::Get())
        {
            switch(pDB->GetDBState())
            {
            case THREAD_IN_PROGRESS:
                if (m_pTimer)
                    m_pTimer->start(100);
                return;
            //case XML_CORRUPT_FILE:
            case THREAD_FAILED:
            //case XML_NO_FILE:
            {
                QErrorMessage *pMsg = new QErrorMessage(this);
                pMsg->showMessage("Unable to read GameDatabase.");
            }
            case THREAD_DONE:
            default:
                m_WindowAction = WINDOW_DO;
                break;
            }
        }
        break;
    default:
        break;
    }

    if (m_WindowAction == WINDOW_DO)
    {
        if (m_pTimer)
        {
            if (QWidget *pParent = (QWidget*)m_pTimer->parent())
                pParent->setEnabled(true);
            else
                setEnabled(true);
            disconnect(m_pTimer, SIGNAL(timeout()), this, SLOT(_customUpdate()));
            delete m_pTimer;
            m_pTimer = NULL;

            emit WindowActionDone();
        }
    }
}