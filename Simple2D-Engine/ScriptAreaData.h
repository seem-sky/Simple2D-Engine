#ifndef SCRIPT_AREA_DATA
#define SCRIPT_AREA_DATA

#include "ScriptArea.h"

namespace MAP
{
    namespace SCRIPT_AREA
    {
        class ScriptAreaData
        {
        private:
            GUID _getNewGUID() const;

        public:
            ScriptAreaData() = default;

            ScriptArea* addScriptArea(ScriptArea* pScript);
            ScriptArea* addScriptArea(GEOMETRY::ComplexGeometricShape<int32>* pArea);
            ScriptArea* getScriptArea(GUID guid);
            const ScriptArea* getScriptArea(GUID guid) const;
            const std::vector<std::unique_ptr<ScriptArea>>& getScriptAreas() const;
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
