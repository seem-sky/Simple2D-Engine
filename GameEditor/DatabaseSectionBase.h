#pragma once

#include <QtWidgets/QWidget>
#include <QtWidgets/QGridLayout>
#include <database/Merger.h>
#include "DatabaseModuleList.h"
#include "DatabaseModuleResize.h"
#include "DatabaseModulePrototypeInterface.h"
#include <Database/Prototype/Compare.h>
#include "DatabaseModel.h"
#include "DatabaseSectionInterface.h"

namespace database
{
    namespace ui
    {
        namespace section
        {
            template <class DATABASE, class PROTOTYPE> 
            class Base : public Interface
            {
            private:
                void _saveCurrent(uint32 ID)
                {
                    if (ID)
                    {
                        std::unique_ptr<PROTOTYPE> pPrototype(new PROTOTYPE(ID));
                        m_pPrototypeModule->setupFrom(pPrototype.get());
                        m_Merger.setPrototype(pPrototype.release());
                    }
                }

                void _saveChanges()
                {
                    _saveCurrent(m_pModuleList->getCurrentID());
                    m_Merger.merge();
                }

                void _changeSelection(uint32 newID, uint32 oldID)
                {
                    // build prototype from widgets
                    _saveCurrent(oldID);

                    // setup widgets
                    if (newID)
                    {
                        m_pPrototypeModule->clear();
                        auto pPrototype = m_Merger.getPrototype(newID);
                        if (!pPrototype)
                        {
                            pPrototype = new PROTOTYPE(newID);
                            m_Merger.setPrototype(pPrototype);
                        }
                        m_pPrototypeModule->setup(pPrototype);
                    }
                }

            public:
                Base(DATABASE& db, module::PrototypeInterface<PROTOTYPE>* pModule, QWidget* pParent)
                    : Interface(pParent), m_pPrototypeModule(pModule), m_Merger(db), m_pModuleList(new module::List(m_Merger, this))
                {
                    m_pPrototypeModule->setParent(this);

                    auto pLayout = new QGridLayout(this);
                    setLayout(pLayout);
                    pLayout->setHorizontalSpacing(10);
                    // module list
                    pLayout->addWidget(m_pModuleList, 0, 0);
                    connect(m_pModuleList, SIGNAL(selectionChanged(uint32, uint32)), this, SLOT(_onSelectionChanged(uint32, uint32)));

                    // module resize
                    auto pModuleResize = new module::ResizeButton(m_Merger, this);
                    pLayout->addWidget(pModuleResize, 1, 0);
                    connect(pModuleResize, SIGNAL(sizeChanged(uint32, uint32)), m_pModuleList, SLOT(onSizeChanged(uint32, uint32)));

                    // module prototype base
                    pLayout->addWidget(m_pPrototypeModule, 0, 1, -1, 1);

                    pLayout->setColumnStretch(2, 1);

                    // select first
                    if (!m_Merger.isEmpty())
                        m_pModuleList->setCurrentIndex(m_pModuleList->model()->index(0, 0));
                }

                database::Model* getModel() const
                {
                    return dynamic_cast<database::Model*>(m_pModuleList->model());
                }

                const Merger<DATABASE, PROTOTYPE, &prototype::compare>& getDatabaseMerger() const
                {
                    return m_Merger;
                }

            protected:
                Merger<DATABASE, PROTOTYPE, &prototype::compare> m_Merger;

                // modules
                module::PrototypeInterface<PROTOTYPE>* m_pPrototypeModule = nullptr;
                module::List* m_pModuleList = nullptr;
            };
        }
    }
}