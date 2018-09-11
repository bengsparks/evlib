#pragma once


#include <functional>
#include <map>
#include <memory>
#include <set>
#include <thread>
#include <vector>

#include <include/event.hh>
#include <include/listener.hh>


#include <include/singleton.hh>


namespace evlib {


template <typename Trigger>
class emitter : public crtp::singleton<emitter<Trigger>> {
public:
    friend class crtp::singleton<emitter>;


    emitter(const emitter&) = delete;
    emitter(emitter&&) noexcept = default;

    auto operator=(const emitter&) -> emitter& = delete;
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
            std::vector<std::thread> pool(it->second.begin(), it->second.end());

            for (auto&& th : pool) {
                th.join();
            }
        }
    }


private:
    emitter() = default;
    ~emitter() = default;

    std::map<
            event<Trigger>,
            std::set<listener>
    > __m_listen;
};



}
