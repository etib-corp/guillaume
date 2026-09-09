# Set test directories
set(TEST_HEADERS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/headers")

# Explicit test source list (no file(GLOB)) for reproducible builds.
set(TEST_SOURCES
    sources/components/test_bound.cpp
    sources/components/test_focus.cpp
    sources/components/test_text.cpp
    sources/components/test_transform.cpp
    sources/ecs/test_component.cpp
    sources/ecs/test_component_registry.cpp
    sources/ecs/test_component_registry_filler.cpp
    sources/ecs/test_entity.cpp
    sources/ecs/test_entity_filler.cpp
    sources/ecs/test_entity_registry_container.cpp
    sources/ecs/test_system.cpp
    sources/ecs/test_system_filler.cpp
    sources/ecs/test_system_registry.cpp
    sources/entities/test_button.cpp
    sources/entities/test_segmented_button.cpp
    sources/entities/test_extended_floating_action_button.cpp
    sources/entities/test_floating_action_button.cpp
    sources/entities/test_floating_action_button_menu.cpp
    sources/entities/test_icon_button.cpp
    sources/entities/test_split_button.cpp
    sources/entities/test_standard_button_group.cpp
    sources/event/test_event_bus.cpp
    sources/event/test_event_handler.cpp
    sources/event/test_event_subscriber.cpp
    sources/systems/test_focus.cpp
    sources/systems/test_interaction.cpp
    sources/systems/test_keyboard_control.cpp
    sources/systems/test_measure_text.cpp
    sources/systems/test_text_input.cpp
    sources/systems/test_text_render.cpp
    sources/systems/text_glyph_render.cpp
    sources/test_application.cpp
    sources/test_ecs.cpp
    sources/test_engine.cpp
    sources/test_metadata.cpp
    sources/test_storage.cpp
)

# Fetch GoogleTest
include(FetchContent)
FetchContent_Declare(
    googletest
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../third_party/googletest
)
# Prevent GoogleTest from overriding our compiler/linker settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Create test executable
add_executable(test_${PROJECT_NAME} ${TEST_SOURCES})

# Configure include directories
target_include_directories(test_${PROJECT_NAME} PRIVATE
    ${TEST_HEADERS_DIR}
)

# Link libraries
target_link_libraries(test_${PROJECT_NAME} PRIVATE
    guillaume
    gtest_main
)

# Discover and register tests
include(GoogleTest)
gtest_discover_tests(test_${PROJECT_NAME}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
