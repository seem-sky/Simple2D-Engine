#pragma once

#include <QtWidgets/QPushButton>
#include <Global.h>
#include "UI/UI_DatabaseDialogResize.h"

namespace database
{
    class Interface;
    namespace ui
    {
        namespace module
        {
            class ResizeButton : public QPushButton
            {
                Q_OBJECT
            public:
                ResizeButton(Interface& db, QWidget* pParent);

            private slots:
                void _clicked();

            signals:
                void sizeChanged(uint32 newSize, uint32 oldSize);

            private:
                Interface& m_DB;
            };

            class ResizeDialog : public QDialog, Ui_DatabaseDialogResize
            {
            public:
                ResizeDialog(uint32 curSize, uint32 maxSize, QWidget* pParent);

                uint32 getNewSize() const;

            private:
            };
        }
    }
}
