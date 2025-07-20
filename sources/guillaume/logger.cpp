#include "guillaume/logger.hpp"
#include <iostream>
#include <iomanip>

namespace guigui {

// Static member definitions
std::unique_ptr<Logger> Logger::_instance = nullptr;
std::mutex Logger::_instance_mutex;

Logger::Logger() 
    : _current_level(LogLevel::INFO)
    , _log_to_console(true)
    , _log_to_file(false)
    , _log_format("[{}] [{}] {}")
{
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
        case LogLevel::TRACE: return "TRACE";
        case LogLevel::DEBUG: return "DEBUG";
        case LogLevel::INFO:  return "INFO ";
        case LogLevel::WARN:  return "WARN ";
        case LogLevel::ERROR: return "ERROR";
        case LogLevel::FATAL: return "FATAL";
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
    
    // Format: [timestamp] [level] message
    std::string formatted_message = "[" + timestamp + "] [" + level_str + "] " + message;
    
    // Output to console
    if (_log_to_console) {
        if (level >= LogLevel::WARN) {
            std::cerr << formatted_message << std::endl;
        } else {
            std::cout << formatted_message << std::endl;
        }
    }
    
    // Output to file - remove const from file stream since we need to modify it
    if (_log_to_file && _file_stream.is_open()) {
        // Cast away const for file operations (this is safe since we're only modifying the file stream)
        auto& file_stream = const_cast<std::ofstream&>(_file_stream);
        file_stream << formatted_message << std::endl;
        file_stream.flush(); // Ensure immediate write
    }
}

void Logger::trace(const std::string& message) const {
    _log(LogLevel::TRACE, message);
}

void Logger::debug(const std::string& message) const {
    _log(LogLevel::DEBUG, message);
}

void Logger::info(const std::string& message) const {
    _log(LogLevel::INFO, message);
}

void Logger::warn(const std::string& message) const {
    _log(LogLevel::WARN, message);
}

void Logger::error(const std::string& message) const {
    _log(LogLevel::ERROR, message);
}

void Logger::fatal(const std::string& message) const {
    _log(LogLevel::FATAL, message);
}

} // namespace guigui
