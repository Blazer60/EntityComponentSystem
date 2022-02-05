/**
 * @file Core.h
 * @author Ryan Purse
 * @date 20/01/2022
 */


#pragma once

#include "Common.h"
#include "src/EntityManager.h"
#include "src/components/ArchetypeManager.h"
#include "src/systems/SystemManager.h"

#include <unordered_map>
#include <typeinfo>
#include <memory>

/**
 * @brief Provides helpful functions to access lower level code.
 * Static members should not be accessed directly.
 * Use init() to generate the ecs system.
 */
namespace ecs
{
    // Should these be in a struct or not (E.g.: impl->entityManager.create())?
    extern std::unique_ptr<EntityManager>    sEntityManager;
    extern std::unique_ptr<ArchetypeManager> sArchetypeManager;
    extern std::unique_ptr<SystemManager>    sSystemManager;
    
    /** @brief Setup for the ecs system. Throws an error if it is unable to initialise. */
    void init();
    
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
     * uType[i] MUST pair with underlyingTypes[i].
     * @param uType - The components that you want to check.
     * @param underlyingTypes - The hash codes of each actual type.
     */
    void verifySystem(const UType &uType, const std::vector<uint64_t> &underlyingTypes);
    
    /**
     * @brief Creates a system that can be used within the ecs system.
     * @tparam System - The type of system. MUST inherit from base system.
     * @tparam Args - The type of args passed to the constructor of the system.
     * @param uType - The types of entities that you want this system to operate on.
     * @param args - The arguments passed into the systems constructor.
     */
    template<typename System, typename ...Args>
    void createSystem(const UType &uType, const Args &...args);
    
    /**
     * @brief Creates a system that can be used within the ecs system.
     * @tparam System - The type of system. MUST inherit from base system.
     * @tparam Args - The type of args passed to the constructor of the system.
     * @param args - The arguments passed into the systems constructor.
     */
    template<typename System, typename ...Args>
    void createSystem(const Args &...args);
    
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
    void update();
    
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
     * @brief Removes a component from an entity.
     * @param entity - The entity you want to target
     * @param component - The component that you want to remove.
     */
    void remove(Entity entity, Component component);
    
    
    // Implementations
    
    template<typename T>
    Component create(const creationType flag)
    {
        Component out = sEntityManager->createComponent<T>();
        if (flag == creationType::TypeDefault)
            sEntityManager->makeFoundationComponent(out);
        return out;
    }
    
    template<typename T, typename... Args>
    void createSystem(const UType &uType, const Args &... args)
    {
        std::unique_ptr<IBaseSystem> system = std::make_unique<T>(args...);
        
        verifySystem(uType, system->getUnderlyingTypes());
        
        sSystemManager->addSystem(uType, std::move(system));
    }
    
    template<typename T, typename... Args>
    void createSystem(const Args &... args)
    {
        std::unique_ptr<IBaseSystem> system = std::make_unique<T>(args...);
        
        // Deduce component ids based on the underlying types.
        std::vector<uint64_t> underlyingTypes = system->getUnderlyingTypes();
        
        UType uType;
        uType.reserve(underlyingTypes.size());
        for (const uint64_t &underlyingType : underlyingTypes)
            uType.emplace_back(sEntityManager->getComponentIdOf(underlyingType));
        
        verifySystem(uType, underlyingTypes);  // Should never throw here, but it's a nice redundancy check.
        
        sSystemManager->addSystem(uType, std::move(system));
    }
    
    template<typename T>
    void add(Entity eId, Component cId, const T &value)
    {
        sArchetypeManager->add(eId, cId, value);
    }
    
    template<typename T>
    void add(Entity eId, const T &value)
    {
        const auto cId = sEntityManager->getComponentIdOf<T>();
        sArchetypeManager->add(eId, cId, value);
    }
    
    template<typename... EArgs>
    void processEntities(Entities<EArgs...> &entities, const UType &uType)
    {
        std::vector<Archetype*> archetypes = sArchetypeManager->getArchetypesWithSubset(uType);
        
        for (Archetype *archetype : archetypes)
        {
            auto uTypeIt = uType.begin();
            std::tuple<ComponentArray<EArgs>*...> arrays = archetype->getArraysOfType_s<EArgs...>(uTypeIt);
            
            for (int i = 0; i < std::get<0>(arrays)->data.size(); ++i)
                entities.invoke(std::forward_as_tuple(std::get<ComponentArray<EArgs>*>(arrays)->data[i]...));
        }
    }
    
    template<typename T>
    Component getComponentIdOf()
    {
        return sEntityManager->getComponentIdOf<T>();
    }
}
