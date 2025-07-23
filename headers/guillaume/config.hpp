/**
 * @file config.hpp
 * @brief Configuration system for the Guillaume GUI framework
 */

#pragma once

#include <cstdio>
#include <iostream>
#include <string>

namespace guigui {

/**
 * @brief Configuration class for Guillaume framework settings
 *
 * This class provides centralized configuration for various framework settings
 * including window mode, debugging options, and rendering preferences.
 */
class Config {
public:
    /**
     * @brief Window display modes
     */
    enum class WindowMode {
        WINDOWED, ///< Windowed mode with decorations
        FULLSCREEN ///< Fullscreen mode
    };

private:
    // Window configuration
    WindowMode _window_mode = WindowMode::WINDOWED; ///< Default to windowed mode
    int _window_width = 1152; ///< Default window width (80% of 1440)
    int _window_height = 720; ///< Default window height (80% of 900)
    bool _window_resizable = true; ///< Whether window is resizable

    // Debug configuration
    bool _debug_mouse_coordinates = false; ///< Enable mouse coordinate debugging

    static Config* _instance; ///< Singleton instance

    Config() = default;

public:
    /**
     * @brief Get the singleton instance
     * @return Reference to the Config instance
     */
    static Config& get_instance()
    {
        if (!_instance) {
            _instance = new Config();
        }
        return *_instance;
    }

    // Window configuration getters/setters
    WindowMode get_window_mode() const { return _window_mode; }
    void set_window_mode(WindowMode mode) { _window_mode = mode; }

    int get_window_width() const { return _window_width; }
    void set_window_width(int width) { _window_width = width; }

    int get_window_height() const { return _window_height; }
    void set_window_height(int height) { _window_height = height; }

    bool is_window_resizable() const { return _window_resizable; }
    void set_window_resizable(bool resizable) { _window_resizable = resizable; }

    // Debug configuration getters/setters
    bool is_debug_mouse_coordinates() const { return _debug_mouse_coordinates; }
    void set_debug_mouse_coordinates(bool debug) { _debug_mouse_coordinates = debug; }

    /**
     * @brief Parse command line arguments
     * @param argc Number of arguments
     * @param argv Array of argument strings
     * @return true if parsing was successful, false if help was shown or error occurred
     */
    bool parse_command_line(int argc, char* const argv[])
    {
        for (int i = 1; i < argc; ++i) {
            std::string arg = argv[i];

            if (arg == "--fullscreen" || arg == "-f") {
                _window_mode = WindowMode::FULLSCREEN;
            } else if (arg == "--windowed" || arg == "-w") {
                _window_mode = WindowMode::WINDOWED;
            } else if (arg == "--debug-mouse" || arg == "-dm") {
                _debug_mouse_coordinates = true;
            } else if (arg == "--help" || arg == "-h") {
                print_help(argv[0]);
                return false;
            } else {
                std::cerr << "Unknown argument: " << arg << std::endl;
                print_help(argv[0]);
                return false;
            }
        }
        return true;
    }

    /**
     * @brief Print help information
     * @param program_name Name of the program executable
     */
    void print_help(const char* program_name = "guillaume")
    {
        printf("Usage: %s [options]\n", program_name);
        printf("Guillaume GUI Framework - Command Line Options:\n");
        printf("  --fullscreen, -f     Run in fullscreen mode\n");
        printf("  --windowed, -w       Run in windowed mode (default)\n");
        printf("  --debug-mouse, -dm   Enable mouse coordinate debugging\n");
        printf("  --help, -h           Show this help message\n");
    }
};

} // namespace guigui
