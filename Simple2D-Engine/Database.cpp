#include "Database.h"
#include "Logfile.h"

using namespace DATABASE;

//void Database::getPrototypeShortInfos(UInt32StringPairVector& result) const
//{
//    for (uint32 i = 0; i < getSize(); ++i)
//    {
//        if (m_Items.at(i)& & m_Items.at(i)->getID() != 0)
//            result.push_back(std::make_pair(m_Items.at(i)->getID(), m_Items.at(i)->getName()));
//    }
//}
//
//UInt32StringPairVector Database::getPrototypeShortInfos() const
//{
//    UInt32StringPairVector result;
//    for (uint32 i = 0; i < getSize(); ++i)
//    {
//        if (m_Items.at(i)& & m_Items.at(i)->getID() != 0)
//            result.push_back(std::make_pair(m_Items.at(i)->getID(), m_Items.at(i)->getName()));
//    }
//    return result;
//}

/*#####
# Special Databases
#####*/
/*#####
# TileDatabase
#####*/
TileDatabase::TileDatabase() : Database()
{
    setMaximumSize(MATH::maximum<TILE_INDEX>()-1);
}

//void TileDatabase::setPrototype(TilePrototype* pItem)
//{
//    Database::setPrototype(pItem);
//}
//
//const TilePrototype* TileDatabase::getPrototype(uint32 ID) const
//{
//    return dynamic_cast<const TilePrototype*>(Database::getPrototype(ID));
//}
//
//TilePrototype* TileDatabase::getPrototype(uint32 ID)
//{
//    return dynamic_cast<TilePrototype*>(Database::getPrototype(ID));
//}
//
//TilePrototype* TileDatabase::getNewPrototype(uint32 uiID) const
//{
//    return new TilePrototype(uiID);
//}

/*#####
# TileSetDatabase
#####*/
//void TileSetDatabase::setPrototype(TILE_SET::TileSetPrototype* pItem)
//{
//    Database::setPrototype(pItem);
//}
//
//const TILE_SET::TileSetPrototype* TileSetDatabase::getPrototype(uint32 ID) const
//{
//    return dynamic_cast<const TILE_SET::TileSetPrototype*>(Database::getPrototype(ID));
//}
//
//TILE_SET::TileSetPrototype* TileSetDatabase::getPrototype(uint32 ID)
//{
//    return dynamic_cast<TILE_SET::TileSetPrototype*>(Database::getPrototype(ID));
//}
//
//TILE_SET::TileSetPrototype* TileSetDatabase::getNewPrototype(uint32 uiID) const
//{
//    return new TILE_SET::TileSetPrototype(uiID);
//}

/*#####
# AutoTileDatabase
#####*/
AutoTileDatabase::AutoTileDatabase() : Database()
{
    setMaximumSize(MATH::maximum<AUTO_TILE_INDEX>()-1);
}

//void AutoTileDatabase::setPrototype(AUTO_TILE::AutoTilePrototype* pItem)
//{
//    Database::setPrototype(pItem);
//}
//
//const AUTO_TILE::AutoTilePrototype* AutoTileDatabase::getPrototype(uint32 ID) const
//{
//    return dynamic_cast<const AUTO_TILE::AutoTilePrototype*>(Database::getPrototype(ID));
//}
//
//AUTO_TILE::AutoTilePrototype* AutoTileDatabase::getPrototype(uint32 ID)
//{
//    return dynamic_cast<AUTO_TILE::AutoTilePrototype*>(Database::getPrototype(ID));
//}
//
//AUTO_TILE::AutoTilePrototype* AutoTileDatabase::getNewPrototype(uint32 uiID) const
//{
//    return new AUTO_TILE::AutoTilePrototype(uiID);
//}

/*#####
# SpriteDatabase
#####*/
//void SpriteDatabase::setPrototype(SpritePrototype* pItem)
//{
//    Database::setPrototype(pItem);
//}
//
//const SpritePrototype* SpriteDatabase::getPrototype(uint32 ID) const
//{
//    return dynamic_cast<const SpritePrototype*>(Database::getPrototype(ID));
//}
//
//SpritePrototype* SpriteDatabase::getPrototype(uint32 ID)
//{
//    return dynamic_cast<SpritePrototype*>(Database::getPrototype(ID));
//}
//
//SpritePrototype* SpriteDatabase::getNewPrototype(uint32 uiID) const
//{
//    return new SpritePrototype(uiID);
//}

/*#####
# AnimationDatabase
#####*/
//void AnimationDatabase::setPrototype(AnimationPrototype* pItem)
//{
//    Database::setPrototype(pItem);
//}
//
//const AnimationPrototype* AnimationDatabase::getPrototype(uint32 ID) const
//{
//    return dynamic_cast<const AnimationPrototype*>(Database::getPrototype(ID));
//}
//
//AnimationPrototype* AnimationDatabase::getPrototype(uint32 ID)
//{
//    return dynamic_cast<AnimationPrototype*>(Database::getPrototype(ID));
//}
//
//AnimationPrototype* AnimationDatabase::getNewPrototype(uint32 uiID) const
//{
//    return new AnimationPrototype(uiID);
//}

/*#####
# AnimationTypeDatabase
#####*/
AnimationTypeDatabase::AnimationTypeDatabase() : Database()
{
    clear();
}

// add standard entrys
void AnimationTypeDatabase::clear()
{
    Database::clear();
    Database::setPrototype(new ANIMATION::AnimationTypePrototype(MAP_STRUCTURE::DIRECTION_UP+1, "STAND_UP"));
    Database::setPrototype(new ANIMATION::AnimationTypePrototype(MAP_STRUCTURE::DIRECTION_RIGHT+1, "STAND_RIGHT"));
    Database::setPrototype(new ANIMATION::AnimationTypePrototype(MAP_STRUCTURE::DIRECTION_DOWN+1, "STAND_DOWN"));
    Database::setPrototype(new ANIMATION::AnimationTypePrototype(MAP_STRUCTURE::DIRECTION_LEFT+1, "STAND_LEFT"));
    Database::setPrototype(new ANIMATION::AnimationTypePrototype(5, "WALK_UP"));
    Database::setPrototype(new ANIMATION::AnimationTypePrototype(6, "WALK_RIGHT"));
    Database::setPrototype(new ANIMATION::AnimationTypePrototype(7, "WALK_DOWN"));
    Database::setPrototype(new ANIMATION::AnimationTypePrototype(8, "WALK_LEFT"));
}

//void AnimationTypeDatabase::setPrototype(AnimationTypePrototype* pItem)
//{
//    if (pItem& & pItem->getID() > MAP_OBJECT::MIN_DYNAMIC_OBJECT_POSE)
//        Database::setPrototype(pItem);
//}

//const AnimationTypePrototype* AnimationTypeDatabase::getPrototype(uint32 ID) const
//{
//    return dynamic_cast<const AnimationTypePrototype*>(Database::getPrototype(ID));
//}
//
//AnimationTypePrototype* AnimationTypeDatabase::getPrototype(uint32 ID)
//{
//    return dynamic_cast<AnimationTypePrototype*>(Database::getPrototype(ID));
//}
//
//AnimationTypePrototype* AnimationTypeDatabase::getNewPrototype(uint32 uiID) const
//{
//    return new AnimationTypePrototype(uiID);
//}

/*#####
# WorldObjectDatabase
#####*/
//void WorldObjectDatabase::setPrototype(MAP_OBJECT::WorldObjectPrototype* pItem)
//{
//    Database::setPrototype(pItem);
//}
//
//const MAP_OBJECT::WorldObjectPrototype* WorldObjectDatabase::getPrototype(uint32 ID) const
//{
//    return dynamic_cast<const MAP_OBJECT::WorldObjectPrototype*>(Database::getPrototype(ID));
//}
//
//MAP_OBJECT::WorldObjectPrototype* WorldObjectDatabase::getPrototype(uint32 ID)
//{
//    return dynamic_cast<MAP_OBJECT::WorldObjectPrototype*>(Database::getPrototype(ID));
//}
//
//MAP_OBJECT::WorldObjectPrototype* WorldObjectDatabase::getNewPrototype(uint32 uiID) const
//{
//    return new MAP_OBJECT::WorldObjectPrototype(uiID);
//}

/*#####
# DynamicObjectDatabase
#####*/
//void DynamicObjectDatabase::setPrototype(MAP_OBJECT::DynamicObjectPrototype* pItem)
//{
//    Database::setPrototype(pItem);
//}
//
//const MAP_OBJECT::DynamicObjectPrototype* DynamicObjectDatabase::getPrototype(uint32 ID) const
//{
//    return dynamic_cast<const MAP_OBJECT::DynamicObjectPrototype*>(Database::getPrototype(ID));
//}
//
//MAP_OBJECT::DynamicObjectPrototype* DynamicObjectDatabase::getPrototype(uint32 ID)
//{
//    return dynamic_cast<MAP_OBJECT::DynamicObjectPrototype*>(Database::getPrototype(ID));
//}
//
//MAP_OBJECT::DynamicObjectPrototype* DynamicObjectDatabase::getNewPrototype(uint32 uiID) const
//{
//    return new MAP_OBJECT::DynamicObjectPrototype(uiID);
//}

/*#####
# LocalisationDatabase
#####*/
//void LocalisationDatabase::setPrototype(LocalisationPrototype* pItem)
//{
//    Database::setPrototype(pItem);
//}
//
//const LocalisationPrototype* LocalisationDatabase::getPrototype(uint32 ID) const
//{
//    return dynamic_cast<const LocalisationPrototype*>(Database::getPrototype(ID));
//}
//
//LocalisationPrototype* LocalisationDatabase::getPrototype(uint32 ID)
//{
//    return dynamic_cast<LocalisationPrototype*>(Database::getPrototype(ID));
//}
//
//LocalisationPrototype* LocalisationDatabase::getNewPrototype(uint32 uiID) const
//{
//    return new LocalisationPrototype(uiID);
//}
