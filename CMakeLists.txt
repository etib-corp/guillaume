cmake_minimum_required(VERSION 3.10)

# Ensure option() honors parent-provided normal variables in subprojects.
if(POLICY CMP0077)
    cmake_policy(SET CMP0077 NEW)
endif()

project(guillaume
    VERSION 1.0.0
    DESCRIPTION "A C++ UI framework designed for modern desktop and extended reality (XR) applications."
    HOMEPAGE_URL "https://etib-corp.github.io/guillaume"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 01. Policy & standard setup

# 02. Options & configuration
option(BUILD_DOCS "Build Doxygen documentation" OFF)
option(BUILD_DOCS_ONLY "Configure only the documentation target" OFF)

if(BUILD_DOCS_ONLY)
    set(BUILD_DOCS ON)
    set(BUILD_TESTING OFF)
endif()

# 03. Global properties

if(NOT BUILD_DOCS_ONLY)
    include(FetchContent)

    # Fetch external dependencies
    FetchContent_Declare(
        sqlite3
        GIT_REPOSITORY https://github.com/sjinks/sqlite3-cmake
        GIT_TAG v3.49.1
    )
    FetchContent_MakeAvailable(sqlite3)

    FetchContent_Declare(utility
        GIT_REPOSITORY https://github.com/etib-corp/utility.git
        GIT_TAG main
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
    )
    FetchContent_MakeAvailable(utility)

    # 04. Library definition

    # Set the files directories
    set(HEADERS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/headers")
    set(TEMPLATES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/templates")
    set(SOURCES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/sources")

    # Add the headers directories
    include_directories(
        ${HEADERS_DIR}
        ${TEMPLATES_DIR}
    )

    # Add the source files
    file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
        "${SOURCES_DIR}/*.cpp"
    )

    add_library(${PROJECT_NAME} ${SOURCES})

    # 07. Link dependencies
    target_link_libraries(${PROJECT_NAME}
        PUBLIC
            utility
            SQLite::SQLite3
    )

    # 05. Include directories
    target_include_directories(${PROJECT_NAME}
        PUBLIC
            $<BUILD_INTERFACE:${HEADERS_DIR}>
            $<INSTALL_INTERFACE:include>
    )

    target_include_directories(${PROJECT_NAME}
        PUBLIC
            $<BUILD_INTERFACE:${TEMPLATES_DIR}>
            $<INSTALL_INTERFACE:include>
    )
endif()

# 08. Documentations

if(BUILD_DOCS)
    message(STATUS "Building documentation...")
    include(cmake/Docs.cmake)
    add_project_docs()
else()
    message(STATUS "Skipping documentation...")
endif()

# 09. Tests

if(BUILD_TESTING)
    message(STATUS "Building tests...")
    add_subdirectory(tests)
else()
    message(STATUS "Skipping tests...")
endif()
