/**
 * @file EntityManager.cpp
 * @author Ryan Purse
 * @date 20/01/2022
 */


#include "EntityManager.h"

namespace ecs
{
    void EntityManager::destroy(Entity id)
    {
        mEntityToHash.erase(id);
    }
    
    bool EntityManager::isValid(Entity id)
    {
        return mEntityToHash.count(id);
    }
    
    Entity EntityManager::createEntity()
    {
        const Entity id = mNextEntityId++ | mEntityGeneration | static_cast<Entity>(entityTypeFlag::Entity);
        mEntityToHash.insert( { id, typeid(Entity).hash_code() } );
        return id;
    }
    
    void EntityManager::makeFoundationComponent(Component id)
    {
        mHashToComponentId.insert( { mEntityToHash.at(id), id } );
    }
    
    bool EntityManager::isValid(Entity id, uint64_t underlyingType)
    {
        if (!isValid(id))
            return false;
        return underlyingType == mEntityToHash.at(id);
    }
    
    Component EntityManager::getComponentIdOf(uint64_t hash)
    {
        return mHashToComponentId.at(hash);
    }
}
