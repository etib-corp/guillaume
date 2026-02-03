# Simple Application - 3D GPU Rendering (Cross-Platform)

This example demonstrates a **3D GPU-accelerated application** using the Guillaume framework with SDL3. The application uses the optimal graphics backend for each platform:

- **Metal** on macOS with MSL (Metal Shading Language)
- **Vulkan** on Linux with SPIR-V shader format
- **Direct3D 12** on Windows with DXIL shader format

## Overview

The simple_application is a complete implementation of **SDL3 GPU acceleration** for real-time 3D rendering. It showcases modern GPU-based rendering with proper 3D transformations, depth testing, and projection matrices. Each platform uses its native graphics API for optimal performance without translation layers.

## Features

✅ **Cross-Platform GPU Rendering** (Metal/Vulkan/D3D12)  
✅ **Automatic Backend Selection** based on platform  
✅ **Depth Buffer Support** for proper 3D layering  
✅ **Projection Matrix** with perspective transformation  
✅ **View Matrix** for camera control  
✅ **Model Matrix** for object transformations  
✅ **Command Buffer Management** for GPU operations  
✅ **Swapchain Texture Acquisition** for rendering to window  
✅ **Color and Depth Rendering** with proper blending  
✅ **Resource Management** with proper cleanup  
✅ **Error Handling** with logging  
✅ **3D Shape Support** with full position vectors  

## Architecture

```
Application
    ↓
Shape (3D position + 2D size)
    ↓
Renderer (3D transformations)  
    ↓  
GPU Pipeline (Model, View, Projection matrices)
    ↓
Platform-Specific Backend
    ├─ Metal (macOS)
    ├─ Vulkan (Linux/Windows)
    └─ Direct3D 12 (Windows)
    ↓
Depth Buffer + Swapchain
    ↓
GPU Hardware
    ↓
GPU Pipeline (Model, View, Projection matrices)
    ↓
SDL3 GPU Context (Vulkan)
    ↓
Depth Buffer + Swapchain
    ↓
GPU Hardware (Vulkan rendering)
```

## Technical Components

### Renderer Implementation

#### GPU Initialization

- SDL3 video subsystem initialization
- Fullscreen SDL window creation (native display resolution)
- GPU device creation with platform-specific shader format support
- Window claim for GPU device
- Depth buffer creation for 3D rendering

#### 3D Graphics Pipeline

- **Depth Buffer**: D24 unsigned normalized format
- **Render Pass**: Color and depth attachments
- **Command Buffers**: GPU command submission
- **Swapchain Textures**: Window surface rendering

#### 3D Transformation Matrices

**Projection Matrix:**

- Type: Perspective projection
- FOV: 45°
- Aspect Ratio: Dynamic (updated on window resize)
- Near Plane: 0.1 units
- Far Plane: 100.0 units

**View Matrix:**

- Camera Position: (0, 0, 3)
- Target: (0, 0, 0)
- Up Vector: (0, 1, 0)

**Model Matrix:**

- Default: Identity matrix
- Supports: Translation, rotation, scale transformations

### Rendering System

The renderer handles:

- GPU command buffer acquisition and submission
- Swapchain texture management
- Depth target configuration
- Matrix updates on window resize
- Color and depth clearing
- Resource cleanup on shutdown

### Shape Implementation

Rectangles are rendered as 3D objects with:

- Full 3D position coordinates (x, y, z)
- 2D size (width, height)
- GPU-accelerated drawing
- Depth-tested rendering

## Building

### Prerequisites

- CMake 3.10+
- C++20 compiler
- Vulkan-capable GPU
- SDL3 library (fetched automatically)
- GLM library (fetched automatically)

### Build Commands

```bash
# Configure and build
cmake --build build

# Or manually configure first
mkdir -p build
cd build
cmake ..
make
```

### Output

```
./build/examples/simple_application/simple_application
```

## Dependencies

| Dependency | Version | Purpose |
|-----------|---------|---------|
| SDL3 | 3.4.0 | GPU rendering and window management |
| SDL3_ttf | 3.2.2 | Text rendering |
| GLM | 1.0.1 | 3D mathematics and transformations |

All dependencies are automatically fetched via CMake FetchContent.

## Coordinate System

- **X-axis**: Right
- **Y-axis**: Up  
- **Z-axis**: Back (towards camera)
- **Handedness**: Right-handed

## File Structure

```
simple_application/
├── CMakeLists.txt                    # Build configuration
├── README.md                         # This file
├── headers/
│   ├── application.hpp               # Application class
│   ├── event_handler.hpp             # Event handling
│   ├── renderer.hpp                  # 3D GPU renderer
│   └── shapes/
│       └── rectangle.hpp             # 3D rectangle shape
└── sources/
    ├── application.cpp
    ├── event_handler.cpp
    ├── main.cpp
    ├── renderer.cpp                  # GPU implementation
    └── shapes/
        └── rectangle.cpp             # 3D drawing code
```

## Future Enhancements

### Immediate Next Steps

1. **Shader Implementation**
   - Vertex shader for MVP transformations
   - Fragment shader for per-pixel lighting
   - Shader compilation and pipeline creation

2. **Geometry Rendering**
   - Vertex/index buffer population
   - 3D mesh generation
   - Multiple shape types (cubes, spheres, etc.)

### Advanced Features

- **Texture Mapping**: UV coordinate support and samplers
- **Lighting Systems**: Phong, PBR, deferred rendering
- **Camera Control**: Orbit, first-person, smooth transitions
- **Animation**: Skeletal animation, procedural generation
- **Performance**: Frustum culling, LOD systems, batch rendering

## API Reference

### Renderer Methods

```cpp
// Transformation matrices
void setViewMatrix(const glm::mat4 &view);
void setProjectionMatrix(const glm::mat4 &projection);
void setModelMatrix(const glm::mat4 &model);

// Rendering
void clear(void) override;
void present(void) override;

// 3D Drawing
void drawRectangle3D(const utility::math::Vector<std::float_t, 3> &position,
                    const utility::math::Vector<std::float_t, 2> &size,
                    const utility::math::Color<uint8_t> &color);

// Color management
void setDrawColor(const utility::math::Color<uint8_t> &color);
```

## Technical Specifications

### Graphics API (Platform-Specific)

| Platform | Graphics API | Shader Format                |
|----------|--------------|------------------------------|
| macOS    | Metal        | MSL (Metal Shading Language) |
| Linux    | Vulkan       | SPIR-V                       |
| Windows  | Direct3D 12  | DXIL/DXBC                    |

### Rendering Configuration

- **Window Mode**: Fullscreen (native display resolution)
- **Depth Format**: D32_FLOAT
- **Color Format**: Standard RGBA32F
- **Depth Testing**: Enabled (when supported)
- **C++ Standard**: C++20

## Platform-Specific Behavior

### macOS

- Uses **Metal** backend for GPU acceleration
- MSL (Metal Shading Language) for shaders
- Native performance without translation layers
- No special window flags required

### Linux

- Uses **Vulkan** backend for GPU acceleration
- SPIR-V shader format
- Cross-vendor GPU support
- Requires SDL3 with Vulkan support
- Uses `SDL_WINDOW_VULKAN` flag for window creation

### Windows

- Uses **Direct3D 12** backend for GPU acceleration
- DXIL (Direct3D Intermediate Language) for shaders
- Fallback to DXBC for older DirectX versions
- Native Windows GPU integration
- No special window flags required

## Known Limitations

- Shaders not yet implemented (infrastructure ready)
- No actual 3D geometry rendering (pipeline established)
- Single camera view (extensible)
- No animation system (math ready)
- Depth buffer optional (graceful fallback on Metal)

## Performance Considerations

- GPU command buffers are submitted per frame
- Depth testing is enabled for all rendering
- Swapchain textures are acquired fresh each frame
- Window resize triggers matrix recalculation

## Troubleshooting

### Build Issues

- Ensure CMake 3.10+ is installed
- Verify C++20 compiler support
- On Linux/Windows: Check for Vulkan SDK installation
- On macOS: Ensure Xcode command line tools are installed

### Runtime Issues - Window Creation Failed

**macOS Fix**: The application now automatically uses Metal backend. If you still get errors:

- Ensure you're running on a Mac with Metal support (2012 or later)
- Update SDL3 to latest version
- Check system logs for Metal-specific errors

**Linux/Windows Fix**:

- Verify Vulkan drivers are properly installed
- Run `vulkaninfo` command to test Vulkan setup
- Check GPU driver version compatibility

### Runtime Issues - GPU Device Creation Failed

- Verify your GPU supports the selected graphics API
- Update GPU drivers to latest version
- On older hardware, some texture formats may not be supported
- The application will log warnings if depth buffer creation fails (non-fatal)

## References

- [SDL3 Documentation](https://wiki.libsdl.org/SDL3/)
- [GLM Documentation](https://glm.g-truc.net/)
- [Vulkan Specification](https://www.vulkan.org/)
- [Guillaume Framework](../../../headers/guillaume/)
