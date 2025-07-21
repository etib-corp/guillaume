#include "guillaume/logger.hpp"
#include <iostream>
#include <iomanip>

namespace guigui {

// Static member definitions
std::unique_ptr<Logger> Logger::_instance = nullptr;
std::mutex Logger::_instance_mutex;

Logger::Logger() 
    : _current_level(LogLevel::INFO_LEVEL)
    , _log_to_console(true)
    , _log_to_file(false)
    , _log_format("[{}] [{}] {}")
    , _color_enabled(true)  // Enable colors by default
{
    // Enable Windows console colors if on Windows
    _enable_windows_colors();
}

Logger::~Logger() {
    if (_file_stream.is_open()) {
        _file_stream.close();
    }
}

Logger& Logger::get_instance() {
    std::lock_guard<std::mutex> lock(_instance_mutex);
    if (_instance == nullptr) {
        _instance = std::unique_ptr<Logger>(new Logger());
    }
    return *_instance;
}

void Logger::set_log_level(LogLevel level) {
    std::lock_guard<std::mutex> lock(_log_mutex);
    _current_level = level;
}

LogLevel Logger::get_log_level() const {
    std::lock_guard<std::mutex> lock(_log_mutex);
    return _current_level;
}

void Logger::set_console_logging(bool enable) {
    std::lock_guard<std::mutex> lock(_log_mutex);
    _log_to_console = enable;
}

void Logger::set_file_logging(bool enable, const std::string& filename) {
    std::lock_guard<std::mutex> lock(_log_mutex);
    
    if (_file_stream.is_open()) {
        _file_stream.close();
    }
    
    _log_to_file = enable;
    
    if (enable) {
        _file_stream.open(filename, std::ios::app);
        if (!_file_stream.is_open()) {
            std::cerr << "Warning: Could not open log file: " << filename << std::endl;
            _log_to_file = false;
        }
    }
}

bool Logger::is_console_logging_enabled() const {
    std::lock_guard<std::mutex> lock(_log_mutex);
    return _log_to_console;
}

bool Logger::is_file_logging_enabled() const {
    std::lock_guard<std::mutex> lock(_log_mutex);
    return _log_to_file;
}

void Logger::set_color_output(bool enable) {
    std::lock_guard<std::mutex> lock(_log_mutex);
    _color_enabled = enable;
}

bool Logger::is_color_output_enabled() const {
    std::lock_guard<std::mutex> lock(_log_mutex);
    return _color_enabled;
}

bool Logger::_is_color_supported(std::ostream& stream) const {
    if (!_color_enabled) {
        return false;
    }

#ifdef _WIN32
    // On Windows, check if we have a console handle
    if (&stream == &std::cout) {
        return _isatty(_fileno(stdout));
    } else if (&stream == &std::cerr) {
        return _isatty(_fileno(stderr));
    }
    return false;
#else
    // On Unix-like systems (Linux, macOS), check if output is a terminal
    if (&stream == &std::cout) {
        return isatty(STDOUT_FILENO);
    } else if (&stream == &std::cerr) {
        return isatty(STDERR_FILENO);
    }
    return false;
#endif
}

std::string Logger::_get_color_code(LogLevel level) const {
    if (!_color_enabled) {
        return "";
    }

    switch (level) {
        case LogLevel::TRACE_LEVEL: return "\033[37m";      // White
        case LogLevel::DEBUG_LEVEL: return "\033[36m";      // Cyan
        case LogLevel::INFO_LEVEL:  return "\033[32m";      // Green
        case LogLevel::WARN_LEVEL:  return "\033[33m";      // Yellow
        case LogLevel::ERROR_LEVEL: return "\033[31m";      // Red
        case LogLevel::FATAL_LEVEL: return "\033[35m\033[1m"; // Magenta + Bold
        default: return "";
    }
}

std::string Logger::_get_reset_code() const {
    if (!_color_enabled) {
        return "";
    }
    return "\033[0m";  // Reset all formatting
}

void Logger::_enable_windows_colors() const {
#ifdef _WIN32
    // Enable ANSI escape sequence processing on Windows 10+
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    HANDLE hErr = GetStdHandle(STD_ERROR_HANDLE);
    
    if (hOut != INVALID_HANDLE_VALUE) {
        DWORD dwMode = 0;
        if (GetConsoleMode(hOut, &dwMode)) {
            dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
            SetConsoleMode(hOut, dwMode);
        }
    }
    
    if (hErr != INVALID_HANDLE_VALUE) {
        DWORD dwMode = 0;
        if (GetConsoleMode(hErr, &dwMode)) {
            dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
            SetConsoleMode(hErr, dwMode);
        }
    }
#endif
}

std::string Logger::_get_timestamp() const {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()) % 1000;
    
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
    ss << '.' << std::setfill('0') << std::setw(3) << ms.count();
    
    return ss.str();
}

std::string Logger::_level_to_string(LogLevel level) const {
    switch (level) {
        case LogLevel::TRACE_LEVEL: return "TRACE";
        case LogLevel::DEBUG_LEVEL: return "DEBUG";
        case LogLevel::INFO_LEVEL:  return "INFO ";
        case LogLevel::WARN_LEVEL:  return "WARN ";
        case LogLevel::ERROR_LEVEL: return "ERROR";
        case LogLevel::FATAL_LEVEL: return "FATAL";
        default: return "UNKNOWN";
    }
}

void Logger::_log(LogLevel level, const std::string& message) const {
    if (level < _current_level) {
        return;
    }
    
    std::lock_guard<std::mutex> lock(_log_mutex);
    
    std::string timestamp = _get_timestamp();
    std::string level_str = _level_to_string(level);
    
    // Format for file output (no colors): [timestamp] [level] message
    std::string file_message = "[" + timestamp + "] [" + level_str + "] " + message;
    
    // Output to console with colors
    if (_log_to_console) {
        std::ostream& output_stream = (level >= LogLevel::WARN_LEVEL) ? std::cerr : std::cout;
        
        if (_is_color_supported(output_stream)) {
            // Colored console output
            std::string color_code = _get_color_code(level);
            std::string reset_code = _get_reset_code();
            
            output_stream << "[" << timestamp << "] " 
                         << color_code << "[" << level_str << "]" << reset_code 
                         << " " << message << std::endl;
        } else {
            // Fallback to non-colored output
            output_stream << file_message << std::endl;
        }
    }
    
    // Output to file (always without colors)
    if (_log_to_file && _file_stream.is_open()) {
        // Cast away const for file operations (this is safe since we're only modifying the file stream)
        auto& file_stream = const_cast<std::ofstream&>(_file_stream);
        file_stream << file_message << std::endl;
        file_stream.flush(); // Ensure immediate write
    }
}

void Logger::trace(const std::string& message) const {
    _log(LogLevel::TRACE_LEVEL, message);
}

void Logger::debug(const std::string& message) const {
    _log(LogLevel::DEBUG_LEVEL, message);
}

void Logger::info(const std::string& message) const {
    _log(LogLevel::INFO_LEVEL, message);
}

void Logger::warn(const std::string& message) const {
    _log(LogLevel::WARN_LEVEL, message);
}

void Logger::error(const std::string& message) const {
    _log(LogLevel::ERROR_LEVEL, message);
}

void Logger::fatal(const std::string& message) const {
    _log(LogLevel::FATAL_LEVEL, message);
}

} // namespace guigui
