#include "MainWindow.h"
#include "moc_MainWindow.h"
#include "DatabaseWindow.h"
#include <QTGui/QFileDialog>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QTextstream>
#include <QtCore/QTimer>
#include <QtGui/QErrorMessage>
#include <Database.h>
#include <Logfile.h>

using namespace DATABASE;
using namespace XML;

MainWindow::MainWindow(QMainWindow *p_pParent) : QMainWindow(p_pParent), m_WindowAction(WINDOW_DO), m_pTimer(NULL)
{
    setupUi(this);

    Database::Get();
    
    connect(menuDatabase, SIGNAL(aboutToShow()), this, SLOT(OpenDatabase()));
    connect(actionNew, SIGNAL(activated()), this, SLOT(NewProject()));
    connect(actionLoad, SIGNAL(activated()), this, SLOT(LoadProject()));
}

MainWindow::~MainWindow(void)
{
    if (Database *t_pDB = Database::Get())
        t_pDB->Del();

    if (m_pTimer)
    {
        delete m_pTimer;
        m_pTimer = NULL;
    }
}

void MainWindow::OpenDatabase()
{
    DatabaseWindow *t_pDB = new DatabaseWindow(this);
    t_pDB->show();
    setEnabled(false);
    t_pDB->setEnabled(true);
}

void MainWindow::NewProject()
{
    QString t_sFileName = QFileDialog::getSaveFileName(this, tr("New Project"), "projects/untitled", tr("Project File"));
    QDir t_dir(t_sFileName);
    if (!t_dir.exists())
    {
        t_dir.mkpath(t_sFileName);
        t_dir.mkpath(t_sFileName + "/Game");
        QFile t_newFile(t_sFileName + "/Game/GameDatabase.db");
        if (t_newFile.open(QIODevice::WriteOnly))
        {
            QTextStream t_stream(&t_newFile);
            t_stream << "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>" << endl;
            t_stream << "<Database>" << endl;
            t_stream << "</Database>" << endl;
            t_newFile.close();
        }
    }
}

void MainWindow::LoadProject()
{
    QDir t_sAppDir = QDir::current();
    QString t_sFileName = QFileDialog::getExistingDirectory(this, tr("Open Project"), t_sAppDir.path()+"/projects", QFileDialog::ShowDirsOnly);
    QDir t_dir(t_sFileName + "/Game");
    bool t_bSuccess = false;
    if (t_dir.exists())
    {
        if (Database *t_pDB = Database::Get())
        {
            t_pDB->LoadDB(t_sFileName.toStdString()+"/Game/GameDatabase.db");
            if (!m_pTimer)
                m_pTimer = new QTimer(this);
            connect(m_pTimer,SIGNAL(timeout()),SLOT(CustomUpdate()));
            m_pTimer->start(100);
            m_WindowAction = WINDOW_LOAD_DB;
            setEnabled(false);
            m_sProjectDir = t_sFileName;
            t_bSuccess = true;
        }
    }

    if (!t_bSuccess)
    {
        QErrorMessage *t_Msg = new QErrorMessage(this);
        t_Msg->showMessage(t_sFileName + " is no valid project directory.");
    }
}

void MainWindow::CustomUpdate()
{
    switch(m_WindowAction)
    {
    case WINDOW_LOAD_DB:
        if (Database *t_pDB = Database::Get())
        {
            switch(t_pDB->GetDBState())
            {
            case XML_IN_PROGRESS:
                if (m_pTimer)
                    m_pTimer->start(100);
                return;
            case XML_CORRUPT_FILE:
            case XML_FAILED:
            case XML_NO_FILE:
            {
                QErrorMessage *t_Msg = new QErrorMessage(this);
                t_Msg->showMessage("Unable to read GameDatabase.");
            }
            case XML_DONE:
            default:
                break;
            }
        }
    case WINDOW_DO:
    default:
        if (m_pTimer)
        {
            setEnabled(true);
            disconnect(m_pTimer, SIGNAL(timeout()), this, SLOT(CustomUpdate()));
            delete m_pTimer;
            m_pTimer = NULL;
        }
        break;
    }
}