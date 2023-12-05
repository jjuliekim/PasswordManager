cmake_minimum_required(VERSION 3.22)
project(PasswordManager)

set(CMAKE_CXX_STANDARD 17)

find_package(SDL2 REQUIRED)
include_directories(${CMAKE_PROJECT_NAME} ${SDL2_INCLUDE_DIRS})
#find_package(SDL2_ttf REQUIRED)
#include_directories(${CMAKE_PROJECT_NAME} ${SDL2TTF_INCLUDE_DIRS})

add_library(pwLib
        lib/include/PasswordTerminal.h
        lib/src/PasswordTerminal.cpp
        lib/include/JsonManager.h
        lib/src/JsonManager.cpp
        lib/include/Data.h
        lib/src/Data.cpp
        lib/include/colormod.h
        lib/include/PasswordManager.h
        lib/src/PasswordManager.cpp)
add_executable(${CMAKE_PROJECT_NAME} main.cpp)

target_include_directories(pwLib PRIVATE lib/include lib/rapidjson)
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE lib/include)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE pwLib ${SDL2_LIBRARIES}) #${SDL2TTF_LIBRARIES})