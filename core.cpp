#include "core.h"

#include <memory>

namespace lab
{
    template<typename T, typename ... Args>
    constexpr Scope<T> MakeScope(Args&& ... args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    template<typename T, typename ... Args>
    constexpr Ref<T> MakeRef(Args&& ... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }
}
