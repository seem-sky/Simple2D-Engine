#ifndef DATABASE_OUTPUT_H
#define DATABASE_OUTPUT_H

#include "Singleton.h"
#include <Database.h>

namespace DATABASE
{
    class DatabaseOutput : public TSingleton<DatabaseOutput>
    {
    public:
        DatabaseOutput(void);
        ~DatabaseOutput(void);

        void ClearOutput();
        bool HasSprite(std::string p_sType, UINT p_uiID);
        void ChangeSprite(std::string p_sType, SpritePrototype &p_ChangedProto);

    private:
        SpriteList m_ChangedSprites;
    };
}
#endif