# Contributing to Guillaume

Thank you for your interest in contributing to Guillaume! This document provides guidelines and information for contributors.

## Table of Contents

- [Code of Conduct](#code-of-conduct)
- [Getting Started](#getting-started)
- [Development Workflow](#development-workflow)
- [Code Style](#code-style)
- [Testing](#testing)
- [Documentation](#documentation)
- [Pull Request Process](#pull-request-process)
- [Issue Reporting](#issue-reporting)

## Code of Conduct

We are committed to providing a welcoming and inspiring community for all. Please be respectful and professional in all interactions.

### Our Standards

- **Be Respectful**: Treat everyone with respect, regardless of background or experience level
- **Be Collaborative**: Work together constructively and help others learn
- **Be Inclusive**: Welcome newcomers and encourage diverse perspectives
- **Be Professional**: Keep discussions focused on technical matters

## Getting Started

### Prerequisites

Before contributing, ensure you have:

- C++20 compatible compiler (GCC 10+, Clang 10+, MSVC 2019+)
- CMake 3.10 or higher
- Git with SSH key configured
- Basic understanding of SDL3 and modern C++

### Setting Up Development Environment

1. **Fork the Repository**
   ```bash
   # Click "Fork" on GitHub, then clone your fork
   git clone git@github.com:YOUR_USERNAME/guillaume.git
   cd guillaume
   git remote add upstream git@github.com:etib-corp/guillaume.git
   ```

2. **Initialize Submodules**
   ```bash
   git submodule update --init --recursive
   ```

3. **Build in Debug Mode**
   ```bash
   cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
   cmake --build build
   ```

4. **Verify Installation**
   ```bash
   ./build/guillaume
   ```

## Development Workflow

### Branch Strategy

- `main`: Stable release branch
- `develop`: Integration branch for features
- `feature/`: Feature development branches
- `bugfix/`: Bug fix branches
- `hotfix/`: Critical fixes for production

### Working on Features

1. **Create Feature Branch**
   ```bash
   git checkout develop
   git pull upstream develop
   git checkout -b feature/your-feature-name
   ```

2. **Make Changes**
   - Write code following our style guidelines
   - Add tests for new functionality
   - Update documentation as needed

3. **Test Your Changes**
   ```bash
   cmake --build build
   ./build/guillaume
   # Run any additional tests
   ```

4. **Commit Changes**
   ```bash
   git add .
   git commit -m "feat: add new button component"
   ```

5. **Push and Create PR**
   ```bash
   git push origin feature/your-feature-name
   # Create PR on GitHub targeting 'develop' branch
   ```

## Code Style

Guillaume follows modern C++ best practices with specific style guidelines.

### General Guidelines

- **Use Modern C++20**: Leverage features like concepts, ranges, and modules where appropriate
- **RAII**: All resources should be managed through RAII
- **Smart Pointers**: Prefer `std::unique_ptr` and `std::shared_ptr` over raw pointers
- **Const Correctness**: Use `const` wherever possible
- **Exception Safety**: Ensure strong exception safety guarantees

### Naming Conventions

```cpp
// Classes: PascalCase
class ButtonComponent;

// Functions/Methods: snake_case
void handle_mouse_event();

// Variables: snake_case
int button_count;

// Constants: SCREAMING_SNAKE_CASE
const int MAX_BUTTONS = 100;

// Private members: leading underscore
class Button {
private:
    int _width;
    std::string _label;
};

// Namespaces: lowercase
namespace guigui {
    // ...
}
```

### File Organization

```cpp
// header.hpp
#pragma once

// System includes first
#include <memory>
#include <string>

// Third-party includes
#include <SDL3/SDL.h>

// Project includes
#include "guillaume/base_class.hpp"

namespace guigui {

class MyClass {
public:
    // Public types first
    using MyType = std::string;
    
    // Constructors/Destructor
    MyClass();
    virtual ~MyClass() = default;
    
    // Public methods
    void public_method();
    
private:
    // Private methods
    void _private_method();
    
    // Private data members
    std::string _data;
};

} // namespace guigui
```

### Documentation Standards

Use Doxygen-style comments for all public APIs:

```cpp
/**
 * @brief Brief description of the class
 * 
 * Detailed description of what the class does,
 * how to use it, and any important notes.
 * 
 * @example
 * @code
 * auto button = Button::create("Click me");
 * button->set_position({100, 100});
 * @endcode
 */
class Button : public Component {
public:
    /**
     * @brief Creates a new button instance
     * 
     * @param label The text label for the button
     * @param size Optional size specification
     * @return std::unique_ptr<Button> The created button
     * 
     * @throws std::invalid_argument If label is empty
     */
    static std::unique_ptr<Button> create(const std::string& label, 
                                        const Vector2& size = {});
    
    /**
     * @brief Sets the button's click handler
     * 
     * @param handler Function to call when button is clicked
     */
    void on_click(std::function<void()> handler);
};
```

## Testing

### Test Organization

Tests should be comprehensive and follow these guidelines:

1. **Unit Tests**: Test individual classes and functions
2. **Integration Tests**: Test component interactions
3. **Visual Tests**: Test rendering output (when applicable)

### Writing Tests

```cpp
// test_button.cpp
#include "guillaume/components/button.hpp"
#include <gtest/gtest.h>

class ButtonTest : public ::testing::Test {
protected:
    void SetUp() override {
        button = Button::create("Test");
    }
    
    std::unique_ptr<Button> button;
};

TEST_F(ButtonTest, CreatesWithCorrectLabel) {
    EXPECT_EQ(button->get_label(), "Test");
}

TEST_F(ButtonTest, HandlesClickEvent) {
    bool clicked = false;
    button->on_click([&clicked]() { clicked = true; });
    
    button->handle_mouse_event(MouseEvent::click({100, 100}));
    
    EXPECT_TRUE(clicked);
}
```

### Running Tests

```bash
# Build and run all tests
cmake --build build --target test

# Run specific test suite
./build/test_button

# Run with verbose output
./build/test_button --gtest_verbose
```

## Documentation

### API Documentation

- Use Doxygen comments for all public APIs
- Include usage examples in documentation
- Document preconditions and postconditions
- Specify exception behavior

### Generating Documentation

```bash
# Generate API documentation
doxygen Doxyfile

# View generated docs
open docs/html/index.html
```

### README Updates

When adding new features:

1. Update the feature list in README.md
2. Add usage examples if applicable
3. Update the API reference section
4. Add to the examples directory if needed

## Pull Request Process

### Before Submitting

- [ ] Code follows style guidelines
- [ ] All tests pass
- [ ] Documentation is updated
- [ ] Commit messages follow conventional format
- [ ] Branch is up to date with target branch

### PR Requirements

1. **Title**: Use conventional commit format
   - `feat: add new component`
   - `fix: resolve memory leak in renderer`
   - `docs: update API documentation`

2. **Description**: Include:
   - What changes were made
   - Why the changes were necessary
   - How to test the changes
   - Any breaking changes

3. **Reviewers**: Request review from appropriate maintainers

### Review Process

1. **Automated Checks**: CI builds and tests must pass
2. **Code Review**: At least one maintainer approval required
3. **Testing**: Manual testing may be required for UI changes
4. **Documentation**: Ensure docs are complete and accurate

## Issue Reporting

### Bug Reports

When reporting bugs, please include:

- **Environment**: OS, compiler version, CMake version
- **Steps to Reproduce**: Detailed steps to trigger the bug
- **Expected Behavior**: What should happen
- **Actual Behavior**: What actually happens
- **Code Sample**: Minimal example demonstrating the issue
- **Screenshots**: If applicable for visual bugs

### Feature Requests

For new features, please include:

- **Use Case**: Why is this feature needed?
- **Proposed API**: How should it work?
- **Alternatives**: What alternatives were considered?
- **Implementation**: Any thoughts on implementation approach?

### Issue Templates

Use the provided issue templates when available:

- Bug Report Template
- Feature Request Template
- Question Template

## Development Guidelines

### Performance Considerations

- **Minimize Allocations**: Reuse objects where possible
- **Cache Friendly**: Consider data locality in design
- **Lazy Evaluation**: Defer expensive operations until needed
- **Profiling**: Use profiling tools to identify bottlenecks

### Memory Management

- **RAII**: All resources managed through constructors/destructors
- **Smart Pointers**: Use appropriate smart pointer types
- **Avoid Leaks**: All allocated memory must be properly released
- **Exception Safety**: Ensure cleanup happens even with exceptions

### Platform Considerations

- **Cross-Platform**: Code must work on Windows, macOS, and Linux
- **Endianness**: Be aware of byte order differences
- **File Paths**: Use portable path handling
- **Dependencies**: Minimize platform-specific dependencies

## Release Process

Releases follow semantic versioning (MAJOR.MINOR.PATCH):

- **MAJOR**: Breaking API changes
- **MINOR**: New features, backward compatible
- **PATCH**: Bug fixes, backward compatible

### Release Checklist

- [ ] All tests pass
- [ ] Documentation is updated
- [ ] CHANGELOG.md is updated
- [ ] Version numbers are bumped
- [ ] Release notes are prepared
- [ ] Tags are created and pushed

## Getting Help

If you need help:

1. **Documentation**: Check the API documentation first
2. **Examples**: Look at the examples directory
3. **Issues**: Search existing issues for similar problems
4. **Discussions**: Use GitHub Discussions for questions
5. **Discord**: Join our community Discord server

## Recognition

Contributors will be recognized in:

- CONTRIBUTORS.md file
- Release notes
- GitHub contributor statistics
- Special mentions for significant contributions

Thank you for contributing to Guillaume! 🎉
