#ifndef CONTEXT_MENU_H
#define CONTEXT_MENU_H
#include <QtGui/QMenu>

namespace CONTEXT_MENU
{
    /*#####
    # ScriptLineContextMenu
    #####*/
    class ScriptLineContextMenu : public QMenu
    {
    public:
        ScriptLineContextMenu(QWidget *p_pParent = NULL);
    };

    class ScriptLineActionEdit : public QAction
    {
        Q_OBJECT
    public:
        ScriptLineActionEdit(ScriptLineContextMenu *p_pParent = NULL);

    private slots:
        void Activated();
    };

    class ScriptLineActionDelete : public QAction
    {
        Q_OBJECT
    public:
        ScriptLineActionDelete(ScriptLineContextMenu *p_pParent = NULL);

    private slots:
        void Activated();
    };
}
#endif