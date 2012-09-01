#ifndef DATABASE_OUTPUT_H
#define DATABASE_OUTPUT_H

#include "Singleton.h"
#include <Database.h>
#include <XML_Writer.h>

namespace DATABASE
{
    enum CUSTOM_VARIABLE_TYPE
    {
        VARIABLE_NONE,
        VARIABLE_BOOL,
        VARIABLE_INT,
        VARIABLE_FLOAT,
        VARIABLE_STRING,
    };

    class DatabaseOutput : public TSingleton<DatabaseOutput>
    {
    public:
        DatabaseOutput(void);
        ~DatabaseOutput(void);

        void ClearOutput();
        SpritePrototype* GetSpritePrototype(std::string p_sType, uint32 p_uiID);
        void ChangeSpritePrototype(std::string p_sType, SpritePrototype &p_ChangedProto);
        void DeleteSpritePrototype(std::string p_sType, SpritePrototype &p_DelProto);
        bool IsSpritePrototypeDeleted(std::string p_sType, uint32 p_uiID);
        void GetTextureNames(std::string p_sType, std::map<uint32, std::string> &p_lTextureNames);

        /*#####
        # Objects
        #####*/
        ObjectPrototype* GetObjectPrototype(uint32 p_uiID);
        void ChangeObjectPrototype(ObjectPrototype &p_ChangedProto);
        void DeleteObjectPrototype(ObjectPrototype &p_DelProto);
        bool IsObjectPrototypeDeleted(uint32 p_uiID);
        void GetObjectNames(std::map<uint32, std::string> &p_lObjectNames);
        // Objects - Custom Variables
        uint32 AddNewCustomObjectVariable(uint32 p_uiObjectID, CUSTOM_VARIABLE_TYPE p_Type);
        bool DeleteCustomObjectVariable(uint32 p_uiObjectID, CUSTOM_VARIABLE_TYPE p_Type, uint32 p_uiVariableID);

        XML::XML_STATE GetDBState();

        void SaveChangesTo(std::string p_sFileName);

    private:
        bool ParseSpriteChange(XML::XML_WriteData *p_pElement, const SpritePrototype *p_pProto);
        bool ParseObjectChange(XML::XML_WriteData *p_pElement, const ObjectPrototype *p_pProto);

        SpriteList m_ChangedSprites;
        ObjectList m_ChangedObjects;
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