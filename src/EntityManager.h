/**
 * @file EntityManager.h
 * @author Ryan Purse
 * @date 20/01/2022
 */


#pragma once

#include "Common.h"

#include <unordered_map>
#include <typeinfo>

namespace ecs
{
    /**
     * Handles the creation of all entities and knows what components are attached to them. It doesn't contain the actual data.
     * @author Ryan Purse
     * @date 20/01/2022
     */
    class EntityManager
    {
    public:
        /**
         * @brief Creates an Entity Id with the Type Entity.
         * @return Entity - A unique Id for an Entity.
         */
        [[nodiscard]] Entity createEntity();
    
        /**
         * @brief Creates an Entity Id with the Type Component.
         * @return Component - A unique Id for an Entity.
         */
        template<typename T>
        Component createComponent();
    
        /**
         * @brief Makes the given Id the default id when handling components with the same type.
         * @param id - The id that you want to make the default.
         */
        void makeFoundationComponent(Component id);
        
        /**
         * @brief Destroys an Entity
         * @param id - The Id of the entity that you want to destroy.
         */
        void destroy(Entity id);
    
        /**
         * @brief Checks if the given Entity is exists in the world.
         * @param id - The Id that you want to check.
         * @returns True if it is a valid Id. False otherwise.
         */
        [[nodiscard]] bool isValid(Entity id);
        
        /**
         * @brief Checks if the given Entity can be paired with the underlying type.
         * @param id - The Id that you want to check.
         * @param underlyingType - The hashed type that you want to compare it to.
         * @returns True if it is a valid Id. False otherwise.
         */
        [[nodiscard]] bool isValid(Entity id, uint64_t underlyingType);
    
        /**
         * @brief Gets the component Id of T.
         * @tparam T - The type you want the Id of.
         * @returns The component Id.
         * @see makeFoundationType();
         */
        template<typename T>
        [[nodiscard]] Component getComponentIdOf();
        
        /**
         * @brief Gets the component Id of a hashed type.
         * @param hash - The hash you want the Id of.
         * @returns The component Id.
         * @see makeFoundationType();
         */
        [[nodiscard]] Component getComponentIdOf(uint64_t hash);

    protected:
        std::unordered_map<Entity, uint64_t>    mEntityToHash;  // Everything at what they are.
        std::unordered_map<uint64_t, Component> mHashToComponentId;  // The foundation types only.
    
        Entity mNextEntityId     { 1 };
        Entity mNextComponentId  { 1 };
        Entity mEntityGeneration { 1ull << entityFlagShifts::Generation };
    
        const Entity mComponentIdShift { 24 };
    };
    
    // Implementation
    
    template<typename T>
    Component EntityManager::createComponent()
    {
        Component id = mNextComponentId++ << mComponentIdShift | static_cast<Component>(entityTypeFlag::Component);
        mEntityToHash.insert( { id, typeid(T).hash_code() } );
        return id;
    }
    
    template<typename T>
    Component EntityManager::getComponentIdOf()
    {
        const auto key = typeid(T).hash_code();
        
        // T has no default value. Assign it before using it.
        if (mHashToComponentId.count(key) < 1)
            throw std::exception();
        
        return mHashToComponentId.at(key);
    }
}
