# Simple Application

This example demonstrates a GPU-accelerated 3D application built with Guillaume
and SDL3.

## Platform Backends

- macOS: Metal (MSL).
- Linux: Vulkan (SPIR-V).
- Windows: Direct3D 12 (DXIL).

The backend is selected automatically for each platform.

## Rendering Features

- Depth buffer support.
- Perspective projection matrix.
- View and model matrix transforms.
- Command buffer-based rendering flow.
- Swapchain texture acquisition.
- Color and depth rendering passes.
- Resource lifecycle management.
- Error logging and handling.

## Storage Demonstration

This example also showcases storage utilities:

- `LocalStorage` persists data in `.simple-application-storage.db`.
- `SessionStorage` keeps data only during the current process.

Behavior:

- On startup, `launchCount` is incremented in `LocalStorage`.
- Button clicks update `totalClicks` in `LocalStorage`.
- Button clicks update `sessionClicks` in `SessionStorage`.

The button label displays both counters to make persistence behavior explicit.
