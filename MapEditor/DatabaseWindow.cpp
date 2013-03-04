#include "DatabaseWindow.h"
#include "moc_DatabaseWindow.h"
#include <QTGui/QFileDialog>
#include <StringAdditions.h>
#include <QtCore/QTimer>
#include <QtGui/QPainter>
#include <QtGui/QGraphicsPixmapItem>
#include "Config.h"
#include "MainWindow.h"

using namespace DATABASE;

DatabaseWindow::DatabaseWindow(QWidget *p_pParent) : QDialog(p_pParent), Ui_Database()
{
    setupUi(this);

    m_ModifyObj.setWidget(m_pSections, MODIFY_RESIZE, QPoint(10, ButtonCancel->height()+10));
    m_ModifyObj.setWidget(ButtonApply, MODIFY_MOVE, QPoint(10, 10));
    m_ModifyObj.setWidget(ButtonCancel, MODIFY_MOVE, QPoint(ButtonApply->width()+10, 10));
    m_ModifyObj.setWidget(ButtonOK, MODIFY_MOVE, QPoint(ButtonApply->width()+ButtonOK->width()+10, 10));

    m_sLogLocationName = LOGFILE_ENGINE_LOG_NAME + "DatabaseWindow : ";

    connect(ButtonOK, SIGNAL(clicked()), this, SLOT(ClickButtonOK()));
    connect(ButtonApply, SIGNAL(clicked()), this, SLOT(ClickButtonApply()));
    for (int i = 0; i < m_pSections->count(); ++i)
        connect(this, SIGNAL(ReloadPage()), m_pSections->widget(i), SLOT(LoadPage()));
    m_pSections->setCurrentIndex(0);
}

void DatabaseWindow::closeEvent(QCloseEvent *event)
{
    event->accept();

    if (DatabaseOutput *t_pDBOut = DatabaseOutput::Get())
        t_pDBOut->ClearOutput();

    delete this;
}

void DatabaseWindow::SaveDatabase()
{
    QString t_sDBDir;
    if (Config *t_pConfig = Config::Get())
        t_sDBDir = QString::fromStdString(t_pConfig->getProjectDirectory());

    t_sDBDir += "\\Game\\GameDatabase.xml";

    if (DatabaseOutput *t_pDBOut = DatabaseOutput::Get())
    {
        t_pDBOut->SaveChangesTo(t_sDBDir.toStdString());
        if (MainWindow *t_pParent = (MainWindow*)parent())
        {
            QTimer *t_pTimer = new QTimer(this);
            t_pParent->setNewTimer(t_pTimer);
            t_pParent->setWindowAction(WINDOW_SAVE_DB);
            connect(t_pTimer,SIGNAL(timeout()), t_pParent, SLOT(CustomUpdate()));
            t_pTimer->start(100);
            setEnabled(false);
        }
    }
}

void DatabaseWindow::DBSaved()
{
    emit ReloadPage();
}

void DatabaseWindow::ClickButtonApply()
{
    SaveDatabase();
    if (MainWindow *p_pParent = (MainWindow*)parent())
        connect(p_pParent, SIGNAL(WindowActionDone()), this, SLOT(DBSaved()));
}

void DatabaseWindow::ClickButtonOK()
{
    SaveDatabase();
    if (MainWindow *p_pParent = (MainWindow*)parent())
        connect(p_pParent, SIGNAL(WindowActionDone()), this, SLOT(close()));
}