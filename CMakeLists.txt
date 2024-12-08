cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
set(CMAKE_COLOR_MAKEFILE ON)

project(UTT
    VERSION 1.0
    LANGUAGES CXX
    DESCRIPTION "Ultimate Tic-Tac-Toe"
)

# Determine LTO ON/OFF
include(CheckIPOSupported)
check_ipo_supported(RESULT LTO_SUPPORT OUTPUT LTO_ERROR)

set(UTT_LTO_ENABLED FALSE)

if(LTO_SUPPORT)
    if (CMAKE_BUILD_TYPE STREQUAL "RELEASE")
        message(STATUS "LTO is supported and will be enabled by default.")
        set(UTT_LTO_ENABLED TRUE)
    endif()
else()
    message(WARNING "LTO is not supported: ${LTO_ERROR}")
endif()

set(CMAKE_CXX_STANDARD 17)

# Add dependencies
if (CMAKE_BUILD_TYPE STREQUAL "RELEASE")
    set(ADORE_BUILD_SHARED OFF)
else()
    set(ADORE_BUILD_SHARED ON)
endif()

add_subdirectory(Adore)

# Add source code
set(SOURCES
    main.cpp
)

add_executable(UTT ${SOURCES})
target_link_libraries(UTT PUBLIC Adore)
target_include_directories(UTT PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
set_target_properties(UTT PROPERTIES
                        OUTPUT_NAME ${PROJECT_NAME}
                        INTERPROCEDURAL_OPTIMIZATION ${UTT_LTO_ENABLED}
)

add_subdirectory(Shaders)

# Install assets dir with install(... Assets ... bin)