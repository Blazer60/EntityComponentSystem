/**
 * @file Core.cpp
 * @author Ryan Purse
 * @date 13/04/2022
 */


#include "Core.h"

namespace ecs
{
    Core::Core(int flags) :
        mInitSettings(flags),
        mEntityManager(flags & initFlag::AutoInitialise)
    {
    }
    
    Entity Core::create()
    {
        return mEntityManager.createEntity();
    }
    
    void Core::update()
    {
        mSystemManager.update();
    }
    
    void Core::render()
    {
        mSystemManager.render();
    }
    
    void Core::imGui()
    {
        mSystemManager.imGui();
    }
    
    void Core::makeFoundationComponent(Component id)
    {
        mEntityManager.makeFoundationComponent(id);
    }
    
    void Core::verifySystem(const UType &uType, const std::vector<uint64_t> &underlyingTypeHashes)
    {
        // Miss-matched alignment. Make sure the length of uTypes matches the Systems type length.
        if (underlyingTypeHashes.size() != uType.size())
            throw std::exception();
        for (int i = 0; i < underlyingTypeHashes.size(); ++i)
        {
            if (!mEntityManager.isValid(uType[i], underlyingTypeHashes[i]))
                throw std::exception();  // The type has not been registered yet. The system will produce undefined results.
            // You could also have miss-aligned the types with the underlying types.
        }
    }
    
    void Core::remove(Entity entity, Component component)
    {
        mArchetypeManager.remove(entity, component);
    }
    
    bool Core::hasComponent(Entity entity, Component component)
    {
        return mArchetypeManager.hasComponent(entity, component);
    }
}
