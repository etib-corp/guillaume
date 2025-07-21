#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <memory>
#include <mutex>
#include <chrono>
#include <iomanip>

#ifdef _WIN32
    #include <windows.h>
    #include <io.h>
#else
    #include <unistd.h>
#endif

namespace guigui {

/**
 * @brief Log level enumeration to categorize log messages by severity
 */
enum class LogLevel {
    TRACE_LEVEL = 0,
    DEBUG_LEVEL = 1,
    INFO_LEVEL = 2,
    WARN_LEVEL = 3,
    ERROR_LEVEL = 4,
    FATAL_LEVEL = 5
};

/**
 * @brief Logger class providing thread-safe logging functionality
 * 
 * This logger supports multiple log levels, console and file output,
 * and automatic timestamp formatting. It follows the singleton pattern
 * to ensure a single logging instance throughout the application.
 */
class Logger {
private:
    static std::unique_ptr<Logger> _instance;
    static std::mutex _instance_mutex;
    
    mutable std::mutex _log_mutex;
    LogLevel _current_level;
    std::ofstream _file_stream;
    bool _log_to_console;
    bool _log_to_file;
    std::string _log_format;
    bool _color_enabled;

    /**
     * @brief Private constructor for singleton pattern
     */
    Logger();

    /**
     * @brief Check if the output stream supports color (is a terminal)
     * @param stream The output stream to check
     * @return True if colors are supported
     */
    bool _is_color_supported(std::ostream& stream) const;

    /**
     * @brief Get ANSI color code for log level
     * @param level The log level
     * @return ANSI color escape sequence
     */
    std::string _get_color_code(LogLevel level) const;

    /**
     * @brief Get color reset sequence
     * @return ANSI reset escape sequence
     */
    std::string _get_reset_code() const;

    /**
     * @brief Enable Windows console color support if on Windows
     */
    void _enable_windows_colors() const;

    /**
     * @brief Get current timestamp as formatted string
     * @return Formatted timestamp string
     */
    std::string _get_timestamp() const;

    /**
     * @brief Convert log level to string representation
     * @param level The log level to convert
     * @return String representation of the log level
     */
    std::string _level_to_string(LogLevel level) const;

    /**
     * @brief Internal logging method that handles the actual output
     * @param level Log level of the message
     * @param message The message to log
     */
    void _log(LogLevel level, const std::string& message) const;

public:
    /**
     * @brief Destructor - ensures file stream is properly closed
     */
    ~Logger();

    // Delete copy constructor and assignment operator for singleton
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    /**
     * @brief Get the singleton instance of the logger
     * @return Reference to the logger instance
     */
    static Logger& get_instance();

    /**
     * @brief Set the minimum log level for filtering messages
     * @param level The minimum log level to display
     */
    void set_log_level(LogLevel level);

    /**
     * @brief Get the current minimum log level
     * @return The current minimum log level
     */
    LogLevel get_log_level() const;

    /**
     * @brief Enable or disable console logging
     * @param enable True to enable console logging, false to disable
     */
    void set_console_logging(bool enable);

    /**
     * @brief Enable or disable file logging
     * @param enable True to enable file logging, false to disable
     * @param filename Optional filename for log file (default: "guillaume.log")
     */
    void set_file_logging(bool enable, const std::string& filename = "guillaume.log");

    /**
     * @brief Check if console logging is enabled
     * @return True if console logging is enabled
     */
    bool is_console_logging_enabled() const;

    /**
     * @brief Check if file logging is enabled
     * @return True if file logging is enabled
     */
    bool is_file_logging_enabled() const;

    /**
     * @brief Enable or disable colored console output
     * @param enable True to enable colors, false to disable
     */
    void set_color_output(bool enable);

    /**
     * @brief Check if colored output is enabled
     * @return True if colored output is enabled
     */
    bool is_color_output_enabled() const;

    /**
     * @brief Log a trace message
     * @param message The message to log
     */
    void trace(const std::string& message) const;

    /**
     * @brief Log a debug message
     * @param message The message to log
     */
    void debug(const std::string& message) const;

    /**
     * @brief Log an info message
     * @param message The message to log
     */
    void info(const std::string& message) const;

    /**
     * @brief Log a warning message
     * @param message The message to log
     */
    void warn(const std::string& message) const;

    /**
     * @brief Log an error message
     * @param message The message to log
     */
    void error(const std::string& message) const;

    /**
     * @brief Log a fatal message
     * @param message The message to log
     */
    void fatal(const std::string& message) const;

    /**
     * @brief Template method for logging with formatted strings
     * @tparam Args Variadic template for format arguments
     * @param level Log level for the message
     * @param format Format string (printf-style)
     * @param args Arguments for the format string
     */
    template<typename... Args>
    void log(LogLevel level, const std::string& format, Args&&... args) const {
        if (level < _current_level) {
            return;
        }

        // Use stringstream for safe string formatting
        std::ostringstream oss;
        _format_message(oss, format, std::forward<Args>(args)...);
        _log(level, oss.str());
    }

    /**
     * @brief Template method for trace logging with formatted strings
     */
    template<typename... Args>
    void trace(const std::string& format, Args&&... args) const {
        log(LogLevel::TRACE_LEVEL, format, std::forward<Args>(args)...);
    }

    /**
     * @brief Template method for debug logging with formatted strings
     */
    template<typename... Args>
    void debug(const std::string& format, Args&&... args) const {
        log(LogLevel::DEBUG_LEVEL, format, std::forward<Args>(args)...);
    }

    /**
     * @brief Template method for info logging with formatted strings
     */
    template<typename... Args>
    void info(const std::string& format, Args&&... args) const {
        log(LogLevel::INFO_LEVEL, format, std::forward<Args>(args)...);
    }

    /**
     * @brief Template method for warning logging with formatted strings
     */
    template<typename... Args>
    void warn(const std::string& format, Args&&... args) const {
        log(LogLevel::WARN_LEVEL, format, std::forward<Args>(args)...);
    }

    /**
     * @brief Template method for error logging with formatted strings
     */
    template<typename... Args>
    void error(const std::string& format, Args&&... args) const {
        log(LogLevel::ERROR_LEVEL, format, std::forward<Args>(args)...);
    }

    /**
     * @brief Template method for fatal logging with formatted strings
     */
    template<typename... Args>
    void fatal(const std::string& format, Args&&... args) const {
        log(LogLevel::FATAL_LEVEL, format, std::forward<Args>(args)...);
    }

private:
    /**
     * @brief Recursive template function to format messages safely
     */
    template<typename T>
    void _format_message(std::ostringstream& oss, const std::string& format, T&& value) const {
        size_t pos = format.find("{}");
        if (pos != std::string::npos) {
            oss << format.substr(0, pos) << std::forward<T>(value) << format.substr(pos + 2);
        } else {
            oss << format;
        }
    }

    template<typename T, typename... Args>
    void _format_message(std::ostringstream& oss, const std::string& format, T&& value, Args&&... args) const {
        size_t pos = format.find("{}");
        if (pos != std::string::npos) {
            oss << format.substr(0, pos) << std::forward<T>(value);
            _format_message(oss, format.substr(pos + 2), std::forward<Args>(args)...);
        } else {
            oss << format;
        }
    }
};

} // namespace guigui

// Convenience macros for easier logging
#define LOG_TRACE(msg) guigui::Logger::get_instance().trace(msg)
#define LOG_DEBUG(msg) guigui::Logger::get_instance().debug(msg)
#define LOG_INFO(msg) guigui::Logger::get_instance().info(msg)
#define LOG_WARN(msg) guigui::Logger::get_instance().warn(msg)
#define LOG_ERROR(msg) guigui::Logger::get_instance().error(msg)
#define LOG_FATAL(msg) guigui::Logger::get_instance().fatal(msg)

// Formatted logging macros
#define LOG_TRACE_F(fmt, ...) guigui::Logger::get_instance().trace(fmt, __VA_ARGS__)
#define LOG_DEBUG_F(fmt, ...) guigui::Logger::get_instance().debug(fmt, __VA_ARGS__)
#define LOG_INFO_F(fmt, ...) guigui::Logger::get_instance().info(fmt, __VA_ARGS__)
#define LOG_WARN_F(fmt, ...) guigui::Logger::get_instance().warn(fmt, __VA_ARGS__)
#define LOG_ERROR_F(fmt, ...) guigui::Logger::get_instance().error(fmt, __VA_ARGS__)
#define LOG_FATAL_F(fmt, ...) guigui::Logger::get_instance().fatal(fmt, __VA_ARGS__)

