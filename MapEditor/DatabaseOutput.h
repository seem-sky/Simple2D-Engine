#ifndef DATABASE_OUTPUT_H
#define DATABASE_OUTPUT_H

#include "Singleton.h"
#include <Database.h>
#include <XML_Writer.h>

namespace DATABASE
{
    class DatabaseOutput : public TSingleton<DatabaseOutput>
    {
    public:
        DatabaseOutput(void);
        ~DatabaseOutput(void);

        void ClearOutput();
        SpritePrototype* GetSpritePrototype(std::string p_sType, UINT p_uiID);
        void ChangeSpritePrototype(std::string p_sType, SpritePrototype &p_ChangedProto);
        void DeleteSpritePrototype(std::string p_sType, SpritePrototype &p_DelProto);
        bool IsSpritePrototypeDeleted(std::string p_sType, UINT p_uiID);
        void GetTextureNames(std::string p_sType, std::map<UINT, std::string> &p_lTextureNames);

        XML::XML_STATE GetDBState();

        void SaveChangesTo(std::string p_sFileName);

    private:
        bool ParseSpriteChange(XML::XML_WriteData *p_pElement, SpritePrototype *p_pProto);

        SpriteList m_ChangedSprites;
        XML::XML_Writer *m_pWriter;
        inline void KillXMLThread()
        {
            if (m_pWriter)
            {
                m_pWriter->Kill();
                m_pWriter = NULL;
            }
        }
    };
}
#endif