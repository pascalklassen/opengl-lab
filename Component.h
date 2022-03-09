#pragma once

namespace lab
{
    class Component
    {
    public:
        virtual void OnUpdate(float deltaTime) = 0;
        virtual void OnRender() = 0;
        virtual void OnGui() = 0;
    };
}
