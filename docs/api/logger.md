# Logger API Documentation

The Guillaume Logger provides thread-safe logging functionality with support for multiple log levels, console and file output, and colored terminal output.

## Features

- **Thread-safe**: Multiple threads can safely log simultaneously
- **Multiple log levels**: TRACE, DEBUG, INFO, WARN, ERROR, FATAL
- **Colored output**: Automatic color support for Linux, Windows, and macOS terminals
- **File logging**: Optional file output without color codes
- **Singleton pattern**: Single logger instance throughout the application
- **Formatted messages**: Support for template-based message formatting

## Basic Usage

```cpp
#include "guillaume/logger.hpp"

using namespace guigui;

int main() {
    Logger& logger = Logger::get_instance();
    
    // Simple logging
    logger.info("Application started");
    logger.warn("This is a warning");
    logger.error("An error occurred");
    
    return 0;
}
```

## Log Levels

The logger supports six log levels in increasing order of severity:

| Level | Color | Description |
|-------|-------|-------------|
| TRACE | White | Detailed execution information |
| DEBUG | Cyan | Diagnostic information for debugging |
| INFO | Green | General information about program execution |
| WARN | Yellow | Warning about potential issues |
| ERROR | Red | Error occurred but program can continue |
| FATAL | Magenta (Bold) | Critical error, program may terminate |

## Color Support

Colors are automatically enabled by default and work on:

- **Linux**: ANSI escape sequences in terminals
- **Windows**: ANSI escape sequences (Windows 10+) with automatic console configuration
- **macOS**: ANSI escape sequences in Terminal.app and other compatible terminals

### Controlling Colors

```cpp
Logger& logger = Logger::get_instance();

// Check if colors are enabled
bool has_colors = logger.is_color_output_enabled();

// Disable colors
logger.set_color_output(false);

// Re-enable colors
logger.set_color_output(true);
```

Colors are automatically disabled when:

- Output is redirected to a file
- Terminal doesn't support colors
- Color output is explicitly disabled

## Configuration

### Log Level

```cpp
Logger& logger = Logger::get_instance();

// Set minimum log level (filters out lower levels)
logger.set_log_level(LogLevel::WARN);  // Only WARN, ERROR, FATAL will be shown

// Get current log level
LogLevel current_level = logger.get_log_level();
```

### Console Logging

```cpp
Logger& logger = Logger::get_instance();

// Disable console output
logger.set_console_logging(false);

// Re-enable console output
logger.set_console_logging(true);

// Check if console logging is enabled
bool console_enabled = logger.is_console_logging_enabled();
```

### File Logging

```cpp
Logger& logger = Logger::get_instance();

// Enable file logging
logger.set_file_logging(true, "app.log");

// Disable file logging
logger.set_file_logging(false);

// Check if file logging is enabled
bool file_enabled = logger.is_file_logging_enabled();
```

**Note**: File output never contains color codes, ensuring clean log files.

## Formatted Logging

The logger supports template-based message formatting:

```cpp
Logger& logger = Logger::get_instance();

// Using template methods
logger.info("User {} logged in with ID {}", username, user_id);
logger.error("Failed to process {} items out of {}", failed_count, total_count);

// Using the generic log method
logger.log(LogLevel::DEBUG, "Processing file: {}", filename);
```

## Convenience Macros

For easier logging, use the provided macros:

```cpp
// Simple logging
LOG_INFO("Application started");
LOG_WARN("Low memory warning");
LOG_ERROR("Database connection failed");

// Formatted logging
LOG_INFO_F("Processing {} files", file_count);
LOG_ERROR_F("Error code: {} - {}", error_code, error_message);
```

Available macros:

- `LOG_TRACE(msg)`, `LOG_TRACE_F(fmt, ...)`
- `LOG_DEBUG(msg)`, `LOG_DEBUG_F(fmt, ...)`
- `LOG_INFO(msg)`, `LOG_INFO_F(fmt, ...)`
- `LOG_WARN(msg)`, `LOG_WARN_F(fmt, ...)`
- `LOG_ERROR(msg)`, `LOG_ERROR_F(fmt, ...)`
- `LOG_FATAL(msg)`, `LOG_FATAL_F(fmt, ...)`

## Output Examples

### Console Output (with colors)

```text
[2025-07-21 11:16:14.742] [INFO ] Application started successfully
[2025-07-21 11:16:14.743] [WARN ] Low disk space detected
[2025-07-21 11:16:14.744] [ERROR] Failed to save configuration
```

### File Output (plain text)

```text
[2025-07-21 11:16:14.742] [INFO ] Application started successfully
[2025-07-21 11:16:14.743] [WARN ] Low disk space detected
[2025-07-21 11:16:14.744] [ERROR] Failed to save configuration
```

## Thread Safety

The logger is fully thread-safe. Multiple threads can log simultaneously without external synchronization:

```cpp
// Safe to use from multiple threads
std::thread t1([]() { LOG_INFO("Thread 1 message"); });
std::thread t2([]() { LOG_INFO("Thread 2 message"); });
```

## Best Practices

1. **Use appropriate log levels**: Reserve FATAL for critical errors, ERROR for recoverable errors, WARN for potential issues, INFO for general information, DEBUG for development, and TRACE for detailed execution flow.

2. **Enable file logging for production**: Keep console logging for development and file logging for production environments.

3. **Use formatted logging**: Prefer `logger.info("Value: {}", value)` over `logger.info("Value: " + std::to_string(value))` for better performance.

4. **Set appropriate log levels**: In production, consider setting the log level to INFO or WARN to reduce output volume.

5. **Consider color preferences**: Colors improve readability in development but can be disabled for scripting or environments that don't support them.
