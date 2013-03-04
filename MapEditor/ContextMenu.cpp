#include "ContextMenu.h"
#include "moc_ContextMenu.h"
#include "EventEditorWidget.h"
#include "CommandWindow.h"
#include "MapTree.h"

namespace CONTEXT_MENU
{
    /*#####
    # Map
    #####*/
    MapTreeContextMenu::MapTreeContextMenu(QWidget *pParent) : QMenu(pParent)
    {
        QAction *pAction = new QAction("new map", this);
        addAction(pAction);
        connect(pAction, SIGNAL(triggered()), parent(), SLOT(_newMap()));
    }

    MapItemContextMenu::MapItemContextMenu(QWidget *pParent) : MapTreeContextMenu(pParent)
    {
        QAction *pAction = new QAction("open map", this);
        addAction(pAction);
        connect(pAction, SIGNAL(triggered()), parent(), SLOT(_openMap()));

        pAction = new QAction("settings", this);
        addAction(pAction);
        connect(pAction, SIGNAL(triggered()), parent(), SLOT(_openMapSettingsDialog()));

        pAction = new QAction("delete map", this);
        addAction(pAction);
        connect(pAction, SIGNAL(triggered()), parent(), SLOT(_deleteMap()));
    }

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

        EVENT_SCRIPT::EventScriptCommandPtr t_pCommand = ((ScriptLine*)t_pLine)->GetCommand();
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