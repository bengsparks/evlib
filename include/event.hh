#pragma once


#include <unordered_map>
#include <utility>


namespace evlib {


template<typename Trigger>
class event {
public:
    event() = delete;
    ~event() = default;

    event(const event<Trigger>&) = default;
    event(event<Trigger>&&) noexcept = default;

    auto operator=(const event<Trigger>&) -> event<Trigger>& = default;
    auto operator=(event<Trigger>&&) noexcept -> event<Trigger>& = default;



    explicit event(const Trigger& trigger) :
        __m_trigger{trigger}
    { }

    explicit event(Trigger&& trigger) :
        __m_trigger{std::move(trigger)}
    { }


    friend bool operator<(const event<Trigger>& lhs, const event<Trigger>& rhs) {
        return lhs.hash() < rhs.hash();
    }


    auto hash() const noexcept -> std::size_t {
        return std::hash<decltype(__m_trigger)>{}(__m_trigger);
    }


private:
    Trigger __m_trigger;
};


}


namespace std {
    template <typename Trigger>
    struct hash<evlib::event<Trigger>> {
        auto operator()(const evlib::event<Trigger>& event) const noexcept -> std::size_t {
            return event.hash();
        }
    };
}
