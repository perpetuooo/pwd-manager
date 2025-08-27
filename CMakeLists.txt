cmake_minimum_required(VERSION 3.15)
project(pwd-manager)

find_package(unofficial-sodium CONFIG REQUIRED)
add_executable(password_manager pwd-manager/main.cpp)
target_link_libraries(password_manager PRIVATE unofficial-sodium::sodium)
