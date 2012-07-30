#include "DatabaseOutput.h"
#include <XML_Writer.h>

using namespace XML;

namespace DATABASE
{
    DatabaseOutput::DatabaseOutput(void) : m_pWriter(NULL), TSingleton()
    {
        m_sLogLocationName = LOGFILE_ENGINE_LOG_NAME + "DatabaseOutput : ";
    }

    DatabaseOutput::~DatabaseOutput(void)
    {
        KillXMLThread();
    }

    void DatabaseOutput::ClearOutput()
    {
        m_ChangedSprites.clear();
    }

    void DatabaseOutput::ChangeSpritePrototype(std::string p_sType, SpritePrototype &p_ChangedProto)
    {
        // find type or create it
        SpriteList::iterator t_TypeItr = m_ChangedSprites.find(p_sType);
        if (t_TypeItr == m_ChangedSprites.end())
        {
            std::map<UINT, SpritePrototype> t_NewMap;
            m_ChangedSprites.insert(std::make_pair(p_sType, t_NewMap));
            t_TypeItr = m_ChangedSprites.find(p_sType);
        }

        // find prototype and overwrite it, or create it
        std::map<UINT, SpritePrototype>::iterator t_SpriteItr = t_TypeItr->second.find(p_ChangedProto.m_uiID);
        if (t_SpriteItr == t_TypeItr->second.end())
            t_TypeItr->second.insert(std::make_pair(p_ChangedProto.m_uiID, p_ChangedProto));
        else
            t_SpriteItr->second = p_ChangedProto;
    }

    SpritePrototype* DatabaseOutput::GetSpritePrototype(std::string p_sType, UINT p_uiID)
    {
        // find type
        SpriteList::iterator t_TypeItr = m_ChangedSprites.find(p_sType);
        if (t_TypeItr == m_ChangedSprites.end())
            return NULL;

        // find sprite
        std::map<UINT, SpritePrototype>::iterator t_SpriteItr = t_TypeItr->second.find(p_uiID);
        if (t_SpriteItr == t_TypeItr->second.end())
            return NULL;

        return &t_SpriteItr->second;
    }

    void DatabaseOutput::DeleteSpritePrototype(std::string p_sType, SpritePrototype &p_DelProto)
    {
        ChangeSpritePrototype(p_sType, p_DelProto);
        if (SpritePrototype *t_pProto = GetSpritePrototype(p_sType, p_DelProto.m_uiID))
            t_pProto->m_uiID = 0;
    }

    bool DatabaseOutput::IsSpritePrototypeDeleted(std::string p_sType, UINT p_uiID)
    {
        if (SpritePrototype *t_pProto = GetSpritePrototype(p_sType, p_uiID))
            if (t_pProto->m_uiID == 0)
                return true;

        return false;
    }

    void DatabaseOutput::SaveChangesTo(std::string p_sFileName)
    {
        if (m_pWriter)
            return;

        XML_WriteData t_NewChildren;
        t_NewChildren.m_ChildList.insert(std::make_pair("SpriteDatabase", XML_WriteData()));

        // parse sprite changes
        WriteChildList::iterator t_DBItr = t_NewChildren.m_ChildList.find("SpriteDatabase");
        if (t_DBItr != t_NewChildren.m_ChildList.end())
        {
            // iterate through type
            for (SpriteList::iterator t_TypeItr = m_ChangedSprites.begin(); t_TypeItr != m_ChangedSprites.end(); ++t_TypeItr)
            {
                WriteChildList::iterator t_InsertTypeItr = t_DBItr->second.m_ChildList.find(t_TypeItr->first);
                if (t_InsertTypeItr == t_DBItr->second.m_ChildList.end())
                {
                    t_DBItr->second.m_ChildList.insert(std::make_pair(t_TypeItr->first, XML_WriteData()));
                    t_InsertTypeItr = t_DBItr->second.m_ChildList.find(t_TypeItr->first);
                }

                // iterate through prototypes
                for (std::map<UINT, SpritePrototype>::iterator t_SpriteItr = t_TypeItr->second.begin(); t_SpriteItr != t_TypeItr->second.end(); ++t_SpriteItr)
                {
                    XML_WriteData t_NewElement;
                    // parse added or changed sprites
                    if (t_SpriteItr->second.m_uiID != 0)
                    {
                        if (ParseSpriteChange(&t_NewElement, &t_SpriteItr->second))
                            t_InsertTypeItr->second.m_ChildList.insert(std::make_pair("Sprite", t_NewElement));
                    }
                    // deleted sprite
                    else
                    {
                        VARIANT t_Value;
                        VariantInit(&t_Value);
                        t_Value.vt = VT_UINT;
                        t_Value.uintVal = t_SpriteItr->first;
                        t_NewElement.m_AttributeList.insert(std::make_pair("ID", t_Value));
                        t_NewElement.SetWriteState(XML_WRITE_DELETE);
                        t_InsertTypeItr->second.m_ChildList.insert(std::make_pair("Sprite", t_NewElement));
                    }
                }
            }
        }
        XML_WriteData *t_DBChanges = new XML_WriteData();
        t_DBChanges->m_ChildList.insert(std::make_pair("Database", t_NewChildren));

        m_pWriter = new XML_Writer(p_sFileName, t_DBChanges);
        ClearOutput();
    }

    bool DatabaseOutput::ParseSpriteChange(XML_WriteData *p_pElement, SpritePrototype *p_pProto)
    {
        if (!p_pElement || !p_pProto)
            return false;

        // filename
        p_pElement->m_AttributeList.insert(std::make_pair("FileName", (LPCOLESTR)_bstr_t(p_pProto->m_sFileName.c_str())));

        // ID
        p_pElement->m_AttributeList.insert(std::make_pair("ID", p_pProto->m_uiID));

        // Type
        p_pElement->m_AttributeList.insert(std::make_pair("Type", p_pProto->m_uiSpriteType));

        // transparent_color
        p_pElement->m_AttributeList.insert(std::make_pair("transparent_color", (LPCOLESTR)_bstr_t(p_pProto->m_sTransparentColor.c_str())));

        // type specific stuff
        switch (p_pProto->m_uiSpriteType)
        {
        case SPRITE_TYPE_TILE:
            // passability
            p_pElement->m_AttributeList.insert(std::make_pair("passability", p_pProto->Type.Tile.m_uiPassable));

            // terraintype
            p_pElement->m_AttributeList.insert(std::make_pair("terraintype", p_pProto->Type.Tile.m_uiTerrainType));

            // is autotile
            p_pElement->m_AttributeList.insert(std::make_pair("auto_tile", (UINT)p_pProto->Type.Tile.m_bAutotile));
        	break;

        case SPRITE_TYPE_OBJECT:
            // bounding rect
            p_pElement->m_AttributeList.insert(std::make_pair("boundingXBegin", p_pProto->Type.Object.m_uiBoundingXBegin));
            p_pElement->m_AttributeList.insert(std::make_pair("boundingXRange", p_pProto->Type.Object.m_uiBoundingXRange));
            p_pElement->m_AttributeList.insert(std::make_pair("boundingYBegin", p_pProto->Type.Object.m_uiBoundingYBegin));
            p_pElement->m_AttributeList.insert(std::make_pair("boundingYRange", p_pProto->Type.Object.m_uiBoundingYRange));
            break;
        default:
            break;
        }

        // set change flag
        p_pElement->SetWriteState(XML_WRITE_CHANGE);

        return true;
    }

    void DatabaseOutput::GetTextureNames(std::string p_sType, std::map<UINT, std::string> &p_lTextureNames)
    {
        // find type
        SpriteList::iterator t_TypeItr = m_ChangedSprites.find(p_sType);
        if (t_TypeItr == m_ChangedSprites.end())
            return;

        std::map<UINT, std::string>::iterator t_NameItr;
        for (std::map<UINT, SpritePrototype>::iterator t_SpriteItr = t_TypeItr->second.begin(); t_SpriteItr != t_TypeItr->second.end(); ++t_SpriteItr)
        {
            t_NameItr = p_lTextureNames.find(t_SpriteItr->first);
            if (t_NameItr != p_lTextureNames.end())
                t_NameItr->second = t_SpriteItr->second.m_sFileName;
            else
                p_lTextureNames.insert(std::make_pair(t_SpriteItr->first, t_SpriteItr->second.m_sFileName));
        }
    }

    XML_STATE DatabaseOutput::GetDBState()
    {
        // progress XML writer
        XML_STATE t_State = XML_NONE;
        if (m_pWriter)
        {
            t_State = m_pWriter->GetWriterState();
            switch(t_State)
            {
            case XML_IN_PROGRESS:
                break;
            case XML_DONE:
                BASIC_LOG(m_sLogLocationName + "Database save complete.");
            default:
                KillXMLThread();
                break;
            }
        }
        return t_State;
    }
}