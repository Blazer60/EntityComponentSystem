/**
 * @file ArchetypeManager.h
 * @author Ryan Purse
 * @date 20/01/2022
 */


#pragma once

#include "Common.h"
#include "Archetype.h"

#include <iostream>
#include <unordered_map>
#include <map>
#include <set>

namespace ecs
{
    /**
     * @brief Information stored about an entity within the archetype manager.
     */
    struct EntityInformation
    {
        Type type;
        uint64_t componentIndex { 0 };
    
        bool operator==(const EntityInformation &rhs) const;
    
        bool operator!=(const EntityInformation &rhs) const;
    };
    
    /**
     * Handles the creation and deletion or all data within the ECS.
     * @author Ryan Purse
     * @date 20/01/2022
     */
    class ArchetypeManager
    {
    public:
        /**
         * @brief Add a component to an entity and takes in the value.
         * @tparam T - The type that component is.
         * @param entity - The entity that you want to add it to.
         * @param component - The id of the component.
         * @param value - The actual data you want to add to entity.
         */
        template<typename T>
        void add(Entity entity, Component component, const T &value);
        
        void remove(Entity entity, Component component);
    
        /**
         * @brief Adds an component to an entity that does not exist in the system.
         * @tparam T - The type that component is.
         * @param entity - The entity that you want to add it to.
         * @param component - The id of the component.
         * @param value - The actual data you want to add to entity.
         */
        template<typename T>
        void addNew(Entity entity, Component component, const T &value);
    
        /**
         * @brief Adds an component to an entity that already exists within the system.
         * @tparam T - The type that component is.
         * @param entity - The entity that you want to add it to.
         * @param component - The id of the component.
         * @param value - The actual data you want to add to entity.
         */
        template<typename T>
        void addOld(Entity entity, Component component, const T &value);
        
        /**
         * @brief Updates an Entity's info based on where its new position is.
         * @param newIndex - The index where the entity moved to.
         * @param entityInformation - The old information of the entity that needs to be looked-up.
         */
        void entityMovedIndex(uint64_t newIndex, const EntityInformation &entityInformation);
        
        /**
         * @brief Creates an Archetype with a Component Id and type T.
         * @tparam T - The type you want the Archetype to store.
         * @param id - The Id associated with the Type.
         */
        template<typename T>
        void createArchetype(Component id);
    
        /**
         * @brief Creates an Archetype with multiple component Ids and types.
         * @tparam Types - The types you want the Archetype to store.
         * @tparam Components - Components (Entities) that you want to associate with each type.
         * @param components - Components (Entities) that you want to associate with each type.
         */
        template<typename ...Types, typename ...Components>
        void createArchetype(const Components &... components);
        
        /**
         * @brief Finds an Archetype based on the type you provide.
         * @param type - The type of Archetype you want to find.
         * @returns The Archetype if it exists, nullptr otherwise.
         */
        Archetype *findArchetype(const Type &type);
    
        /**
         * @brief Performs a shallow copy of the Archetype with BaseType and then adds id to the type.
         * @tparam T - The type you want the Archetype to store.
         * @param id - A Component (Entity) that you want to associate with type T.
         * @param baseType - The Archetype type that you want to base the new Archetype off of.
         */
        template<typename T>
        void cloneArchetype(Component id, const Type &baseType);
        
        /**
         * @brief Performs a shallow copy of the Archetype baseArchetype and then adds id to the type.
         * @tparam T - The type you want the Archetype to store.
         * @param id - A component (Entity) that you want to associate with type T.
         * @param baseType - The Archetype type that you want to base the new Archetype off of.
         * @param baseArchetype - The base Archetype that you want to clone (no look-up version).
         */
        template<typename T>
        void cloneArchetype(Component id, const Type &baseType, const Archetype &baseArchetype);
        
        /**
         * @brief Clones an existing archetype but only uses parts of it.
         * @param subType - The new type that you want the new archetype to have.
         * @param baseType - The original type that you want to clone from.
         */
        void subCloneArchetype(const Type &subType, const Type &baseType);
    
        /**
         * @brief Gets all of the archetypes that match the given type.
         * @param uType - The type you want to retrieve.
         * @returns All Archetypes with at least the given type.
         */
        [[nodiscard]] std::vector<Archetype*> getArchetypesWithSubset(const UType &uType);
        
    protected:
        // It doesn't like unordered map, Type cannot be converted into a hash function.
        std::map<Type, Archetype> mArchetypes;
        
        /**
         * Tells us where an Entity's information is stored and at what location.
         */
        std::unordered_map<Entity, EntityInformation> mEntityInformation;
    };
    
    template<typename T>
    void ArchetypeManager::add(Entity entity, Component component, const T &value)
    {
        mEntityInformation.count(entity)
        ? addOld(entity, component, value)
        : addNew(entity, component, value);
    }
    
    template<typename T>
    void ArchetypeManager::addNew(Entity entity, Component component, const T &value)
    {
        createArchetype<T>(component);
        Archetype * const archetype = findArchetype( { component } );
        const uint64_t index = archetype->pushBack(component, value);
        
        EntityInformation information { { component }, index };
        
        mEntityInformation.insert( { entity, information } );
    }
    
    template<typename T>
    void ArchetypeManager::addOld(Entity entity, Component component, const T &value)
    {
        EntityInformation &info = mEntityInformation.at(entity);
        Type newType = info.type;
        newType.emplace(component);
        
        Archetype &oldArchetype = *findArchetype(info.type);  // If nullptr, I've fucked up.
        
        cloneArchetype<T>(component, info.type, oldArchetype);
        
        Archetype &newArchetype = *findArchetype(newType);  // Again, should never be nullptr.
        
        const uint64_t movedIndex = oldArchetype.transferTo(newArchetype, info.componentIndex);
        
        // Update the moved item's index so that it points to the correct place.
        entityMovedIndex(info.componentIndex, { info.type, movedIndex });
        
        // Add in the new item.
        info.componentIndex = newArchetype.pushBack(component, value);
        info.type = newType;
    }
    
    template<typename T>
    void ArchetypeManager::createArchetype(Component id)
    {
        if (findArchetype( { id } ))
            return;  // Archetype already exist, no need to make a new one.
    
        Archetype archetype;
        archetype.createComponentArray<T>(id);
        mArchetypes.emplace(Type { id }, std::move(archetype));
    }
    
    template<typename ...Types, typename ...Components>
    void ArchetypeManager::createArchetype(const Components &... components)
    {
        if (findArchetype( { components... } ))
            return;  // Archetype already exist, no need to make a new one.
    
        Archetype archetype;
        archetype.createComponentArray<Types...>(components...);
        mArchetypes.emplace( Type { components... }, std::move(archetype));
    }
    
    template<typename T>
    void ArchetypeManager::cloneArchetype(Component id, const Type &baseType)
    {
        if (Archetype *baseArchetypePtr = findArchetype(baseType))
        {
            cloneArchetype<T>(id, baseType, *baseArchetypePtr);
            return;
        }
        
        // No base type has been created yet.
        throw std::exception();
    }
    
    template<typename T>
    void ArchetypeManager::cloneArchetype(Component id, const Type &baseType, const Archetype &baseArchetype)
    {
        Type newType(baseType);
        newType.insert(id);
        if (findArchetype(newType))
            return;  // Archetype already exists.
        
        Archetype derived(baseArchetype);
        derived.createComponentArray<T>(id);
        
        mArchetypes.emplace(newType, std::move(derived));
    }
}



