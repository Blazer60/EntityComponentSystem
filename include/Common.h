/**
 * @file Common.h Common definitions for the ECS Architecture.
 * @author Ryan Purse
 * @date 20/01/2022
 */


#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <set>
#include <algorithm>

/** An Entity ID that can be used to get data from the Entity Component System */
typedef uint64_t                Entity;

/** A Component ID to identify components (Identical to Entity and is used for clarity). */
typedef Entity                  Component;

namespace ecs
{
    /** An Entity ID that can be used to get data from the Entity Component System */
    typedef ::Entity               Entity;

    /** A Component ID to identify components (Identical to Entity and is used for clarity). */
    typedef ::Component                 Component;

    /** A list of components that an entity has. Entities can have both components and entities attached to them. */
    typedef std::set<Component>     ComponentVector;
    
    /** A list of component that an entity has. Identical to ComponentVector. Used when the order of insertion matters. */
    typedef std::vector<Component>  UComponentVector;

    /** The type that an entity is (identical to ComponentVector). @see ComponentVector */
    typedef ComponentVector         Type;
    
    /** The type that an entity is (identical to UComponentVector) @see UComponentVector */
    typedef UComponentVector        UType;
    
    namespace initFlag
    {
        enum init : int
        {
            None = 0b0,
            AutoInitialise = 0b10,
        };
    }

    /** The location of the first bit that represents a given enum within an Entity Id. */
    namespace entityFlagShifts
    {
        /** Not an enum class since the underlying number is using for bitwise operations. */
        enum shifts : Entity {
            /** The first 32 Bits (4 Bytes) of an Entity is always an Id */
            Id          = 0,
            
            /** The first bit that represents the generation of an Entity Id. Note: relationships do not use generations. */
            Generation  = 32,
            
            /** The first bit that represents the type of an Entity Id. Note: relationships do not use types. */
            Type        = 56,
        };
    }

    /** Default types flags that are used within the Entity Component System */
    namespace entityTypeFlag
    {
        enum flag : Entity {
            /** All Entities are tagged with this type. */
            Entity      = 1ull << entityFlagShifts::Type,
            
            /** All components (within \<>) are assigned with this type. */
            Component   = 2ull << entityFlagShifts::Type,
            
            /** Entities with this tag are a parent of the entity given. */
            ParentOf    = 3ull << entityFlagShifts::Type,
        };
    }
    
    namespace entityMask
    {
        /** Masks an entity id by a specified type. */
        enum entityMask : Entity {
            /** The first 32 Bits (4 Bytes) of an Entity is always an Id */
            Id          = 0x00'00'00'00'FF'FF'FF'FF,
            
            /** The generation of an Entity. Note: relationships do not use generations. */
            Generation  = 0x00'00'FF'FF'00'00'00'00,
            
            /** The type of an Entity. Note: relationships do not use types. */
            Type        = 0xFF'00'00'00'00'00'00'00,
        };
    }
    
    /**
     * @brief Flags used to alter the creation of the an entity with the ecs system.
     */
    enum creationType : char {
        /** @brief No change to how the type is made. */
        Default,
        
        /** @brief Lets the ecs know that this type id should be used when no type id is specified. */
        TypeDefault,
    };
    
    typedef uint32_t ExecutionOrder;
    enum executionOrderFlags : ExecutionOrder {
        PreUpdate, Update, PreRender, Render, ImGui
    };
    
    /**
     * @brief Checks to see if the subset is wholly contained within set.
     * @param set - The set you want to check in. E.g.: (A, B, C, D)
     * @param subset - The subset of item you want to check are in set. E.g.: (B, D)
     * @returns True if all of subset is in set, false otherwise. E.g.: B and D are in (A, B, C, D).
     */
    bool includes(const Type &set, const UType &subset);
    
    /**
     * @brief Reinterprets the type flag of entity and returns its type.
     * @param id - The id of the entity that you want to check.
     * @returns The string version of the type flag.
     */
    std::string typeToString(Entity id);
    
    /**
     * @brief Prints information about entity.
     * @param entity - The entity that you want information about.
     */
    void print(Entity entity);
}
