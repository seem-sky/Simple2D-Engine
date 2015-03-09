#ifndef DATABASE_MODULE_LIST_H
#define DATABASE_MODULE_LIST_H

#include <QtWidgets/QTreeView>
#include <Global.h>

namespace database
{
    class Model;
    class Interface;
    namespace ui
    {
        namespace module
        {
            class List : public QTreeView
            {
                Q_OBJECT
            public:
                List(const Interface& DB, QWidget* pParent = nullptr);
                List(Model* pModel, QWidget* pParent = nullptr);
                List(QWidget* pParent = nullptr);

                void setup(const Interface& DB);
                void setup(Model* pModel);

                void selectPrototype(uint32 ID);
                uint32 getCurrentID() const;

            private slots:
                void _onSelectionChanged(const QModelIndex& current, const QModelIndex& previous);

            public slots:
                void onSizeChanged(uint32 newSize, uint32 oldSize);

            signals:
                void selectionChanged(uint32 newID, uint32 oldID);
            };
        }
    }
}
#endif
