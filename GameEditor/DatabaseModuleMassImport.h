#pragma once

#include <QtWidgets/QPushButton>

namespace database
{
    namespace ui
    {
        namespace module
        {
            class MassImport : public QPushButton
            {
                Q_OBJECT
            public:
                MassImport(const QString& dir, const QString& extensions, QWidget* pParent = nullptr);

            private slots:
                void _onClicked();  

            signals:
                void selected(const QStringList& files);

            private:
                QString m_Directory;
                QString m_FileExtensions;
            };
        }
    }
}
