set(CMAKE_CXX_STANDARD 20)

message(STATUS "CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}")

add_compile_definitions(FMT_HEADER_ONLY)

set(BOOST_ROOT /opt/homebrew/Cellar/boost/1.86.0)
set(BOOST_INCLUDE_DIR ${BOOST_ROOT}/include)
set(BOOST_LIBRARY_DIR ${BOOST_ROOT}/lib)

find_package(Boost 1.86.0 REQUIRED COMPONENTS system filesystem thread)
include_directories(${Boost_INCLUDE_DIRS})

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

target_link_libraries(${PROJECT_NAME} Boost::system Boost::filesystem Boost::thread)