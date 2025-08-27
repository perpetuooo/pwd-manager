cmake_minimum_required(VERSION 3.15)
project(Password-Manager)

add_executable(password_manager pwd-manager/main.cpp)

if(WIN32) # Windows + vcpkg
    find_package(unofficial-sodium CONFIG REQUIRED)
    target_link_libraries(password_manager PRIVATE unofficial-sodium::sodium)

else() # Linux, macOS, etc
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(SODIUM REQUIRED libsodium)
    target_include_directories(password_manager PRIVATE ${SODIUM_INCLUDE_DIRS})
    target_link_libraries(password_manager PRIVATE ${SODIUM_LIBRARIES})
endif()
