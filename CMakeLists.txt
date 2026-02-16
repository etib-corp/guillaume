cmake_minimum_required(VERSION 3.10)

project(guillaume
    VERSION 1.0.0
    DESCRIPTION "A C++ UI framework designed for modern desktop and extended reality (XR) applications."
    HOMEPAGE_URL "https://etib-corp.github.io/guillaume"
    LANGUAGES CXX
)

# Fetch external dependencies
include(FetchContent)

FetchContent_Declare(
    utility
    GIT_REPOSITORY https://github.com/etib-corp/utility
    GIT_TAG main
)

FetchContent_MakeAvailable(utility)

# Include common functions
include(cmake/CommonFunctions.cmake)

# Platform-specific configuration
if(WIN32)
    include(cmake/ConfigWindows.cmake)
elseif(APPLE)
    include(cmake/ConfigMacOS.cmake)
elseif(UNIX)
    include(cmake/ConfigLinux.cmake)
endif()

# Set global C++ standard and properties
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Set default build type if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

# Set the files directories
set(HEADERS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/headers")
set(SOURCES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/sources")

# Add the headers directories
include_directories(
    ${HEADERS_DIR}
)

# Add the source files
file(GLOB_RECURSE SOURCES
    "${SOURCES_DIR}/*.cpp"
)

# Create the main library
add_library(${PROJECT_NAME} ${SOURCES})

# Set common target properties
set_guillaume_target_properties(${PROJECT_NAME})

# Link private dependencies
target_link_libraries(${PROJECT_NAME}
    PUBLIC
        utility
)

# Configure public interface
target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${HEADERS_DIR}>
        $<INSTALL_INTERFACE:include>
)

# Optional components
option(BUILD_TESTING "Build the tests" OFF)
option(BUILD_DOCS "Build documentation" OFF)
option(BUILD_EXAMPLES "Build the examples" OFF)

if(BUILD_TESTING)
    add_subdirectory(tests)
else()
    message(STATUS "Skipping tests...")
endif()

if(BUILD_DOCS)
    # Find Doxygen package (keep usage compatible across CMake versions and CI images)
    find_package(Doxygen REQUIRED)
    if(NOT Doxygen_FOUND)
        message(FATAL_ERROR "Doxygen not found. Documentation will not be built. Install doxygen and try again.")
    endif()

    find_program(DOT_EXECUTABLE dot)
    if(NOT DOT_EXECUTABLE)
        message(FATAL_ERROR "Graphviz 'dot' executable not found. Please install Graphviz to enable diagram generation in the documentation.")
    endif()

    # Set up doxygen-awesome-css theme (optional styling)
    FetchContent_Declare(
        doxygen-awesome-css
        GIT_REPOSITORY https://github.com/jothepro/doxygen-awesome-css.git
        GIT_TAG v2.4.1
    )
    FetchContent_MakeAvailable(doxygen-awesome-css)
    FetchContent_GetProperties(doxygen-awesome-css SOURCE_DIR AWESOME_CSS_DIR)

    # Configure Doxygen settings using modern CMake variables
    # These variables are automatically processed by doxygen_add_docs()
    set(DOXYGEN_PROJECT_NAME "${PROJECT_NAME}")
    set(DOXYGEN_PROJECT_BRIEF "${PROJECT_DESCRIPTION}")
    set(DOXYGEN_PROJECT_NUMBER "${PROJECT_VERSION}")

    # Configure appearance and theme
    set(DOXYGEN_HTML_EXTRA_STYLESHEET "${AWESOME_CSS_DIR}/doxygen-awesome.css")
    set(DOXYGEN_HTML_COLORSTYLE "LIGHT")
    set(DOXYGEN_GENERATE_TREEVIEW YES)

    # Configure input files and patterns
    set(DOXYGEN_USE_MDFILE_AS_MAINPAGE "${CMAKE_SOURCE_DIR}/README.md")
    set(DOXYGEN_FILE_PATTERNS *.cpp *.hpp *.md)
    set(DOXYGEN_EXCLUDE_PATTERNS
        */build/*
        */cmake/*
        */.*
        */CMakeFiles/*
    )

    # Additional options for better documentation quality
    set(DOXYGEN_EXTRACT_ALL NO)
    set(DOXYGEN_EXTRACT_PRIVATE NO)
    set(DOXYGEN_EXTRACT_STATIC YES)
    set(DOXYGEN_GENERATE_LATEX NO)
    set(DOXYGEN_WARN_IF_UNDOCUMENTED YES)
    set(DOXYGEN_WARN_IF_DOC_ERROR YES)
    set(DOXYGEN_WARN_NO_PARAMDOC YES)
    set(DOXYGEN_QUIET NO)
    set(DOXYGEN_WARN_AS_ERROR YES)

    # Source browsing
    set(DOXYGEN_SOURCE_BROWSER YES)
    set(DOXYGEN_INLINE_SOURCES NO)

    # Class diagrams and graphs
    set(DOXYGEN_HAVE_DOT YES)
    set(DOXYGEN_CLASS_DIAGRAMS YES)
    set(DOXYGEN_COLLABORATION_GRAPH YES)
    set(DOXYGEN_INCLUDE_GRAPH YES)
    set(DOXYGEN_INCLUDED_BY_GRAPH YES)
    set(DOXYGEN_CALL_GRAPH YES)
    set(DOXYGEN_CALLER_GRAPH YES)
    set(DOXYGEN_DOT_MULTI_TARGETS YES)

    # Collect source files for documentation
    file(GLOB_RECURSE HEADER_FILES "${CMAKE_SOURCE_DIR}/headers/*.hpp" "${CMAKE_SOURCE_DIR}/headers/*.h")

    # Create documentation using modern doxygen_add_docs function
    doxygen_add_docs(
        guillaume_docs
        ${HEADER_FILES}
        ${CMAKE_SOURCE_DIR}/README.md
        ALL
        USE_STAMP_FILE
        COMMENT "Generating API documentation with Doxygen"
    )
else()
    message(STATUS "Skipping docs...")
endif()

if(BUILD_EXAMPLES)
    add_subdirectory(examples)
else()
    message(STATUS "Skipping examples...")
endif()
