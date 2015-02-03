#ifndef SCRIPT_AREA_MAPPING_REVERT_H
#define SCRIPT_AREA_MAPPING_REVERT_H

#include <RevertInterface.h>
#include <ScriptArea.h>

class MapEditor;

namespace MAPPING_MODE
{
    namespace SCRIPT_AREA
    {
        namespace REVERT
        {
            class Base : public MAP::REVERT::Interface
            {
            public:
                Base(MapEditor& editor);

            protected:
                MapEditor& m_Editor;
            };

            class Add : public Base
            {
            public:
                Add(MAP::GUID guid, MapEditor& editor);

                void revert();
                bool isEmpty() const { return !m_GUID; }

            private:
                MAP::GUID m_GUID;
            };

            class Modify : public Base
            {
            public:
                Modify(MAP::SCRIPT_AREA::ScriptArea* pArea, MapEditor& editor);

                void revert();
                bool isEmpty() const { return !m_pArea; }

            private:
                std::unique_ptr<MAP::SCRIPT_AREA::ScriptArea> m_pArea;
            };

            class Remove : public Base
            {
            public:
                Remove(MAP::SCRIPT_AREA::ScriptArea* pArea, MapEditor& editor);

                void revert();

            private:
                std::unique_ptr<MAP::SCRIPT_AREA::ScriptArea> m_pArea;
                bool isEmpty() const { return !m_pArea; }
            };
        }
    }
}
#endif
