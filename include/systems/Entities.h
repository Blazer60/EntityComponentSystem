/**
 * @file Entities.h
 * @author Ryan Purse
 * @date 30/01/2022
 */


#pragma once

#include "Core.h"

#include <functional>

namespace ecs
{
    /**
     * @brief An interface for the Entities class.
     */
    class IEntities
    {
    public:
        /**
         * @brief Calls process entities with the correct types.
         * @param uType - The component Id that pair with each type.
         */
        virtual void callbackProcessEntities(const UType &uType) = 0;
    
        /**
         * @brief Gets the interface of the entities class so that it can be handled separately.
         * @returns IEntities interface class.
         */
        [[nodiscard]] virtual UType getDefaultComponents() const = 0;
    
        /**
         * @brief Gets the hash code of all types provided in Entities<>. NOT the ids of components.
         * @returns hash code of all types.
         */
        [[nodiscard]] virtual std::vector<uint64_t> getUnderlyingTypeHashes() const = 0;
    };
    
    /**
     * @brief Holds a function pointer that the ecs system can use to process entities with.
     * Does NOT hold any entity information.
     * Use in conjunction with systems E.g.: mEntities.forEach(...) => hence the name, entities.
     * @tparam Args - The types of data that will processed.
     */
    template<class ...Args>
    class Entities
            : public IEntities
    {
        // Reduces repetition.
        using FuncSignature = std::function<void(Args &...)>;
    public:
        /**
         * @brief Defines what you want to do for each entity that is pass through Entities.
         * @param func - The function. This can be a lambda.
         */
        void forEach(const FuncSignature &func);
        
        /**
         * @brief Defines what you want to do for each entity that is pass through Entities.
         * @param func - The function. This can be a lambda.
         */
        void forEach(FuncSignature &&func);
    
        /**
         * @brief Calls the delegate set previously.
         * @tparam TArgs - The entities information types.
         * @param tuple - All of the entities information that you want to pass into it.
         */
        template<typename ...TArgs>
        void invoke(std::tuple<TArgs...> &tuple) const;
    
        /**
         * @brief Calls the delegate set previously.
         * @tparam TArgs - The entities information types.
         * @param tuple - All of the entities information that you want to pass into it.
         */
        template<typename ...TArgs>
        void invoke(std::tuple<TArgs...> &&tuple) const;
    
        /**
         * @brief Calls the delegate set previously.
         * @tparam TArgs - The entities information types.
         * @param args - All of the entities information that you want to pass into it.
         */
        template<typename ...TArgs>
        void invoke(TArgs &... args) const;
        
        /**
         * @brief Used to obtain the correct types (Args) within this Entities.
         * @param uType - The component Id that will be paired with each Args.
         */
        void callbackProcessEntities(const UType &uType) override;
    
        /**
         * @brief Gets the interface of the entities class so that it can be handled separately.
         * @returns IEntities interface class.
         */
        [[nodiscard]] UType getDefaultComponents() const override;
    
        /**
         * @brief Gets the hash code of all types provided in Entities<>. NOT the ids of components.
         * @returns hash code of all types.
         */
        [[nodiscard]] std::vector<uint64_t> getUnderlyingTypeHashes() const override;

    protected:
        FuncSignature mForEachDelegate { [](Args &... args) { } };
    };
    
    template<class... Args>
    void Entities<Args...>::callbackProcessEntities(const UType &uType)
    {
        ecs::processEntities(*this, uType);
    }
    
    template<class... Args>
    void Entities<Args...>::forEach(const Entities::FuncSignature &func)
    {
        mForEachDelegate = func;
    }
    
    template<class... Args>
    void Entities<Args...>::forEach(Entities::FuncSignature &&func)
    {
        mForEachDelegate = std::move(func);
    }
    
    template<class... Args>
    template<typename... TArgs>
    void Entities<Args...>::invoke(std::tuple<TArgs...> &tuple) const
    {
        mForEachDelegate((std::get<Args&>(tuple))...);
    }
    
    template<class... Args>
    template<typename... TArgs>
    void Entities<Args...>::invoke(std::tuple<TArgs...> &&tuple) const
    {
        mForEachDelegate((std::get<Args&>(tuple))...);
    }
    
    template<class... Args>
    template<typename... TArgs>
    void Entities<Args...>::invoke(TArgs &... args) const
    {
        mForEachDelegate(args...);
    }
    
    template<class... Args>
    UType Entities<Args...>::getDefaultComponents() const
    {
        return { getComponentIdOf<Args>()... };
    }
    
    template<class... Args>
    std::vector<uint64_t> Entities<Args...>::getUnderlyingTypeHashes() const
    {
        return { typeid(Args).hash_code()... };
    }
}