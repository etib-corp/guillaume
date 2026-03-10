# Simple Application - 3D GPU Rendering (Cross-Platform)

This example demonstrates a **3D GPU-accelerated application** using the Guillaume framework with SDL3. The application uses the optimal graphics backend for each platform:

- **Metal** on macOS with MSL (Metal Shading Language)
- **Vulkan** on Linux with SPIR-V shader format
- **Direct3D 12** on Windows with DXIL shader format

## Overview

The simple_application is a complete implementation of **SDL3 GPU acceleration** for real-time 3D rendering. It showcases modern GPU-based rendering with proper 3D transformations, depth testing, and projection matrices. Each platform uses its native graphics API for optimal performance without translation layers.

## Features

**Cross-Platform GPU Rendering** (Metal/Vulkan/D3D12)  
**Automatic Backend Selection** based on platform  
**Depth Buffer Support** for proper 3D layering  
**Projection Matrix** with perspective transformation  
**View Matrix** for camera control  
**Model Matrix** for object transformations  
**Command Buffer Management** for GPU operations  
**Swapchain Texture Acquisition** for rendering to window  
**Color and Depth Rendering** with proper blending  
**Resource Management** with proper cleanup  
**Error Handling** with logging  
**3D Shape Support** with full position vectors  

## Storage Usage Example

The example now demonstrates Guillaume storage utilities:

- `LocalStorage` persists data across restarts in
  `.simple-application-storage.db`.
- `SessionStorage` keeps data only for the current process session.

On startup, the app increments a persistent `launchCount` value. When clicking
the button, it updates:

- `totalClicks` in `LocalStorage` (persisted)
- `sessionClicks` in `SessionStorage` (session scoped)

The button label displays both counters to make storage behavior visible.
