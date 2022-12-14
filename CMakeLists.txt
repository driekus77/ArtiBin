cmake_minimum_required(VERSION 3.1)

set(project_name artibin)

project(${project_name})

set(CMAKE_CXX_STANDARD 11)

add_library(${project_name}-lib
        src/AppComponent.hpp
        src/controller/DockerRepositoryController.cpp
        src/controller/DockerRepositoryController.hpp)

## link libs

find_package(oatpp 1.3.0 REQUIRED)
find_package(oatpp-openssl 1.3.0 REQUIRED)
find_package(OpenSSL REQUIRED)

target_link_libraries(${project_name}-lib
        PUBLIC oatpp::oatpp
        PUBLIC oatpp::oatpp-test
        PUBLIC oatpp::oatpp-openssl
        PUBLIC OpenSSL::SSL
)

target_include_directories(${project_name}-lib PUBLIC src)

## add executables

add_executable(${project_name}
        src/App.cpp
        src/Utils.cpp src/Utils.hpp)
target_link_libraries(${project_name} ${project_name}-lib)
add_dependencies(${project_name} ${project_name}-lib)

add_executable(${project_name}-test
        test/tests.cpp
        )

target_link_libraries(${project_name}-test ${project_name}-lib)
add_dependencies(${project_name}-test ${project_name}-lib)

set_target_properties(${project_name}-lib ${project_name} ${project_name}-test PROPERTIES
        CXX_STANDARD 11
        CXX_EXTENSIONS OFF
        CXX_STANDARD_REQUIRED ON
)

enable_testing()
add_test(project-tests ${project_name}-test)
