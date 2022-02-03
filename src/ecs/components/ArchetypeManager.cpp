/**
 * @file ArchetypeManager.cpp
 * @author Ryan Purse
 * @date 23/01/2022
 */


#include "ecs/components/ArchetypeManager.h"

namespace ecs
{
    Archetype* ArchetypeManager::findArchetype(const Type &type)
    {
        // todo: Find a way to inline this since you still need to check if this is nullptr.
        if (mArchetypes.count(type))  // Count is always 0 or 1.
            return &mArchetypes.at(type);
        return nullptr;
    }
    
    void ArchetypeManager::entityMovedIndex(uint64_t newIndex, const EntityInformation &entityInformation)
    {
        for (auto &[_, value] : mEntityInformation)
        {
            if (value != entityInformation)
                continue;
            value.componentIndex = newIndex;
            return;
        }
    }
    
    std::vector<Archetype *> ArchetypeManager::getArchetypesWithSubset(const UType &uType)
    {
        std::vector<Archetype*> out;
        for (auto &[key, value] : mArchetypes)
        {
            if (ecs::includes(key, uType))
                out.push_back(&value);
        }
        return out;
    }
    
    void ArchetypeManager::remove(Entity entity, Component component)
    {
        EntityInformation &info = mEntityInformation.at(entity);
        Type newType = info.type;
        newType.erase(component);
        
        Archetype &oldArchetype = *findArchetype(info.type);
    
        subCloneArchetype(newType, info.type);
    
        Archetype &newArchetype = *findArchetype(newType);
        
        const auto [moveIndex, count]  = newArchetype.transferFrom(oldArchetype, info.componentIndex);
        
        // Move the trailing item that won't get picked up by transfer from.
        oldArchetype.moveLastComponent(component, info.componentIndex);
    
        entityMovedIndex(info.componentIndex, { info.type, moveIndex } );
        
        // Count - 1 is always where the component index will end up.
        info.componentIndex = count - 1;
        info.type = newType;
    }
    
    void ArchetypeManager::subCloneArchetype(const Type &subType, const Type &baseType)
    {
        if (findArchetype(subType))
            return;  // The subType archetype already exists, we can use that.
        
        Archetype *base = findArchetype(baseType);
        if (!base)
            throw std::exception();  // No base type has been created yet.
            
        mArchetypes.emplace(subType, Archetype(*base, subType));
    }
    
    bool EntityInformation::operator==(const EntityInformation &rhs) const
    {
        return type == rhs.type &&
               componentIndex == rhs.componentIndex;
    }
    
    bool EntityInformation::operator!=(const EntityInformation &rhs) const
    {
        return !(rhs == *this);
    }
}
