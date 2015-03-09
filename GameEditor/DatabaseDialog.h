#pragma once

#include <QtWidgets/QDialog>
#include "HelperTileCreator.h"
#include "HelperSpriteCreator.h"

namespace database
{
    class Manager;
    namespace ui
    {
        class MainDialog : public QDialog
        {
            Q_OBJECT
        public:
            MainDialog(Manager& DBMgr, QWidget* pParent = nullptr);

            QString getCurrentDatabaseSection();

        private slots:
            void _onOKButtonClicked();
            void _onCancelButtonClicked();
            void _onApplyButtonClicked();

        signals:
            void storeChanges();

        private:
            Manager& m_DBMgr;
            std::unique_ptr<helper::TileCreator> m_pTileCreator;
            std::unique_ptr<helper::SpriteCreator> m_pSpriteCreator;
        };
    }
}
