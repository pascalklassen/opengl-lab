#pragma once

namespace lab
{
    class Entity
    {
    public:
        virtual void OnUpdate(float deltaTime) = 0;
        virtual void OnRender() = 0;
        virtual void OnGui() = 0;
    };
}
