#ifndef CONTEXT_MENU_H
#define CONTEXT_MENU_H

#include <QtWidgets/QMenu>

namespace CONTEXT_MENU
{
    /*#####
    # Map
    #####*/
    class MapTreeContextMenu : public QMenu
    {
    public:
        MapTreeContextMenu(QWidget *pParent);
    };

    class MapItemContextMenu : public MapTreeContextMenu
    {
    public:
        MapItemContextMenu(QWidget *pParent);

        void openMapSettings();
    };

    ///*#####
    //# ScriptLineContextMenu
    //#####*/
    //class ScriptLineContextMenu : public QMenu
    //{
    //public:
    //    ScriptLineContextMenu(QWidget *p_pParent = NULL);
    //};

    //class ScriptLineActionEdit : public QAction
    //{
    //    Q_OBJECT
    //public:
    //    ScriptLineActionEdit(ScriptLineContextMenu *p_pParent = NULL);

    //private slots:
    //    void Activated();
    //};

    //class ScriptLineActionDelete : public QAction
    //{
    //    Q_OBJECT
    //public:
    //    ScriptLineActionDelete(ScriptLineContextMenu *p_pParent = NULL);

    //private slots:
    //    void Activated();
    //};
}
#endif