#pragma once

#include <QtWidgets/QWidget>
#include "DatabaseModel.h"

namespace database
{
    namespace ui
    {
        namespace section
        {
            class Interface : public QWidget
            {
                Q_OBJECT
            private:
                virtual void _changeSelection(uint32 newID, uint32 oldID) = 0;
                virtual void _saveChanges() = 0;

            public:
                Interface(QWidget* pParent = nullptr);

            public slots:
                void onSaveChanges();

            protected slots :
                void _onSelectionChanged(uint32 newID, uint32 oldID);
            };
        }
    }
}