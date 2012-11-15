#include "GlobalVariableDatabaseWindow.h"
#include "moc_GlobalVariableDatabaseWindow.h"
#include "DatabaseWindow.h"

GlobalVariableDatabaseWindow::GlobalVariableDatabaseWindow(QWidget *p_pParent) : QWidget(p_pParent), Ui_GlobalVariableDatabase()
{
    setupUi(this);
    LoadPage();
}

void GlobalVariableDatabaseWindow::LoadPage()
{
    if (DATABASE::DatabaseOutput *t_pDBOut = DATABASE::DatabaseOutput::Get())
    {
        if (DATABASE::Database *t_pDB = DATABASE::Database::Get())
            t_pDBOut->SetGlobalVariables(*t_pDB->GetGlobalVariables());

        m_pBools->SetVariableHolder(t_pDBOut->GetGlobalVariables());
        m_pInts->SetVariableHolder(t_pDBOut->GetGlobalVariables());
        m_pFloats->SetVariableHolder(t_pDBOut->GetGlobalVariables());
        m_pStrings->SetVariableHolder(t_pDBOut->GetGlobalVariables());
    }
}