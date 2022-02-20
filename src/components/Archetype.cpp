/**
 * @file Archetype.cpp
 * @author Ryan Purse
 * @date 21/01/2022
 */


#include "Archetype.h"
#include "ComponentArray.h"

namespace ecs
{
    Archetype::Archetype(const Archetype &archetype)
        : mIdToComponentIndex(archetype.mIdToComponentIndex)
    {
        for (const std::unique_ptr<IComponentArray> &item : archetype.mComponents)
            mComponents.emplace_back(item->makeArray());
    }
    
    Archetype::Archetype(const Archetype &archetype, const Type &type)
    {
        uint64_t index { 0 };
        for (const Component &component : type)
        {
            const uint64_t archetypeIndex = archetype.mIdToComponentIndex.at(component);
            auto *componentArray = archetype.mComponents[archetypeIndex].get();
            mComponents.emplace_back(componentArray->makeArray());
            mIdToComponentIndex.emplace(component, index++);
        }
    }
    
    uint64_t Archetype::transferTo(Archetype &newArchetype, uint64_t dataIndex)
    {
        uint64_t movedIndex = 0;
        for (const auto &[id, index] : mIdToComponentIndex)
        {
            // Get both component arrays that are the same type.
            auto *oldIComponentArray = mComponents[index].get();
            auto *newIComponentArray = newArchetype.mComponents[newArchetype.mIdToComponentIndex.at(id)].get();
        
            movedIndex = oldIComponentArray->transferItemTo(newIComponentArray, dataIndex);
            // Note: This can be used as a check to see if there's parity between all arrays.
        }
        return movedIndex;
    }
    
    std::pair<uint64_t, uint64_t> Archetype::transferFrom(Archetype &oldArchetype, uint64_t dataIndex)
    {
        uint64_t movedIndex = 0;
        uint64_t count = 0;
        for (const auto &[id, index] : mIdToComponentIndex)
        {
            // Get both component arrays that are the same type.
            auto *newIComponentArray = mComponents[index].get();
            auto *oldIComponentArray = oldArchetype.mComponents[oldArchetype.mIdToComponentIndex.at(id)].get();
    
            movedIndex = oldIComponentArray->transferItemTo(newIComponentArray, dataIndex);
            count = oldIComponentArray->count();
            // Note: This can be used as a check to see if there's parity between all arrays.
        }
        return { movedIndex, count };
    }
    
    Archetype::~Archetype() = default;
    
    void Archetype::moveLastComponent(Component component, uint64_t index)
    {
        mComponents[mIdToComponentIndex.at(component)]->moveLastItem(index);
    }
}


