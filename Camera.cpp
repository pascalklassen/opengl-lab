#include "Camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

#include <algorithm>

namespace lab
{
    const glm::vec3 Camera::front = { 0.0f, 0.0f, -1.0f };
    const glm::vec3 Camera::up = { 0.0f, 1.0f, 0.0f };

//         /* Gram-Schmidt process for defining the axis vectors in camera space */
//         glm::vec3 zAxis{ glm::normalize(position - target) };
// 
//         // to get the x-Axis we need to specify an *up*-vector and
//         // calc the cross product of the specified vector and the z-Axis
//         glm::vec3 xAxis{ glm::normalize(glm::cross({ 0.0f, 1.0f, 0.0f }, zAxis)) };
// 
//         // to get the y-Axis we can simply calculate the cross product
//         // of the z-Axis and y-Axis
//         glm::vec3 yAxis{ glm::cross(zAxis, xAxis) };
    Camera::Camera(const glm::vec3& position, const glm::vec3& target /*= { 0.0f, 0.0f, 0.0f }*/)
        : m_FieldOfView{ 45.0f }, m_Yaw{ -90.0f }, m_Pitch{ 0 },
          m_LastX{ 400 }, m_LastY{ 300 }, // TODO add respective viewport sizes
          m_Direction{ front }, m_Position{ position }, m_Target{ target },
          m_View{ glm::lookAt(m_Position, m_Position + front, up) }
    {
    }
    
    void Camera::OnUpdate(float deltaTime)
    {
        float speed = deltaTime * Camera::speed;
        glm::vec3 front = glm::normalize(m_Direction);

        if (ImGui::IsKeyDown(ImGuiKey_W))
        {
            m_Position += speed * front;
        }

        if (ImGui::IsKeyDown(ImGuiKey_A))
        {
            m_Position -= speed * glm::normalize(glm::cross(front, up));
        }

        if (ImGui::IsKeyDown(ImGuiKey_S))
        {
            m_Position -= speed * front;
        }

        if (ImGui::IsKeyDown(ImGuiKey_D))
        {
            m_Position += speed * glm::normalize(glm::cross(front, up));
        }

        if (ImGui::IsMouseDown(ImGuiMouseButton_Left))
        {
            ImGui::SetMouseCursor(ImGuiMouseCursor_::ImGuiMouseCursor_None);
            ImVec2 pos = ImGui::GetMousePos();
            float xOffset = (pos.x - m_LastX) * sensitivity;
            float yOffset = (m_LastY - pos.y) * sensitivity;
            m_LastX = pos.x;
            m_LastY = pos.y;

            m_Yaw += xOffset;
            m_Pitch += yOffset;

            m_Pitch = std::clamp(m_Pitch, -89.0f, 89.0f);

            m_Direction.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
            m_Direction.y = sin(glm::radians(m_Pitch));
            m_Direction.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));

            auto& io = ImGui::GetIO();
            float scrollOffset = io.MouseWheel;

            m_FieldOfView -= scrollOffset;
            m_FieldOfView = std::clamp(m_FieldOfView, 1.0f, 45.0f);
        }

        m_View = glm::lookAt(m_Position, m_Position + front, up);
    }

    void Camera::OnRender()
    {
    }

    void Camera::OnGui()
    {
        ImGui::Begin("Camera");
        ImGui::InputFloat3("Direction", glm::value_ptr(m_Direction), "%.3f", ImGuiInputTextFlags_ReadOnly);
        ImGui::InputFloat3("Position", glm::value_ptr(m_Position), "%.3f", ImGuiInputTextFlags_ReadOnly);
        ImGui::InputFloat3("Target", glm::value_ptr(m_Target), "%.3f", ImGuiInputTextFlags_ReadOnly);

        ImGui::InputFloat("Field of View", &m_FieldOfView, 0.0f, 0.0f, "%.3f", ImGuiInputTextFlags_ReadOnly);
        ImGui::InputFloat("Yaw", &m_Yaw, 0.0f, 0.0f, "%.3f", ImGuiInputTextFlags_ReadOnly);
        ImGui::InputFloat("Pitch", &m_Pitch, 0.0f, 0.0f, "%.3f", ImGuiInputTextFlags_ReadOnly);

        ImGui::InputFloat("Last Mouse X", &m_LastX, 0.0f, 0.0f, "%.3f", ImGuiInputTextFlags_ReadOnly);
        ImGui::InputFloat("Last Mouse Y", &m_LastY, 0.0f, 0.0f, "%.3f", ImGuiInputTextFlags_ReadOnly);
        ImGui::End();
    }
}
