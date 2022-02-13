/**
 * @file SystemManager.cpp
 * @author Ryan Purse
 * @date 26/01/2022
 */


#include "SystemManager.h"
#include "Entities.h"

namespace ecs
{
    void SystemManager::addSystem(const UType &uType, std::unique_ptr<IBaseSystem> iBaseSystem)
    {
        mSystems.push_back(std::move(iBaseSystem));
        mSystemUTypes.emplace_back(uType);
    }
    
    void SystemManager::start()
    {
        for (int i = 0; i < mSystems.size(); ++i)
        {
            auto &sys   = mSystems[i];
            auto &sysUType = mSystemUTypes[i];
            
            sys->onStart();
            
            auto *e = sys->getEntities();
            e->callbackProcessEntities(sysUType);
        }
    }
    
    void SystemManager::update()
    {
        for (int i = 0; i < mSystems.size(); ++i)
        {
            auto &sys   = mSystems[i];
            auto &sysUType = mSystemUTypes[i];
        
            sys->onUpdate();
            
            auto *e = sys->getEntities();
            e->callbackProcessEntities(sysUType);
        }
    }
}


