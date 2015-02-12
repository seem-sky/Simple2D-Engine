#ifndef SCRIPT_AREA_MAPPING_REVERT_H
#define SCRIPT_AREA_MAPPING_REVERT_H

#include <RevertInterface.h>
#include <Map/ScriptArea/ScriptArea.h>

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

            class Move : public Base
            {
            public:
                Move(MAP::SCRIPT_AREA::ScriptArea* pScriptArea, uint32 index, const GEOMETRY::Point<int32>& pos, MapEditor& editor);

                void revert();
                bool isEmpty() const { return !m_pScriptArea; }

            private:
                uint32 m_Index;
                GEOMETRY::Point<int32> m_Pos;
                MAP::SCRIPT_AREA::ScriptArea* m_pScriptArea = nullptr;
            };

            class Remove : public Base
            {
            public:
                Remove(MAP::SCRIPT_AREA::ScriptArea* pArea, MapEditor& editor);

                void revert();
                bool isEmpty() const { return !m_pArea; }

            private:
                std::unique_ptr<MAP::SCRIPT_AREA::ScriptArea> m_pArea;
            };
        }
    }
}
#endif
