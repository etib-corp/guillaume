# Development Setup Guide

This guide will help you set up a development environment for Guillaume on different platforms.

## Table of Contents

- [Prerequisites](#prerequisites)
- [Platform-Specific Setup](#platform-specific-setup)
  - [macOS](#macos)
  - [Linux](#linux)  
  - [Windows](#windows)
- [IDE Configuration](#ide-configuration)
- [Building the Project](#building-the-project)
- [Running Tests](#running-tests)
- [Development Tools](#development-tools)
- [Troubleshooting](#troubleshooting)

## Prerequisites

Before setting up Guillaume, ensure you have:

- **C++20 Compatible Compiler**
  - GCC 10+ (Linux/macOS)
  - Clang 10+ (macOS/Linux)
  - MSVC 2019+ (Windows)
- **CMake 3.10+**
- **Git** with submodule support
- **Python 3.7+** (for build scripts)

## Platform-Specific Setup

### macOS

#### Install Development Tools

1. **Install Xcode Command Line Tools:**

   ```bash
   xcode-select --install
   ```

2. **Install Homebrew** (if not already installed):

   ```bash
   /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
   ```

3. **Install Required Packages:**

   ```bash
   brew install cmake git python3
   
   # Optional: Install newer GCC if needed
   brew install gcc
   ```

#### SDL3 Dependencies

SDL3 is included as a submodule, but you may want to install system dependencies:

```bash
# Install audio/video libraries (optional)
brew install libvorbis libpng jpeg freetype
```

### Linux

#### Ubuntu/Debian

1. **Update Package Lists:**

   ```bash
   sudo apt update
   ```

2. **Install Build Tools:**

   ```bash
   sudo apt install build-essential cmake git python3 python3-pip
   ```

3. **Install C++20 Compiler:**

   ```bash
   # GCC 10+ (recommended)
   sudo apt install gcc-10 g++-10
   
   # Or Clang 10+
   sudo apt install clang-10
   ```

4. **Install Development Libraries:**

   ```bash
   sudo apt install \
       libxrandr-dev \
       libxinerama-dev \
       libxcursor-dev \
       libxi-dev \
       libxext-dev \
       libwayland-dev \
       libxkbcommon-dev \
       libegl1-mesa-dev \
       libgles2-mesa-dev \
       libgl1-mesa-dev \
       libglu1-mesa-dev
   ```

#### Fedora/RHEL

1. **Install Build Tools:**

   ```bash
   sudo dnf groupinstall "Development Tools"
   sudo dnf install cmake git python3 python3-pip
   ```

2. **Install Dependencies:**

   ```bash
   sudo dnf install \
       libXrandr-devel \
       libXinerama-devel \
       libXcursor-devel \
       libXi-devel \
       libXext-devel \
       wayland-devel \
       libxkbcommon-devel \
       mesa-libEGL-devel \
       mesa-libGLES-devel \
       mesa-libGL-devel \
       mesa-libGLU-devel
   ```

### Windows

#### Visual Studio

1. **Install Visual Studio 2019 or later** with:
   - C++ development workload
   - CMake tools
   - Git for Windows

2. **Verify Installation:**

   ```cmd
   cl
   cmake --version
   git --version
   ```

#### MSYS2/MinGW-w64

1. **Install MSYS2** from [https://www.msys2.org/](https://www.msys2.org/)

2. **Install Build Tools:**

   ```bash
   pacman -S mingw-w64-x86_64-toolchain
   pacman -S mingw-w64-x86_64-cmake
   pacman -S git python3
   ```

3. **Add to PATH:**
   - Add `C:\msys64\mingw64\bin` to your system PATH

## IDE Configuration

### Visual Studio Code

1. **Install Extensions:**
   - C/C++ (Microsoft)
   - CMake Tools (Microsoft)
   - GitLens (GitKraken)
   - Doxygen Documentation Generator

2. **Configure CMake:**

   Create `.vscode/settings.json`:

   ```json
   {
       "cmake.configureArgs": [
           "-DCMAKE_BUILD_TYPE=Debug"
       ],
       "cmake.buildArgs": [
           "--parallel 4"
       ],
       "C_Cpp.default.configurationProvider": "ms-vscode.cmake-tools",
       "C_Cpp.default.cppStandard": "c++20"
   }
   ```

3. **Configure Tasks:**

   Create `.vscode/tasks.json`:

   ```json
   {
       "version": "2.0.0",
       "tasks": [
           {
               "label": "Build Guillaume",
               "type": "shell",
               "command": "cmake",
               "args": ["--build", "build"],
               "group": {
                   "kind": "build",
                   "isDefault": true
               },
               "problemMatcher": "$gcc"
           },
           {
               "label": "Run Guillaume",
               "type": "shell",
               "command": "./build/guillaume",
               "group": "test",
               "dependsOn": "Build Guillaume"
           }
       ]
   }
   ```

### CLion

1. **Open Project:**
   - File → Open → Select Guillaume directory

2. **Configure CMake:**
   - File → Settings → Build, Execution, Deployment → CMake
   - Set Build Type to Debug or Release
   - Add CMake options if needed

3. **Configure Code Style:**
   - File → Settings → Editor → Code Style → C/C++
   - Import Guillaume code style settings

### Qt Creator

1. **Open CMake Project:**
   - File → Open File or Project → Select CMakeLists.txt

2. **Configure Build:**
   - Select appropriate kit (compiler)
   - Set build directory to `build`

## Building the Project

### Clone Repository

```bash
git clone https://github.com/etib-corp/guillaume.git --recursive
cd guillaume
```

### Configure Build

```bash
# Debug build
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug

# Release build  
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release

# With additional options
cmake -S . -B build \
    -DCMAKE_BUILD_TYPE=Debug \
    -DSDLTTF_HARFBUZZ=OFF \
    -DSDLTTF_FREETYPE=ON
```

### Build

```bash
# Build project
cmake --build build

# Build with multiple cores
cmake --build build --parallel 4

# Build specific target
cmake --build build --target guillaume
```

### Install (Optional)

```bash
# Install to system directories
sudo cmake --install build

# Install to custom directory
cmake --install build --prefix /usr/local/guillaume
```

## Running Tests

### Basic Test Run

```bash
# Run main executable
./build/guillaume

# Run with debug output
GUILLAUME_LOG_LEVEL=DEBUG ./build/guillaume
```

### Memory Testing

```bash
# Valgrind (Linux/macOS)
valgrind --leak-check=full ./build/guillaume

# AddressSanitizer
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_FLAGS="-fsanitize=address"
cmake --build build
./build/guillaume
```

### Performance Testing

```bash
# Profile with gprof
cmake -S . -B build -DCMAKE_CXX_FLAGS="-pg"
cmake --build build
./build/guillaume
gprof ./build/guillaume gmon.out > profile.txt
```

## Development Tools

### Code Formatting

Guillaume uses clang-format for consistent code style:

```bash
# Format all source files
find headers sources -name "*.hpp" -o -name "*.cpp" | xargs clang-format -i

# Check formatting
find headers sources -name "*.hpp" -o -name "*.cpp" | xargs clang-format --dry-run -Werror
```

### Static Analysis

```bash
# Clang Static Analyzer
scan-build cmake --build build

# Cppcheck
cppcheck --enable=all --std=c++20 headers/ sources/

# Include what you use
iwyu_tool.py -p build
```

### Documentation Generation

```bash
# Generate Doxygen docs
doxygen Doxyfile

# View generated documentation
open docs/html/index.html
```

### Git Hooks

Set up pre-commit hooks for quality assurance:

```bash
# Install pre-commit
pip3 install pre-commit

# Install hooks
pre-commit install

# Run manually
pre-commit run --all-files
```

## Environment Variables

Guillaume recognizes these environment variables:

```bash
# Logging level (DEBUG, INFO, WARN, ERROR)
export GUILLAUME_LOG_LEVEL=DEBUG

# Asset directory
export GUILLAUME_ASSETS_DIR=/path/to/assets

# Force software rendering
export GUILLAUME_SOFTWARE_RENDER=1
```

## Troubleshooting

### Common Build Issues

#### CMake Configuration Fails

```bash
# Clear CMake cache
rm -rf build/CMakeCache.txt build/CMakeFiles

# Regenerate
cmake -S . -B build
```

#### Submodule Issues

```bash
# Update submodules
git submodule update --init --recursive

# Force clean and update
git submodule foreach --recursive git clean -ffd
git submodule update --init --recursive
```

#### Compiler Issues

```bash
# Specify compiler explicitly
cmake -S . -B build -DCMAKE_CXX_COMPILER=g++-10

# Check compiler support
echo '#include <version>' | g++ -x c++ -std=c++20 -E - > /dev/null 2>&1 && echo "C++20 OK"
```

### Runtime Issues

#### Missing Libraries

```bash
# Check dynamic library dependencies
ldd ./build/guillaume  # Linux
otool -L ./build/guillaume  # macOS

# Set library path if needed
export LD_LIBRARY_PATH=/path/to/libs:$LD_LIBRARY_PATH
```

#### Display Issues

```bash
# Test OpenGL support
glxinfo | grep "OpenGL version"  # Linux
system_profiler SPDisplaysDataType  # macOS

# Force software rendering
export LIBGL_ALWAYS_SOFTWARE=1
```

### Performance Issues

#### High CPU Usage

- Check event loop implementation
- Verify VSync is enabled
- Profile with appropriate tools

#### Memory Leaks

- Use Valgrind or AddressSanitizer
- Check for missing destructors
- Verify smart pointer usage

### Getting Help

If you encounter issues:

1. **Check Documentation:** Review API docs and examples
2. **Search Issues:** Look for similar problems on GitHub
3. **Create Issue:** Provide detailed reproduction steps
4. **Ask Community:** Use GitHub Discussions for questions

## Development Workflow

### Daily Development

```bash
# 1. Update from upstream
git fetch upstream
git merge upstream/develop

# 2. Create feature branch
git checkout -b feature/my-feature

# 3. Make changes and test
cmake --build build
./build/guillaume

# 4. Commit changes
git add .
git commit -m "feat: add new feature"

# 5. Push and create PR
git push origin feature/my-feature
```

### Code Review Checklist

Before submitting PRs:

- [ ] Code follows style guidelines
- [ ] All tests pass
- [ ] Documentation is updated
- [ ] No memory leaks
- [ ] Performance impact assessed
- [ ] Cross-platform compatibility verified

## Next Steps

After setting up your development environment:

1. **Explore Examples:** Check the `examples/` directory
2. **Read Documentation:** Study the API reference
3. **Start Contributing:** Look for "good first issue" labels
4. **Join Community:** Participate in discussions and reviews

For more information, see:

- [Contributing Guide](CONTRIBUTING.md)
- [API Documentation](api/)
- [Examples](../examples/)
- [Project Roadmap](../ROADMAP.md)
