#pragma once


#include <cxxabi.h>
#include <functional>
#include <map>
#include <memory>
#include <set>


#include <include/event.hh>
#include <include/listener.hh>


template <class T>
std::string
type_name()
{
    typedef typename std::remove_reference<T>::type TR;
    std::unique_ptr<char, void(*)(void*)> own
            (
#ifndef _MSC_VER
            abi::__cxa_demangle(typeid(TR).name(), nullptr,
                                nullptr, nullptr),
#else
            nullptr,
#endif
            std::free
    );
    std::string r = own != nullptr ? own.get() : typeid(TR).name();
    if (std::is_const<TR>::value)
        r += " const";
    if (std::is_volatile<TR>::value)
        r += " volatile";
    if (std::is_lvalue_reference<T>::value)
        r += "&";
    else if (std::is_rvalue_reference<T>::value)
        r += "&&";
    return r;
}



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