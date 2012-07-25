#include "DatabaseOutput.h"

namespace DATABASE
{
    DatabaseOutput::DatabaseOutput(void) : TSingleton()
    {
    }

    DatabaseOutput::~DatabaseOutput(void)
    {
    }

    void DatabaseOutput::ClearOutput()
    {
        m_ChangedSprites.clear();
    }

    void DatabaseOutput::ChangeSprite(std::string p_sType, SpritePrototype &p_ChangedProto)
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

    bool DatabaseOutput::HasSprite(std::string p_sType, UINT p_uiID)
    {
        // find type
        SpriteList::iterator t_TypeItr = m_ChangedSprites.find(p_sType);
        if (t_TypeItr == m_ChangedSprites.end())
            return false;

        // find sprite
        std::map<UINT, SpritePrototype>::iterator t_SpriteItr = t_TypeItr->second.find(p_uiID);
        if (t_SpriteItr == t_TypeItr->second.end())
            return false;

        return true;
    }
}