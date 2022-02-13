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
        for (auto &sys : mSystems)
            sys->onStart();
    }
    
    void SystemManager::update()
    {
        for (const auto &sys : mSystems)
            sys->onUpdate();
    }
    
    void SystemManager::updateEntities()
    {
        for (int i = 0; i < mSystems.size(); ++i)
        {
            auto *e = mSystems[0]->getEntities();
            e->callbackProcessEntities(mSystemUTypes[i]);
        }
    }
}


