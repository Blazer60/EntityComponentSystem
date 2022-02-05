#include "Ecs.h"

#include <iostream>

struct Velocity
{
    float x;
    float y;
};

struct Position
{
    float x;
    float y;
};

class MockSystem
    : public ecs::BaseSystem<Position, Velocity>
{
public:
    ~MockSystem() override = default;
    
    void onUpdate() override
    {
        gravity += 0.1f;
        
        mEntities.forEach([this](Position &pos, Velocity &velocity) {
            pos.x += 1.f;
            pos.y += 1.f;
        });
    }

    
protected:
    float gravity { 9.81f };
};

int main()
{
    ecs::init();
    
    ecs::create<Velocity>(ecs::TypeDefault);
    ecs::create<Position>(ecs::TypeDefault);
    
    for (int i = 0; i < 10; ++i)
    {
        ecs::Entity entity = ecs::create();
        ecs::add(entity, Velocity { 0.f, 0.f } );
        ecs::add(entity, Position { 0.f, 0.f } );
    }
    
    ecs::Entity e = ecs::create();
    ecs::add(e, Velocity { 0.f, 0.f } );
    ecs::add(e, Position { 0.f, 0.f } );
    
    for (int i = 0; i < 10; ++i)
    {
        ecs::Entity entity = ecs::create();
        ecs::add(entity, Velocity { 0.f, 0.f } );
        ecs::add(entity, Position { 0.f, 0.f } );
    }
    
    ecs::createSystem<MockSystem>();
    
    ecs::update();
    
    ecs::remove(e, ecs::getComponentIdOf<Velocity>());

    std::cout << "Hello, World!" << std::endl;
    return 0;
}
