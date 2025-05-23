cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if ("${GEODE_TARGET_PLATFORM}" STREQUAL "iOS" OR IOS)
        set(CMAKE_OSX_ARCHITECTURES "arm64")
        add_definitions(-DGLES_SILENCE_DEPRECATION)
else()
        set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")
endif()
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

project(PlatformerSaves VERSION 1.0.0)

# source files
file(GLOB_RECURSE SOURCES
	src/ui/*.cpp
	src/hooks/*.cpp
    src/hooks/cocos2d/*.cpp
    src/managers/*.cpp
    src/domain/*.cpp
    src/util/*.cpp
	src/*.cpp
    ${OS_SOURCES}
)

add_library(${PROJECT_NAME} SHARED ${SOURCES})

target_compile_definitions(${PROJECT_NAME}
    PUBLIC
      $<$<CONFIG:Debug>:PS_DEBUG>
      $<$<CONFIG:RelWithDebInfo>:PS_DEBUG>
      $<$<CONFIG:Release>:PS_RELEASE>
      $<$<CONFIG:MinSizeRel>:PS_RELEASE>
)

option(WITH_PS_DESCRIBE "Log in depth info for saved/loaded classes" OFF)
option(WITH_PS_SEPARATORS "Add separators between saved fields for debugging purposes" OFF)

if(WITH_PS_DESCRIBE)
	add_definitions(-DPS_DESCRIBE)
	add_definitions(-DPA_DESCRIBE)
	add_definitions(-DPA_DEBUG)
endif()
if(WITH_PS_SEPARATORS)
	add_definitions(-DPS_SEPARATORS)
	add_definitions(-DPA_SEPARATORS)
	add_definitions(-DPA_DEBUG)
endif()

if (NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR "Unable to find Geode SDK! Please define GEODE_SDK environment variable to point to Geode")
else()
    message(STATUS "Found Geode: $ENV{GEODE_SDK}")
endif()

target_compile_options(${PROJECT_NAME} PRIVATE "-Wno-microsoft-include")
target_compile_options(${PROJECT_NAME} PRIVATE "-Wno-invalid-offsetof")

target_include_directories(${PROJECT_NAME} PRIVATE src/)

add_subdirectory($ENV{GEODE_SDK} ${CMAKE_CURRENT_BINARY_DIR}/geode)

setup_geode_mod(${PROJECT_NAME} EXTERNALS sabe.persistenceapi)