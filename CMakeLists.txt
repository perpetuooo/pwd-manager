cmake_minimum_required(VERSION 3.15)
project(Password-Manager)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_SOURCE_DIR}/include)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)
add_executable(password_manager src/main.cpp)

if(WIN32) # Windows + vcpkg
    find_package(unofficial-sodium CONFIG REQUIRED)
    target_link_libraries(password_manager PRIVATE unofficial-sodium::sodium)

else() # Linux, macOS, etc
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(SODIUM REQUIRED libsodium)
    target_include_directories(password_manager PRIVATE ${SODIUM_INCLUDE_DIRS})
    target_link_libraries(password_manager PRIVATE ${SODIUM_LIBRARIES})
endif()
