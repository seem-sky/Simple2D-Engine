#ifndef OBJECT_MAPPING_REVERT_H
#define OBJECT_MAPPING_REVERT_H

#include <RevertInterface.h>
#include "WorldObjectInfo.h"

class MapEditor;

namespace MAPPING_MODE
{
    namespace OBJECT
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
                Modify(const MAP::MAP_DATA::WorldObjectInfo& info, MapEditor& editor);

                void revert();
                bool isEmpty() const { return !m_Info.isValid(); }

            private:
                MAP::MAP_DATA::WorldObjectInfo m_Info;
            };

            class Remove : public Base
            {
            public:
                Remove(const MAP::MAP_DATA::WorldObjectInfo& info, MapEditor& editor);

                void revert();
                bool isEmpty() const { return !m_Info.isValid(); }

            private:
                MAP::MAP_DATA::WorldObjectInfo m_Info;
            };
        }
    }
}
#endif
