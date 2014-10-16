#ifndef OBJECT_MAPPING_REVERT_H
#define OBJECT_MAPPING_REVERT_H

#include <RevertInterface.h>
#include "WorldObjectInfo.h"

class MapEditor;

namespace MAPPING_MODE
{
    namespace REVERT
    {
        class ObjectBase : public MAP::REVERT::Interface
        {
        public:
            ObjectBase(MapEditor& editor);

        protected:
            MapEditor& m_Editor;
        };

        class ObjectAdd : public ObjectBase
        {
        public:
            ObjectAdd(MAP::MAP_DATA::GUID guid, MapEditor& editor);

            void revert();

        private:
            MAP::MAP_DATA::GUID m_GUID;
        };

        class ObjectModify : public ObjectBase
        {
        public:
            ObjectModify(const MAP::MAP_DATA::WorldObjectInfo& info, MapEditor& editor);

            void revert();

        private:
            MAP::MAP_DATA::WorldObjectInfo m_Info;
        };

        class ObjectRemove : public ObjectBase
        {
        public:
            ObjectRemove(const MAP::MAP_DATA::WorldObjectInfo& info, MapEditor& editor);

            void revert();

        private:
            MAP::MAP_DATA::WorldObjectInfo m_Info;
        };
    }
}
#endif
