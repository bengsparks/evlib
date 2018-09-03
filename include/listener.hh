#pragma once

#include <functional>
#include <iostream>


static auto generate_listener_id() -> std::size_t {
    static thread_local std::size_t id = 0;
    if (id == std::numeric_limits<decltype(id)>::max()) {
        std::cerr << "WARNING: LISTENER ID WILL NOW OVERFLOW!\n";
    }
    return id++;
}



namespace evlib {
    class listener {
    public:
        listener() = delete;
        ~listener() = default;

        listener(const listener&) = default;
        listener(listener&&) noexcept = default;

        auto operator=(const listener&) -> listener& = default;
        auto operator=(listener&&) noexcept -> listener& = default;


        explicit listener(std::function<void(void)> listener) :
            __m_listener{std::move(listener)},
            __m_id{generate_listener_id()}
        { }


        auto operator()() const -> void {
            std::invoke(__m_listener);
        }


        friend auto operator<(const listener& lhs, const listener& rhs) -> bool {
            return lhs.__m_id < rhs.__m_id;
        }


    private:
        std::size_t __m_id;
        std::function<void(void)> __m_listener;
    };
};