cmake_minimum_required(VERSION 3.12)
project(evlib)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)

add_library(evlib evlib.hh include/emitter.hh include/event.hh include/listener.hh include/singleton.hh)
set_target_properties(evlib PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(evlib pthread)

if(MSVC)
    # Force to always compile with W4
    if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
        string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    endif()
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    # Update if necessary
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -Wpedantic -Wunused-function")
endif()


add_executable(main main.cc)
target_link_libraries(main evlib pthread)
