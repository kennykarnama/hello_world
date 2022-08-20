cmake_minimum_required(VERSION 3.1)

set(project_name hello_world) ## rename your project here

project(${project_name})

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(${project_name}-lib
        src/AppComponent.hpp
        src/controller/MyController.cpp
        src/controller/MyController.hpp
        src/dto/DTOs.hpp
        )

## link libs
find_package(Boost REQUIRED)
find_package(folly REQUIRED)
find_package(Threads REQUIRED)
find_package(gflags REQUIRED)
find_package(GLOG REQUIRED)
find_package(oatpp 1.3.0 REQUIRED)

set(Boost_DEBUG ON)
message(STATUS "Boost version: ${Boost_VERSION}")
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR})


set_and_check(FOLLY_INCLUDE_DIR /opt/homebrew/include/folly)
set_and_check(FOLLY_CMAKE_DIR /opt/homebrew/lib/cmake/folly)
if (NOT TARGET Folly::folly)
    include("${FOLLY_CMAKE_DIR}/folly-targets.cmake")
endif()
set(FOLLY_LIBRARIES Folly::folly)


target_link_libraries(${project_name}-lib
        PUBLIC oatpp::oatpp
        PUBLIC oatpp::oatpp-test
        )

target_include_directories(${project_name}-lib PUBLIC src)

## add executables

add_executable(${project_name}-exe
        src/App.cpp)
target_link_libraries(${project_name}-exe ${project_name}-lib ${Boost_LIBRARIES} ${FOLLY_LIBRARIES})
add_dependencies(${project_name}-exe ${project_name}-lib)

set_target_properties(${project_name}-lib ${project_name}-exe PROPERTIES
        CXX_STANDARD 14
        CXX_EXTENSIONS OFF
        CXX_STANDARD_REQUIRED ON
        )