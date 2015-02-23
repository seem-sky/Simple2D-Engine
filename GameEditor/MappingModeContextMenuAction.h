#ifndef MAPPING_MODE_CONTEXT_MENU_H
#define MAPPING_MODE_CONTEXT_MENU_H

#include <QtWidgets/QAction>

class MapEditor;

namespace MAPPING_MODE
{
    class ContextMenuAction : public QAction
    {
        Q_OBJECT
    public:
        ContextMenuAction(const QString& text, MapEditor& editor, const QPoint& pos, QObject* pParent = nullptr);

    private slots:
        void _onTriggered();

    signals:
        void triggered(MapEditor& mapEditor, const QPoint& pos);

    private:
        MapEditor& m_MapEditor;
        QPoint m_Pos;
    };

    ContextMenuAction* addAction(MapEditor& editor, QMenu* pMenu, const QString& text, const QPoint& pos, QActionGroup* pGroup = nullptr);
}
#endif
