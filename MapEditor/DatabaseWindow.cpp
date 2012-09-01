#include "DatabaseWindow.h"
#include "moc_DatabaseWindow.h"
#include <QTGui/QFileDialog>
#include <StringAdditions.h>
#include <QtCore/QTimer>
#include <QtGui/QPainter>
#include <QtGui/QGraphicsPixmapItem>
#include "EventEditorWindow.h"
#include "EditorConfig.h"
#include "MainWindow.h"

using namespace DATABASE;

DatabaseWindow::DatabaseWindow(QMainWindow *p_pParent) : QMainWindow(p_pParent), Ui_Database()
{
    setupUi(this);

    m_ResizeObj.AddResizeWidget(m_pSections, QPoint(10, ButtonCancel->height()));
    m_ResizeObj.AddMoveWidget(ButtonApply);
    m_ResizeObj.AddMoveWidget(ButtonCancel, QPoint(ButtonApply->width(), 0));
    m_ResizeObj.AddMoveWidget(ButtonOK, QPoint(ButtonApply->width()+ButtonOK->width(), 0));

    m_sLogLocationName = LOGFILE_ENGINE_LOG_NAME + "DatabaseWindow : ";

    connect(ButtonOK, SIGNAL(clicked()), this, SLOT(ClickButtonOK()));
    connect(ButtonApply, SIGNAL(clicked()), this, SLOT(ClickButtonApply()));
    m_pSections->setCurrentIndex(0);
}

DatabaseWindow::~DatabaseWindow(void)
{
    if (QWidget *t_pParent = (QWidget*)parent())
        t_pParent->setEnabled(true);
}

void DatabaseWindow::closeEvent(QCloseEvent *event)
{
    event->accept();

    if (DatabaseOutput *t_pDBOut = DatabaseOutput::Get())
        t_pDBOut->ClearOutput();

    delete this;
}

void DatabaseWindow::ClickButtonApply()
{
    QString t_sDBDir;
    if (EditorConfig *t_pConfig = EditorConfig::Get())
        t_sDBDir = QString(t_pConfig->GetProjectDirectory().c_str());

    t_sDBDir += "\\Game\\GameDatabase.db";

    if (DatabaseOutput *t_pDBOut = DatabaseOutput::Get())
    {
        t_pDBOut->SaveChangesTo(t_sDBDir.toStdString());
        if (MainWindow *t_pParent = (MainWindow*)parent())
        {
            QTimer *t_pTimer = new QTimer(this);
            t_pParent->SetNewTimer(t_pTimer);
            t_pParent->SetWindowAction(WINDOW_SAVE_DB);
            connect(t_pTimer,SIGNAL(timeout()), t_pParent, SLOT(CustomUpdate()));
            t_pTimer->start(100);
            setEnabled(false);
        }
    }
}

void DatabaseWindow::ClickButtonOK()
{
    ClickButtonApply();
    if (MainWindow *p_pParent = (MainWindow*)parent())
        connect(p_pParent, SIGNAL(WindowActionDone()), this, SLOT(close()));
}