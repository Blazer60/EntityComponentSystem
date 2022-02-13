/**
 * @file SystemManager.h
 * @author Ryan Purse
 * @date 26/01/2022
 */


#pragma once

#include "BaseSystem.h"

#include <vector>
#include <memory>

namespace ecs
{
    /**
     * Groups all of the system together so that they can be updated all at once.
     * @author Ryan Purse
     * @date 26/01/2022
     */
    class SystemManager
    {
    public:
        /**
         * @brief Adds a system to the system manager.
         * @param uType - The components Ids that this system will work on.
         * @param iBaseSystem - The system itself.
         */
        void addSystem(const UType &uType, std::unique_ptr<IBaseSystem> iBaseSystem);
        
        /**
         * @brief Calls start on all systems assigned to this system manager.
         */
        void start();
        
        /**
         * @brief Updates all of the system assigned to this system manager.
         */
        void update();
        
        /**
         * @brief Updates all of the entities within the system.
         */
        void updateEntities();
        
    protected:
        std::vector<std::unique_ptr<IBaseSystem>>   mSystems;
        std::vector<UType>                          mSystemUTypes;
    };
}




