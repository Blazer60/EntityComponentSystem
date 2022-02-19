/**
 * @file ComponentArray.h
 * @author Ryan Purse
 * @date 26/01/2022
 */


#pragma once

#include <iostream>
#include <memory>
#include <vector>

namespace ecs
{
    /**
     * @brief An interface for ComponentArray<T>.
     * @author Ryan Purse
     * @date 20/01/2022
     */
    struct IComponentArray
    {
        virtual ~IComponentArray() = default;
        
        /**
         * @brief Creates a component array with the underlying type.
         * @returns An interface to the component array.
         */
        [[nodiscard]] virtual std::unique_ptr<IComponentArray> makeArray() = 0;
        
        /**
         * @brief Moves items from this component array to the new component array. Both array MUST be the same type.
         * You can use Component Ids to match component arrays.
         * @param newComponentArray - The array that you want to move the item to.
         * @param itemIndex - The index of the item you want to move.
         * @returns The index of the element that was moved to the index passed in.
         * @see stableRemove
         */
        [[nodiscard]] virtual uint64_t transferItemTo(IComponentArray *newComponentArray, uint64_t itemIndex) = 0;
        
        virtual void moveLastItem(uint64_t itemIndex) = 0;
        
        [[nodiscard]] virtual uint64_t count() = 0;
    };
    
    /**
     * @brief A Component Array holds all data of a given type T.
     * @tparam T - The type that you want the vector to hold.
     * @author Ryan Purse
     * @date 20/01/2022
     */
    template<typename T>
    struct ComponentArray
            : IComponentArray
    {
        ~ComponentArray() override = default;
    
        /**
         * @brief Creates a component array with the underlying type.
         * @returns An interface to the component array.
         */
        [[nodiscard]] std::unique_ptr<IComponentArray> makeArray() override;
    
        /**
         * @brief Moves items from this component array to the new component array. Both array MUST be the same type.
         * You can use Component Ids to match component arrays.
         * @param newComponentArray - The array that you want to move the item to.
         * @param itemIndex - The index of the item you want to move.
         * @returns The index of the element that was moved to the index passed in.
         * @see stableRemove
         */
        [[nodiscard]] uint64_t transferItemTo(IComponentArray *newComponentArray, uint64_t itemIndex) override;
    
        /**
         * @brief Moves the last item to the specified index.
         * @param itemIndex - The index that you want to move it to.
         */
        void moveLastItem(uint64_t itemIndex) override;
    
        /**
         * @returns The number elements in data.
         */
        uint64_t count() override;
    
        std::vector<T> data;
    };
    
    
    template<typename T>
    std::unique_ptr<IComponentArray> ComponentArray<T>::makeArray()
    {
        return std::make_unique<ComponentArray<T>>();
    }
    
    template<typename T>
    uint64_t ComponentArray<T>::transferItemTo(IComponentArray *newComponentArray, uint64_t itemIndex)
    {
        // This may not throw an error when reinterpreting. Make sure that both component arrays are the same type.
        std::vector<T> &newData = reinterpret_cast<ComponentArray<T>*>(newComponentArray)->data;
        newData.emplace_back(std::move(data[itemIndex]));
    
        // Minimises the impact on the number of indices change to reduce overhead.
        std::iter_swap(data.begin() + itemIndex, data.end() - 1);
        data.erase(data.end() - 1);
        return data.size();
    }
    
    template<typename T>
    void ComponentArray<T>::moveLastItem(uint64_t itemIndex)
    {
        data[itemIndex] = data[data.size() - 1];
        data.erase(data.end() - 1);
    }
    
    template<typename T>
    uint64_t ComponentArray<T>::count()
    {
        return data.size();
    }
}
