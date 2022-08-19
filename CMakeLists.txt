cmake_minimum_required(VERSION 3.20)
project(project_6)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(tests tests.cpp hashmap.cpp hashmap.h)
target_link_libraries(tests gtest_main)
include(GoogleTest)
gtest_discover_tests(tests)

add_executable(project_6
        bitstream.h
        hashmap.cpp
        hashmap.h
        main.cpp
        util.h)
