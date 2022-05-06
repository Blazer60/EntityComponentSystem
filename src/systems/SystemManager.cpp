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
        switch (iBaseSystem->getExecutionOrder())
        {
            case PreUpdate:
                mPreUpdateSystems.push_back({ std::move(iBaseSystem), uType });
                break;
            case Update:
                mUpdateSystems.push_back({ std::move(iBaseSystem), uType });
                break;
            case PreRender:
                mPreRenderSystems.push_back({ std::move(iBaseSystem), uType });
                break;
            case Render:
                mRenderSystems.push_back({ std::move(iBaseSystem), uType });
                break;
            case ImGui:
                mImGuiSystems.push_back({ std::move(iBaseSystem), uType });
                break;
            default:
                break;
        }
    }
    
    void SystemManager::update()
    {
        for (const auto &[system, uType] : mPreUpdateSystems)
        {
            system->onUpdate();
            const auto iEntities = system->getEntities();
            iEntities->callbackProcessEntities(uType);
        }
    
        for (const auto &[system, uType] : mUpdateSystems)
        {
            system->onUpdate();
            const auto iEntities = system->getEntities();
            iEntities->callbackProcessEntities(uType);
        }
    }
    
    void SystemManager::render()
    {
        for (const auto &[system, uType] : mPreRenderSystems)
        {
            system->onUpdate();
            const auto iEntities = system->getEntities();
            iEntities->callbackProcessEntities(uType);
        }
    
        for (const auto &[system, uType] : mRenderSystems)
        {
            system->onUpdate();
            const auto iEntities = system->getEntities();
            iEntities->callbackProcessEntities(uType);
        }
    }
    
    void SystemManager::imGui()
    {
        for (const auto &[system, uType] : mImGuiSystems)
        {
            system->onUpdate();
            const auto iEntities = system->getEntities();
            iEntities->callbackProcessEntities(uType);
        }
    }
}


