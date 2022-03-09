#pragma once

#include "Debug.h"

#include "Camera.h"
#include "Component.h"
#include "Scene.h"
#include "SceneRenderer.h"

#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"
#include "VertexBuffer.h"

#include <memory>

namespace lab
{
    template <typename T>
    using Scope = std::unique_ptr<T>;

    template<typename T, typename ... Args>
    constexpr Scope<T> MakeScope(Args&& ... args);

    template <typename T>
    using Ref = std::shared_ptr<T>;

    template<typename T, typename ... Args>
    constexpr Ref<T> MakeRef(Args&& ... args);
}
