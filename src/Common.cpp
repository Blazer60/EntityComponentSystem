/**
 * @file Common.cpp
 * @author Ryan Purse
 * @date 20/01/2022
 */


#include "Common.h"

#include <iostream>
#include <iomanip>

namespace ecs
{
    std::string typeToString(const Entity id)
    {
        switch (entityMask::Type & id)
        {
            case entityTypeFlag::Entity:
                return "Entity";
            case entityTypeFlag::Component:
                return "Component";
            case entityTypeFlag::ParentOf:
                return "Parent Of";
            default:
                return "UNKNOWN";
        }
    }
    
    void print(Entity entity)
    {
        const uint32_t streamWidth = 12;
        
        std::cout << "-- Entity Information --\n";
        std::cout << std::setw(streamWidth) << "Type: "        << typeToString(entity) << "\n";
        std::cout << std::setw(streamWidth) << "Generation: "  << ((entityMask::Generation & entity) >> entityFlagShifts::Generation) << "\n";
        std::cout << std::setw(streamWidth) << "Id: "          << (entityMask::Id & entity) << "\n";
        std::cout << std::setw(streamWidth) << "Hex: "         << std::hex << std::setw(sizeof(Entity) * 2) << std::setfill('0') << entity << std::setfill(' ') << std::dec << "\n";
    }
    
    bool includes(const Type &set, const UType &subset)
    {
        return std::all_of(subset.begin(), subset.end(), [&set](const Component &subSetId) {
            return std::any_of(set.begin(), set.end(), [subSetId](const Component &setId) {
                return setId == subSetId;
            });
        });
    }
}