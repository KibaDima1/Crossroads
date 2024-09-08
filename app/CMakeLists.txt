set(CMAKE_CXX_STANDARD 20)

message(STATUS "CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}")

add_compile_definitions(FMT_HEADER_ONLY)

set(Boost_INCLUDE_DIR /opt/homebrew/Cellar/boost/1.86.0/include)
set(Boost_LIBRARY_DIR /opt/homebrew/Cellar/boost/1.86.0/lib)
find_package(Boost 1.86.0)
include_directories(${Boost_INCLUDE_DIR})

message(STATUS "CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}")
include_directories(${CMAKE_SOURCE_DIR}/app)

if(NOT Boost_FOUND)
    message(FATAL_ERROR "Could not find boost!")
endif()

file(GLOB includes CONFIGURE_DEPENDS
        "app/*.h"
        "app/*/*.h"
        "app/*/*/*.h"
        "app/*/*/*/*.h"
        "app/*/*/*/*/*.h"
        "app/*/*/*/*/*/*.h"
        "app/*/*/*/*/*/*/*.h"
        "app/*/*/*/*/*/*/*/*.h"
        "app/*/*/*/*/*/*/*/*/*.h")

file(GLOB sources CONFIGURE_DEPENDS
        "app/*.cpp"
        "app/*/*.c"
        "app/*/*.cpp"
        "app/*/*/*.c"
        "app/*/*/*.cpp"
        "app/*/*/*/*.c"
        "app/*/*/*/*.cpp"
        "app/*/*/*/*/*.c")

add_executable(${PROJECT_NAME} ${includes} ${sources})