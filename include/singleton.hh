#pragma once


#include <type_traits>

namespace crtp {


template <typename T>
class singleton {
public:
    singleton(const singleton&) = delete;
    singleton& operator=(const singleton&) = delete;

    static auto get_instance() noexcept(std::is_nothrow_constructible_v<T>) -> T& {
        static thread_local T inst{};
        return inst;
    }

protected:
    singleton() noexcept = default;
    virtual ~singleton() noexcept = default;
};


}