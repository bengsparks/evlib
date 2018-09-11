#include <thread>
#include <vector>

#include <include/emitter.hh>

auto multithread(std::size_t) -> bool;


auto main(int, char*[]) -> int {
    auto hw = evlib::listener{[]() -> void {
        std::cout << "hello world!\n";
    }};

    evlib::emitter<std::string>::get_instance().on(
        evlib::event<std::string>{"hello program"}, hw
    );
    evlib::emitter<std::string>::get_instance().emit(
        evlib::event<std::string>{"hello program"}
    );


    evlib::emitter<std::string>::get_instance().off(
        evlib::event<std::string>{"hello program"}, hw
    );
    evlib::emitter<std::string>::get_instance().emit(
        evlib::event<std::string>{"hello program"}
    );

    if (not multithread(10)) {
        std::cerr << "multithread failed\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}


auto multithread(std::size_t thread_count) -> bool {
    auto& emitter = evlib::emitter<decltype(thread_count)>::get_instance();

    auto listeners = std::vector<evlib::listener>{};
    for (decltype(thread_count) thread_no = 0; thread_no < thread_count; ++thread_no) {
        listeners.emplace_back([thread_no]() -> void {
            std::cout << "event called from thread number #" << thread_no << "\n";
        });
    }

    auto enable_event = [&](decltype(thread_count) thread_no, const evlib::listener& listener) -> decltype(thread_no) {
        std::cout << "enabling event from thread number #" << thread_no << "\n";
        emitter.on(evlib::event<decltype(thread_no)>{thread_no}, listener);
        return thread_no;
    };

    auto remove_event = [&](decltype(thread_count) thread_no, const evlib::listener& listener) -> decltype(thread_no) {
        std::cout << "removing event from thread number #" << thread_no << "\n";
        emitter.off(evlib::event<decltype(thread_no)>{thread_no}, listener);
        return thread_no;
    };

    auto emit_event = [&](decltype(thread_count) thread_no) -> decltype(thread_no) {
        std::cout << "emitting event from thread number #" << thread_no << "\n";
        emitter.emit(evlib::event<decltype(thread_no)>{thread_no});
        return thread_no;
    };

    std::vector<std::thread> register_pool{thread_count};
    for (decltype(thread_count) i = 0; i < thread_count; ++i) {
        register_pool[i] = std::thread{enable_event, i, listeners[i]};
    }
    for (auto&& th : register_pool) {
        th.join();
    }
    std::cout << "\nsuccessfully completed registering\n\n";

    std::vector<std::thread> emit_pool{thread_count};
    for (decltype(thread_count) i = 0; i < thread_count; ++i) {
        emit_pool[i] = std::thread{emit_event, i};
    }
    for (auto&& th : emit_pool) {
        th.join();
    }
    std::cout << "\nsuccessfully completed emitting\n\n";

    std::vector<std::thread> remove_pool{thread_count};
    for (decltype(thread_count) i = 0; i < thread_count; ++i) {
        remove_pool[i] = std::thread{remove_event, i, listeners[i]};
    }
    for (auto&& th : remove_pool) {
        th.join();
    }
    std::cout << "\nsuccessfully completed removing\n\n";


    for (decltype(thread_count) i = 0; i < thread_count; ++i) {
        emit_pool[i] = std::thread{emit_event, i};
    }
    for (auto&& th : emit_pool) {
        th.join();
    }
    std::cout << "\nsuccessfully completed emitting\n\n";


    return true;
}
