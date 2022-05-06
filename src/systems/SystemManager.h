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
        struct SystemUTypePair
        {
            std::unique_ptr<IBaseSystem>    system;
            UType                           uType;
        };
        
    public:
        /**
         * @brief Adds a system to the system manager.
         * @param uType - The components Ids that this system will work on.
         * @param iBaseSystem - The system itself.
         */
        void addSystem(const UType &uType, std::unique_ptr<IBaseSystem> iBaseSystem);
        
        /**
         * @brief Updates all of the system assigned to this system manager.
         */
        void update();
        
        /**
         * @brief Renders all of the systems assigned to this system manager.
         */
        void render();
        
        /**
         * @brief Renders all ImGui related systems assigned to this system manager.
         */
        void imGui();

    protected:
        std::vector<SystemUTypePair> mPreUpdateSystems;
        std::vector<SystemUTypePair> mUpdateSystems;
        std::vector<SystemUTypePair> mPreRenderSystems;
        std::vector<SystemUTypePair> mRenderSystems;
        std::vector<SystemUTypePair> mImGuiSystems;
    };
}




