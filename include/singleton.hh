#pragma once


#include <type_traits>

namespace evlib::crtp {


template <typename T>
class singleton {
public:
    singleton(const singleton<T>&) = delete;
    singleton(singleton<T>&&) = default;

    auto operator=(const singleton<T>&) -> singleton<T>& = delete;
    auto operator=(singleton<T>&&) -> singleton<T>& = default;

    static auto get_instance() noexcept(std::is_nothrow_constructible_v<T>) -> T& {
        static thread_local T inst{};
        return inst;
    }

protected:
    singleton() noexcept = default;
    virtual ~singleton() noexcept = default;
};


}
