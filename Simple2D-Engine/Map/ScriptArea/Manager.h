#ifndef MAP_SCRIPT_AREA_MANAGER_H
#define MAP_SCRIPT_AREA_MANAGER_H

#include "ScriptArea.h"

namespace MAP
{
    namespace SCRIPT_AREA
    {
        class Manager
        {
        private:
            GUID _getNewGUID() const;

        public:
            Manager() = default;

            ScriptArea* addScriptArea(const AREA::Data& area, const ACTION::Data& action);
            ScriptArea* addScriptArea(ScriptArea* pScript);
            ScriptArea* addScriptArea(const Data& data);

            const ScriptArea* getScriptArea(GUID guid) const;
            ScriptArea* getScriptArea(GUID guid);
            const std::vector<std::unique_ptr<ScriptArea>>& getScriptAreas() const;

            ScriptArea* takeScriptArea(GUID guid);
            bool hasScriptArea(GUID guid) const;
            void removeScriptArea(GUID guid);

            uint32 count() const;
            bool isEmpty() const;

        private:
            std::vector<std::unique_ptr<ScriptArea>> m_pScriptAreas;
        };
    }
}
#endif
