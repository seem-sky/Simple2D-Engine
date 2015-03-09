#pragma once

#include <Global.h>
#include <QtCore/QString>

namespace database
{
    namespace prototype
    {
        class Base
        {
        public:
            Base() = default;
            Base(uint32 ID);

            uint32 getID() const;
            QString getName() const;
            void setName(const QString& name);

            virtual bool isEmpty() const;

        private:
            QString m_Name;
            uint32 m_ID = 0;
        };
    }
}

