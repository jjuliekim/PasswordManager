cmake_minimum_required(VERSION 3.22)
project(PasswordManager)

set(CMAKE_CXX_STANDARD 17)

add_library(pwLib
        lib/include/PasswordManager.h
        lib/src/PasswordManager.cpp
        lib/include/JsonManager.h
        lib/src/JsonManager.cpp
        lib/include/Data.h
        lib/src/Data.cpp
        lib/include/colormod.h)

target_include_directories(pwLib PRIVATE lib/include lib/rapidjson)

add_executable(${CMAKE_PROJECT_NAME} main.cpp)
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE lib/include)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE pwLib)