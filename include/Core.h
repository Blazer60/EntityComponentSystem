/**
 * @file Core.h
 * @author Ryan Purse
 * @date 13/04/2022
 */


#pragma once

#include "Common.h"
#include "EntityManager.h"
#include "components/ArchetypeManager.h"
#include "systems/SystemManager.h"
#include "Entities.h"

#include <unordered_map>
#include <typeinfo>
#include <memory>

namespace ecs
{
    /**
     * The 'core' of the Entity Component System. Allows you to create Entities that are used for Ids for Components.
     * Components typically C style structs that contain purely data. Systems then manipulate on components.
     * @author Ryan Purse
     * @date 13/04/2022
     */
    class Core
    {
    public:
        /** @brief Setup for the ecs system. Throws an error if it is unable to initialise. */
        explicit Core(int flags=initFlag::None);
    
        /**
         * @brief Creates an entity that components can be added to.
         */
        [[nodiscard]] Entity create();
    
        /**
         * @brief Creates a component that can be attached to entities.
         * @tparam T - The type (class or struct) that you want it to be.
         * @param flag - How the type is interpreted (leave blank for default).
         * @returns The id given to component T.
         */
        template<typename T>
        Component create(creationType flag=Default);
    
        /**
         * @brief Checks to see if the uType and the underlying types match within the system. THROWS if there's an error.
         * uType[i] MUST pair with underlyingTypeHashes[i].
         * @param uType - The components that you want to check.
         * @param underlyingTypeHashes - The hash codes of each actual type.
         */
        void verifySystem(const UType &uType, const std::vector<uint64_t> &underlyingTypeHashes);
    
        /**
         * @brief Creates a system that can be used within the ecs system.
         * @tparam System - The type of system. MUST inherit from base system.
         * @tparam Args - The type of args passed to the constructor of the system.
         * @param uType - The types of entities that you want this system to operate on.
         * @param args - The arguments passed into the systems constructor.
         */
        template<typename System, typename ...Args>
        void createSystem(const UType &uType, Args &&...args);
    
        /**
         * @brief Creates a system that can be used within the ecs system.
         * @tparam System - The type of system. MUST inherit from base system.
         * @tparam Args - The type of args passed to the constructor of the system.
         * @param args - The arguments passed into the systems constructor.
         */
        template<typename System, typename ...Args>
        void createSystem(Args &&...args);
    
        /**
         * @brief Adds a component to the specified entity.
         * @tparam T - The type you want to give to value.
         * @param eId - The entity Id that you want to give the component to.
         * @param cId - The component Id of T.
         * @param value - The actual data assigned to entity.
         */
        template<typename T>
        void add(Entity eId, Component cId, const T &value);
    
        /**
         * @brief Adds a component to the specified entity.
         * @tparam T - The type you want to give to value.
         * @param eId - The entity Id that you want to give the component to.
         * @param value - The actual data assigned to entity.
         */
        template<typename T>
        void add(Entity eId, const T &value);
        
        /**
         * @brief Performs an update on every system and entity in the ecs system.
         */
        void fixedUpdate();
    
        /**
         * @brief Performs an update on every system and entity in the ecs system.
         */
        void update();
        
        /**
         * @brief Performs a render update on every system and entity in the ecs system.
         */
        void render();
        
        /**
         * @brief Performs an imGui update on every system and entity in the ecs system.
         */
        void imGui();
    
        /**
         * @brief
         * @tparam EArgs
         * @param entities
         * @param uType
         */
        template<typename ...EArgs>
        void processEntities(Entities<EArgs...> &entities, const UType &uType);
    
        /**
         * @brief Makes the given Id the default id when handling components with the same type.
         * @param id - The id that you want to make the default.
         */
        void makeFoundationComponent(Component id);
    
        /**
         * @brief Gets the default Component Id assigned to T.
         * @tparam T - The type you want to lookup.
         * @returns A Component Id.
         */
        template<typename T>
        [[nodiscard]] Component getComponentIdOf();
    
        /**
         * @brief Gets the default Component Id assigned to T. (Identical to getComponentIdOf();)
         * @tparam T - The type you want to lookup.
         * @returns A component Id.
         */
        template<typename T>
        [[nodiscard]] Component get();
    
        /**
         * @brief Gets a reference to a component of type T.
         * WARNING: Do not store this value for longer than this function is used.
         * @tparam T - The type of component you're looking for.
         * @param entity - The entity that you'd like to query.
         * @param component - The component Id of T.
         */
        template<typename T>
        [[nodiscard]] T &getComponent(Entity entity, Component component);
    
        /**
         * @brief Checks to see if an entity has a component.
         * @param entity - The entity that may have component.
         * @param component - The component you're querying for.
         * @return True if component was found, false otherwise.
         */
        [[nodiscard]] bool hasComponent(Entity entity, Component component);
    
        /**
         * @brief Checks to see if an entity has component T.
         * @tparam T - The component type you're looking for.
         * @param entity - The entity that may have T.
         * @return True if component was found, false otherwise.
         */
        template<typename T>
        bool hasComponent(Entity entity);
    
        /**
         * @brief Gets a reference to a component of type T.
         * WARNING: Do not store this value for longer than this function is used.
         * @tparam T - The type of component you're looking for.
         * @param entity - The entity that you'd like to query.
         */
        template<typename T>
        [[nodiscard]] T &getComponent(Entity entity);
    
        /**
         * @brief Removes a component from an entity.
         * @param entity - The entity you want to target
         * @param component - The component that you want to remove.
         */
        void remove(Entity entity, Component component);
    
    protected:
        int                 mInitSettings   { initFlag::None };
        EntityManager       mEntityManager;
        ArchetypeManager    mArchetypeManager;
        SystemManager       mSystemManager;
    };
}


namespace ecs
{
    // Deliberately separating with a namespace to show that this is the implementation.
    template<typename T>
    Component Core::create(const creationType flag)
    {
        Component out = mEntityManager.createComponent<T>();
        if (flag == creationType::TypeDefault)
            mEntityManager.makeFoundationComponent(out);
        return out;
    }
    
    template<typename T, typename... Args>
    void Core::createSystem(const UType &uType, Args &&... args)
    {
        static_assert(std::is_base_of<IBaseSystem, T>(),
                      "T must be a base system E.g.: MySystem : public ecs::BaseSystem<>");
        
        std::unique_ptr<IBaseSystem> system = std::make_unique<T>(std::forward<Args>(args)...);
        
        IEntities * const entities       = system->getEntities();
        entities->mEcsRegisteredTo = this;
        const std::vector<uint64_t> typeHashes = entities->getUnderlyingTypeHashes();
        
        if (mInitSettings & initFlag::AutoInitialise)
        {
            // Attempt to autofill the rest to make the system type complete.
            UType components = entities->getDefaultComponents();
            for (int i = 0; i < uType.size(); ++i)
                components[i] = uType[i];  // Switch out the ones that the user has already defined.
            
            verifySystem(components, typeHashes);
            
            mSystemManager.addSystem(components, std::move(system));
            return;
        }
        
        verifySystem(uType, typeHashes);
        
        mSystemManager.addSystem(uType, std::move(system));
    }
    
    template<typename T, typename... Args>
    void Core::createSystem(Args &&... args)
    {
        static_assert(std::is_base_of<IBaseSystem, T>(),
                      "T must be a base system E.g.: MySystem : public ecs::BaseSystem<>");
        
        std::unique_ptr<T> system = std::make_unique<T>(std::forward<Args>(args)...);
        
        IEntities * const     entities    = system->getEntities();
        entities->mEcsRegisteredTo = this;
        const std::vector<uint64_t> typeHashes  = entities->getUnderlyingTypeHashes();
        const UType                 components  = entities->getDefaultComponents();
        
        verifySystem(components, typeHashes);  // Should never throw here, but it's a nice redundancy check.
        
        mSystemManager.addSystem(components, std::move(system));
    }
    
    template<typename T>
    void Core::add(Entity eId, Component cId, const T &value)
    {
        mArchetypeManager.add(eId, cId, value);
    }
    
    template<typename T>
    void Core::add(Entity eId, const T &value)
    {
        const auto cId = mEntityManager.getComponentIdOf<T>();
        mArchetypeManager.add(eId, cId, value);
    }
    
    template<typename... EArgs>
    void Core::processEntities(Entities<EArgs...> &entities, const UType &uType)
    {
        std::vector<Archetype*> archetypes = mArchetypeManager.getArchetypesWithSubset(uType);
        
        for (Archetype *archetype : archetypes)
        {
            auto uTypeIt = uType.begin();
            std::tuple<ComponentArray<EArgs>*...> arrays = archetype->getArraysOfType_s<EArgs...>(uTypeIt);
            
            for (int i = 0; i < std::get<0>(arrays)->data.size(); ++i)
                entities.invoke(std::forward_as_tuple(std::get<ComponentArray<EArgs>*>(arrays)->data[i]...));
        }
    }
    
    template<typename T>
    Component Core::getComponentIdOf()
    {
        return mEntityManager.getComponentIdOf<T>();
    }
    
    template<typename T>
    Component Core::get()
    {
        return mEntityManager.getComponentIdOf<T>();
    }
    
    template<typename T>
    T &Core::getComponent(Entity entity, Component component)
    {
        // Component has not been registered.
        // Type T does not match up with id component.
        if (!mEntityManager.isValid(component, typeid(T).hash_code()))
            throw std::exception();
        return mArchetypeManager.getComponent<T>(entity, component);
    }
    
    template<typename T>
    T &Core::getComponent(Entity entity)
    {
        return getComponent<T>(entity, get<T>());
    }
    
    template<typename T>
    bool Core::hasComponent(Entity entity)
    {
        return hasComponent(entity, get<T>());
    }
}
