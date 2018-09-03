#pragma once


#include <cxxabi.h>
#include <functional>
#include <map>
#include <memory>
#include <set>


#include <include/event.hh>
#include <include/listener.hh>


namespace evlib {

    
template <typename Trigger>
class emitter {
public:
    emitter() = default;
    ~emitter() = default;

    emitter(const emitter&) = default;
    emitter(emitter&&) noexcept = default;

    auto operator=(const emitter&) -> emitter& = default;
    auto operator=(emitter&&) noexcept -> emitter& = default;

    auto on(const event<Trigger>& event, const listener& listener) -> void {
        auto it = __m_listen.find(event);
        if (it == __m_listen.end()) {
            __m_listen[event] = std::set<evlib::listener>{};
        }

        __m_listen.at(event).insert(listener);
    }

    auto off(const event<Trigger>& event, const listener& listener) -> bool {
        auto it = __m_listen.find(event);
        if (it == __m_listen.end()) {
            return false;
        }

        __m_listen.at(event).erase(listener);
        return true;
    }

    auto emit(const event<Trigger>& event) -> void {
        if (const auto it = __m_listen.find(event); it != __m_listen.end()) {
            for (const auto& listener : it->second) {
                std::invoke(listener);
            }
        }
    }


private:
    std::map<
            event<Trigger>,
            std::set<listener>
    > __m_listen;
};



}