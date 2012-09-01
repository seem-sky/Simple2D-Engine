#include "DatabasePageTemplate.h"
#include "moc_DatabasePageTemplate.h"
#include "MainWindow.h"

DatabasePageTemplate::DatabasePageTemplate(QWidget *p_pParent) : QWidget(p_pParent), Ui_DatabasePageTemplate()
{
    setupUi(this);

    connect(m_pButtonNew, SIGNAL(clicked()), this, SLOT(ClickButtonNew()));
    connect(m_pButtonDelete, SIGNAL(clicked()), this, SLOT(ClickButtonDelete()));
    connect(m_pStoreBox, SIGNAL(currentIndexChanged(int)), this, SLOT(ItemBoxChangeCurrent(int)));

    m_sLogLocationName = LOGFILE_ENGINE_LOG_NAME + "DatabasePageTemplate : ";
}

DatabasePageTemplate::~DatabasePageTemplate(void)
{
}

void DatabasePageTemplate::ConnectWidgets()
{
    connect(m_pName, SIGNAL(editingFinished()), this, SLOT(NameChanged()));
}

void DatabasePageTemplate::DisconnectWidgets()
{
    disconnect(m_pName, SIGNAL(editingFinished()), this, SLOT(NameChanged()));
}

void DatabasePageTemplate::ItemBoxChangeCurrent(int p_Index)
{
    ClearWidgets();
    if (p_Index == -1)
        return;

    QString t_sID = m_pStoreBox->currentText();
    t_sID.truncate(t_sID.indexOf(":"));
    SelectItem(t_sID.toUInt());
}
void DatabasePageTemplate::ClickButtonDelete()
{
    ChangeItem(GetCurrentItemID(), true);
    m_pStoreBox->removeItem(m_pStoreBox->currentIndex());
}

int DatabasePageTemplate::InsertItem(uint32 p_uiID, QString p_sData)
{
    m_uilIDCache.insert(p_uiID);
    if ((int)p_uiID > m_pStoreBox->count())
    {
        m_pStoreBox->addItem(p_sData);
        return -1;
    }

    QString t_sText;
    uint32 t_uiID = 0;
    for (int t_uiI = 0; t_uiI < m_pStoreBox->count(); ++t_uiI)
    {
        t_sText = m_pStoreBox->itemText(t_uiI);
        t_sText.truncate(t_sText.indexOf(":"));
        t_uiID = t_sText.toUInt();
        if (p_uiID < t_uiID)
        {
            m_pStoreBox->insertItem(t_uiI, p_sData);
            return t_uiI;
        }
    }

    return -1;
}

void DatabasePageTemplate::ClearWidgets()
{
    m_pID->clear();
    m_pName->clear();
}

uint32 DatabasePageTemplate::GetNewID()
{
    uint32 t_uiID = 1;
    for (std::set<uint32>::iterator t_Itr = m_uilIDCache.begin(); t_Itr != m_uilIDCache.end() && t_uiID < (uint32)(0-1); ++t_Itr)
    {
        if (t_uiID < *t_Itr)
            break;

        ++t_uiID;
    }

    return t_uiID;
}