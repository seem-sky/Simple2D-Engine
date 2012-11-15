#include "CustomVariablePageTemplateWidget.h"
#include "moc_CustomVariablePageTemplateWidget.h"
#include "VariableCountDialog.h"

CustomVariablePageTemplateWidget::CustomVariablePageTemplateWidget(QWidget *p_pParent) : QWidget(p_pParent), Ui_CustomVariablePageTemplate(),
m_uiOwnerID(0), m_pVariableHolder(NULL), m_pPrototype(NULL)
{
    setupUi(this);
    connect(m_pStoreBox, SIGNAL(currentIndexChanged(int)), this, SLOT(IndexChanged(int)));
    connect(m_pResizeButton, SIGNAL(clicked()), this, SLOT(ResizeVariablesClicked()));
}

void CustomVariablePageTemplateWidget::IndexChanged(int p_Index)
{
    ClearWidgets();
    if (p_Index == -1)
        return;

    QString t_sText = m_pStoreBox->currentText();
    QString t_sID = t_sText;
    t_sID.truncate(t_sID.indexOf(":"));
    // if == P, its a parent variable
    if (t_sID == "P")
    {
        if (m_pPrototype)
        {
            t_sText = t_sText.right(t_sText.size()-2);
            QString t_sPID = t_sText;
            t_sPID.truncate(t_sPID.indexOf(":"));
            t_sText = t_sText.right(t_sText.size()-t_sText.indexOf(":")-1);
            QString t_sPVID = t_sText;
            t_sPVID.truncate(t_sPVID.indexOf(":"));
            SelectItem(t_sPVID.toUInt(), t_sPID.toUInt());
        }
    }
    else
        SelectItem(t_sID.toUInt());
}

void CustomVariablePageTemplateWidget::ConnectWidgets()
{
    connect(m_pName, SIGNAL(editingFinished()), this, SLOT(NameChanged()));
}

void CustomVariablePageTemplateWidget::DisconnectWidgets()
{
    disconnect(m_pName, SIGNAL(editingFinished()), this, SLOT(NameChanged()));
}

void CustomVariablePageTemplateWidget::ClearWidgets()
{
    m_pName->clear();
    m_pID->clear();
}

int CustomVariablePageTemplateWidget::InsertItem(uint32 p_uiID, QString p_sData)
{
    if ((int)p_uiID > m_pStoreBox->count())
    {
        m_pStoreBox->addItem(p_sData);
        return m_pStoreBox->count()-1;
    }

    QString t_sText;
    uint32 t_uiID = 0;
    for (int t_uiI = 0; t_uiI < m_pStoreBox->count(); ++t_uiI)
    {
        t_sText = m_pStoreBox->itemText(t_uiI);
        t_sText.truncate(t_sText.indexOf(":"));
        if (t_sText == "P")
            continue;

        t_uiID = t_sText.toUInt();
        if (p_uiID < t_uiID)
        {
            m_pStoreBox->insertItem(t_uiI, p_sData);
            return t_uiI;
        }
    }

    m_pStoreBox->addItem(p_sData);
    return m_pStoreBox->count()-1;
}

void CustomVariablePageTemplateWidget::SetWidgets(uint32 p_uiID, QString p_sName, bool p_bEnabled)
{
    m_pName->setText(p_sName);
    m_pName->setEnabled(p_bEnabled ? true : false);
    m_pID->setValue(p_uiID);
    m_pID->setEnabled(p_bEnabled ? true : false);
}

void CustomVariablePageTemplateWidget::ResizeVariablesClicked()
{
    if (!m_pVariableHolder)
        return;

    uint32 t_uiCounter = GetVariableCount();
    VariableCountDialog t_Dialog(this, &t_uiCounter);
    if (t_Dialog.exec())
        ResizeVariableCount(t_uiCounter);
}