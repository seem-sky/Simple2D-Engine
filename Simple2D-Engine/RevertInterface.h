#ifndef REVERT_INTERFACE_H
#define REVERT_INTERFACE_H

#include <vector>
#include <memory>

namespace MAP
{
    namespace REVERT
    {
        class Interface
        {
        public:
            virtual void revert() = 0;
            virtual bool isEmpty() const = 0;
        };

        class RevertContainer : public Interface
        {
        public:
            void revert()
            {
                for (auto& pRevert : m_Reverts)
                    pRevert->revert();
            }

            void addRevert(Interface* pRevert)
            {
                m_Reverts.push_back(std::unique_ptr<Interface>(pRevert));
            }

            bool isEmpty() const
            {
                return m_Reverts.empty();
            }

        private:
            std::vector<std::unique_ptr<Interface>> m_Reverts;
        };
    }
}

#endif
