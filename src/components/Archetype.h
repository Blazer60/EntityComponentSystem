/**
 * @file Archetype.h
 * @author Ryan Purse
 * @date 21/01/2022
 */


#pragma once

#include "Common.h"
#include "ComponentArray.h"
#include "BaseSystem.h"

#include <iostream>
#include <vector>
#include <utility>
#include <memory>
#include <unordered_map>
#include <algorithm>

namespace ecs
{
    /**
     * @brief A collection of components with the same type. E.getArraysOfType_s.: Everything with only a position and velocity
     * will be stored together.
     * @author Ryan Purse
     * @date 20/01/2022
     */
    class Archetype
    {
    public:
        explicit Archetype() = default;
        
        /**
         * @brief Performs a shallow copy on archetype.
         * @param archetype - The archetype you'd like to shallow copy.
         */
        Archetype(const Archetype &archetype);
        
        /**
         * @brief Performs a shallow copy but only on the types specified.
         * NOTE: archetype must contain all component in type.
         * @param archetype - The archetype you'd like to shallow copy.
         * @param type - The types the new archetype will have.
         */
        Archetype(const Archetype &archetype, const Type &type);
        
        Archetype(Archetype &&archetype) noexcept = default;
        
        ~Archetype(); // = default (Tells us when it's destroyed).
        
        /**
         * @brief Creates a component array within the archetype of type T.
         * @tparam T - The type you want to store.
         */
        template<typename T>
        void createComponentArray(Component id);
        
        /**
         * @brief Creates multiples component arrays with Type and Types.
         * @tparam Type - The first type of component array
         * @tparam Types - The rest of the types of component arrays.
         * @tparam Components - Must be type Component or Entity
         * @param id - The first component Ids.
         * @param args - The rest of the component Ids.
         */
        template<typename Type, typename ...Types, typename ...Components>
        void createComponentArray(Component id, const Components &... args);
        
        /**
         * @brief Adds a component value to the end of the desired component array.
         * @tparam T - The type that value is.
         * @param id - The Id given to the component.
         * @param value - The actual data that you want to store.
         * @returns The index of where it's stored.
         */
        template<typename T>
        uint64_t pushBack(Component id, const T &value);
    
        /**
         * @brief Adds component values to all of the desired component arrays.
         * @tparam T - The type that value is.
         * @tparam Args - The type that all of values are and component Ids.
         * @param id - The Id given to the component.
         * @param value - The actual data that you wan to store.
         * @param values - The rest of the data as well as all of the ids.
         * @returns The index of where it's stored.
         */
        template<typename T, typename ...Args>
        uint64_t pushBack(Component id, const T &value, const Args &... values);
    
        /**
         * @brief Gets an element within a single component array.
         * @tparam T - The type component array.
         * @param component - The component array id and T id.
         * @param index - The retrieved values index.
         */
        template<typename T>
        T &getComponent(Component component, uint64_t index) const;
    
        /**
         * @brief Moves data at dataIndex into newArchetype. The newArchetype MUST be equal or larger to this archetype.
         * @param newArchetype - The archetype that you want to move the data to.
         * @param dataIndex - The index of where the data is.
         * @returns The index of the element that was moved to the index passed in.
         * @see stableRemove()
         */
        [[nodiscard]] uint64_t transferTo(Archetype &newArchetype, uint64_t dataIndex);
        
        /**
         * @brief Moves data at dataIndex into this Archetype. oldArchetype MUST be equal or larger to this archetype.
         * @param oldArchetype - The archetype that you want to move data from.
         * @param dataIndex - The index of where the data is.
         * @returns The index of the element that was moved to the index passed in and the new count of the archetype.
         * @see stableRemove()
         */
        [[nodiscard]] std::pair<uint64_t, uint64_t> transferFrom(Archetype &oldArchetype, uint64_t dataIndex);
    
        /**
         * @brief Gets all the relevant component arrays with their type. WARNING: Not type safe.
         * @tparam T - The first type you want to get.
         * @tparam EArgs - The rest of the types you want to get (optional)
         * @param typeIt - The component Id associated with each type.
         * @returns All ComponentArray<T>s.
         */
        template<typename T, typename ...EArgs>
        std::tuple<ComponentArray<T>*, ComponentArray<EArgs>*...> getArraysOfType_s(UType::const_iterator &typeIt);
    
        /**
         * @deprecated Use getArraysOfType_s instead. This one is ill-defined.
         * @brief Gets all the relevant component arrays with their type. WARNING: Not type safe.
         * @tparam EArgs - The types you want to get.
         * @param typeIt - The component Id associated with each type.
         * @returns All ComponentArray<T>s.
         */
        template<typename ...EArgs>
        [[deprecated("Post Increment is ill defined. Use getArraysOfType_s instead.")]]
        std::tuple<ComponentArray<EArgs>*...> getArraysOfType(UType::const_iterator &typeIt);
        
        /**
         * @brief Performs an iteration over all of the components data.
         * @tparam EArgs - The types you want to iterate over.
         * @param entities - The function you want to pass over them.
         */
        template<typename ...EArgs>
        void process(Entities<EArgs...> &entities);  // Todo: This should not be a member function. Something higher up should use getArraysOfType_s.
        
        /**
         * @brief Performs an iteration over all of the components data.
         * @tparam EArgs - The types you want to iterate over.
         * @tparam IArgs - The associating component types for each EArgs.
         * @param entities - The function you want to pass over them.
         * @param ids - The association component Ids for each EArgs.
         */
        template<typename ...EArgs, typename ...IArgs>
        void process(Entities<EArgs...> &entities, const IArgs &...ids);  // Todo: This should not be a member function. Something higher up should use getArraysOfType_s.
    
        /**
         * @brief Moves the last item in the specified component array to index.
         * @param component - The type that you want to move.
         * @param index - The index you want to move the last item to.
         */
        void moveLastComponent(Component component, uint64_t index);

    protected:
        /**
         * @brief Get the component vector T by using an id. WARNING: There is no bounds checking.
         * @tparam T - The type of component array that you want to get.
         * @param id - The index of the component array within components
         * @returns A vector of type T.
         */
        template<typename T>
        [[nodiscard]] std::vector<T> *get(Component id) const;
        
        std::unordered_map<Component, uint64_t> mIdToComponentIndex;
        std::vector<std::unique_ptr<IComponentArray>> mComponents;
    };
    
    template<typename T>
    void Archetype::createComponentArray(Component id)
    {
        mComponents.emplace_back(std::make_unique<ComponentArray<T>>());
        mIdToComponentIndex[id] = mComponents.size() - 1;
    }
    
    template<typename Type, typename ...Types, typename ...Components>
    void Archetype::createComponentArray(Component id, const Components &... args)
    {
        createComponentArray<Type>(id);
        createComponentArray<Types...>(args...);
    }
    
    template<typename T>
    uint64_t Archetype::pushBack(Component id, const T &value)
    {
        std::vector<T> * const container = get<T>(id);
        container->push_back(value);
        return container->size() - 1;  // It is always the last element in the vector.
    }
    
    template<typename T, typename ...Args>
    uint64_t Archetype::pushBack(Component id, const T &value, const Args &... values)
    {
        const auto index = pushBack(id, value);
        pushBack(values...);
        return index;
    }
    
    template<typename T>
    [[nodiscard]] std::vector<T> *Archetype::get(Component id) const
    {
        const uint64_t index = mIdToComponentIndex.at(id);
        auto * const componentArray = reinterpret_cast<ComponentArray<T>*>(mComponents[index].get());
        return &componentArray->data;
    }
    
    template<typename T, typename ...EArgs>
    std::tuple<ComponentArray<T>*, ComponentArray<EArgs>*...> Archetype::getArraysOfType_s(UType::const_iterator &typeIt)
    {
        std::tuple<ComponentArray<T>*> baseTuple {
            reinterpret_cast<ComponentArray<T>*>(mComponents[mIdToComponentIndex.at(*typeIt)].get())
        };
        
        if constexpr (sizeof...(EArgs) != 0)  // constexpr stops incorrect call to this function.
            return std::tuple_cat(baseTuple, getArraysOfType_s<EArgs...>(++typeIt));
        else
            return baseTuple;
    }
    
    template<typename ...EArgs>
    std::tuple<ComponentArray<EArgs>*...> Archetype::getArraysOfType(UType::const_iterator &typeIt)
    {
        // todo: Operator++(int) is ill-defined.
        return { reinterpret_cast<ComponentArray<EArgs>*>(mComponents[mIdToComponentIndex.at(*typeIt++)].get())... };
    }
    
    template<typename ...EArgs>
    void Archetype::process(Entities<EArgs...> &entities)
    {
        // todo: Should this really be a member function.
        std::tuple<ComponentArray<EArgs>*...> arrays = getArraysOfType_s<EArgs...>(entities.mType.begin());
        
        for (int i = 0; i < std::get<0>(arrays)->data.size(); ++i)
            entities.invoke(std::forward_as_tuple(std::get<ComponentArray<EArgs>*>(arrays)->data[i]...));
    }
    
    template<typename ...EArgs, typename ...IArgs>
    void Archetype::process(Entities<EArgs...> &entities, const IArgs &...ids)
    {
        // todo: This should be a non-member function in Core.h
        // todo: This needs a safety check since it will explode in your face if you pass the wrong items.
        std::tuple<ComponentArray<EArgs>*...> t(reinterpret_cast<ComponentArray<EArgs>*>(mComponents[mIdToComponentIndex.at(ids)].get())...);
        for (int i = 0; i < std::get<0>(t)->data.size(); ++i)
            entities.invoke(std::forward_as_tuple(std::get<ComponentArray<EArgs>*>(t)->data[i]...));
    }
    
    template<typename T>
    T &Archetype::getComponent(Component component, uint64_t index) const
    {
        return (*get<T>(component))[index];
    }
}
