#ifndef MAP_SCRIPT_AREA_MANAGER_H
#define MAP_SCRIPT_AREA_MANAGER_H

#include "ScriptArea.h"

namespace MAP
{
    namespace SCRIPT_AREA
    {
        class Manager
        {
        public:
            Manager() = default;

            ScriptArea* addScriptArea(ScriptArea* pScript);
            ScriptArea* addScriptArea(const Data& data);
            const ScriptArea* getScriptArea(GUID guid) const;
            ScriptArea* getScriptArea(GUID guid);
            const std::vector<std::unique_ptr<ScriptArea>>& getScriptAreas() const;
            ScriptArea* takeScriptArea(GUID guid);
            bool hasScriptArea(GUID guid) const;
            void removeScriptArea(GUID guid);
            GUID getNewGUID() const;

            uint32 count() const;
            bool isEmpty() const;

        private:
            std::vector<std::unique_ptr<ScriptArea>> m_pScriptAreas;
        };
    }
}
#endif
