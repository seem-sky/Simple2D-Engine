#include "MappingModeContextMenuAction.h"
#include "moc_MappingModeContextMenuAction.h"
#include <memory>
#include <QtWidgets/QMenu>

using namespace MAPPING_MODE;

ContextMenuAction::ContextMenuAction(const QString& text, MapEditor& editor, const QPoint& pos, QObject* pParent /* = nullptr */)
    : QAction(text, pParent), m_MapEditor(editor), m_Pos(pos)
{
    connect(this, SIGNAL(triggered()), this, SLOT(_onTriggered()));
}

void ContextMenuAction::_onTriggered()
{
    emit triggered(m_MapEditor, m_Pos);
}

// free functions
ContextMenuAction* MAPPING_MODE::addAction(MapEditor& editor, QMenu* pMenu, const QString& text, const QPoint& pos, QActionGroup* pGroup)
{
    if (!pMenu)
        return nullptr;
    std::unique_ptr<ContextMenuAction> pAction(new MAPPING_MODE::ContextMenuAction(text, editor, pos, pGroup));
    pMenu->addAction(pAction.get());
    return pAction.release();
}
