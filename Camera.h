#pragma once

#include <glm/glm.hpp>

#include "Component.h"

namespace lab
{
    class Camera : public Component
    {
    private:
        const static glm::vec3 front;
        const static glm::vec3 up;
        constexpr static float speed = 2.5f;
        constexpr static float sensitivity = 0.1f;

        glm::vec3 m_Direction;
        glm::vec3 m_Position;
        glm::vec3 m_Target;
        glm::mat4 m_View;

        float m_FieldOfView;
        float m_Yaw;
        float m_Pitch;
        float m_LastX;
        float m_LastY;
    public:
        Camera(const glm::vec3& position, const glm::vec3& target = { 0.0f, 0.0f, 0.0f });

        void OnUpdate(float deltaTime) override;


        void OnRender() override;


        void OnGui() override;

        glm::mat4 GetView() const { return m_View; }
        float GetFieldOfView() const { return m_FieldOfView; }
    };
}
