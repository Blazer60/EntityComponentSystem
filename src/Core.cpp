/**
 * @file Core.cpp
 * @author Ryan Purse
 * @date 20/01/2022
 */


#include "Core.h"

namespace ecs
{
    // Should these be in a struct or not (E.g.: impl->entityManager.create())?
    std::unique_ptr<EntityManager>    sEntityManager;
    std::unique_ptr<ArchetypeManager> sArchetypeManager;
    std::unique_ptr<SystemManager>    sSystemManager;
    
    void init()
    {
        sEntityManager    = std::make_unique<EntityManager>();
        sArchetypeManager = std::make_unique<ArchetypeManager>();
        sSystemManager    = std::make_unique<SystemManager>();
    }
    
    Entity create()
    {
        return sEntityManager->createEntity();
    }
    
    void update()
    {
        sSystemManager->update();
    }
    
    void start()
    {
        sSystemManager->start();
    }
    
    void makeFoundationComponent(Component id)
    {
        sEntityManager->makeFoundationComponent(id);
    }
    
    void verifySystem(const UType &uType, const std::vector<uint64_t> &underlyingTypes)
    {
        if (uType.size() != underlyingTypes.size())
        {
            // The length of uType does not match the length of system's underlying type.
            // The system will therefore have undefined behaviour.
            throw std::exception();
        }
        for (int i = 0; i < underlyingTypes.size(); ++i)
        {
            if (!sEntityManager->isValid(uType[i], underlyingTypes[i]))
                throw std::exception();  // The type has not been registered yet. The system will produce undefined results.
            // You could also have miss-aligned the types with the underlying types.
        }
    }
    
    void remove(Entity entity, Component component)
    {
        sArchetypeManager->remove(entity, component);
    }
}
