cmake_minimum_required(VERSION 3.10)

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
set(DOXYGEN_FILE_PATTERNS *.cpp *.hpp *.h *.md)
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
file(GLOB_RECURSE DOC_FILES "${CMAKE_SOURCE_DIR}/docs/*.md")

# Create documentation using modern doxygen_add_docs function
doxygen_add_docs(
    guillaume_docs
    ${HEADER_FILES}
    ${CMAKE_SOURCE_DIR}/README.md
    ${DOC_FILES}
    ${EXAMPLE_DOC_FILES}
    ALL
    USE_STAMP_FILE
    COMMENT "Generating API documentation with Doxygen"
)
