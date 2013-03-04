#ifndef DATABASE_OUTPUT_H
#define DATABASE_OUTPUT_H

#include "Singleton.h"
#include <Database.h>
#include <XML_Writer.h>
#include <VariableHolder.h>

namespace DATABASE
{
    class DatabaseOutput : public TSingleton<DatabaseOutput>
    {
    public:
        DatabaseOutput(void);

        void ClearOutput();
        const SpritePrototype* GetLatestSpritePrototype(std::string p_sType, uint32 p_uiID);
        SpritePrototype* GetSpritePrototype(std::string p_sType, uint32 p_uiID);
        void ChangeSpritePrototype(std::string p_sType, SpritePrototype &p_ChangedProto);
        void DeleteSpritePrototype(std::string p_sType, SpritePrototype &p_DelProto);
        bool IsSpritePrototypeDeleted(std::string p_sType, uint32 p_uiID);
        void GetTextureNames(std::string p_sType, std::map<uint32, std::string> &p_lTextureNames);

        inline VariableHolder* GetGlobalVariables(){ return &m_Variables; }
        inline const VariableHolder* GetGlobalVariables() const { return &m_Variables; }
        inline void SetGlobalVariables(const VariableHolder &p_VariableHolder) { m_Variables = p_VariableHolder; }

        const VariableHolder* GetVariableHolderFromParent(const DATABASE::PrototypeType p_ProtoType, const uint32 p_uiParentID);

        /*#####
        # Objects
        #####*/
        IDList GetAllParents(const Prototype *p_pProto);
        ObjectPrototype* GetLatestObjectPrototype(uint32 p_uiID);
        ObjectPrototype* GetObjectPrototype(uint32 p_uiID);
        void ChangeObjectPrototype(ObjectPrototype &p_ChangedProto);
        void DeleteObjectPrototype(ObjectPrototype &p_DelProto);
        bool IsObjectPrototypeDeleted(uint32 p_uiID);
        void GetObjectNames(std::map<uint32, std::string> &p_lObjectNames);

        ThreadState GetDBState();

        void SaveChangesTo(std::string p_sFileName);

    private:
        bool ParseSpriteChange(XML::XML_WriteData *p_pElement, const SpritePrototype *p_pProto);
        bool ParseObjectChange(XML::XML_WriteData *p_pElement, const ObjectPrototype *p_pProto);

        SpriteTypeList m_ChangedSprites;
        ObjectPrototypeMap m_ChangedObjects;
        VariableHolder m_Variables;
        XML::XML_Writer *m_pWriter;
    };
}
#endif