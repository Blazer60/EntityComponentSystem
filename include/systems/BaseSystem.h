/**
 * @file BaseSystem.h
 * @author Ryan Purse
 * @date 20/01/2022
 */


#pragma once

#include "Common.h"

#include <typeinfo>
#include <functional>

namespace ecs
{
    class IEntities;
    
    template<class ...Args>
    class Entities;
    
    /**
     * @brief A concrete interface so that all Systems can be put into a single vector.
     */
    class IBaseSystem
    {
    public:
        virtual ~IBaseSystem() = default;
    
        /**
         * @brief Called when ecs::start() is called.
         * Allows you to update member variables and change the foreach function dynamically.
         */
        virtual void onStart() { };
        
        /**
         * @brief Called when ecs::update() is called.
         * Allows you to update member variables and change the foreach function dynamically.
         */
        virtual void onUpdate() { };
        
        /**
         * @brief Gets the hash code of all types provided in BaseSystem<>. NOT the ids of components.
         * @returns hash code of all types.
         */
        [[nodiscard]] virtual std::vector<uint64_t> getUnderlyingTypes() const = 0;
        
        /**
         * @brief Gets the interface of the entities class so that it can be handled separately.
         * @returns IEntities interface class.
         */
        [[nodiscard]] virtual IEntities *getEntities() = 0;
    };
    
    /**
     * A system that can operate on specific entities within the ecs system.
     * Provides a wrapper around the entities class.
     * @author Ryan Purse
     * @date 26/01/2022
     */
    template<class ...Args>
    class BaseSystem
            : public IBaseSystem
    {
    public:
        ~BaseSystem() override = default;
    
        /**
         * @brief Gets the hash code of all types in ...Args. NOT the ids of components.
         * @returns hash code of all types (...Args).
         */
        [[nodiscard]] std::vector<uint64_t> getUnderlyingTypes() const override;
        
        /**
         * @brief Gets the interface of the entities class so that it can be handled separately.
         * @returns IEntities interface class.
         */
        [[nodiscard]] IEntities *getEntities() override;

    protected:
        Entities<Args...> mEntities;
    };
    
    template<class... Args>
    std::vector<uint64_t> BaseSystem<Args...>::getUnderlyingTypes() const
    {
        return { (typeid(Args).hash_code())... };
    }
    
    template<class... Args>
    IEntities *BaseSystem<Args...>::getEntities()
    {
        return &mEntities;
    }
}




