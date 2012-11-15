#include "ContextMenu.h"
#include "moc_ContextMenu.h"
#include "EventEditorWidget.h"
#include "CommandWindow.h"

namespace CONTEXT_MENU
{
    /*#####
    # ScriptLineContextMenu
    #####*/
    ScriptLineContextMenu::ScriptLineContextMenu(QWidget *p_pParent) : QMenu(p_pParent)
    {
        addAction(new ScriptLineActionEdit(this));
        addAction(new ScriptLineActionDelete(this));
    }

    ScriptLineActionEdit::ScriptLineActionEdit(ScriptLineContextMenu *p_pParent) : QAction(p_pParent)
    {
        connect(this, SIGNAL(triggered()), this, SLOT(Activated()));
        setText("Edit");
    }

    void ScriptLineActionEdit::Activated()
    {
        if (!parent())
            return;

        ScriptPage *t_pPage = (ScriptPage*)parent()->parent();
        if (!t_pPage)
            return;

        QTreeWidgetItem *t_pLine = t_pPage->currentItem();
        if (!t_pLine)
            return;

        EVENT_SCRIPT::EventScriptCommand *t_pCommand = ((ScriptLine*)t_pLine)->GetCommand();
        if (!t_pCommand)
            return;

        // if command changed
        if (CommandWindow::ExecCorrectCommandWindow(t_pCommand, t_pPage))
            ((ScriptLine*)t_pLine)->UpdateItemText();
    }

    ScriptLineActionDelete::ScriptLineActionDelete(ScriptLineContextMenu *p_pParent) : QAction(p_pParent)
    {
        connect(this, SIGNAL(triggered()), this, SLOT(Activated()));
        setText("Delete");
    }

    void ScriptLineActionDelete::Activated()
    {
        if (!parent())
            return;

        ScriptPage *t_pPage = (ScriptPage*)parent()->parent();
        if (!t_pPage)
            return;

        QTreeWidgetItem *t_pLine = t_pPage->currentItem();
        if (!t_pLine)
            return;

        if (QTreeWidgetItem *t_pParent = t_pLine->parent())
            ((ScriptLineContainer*)t_pParent)->RemoveCommandLine((ScriptLine*)t_pLine);
        else
            (t_pPage)->RemoveCommandLine((ScriptLine*)t_pLine);
    }
}