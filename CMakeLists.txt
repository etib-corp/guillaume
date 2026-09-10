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

# Enable CTest at the top level so tests are registered in the root
# CTestTestfile.cmake (enable_testing() in a subdirectory is not enough).
include(CTest)

# 01. Policy & standard setup

# 02. Options & configuration
option(BUILD_DOCS "Build Doxygen documentation" OFF)
option(BUILD_DOCS_ONLY "Configure only the documentation target" OFF)
option(BUILD_ALL_DOCS "Build documentation for all libraries" OFF)
option(BUILD_BENCHMARKS "Build the benchmark harness" OFF)

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
        SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party/sqlite3
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

    # Explicit source list (no file(GLOB)) for reproducible packaging.
    include(cmake/Sources.cmake)

    add_library(${PROJECT_NAME} ${GUILLAUME_SOURCES})

    # 05. Include directories (target-scoped, with build/install interfaces)
    target_include_directories(${PROJECT_NAME}
        PUBLIC
            $<BUILD_INTERFACE:${HEADERS_DIR}>
            $<INSTALL_INTERFACE:include>
            $<BUILD_INTERFACE:${TEMPLATES_DIR}>
            $<INSTALL_INTERFACE:include>
    )

    # 06. Link dependencies
    target_link_libraries(${PROJECT_NAME}
        PUBLIC
            utility
            SQLite::SQLite3
    )

    # 07. Packaging & consumability
    include(GNUInstallDirs)

    install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}Targets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )

    install(DIRECTORY ${HEADERS_DIR}/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
    install(DIRECTORY ${TEMPLATES_DIR}/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

    install(EXPORT ${PROJECT_NAME}Targets
        FILE ${PROJECT_NAME}Targets.cmake
        NAMESPACE ${PROJECT_NAME}::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
    )

    include(CMakePackageConfigHelpers)
    configure_package_config_file(
        cmake/${PROJECT_NAME}Config.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
    )
    write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion
    )
    install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
    )

    # 08. CPack packaging
    set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
    set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PROJECT_DESCRIPTION}")
    set(CPACK_PACKAGE_HOMEPAGE_URL "${PROJECT_HOMEPAGE_URL}")
    set(CPACK_PACKAGE_VENDOR "ETIB Corporation")
    set(CPACK_PACKAGE_CONTACT "clement-antoine.xavier@epitech.eu")
    set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
    set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
    include(CPack)
endif()

# 09. Documentations

if(BUILD_ALL_DOCS OR BUILD_DOCS_ONLY)
    message(STATUS "Building documentation...")
    include(cmake/Docs.cmake)
    add_subdirectory(docs)
else()
    message(STATUS "Skipping documentation...")
endif()

# 10. Tests

if(BUILD_TESTING)
    message(STATUS "Building tests...")
    add_subdirectory(tests)
else()
    message(STATUS "Skipping tests...")
endif()

# 11. Benchmarks

if(BUILD_BENCHMARKS)
    message(STATUS "Building benchmarks...")
    add_subdirectory(benchmarks)
endif()

# 12. Examples

add_subdirectory(examples)
