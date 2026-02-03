#include "renderer.hpp"
#include <iostream>
#include <stdexcept>

namespace simple_application {

struct Vertex3D {
    glm::vec3 position;
    glm::vec4 color;
};

struct UniformBuffer3D {
    glm::mat4 projection;
    glm::mat4 view;
    glm::mat4 model;
};

Renderer::Renderer()
    : guillaume::Renderer(), window(nullptr), device(nullptr),
      pipeline(nullptr), vertexBuffer(nullptr), indexBuffer(nullptr),
      uniformBuffer(nullptr), depthTexture(nullptr),
      clearColor(0.0f, 0.0f, 0.0f, 1.0f),
      projectionMatrix(
          glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f)),
      viewMatrix(glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f),
                             glm::vec3(0.0f, 0.0f, 0.0f),
                             glm::vec3(0.0f, 1.0f, 0.0f))),
      modelMatrix(glm::identity<glm::mat4>()) {
    initializeGPU();
#ifdef _WIN32
    getLogger().info("SDL GPU Renderer initialized with 3D DirectX 12 backend");
#elif defined(__APPLE__)
    getLogger().info("SDL GPU Renderer initialized with 3D Metal backend");
#else
    getLogger().info("SDL GPU Renderer initialized with 3D Vulkan backend");
#endif
}

Renderer::~Renderer(void) {
    shutdownGPU();
    getLogger().info("SDL GPU Renderer destroyed");
}

void Renderer::initializeGPU(void) {
    // Initialize SDL video subsystem
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        throw std::runtime_error("Failed to initialize SDL: " +
                                 std::string(SDL_GetError()));
    }

    // Create fullscreen window with GPU support (platform-specific flags
    // handled by SDL) SDL3 will automatically select appropriate backend:
    // - Metal on macOS
    // - Vulkan on Linux
    // - Direct3D 12 on Windows
    Uint32 windowFlags = SDL_WINDOW_FULLSCREEN;
#ifdef _WIN32
    // Windows: Use default flags, SDL3 will use Direct3D 12
    windowFlags |= 0;
#elif defined(__APPLE__)
    // macOS: Use default flags, SDL3 will use Metal
    windowFlags |= 0;
#else
    // Linux: Request Vulkan support
    windowFlags |= SDL_WINDOW_VULKAN;
#endif

    // Get primary display bounds for fullscreen dimensions
    const SDL_DisplayID displayID = SDL_GetPrimaryDisplay();
    SDL_Rect displayBounds;
    if (!SDL_GetDisplayBounds(displayID, &displayBounds)) {
        SDL_Quit();
        throw std::runtime_error("Failed to get display bounds: " +
                                 std::string(SDL_GetError()));
    }

    window = SDL_CreateWindow("Guillaume 3D Application", displayBounds.w,
                              displayBounds.h, windowFlags);
    if (!window) {
        SDL_Quit();
        throw std::runtime_error("Failed to create window: " +
                                 std::string(SDL_GetError()));
    }

    // Create GPU device with platform-appropriate shader format
    // Metal on macOS, Vulkan on Linux, Direct3D 12 on Windows
    SDL_GPUShaderFormat shaderFormat;
#ifdef _WIN32
    // Windows: Use DXIL (Direct3D Intermediate Language) for DirectX 12
    // Fallback to DXBC (Direct3D Bytecode) for older DirectX versions
    shaderFormat = SDL_GPU_SHADERFORMAT_DXIL;
#elif defined(__APPLE__)
    // macOS: Use Metal Shading Language
    shaderFormat = SDL_GPU_SHADERFORMAT_MSL;
#else
    // Linux: Use SPIR-V for Vulkan
    shaderFormat = SDL_GPU_SHADERFORMAT_SPIRV;
#endif

    device = SDL_CreateGPUDevice(shaderFormat, true, nullptr);
    if (!device) {
        SDL_DestroyWindow(window);
        SDL_Quit();
        throw std::runtime_error("Failed to create GPU device: " +
                                 std::string(SDL_GetError()));
    } // Claim the window for this GPU device
    if (!SDL_ClaimWindowForGPUDevice(device, window)) {
        SDL_DestroyGPUDevice(device);
        SDL_DestroyWindow(window);
        SDL_Quit();
        throw std::runtime_error("Failed to claim window for GPU device: " +
                                 std::string(SDL_GetError()));
    }

    createDepthBuffer(displayBounds.w, displayBounds.h);
#ifdef _WIN32
    getLogger().info("GPU device created with 3D DirectX 12 backend");
#elif defined(__APPLE__)
    getLogger().info("GPU device created with 3D Metal backend");
#else
    getLogger().info("GPU device created with 3D Vulkan backend");
#endif
}

void Renderer::createDepthBuffer(Uint32 width, Uint32 height) {
    SDL_GPUTextureCreateInfo textureCreateInfo = {};
    textureCreateInfo.type = SDL_GPU_TEXTURETYPE_2D;
    textureCreateInfo.width = width;
    textureCreateInfo.height = height;
    textureCreateInfo.layer_count_or_depth = 1;
    textureCreateInfo.format = SDL_GPU_TEXTUREFORMAT_D32_FLOAT;
    textureCreateInfo.num_levels = 1;
    textureCreateInfo.sample_count = SDL_GPU_SAMPLECOUNT_1;
    textureCreateInfo.usage = SDL_GPU_TEXTUREUSAGE_DEPTH_STENCIL_TARGET;
    textureCreateInfo.props = 0;

    depthTexture = SDL_CreateGPUTexture(device, &textureCreateInfo);
    if (!depthTexture) {
        // Depth buffer is optional - log warning but continue
        getLogger().warning(
            "Failed to create depth buffer - continuing without depth testing");
    } else {
        getLogger().info("Depth buffer created successfully");
    }
}

void Renderer::createPipeline(void) {
    // Pipeline creation would include:
    // 1. Vertex shader for 3D position transformation
    // 2. Fragment shader for color rendering
    // 3. Pipeline layout with uniform buffers
    // This is a placeholder for future shader implementation
}

void Renderer::updateMatrices(Uint32 width, Uint32 height) {
    // Update projection matrix based on window size
    projectionMatrix = glm::perspective(
        glm::radians(45.0f),
        static_cast<float>(width) / static_cast<float>(height), 0.1f, 100.0f);
}

void Renderer::shutdownGPU(void) {
    if (pipeline) {
        SDL_ReleaseGPUGraphicsPipeline(device, pipeline);
        pipeline = nullptr;
    }

    if (vertexBuffer) {
        SDL_ReleaseGPUBuffer(device, vertexBuffer);
        vertexBuffer = nullptr;
    }

    if (indexBuffer) {
        SDL_ReleaseGPUBuffer(device, indexBuffer);
        indexBuffer = nullptr;
    }

    if (uniformBuffer) {
        SDL_ReleaseGPUBuffer(device, uniformBuffer);
        uniformBuffer = nullptr;
    }

    if (depthTexture) {
        SDL_ReleaseGPUTexture(device, depthTexture);
        depthTexture = nullptr;
    }

    if (device) {
        SDL_DestroyGPUDevice(device);
        device = nullptr;
    }

    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }

    SDL_Quit();
}

void Renderer::clear(void) {
    if (!device || !window) {
        getLogger().warning("Cannot clear: device or window not initialized");
        return;
    }

    // Acquire command buffer
    SDL_GPUCommandBuffer *cmdBuf = SDL_AcquireGPUCommandBuffer(device);
    if (!cmdBuf) {
        getLogger().error("Failed to acquire command buffer");
        return;
    }

    // Acquire swapchain texture for the window
    SDL_GPUTexture *swapchainTexture = nullptr;
    Uint32 textureWidth = 0, textureHeight = 0;
    if (!SDL_AcquireGPUSwapchainTexture(cmdBuf, window, &swapchainTexture,
                                        &textureWidth, &textureHeight)) {
        getLogger().error("Failed to acquire swapchain texture");
        SDL_SubmitGPUCommandBuffer(cmdBuf);
        return;
    }

    if (!swapchainTexture) {
        getLogger().debug("No swapchain texture available");
        SDL_SubmitGPUCommandBuffer(cmdBuf);
        return;
    }

    getLogger().debug("Clearing render target (" + std::to_string(textureWidth) + 
                      "x" + std::to_string(textureHeight) + ")");

    // Update matrices if window size changed
    updateMatrices(textureWidth, textureHeight);

    // Create color target
    SDL_GPUColorTargetInfo colorTargetInfo = {};
    colorTargetInfo.texture = swapchainTexture;
    colorTargetInfo.clear_color.r = clearColor.red();
    colorTargetInfo.clear_color.g = clearColor.green();
    colorTargetInfo.clear_color.b = clearColor.blue();
    colorTargetInfo.clear_color.a = clearColor.alpha();
    colorTargetInfo.load_op = SDL_GPU_LOADOP_CLEAR;
    colorTargetInfo.store_op = SDL_GPU_STOREOP_STORE;

    // Create depth target
    SDL_GPUDepthStencilTargetInfo depthTargetInfo = {};
    if (depthTexture) {
        depthTargetInfo.texture = depthTexture;
        depthTargetInfo.clear_depth = 1.0f;
        depthTargetInfo.load_op = SDL_GPU_LOADOP_CLEAR;
        depthTargetInfo.store_op = SDL_GPU_STOREOP_STORE;
    }

    // Begin render pass with both color and depth targets
    SDL_GPURenderPass *renderPass = SDL_BeginGPURenderPass(
        cmdBuf, &colorTargetInfo, 1, depthTexture ? &depthTargetInfo : nullptr);

    if (renderPass) {
        SDL_EndGPURenderPass(renderPass);
    }

    SDL_SubmitGPUCommandBuffer(cmdBuf);
}

void Renderer::present(void) {
    if (!device) {
        getLogger().warning("Cannot present: device not initialized");
        return;
    }
    // Presentation is handled by SDL_GPU swapchain automatically
    // The swapchain texture is presented when the command buffer is submitted
    getLogger().debug("Frame presented");
}

void Renderer::setDrawColor(const utility::math::Color<uint8_t> &color) {
    clearColor.setRed(static_cast<float>(color.red()) / 255.0f);
    clearColor.setGreen(static_cast<float>(color.green()) / 255.0f);
    clearColor.setBlue(static_cast<float>(color.blue()) / 255.0f);
    clearColor.setAlpha(static_cast<float>(color.alpha()) / 255.0f);
    getLogger().info("Clear color set to RGBA(" + 
                     std::to_string(color.red()) + ", " +
                     std::to_string(color.green()) + ", " +
                     std::to_string(color.blue()) + ", " +
                     std::to_string(color.alpha()) + ")");
}

void Renderer::setViewMatrix(const glm::mat4 &view) { viewMatrix = view; }

void Renderer::setProjectionMatrix(const glm::mat4 &projection) {
    projectionMatrix = projection;
}

void Renderer::setModelMatrix(const glm::mat4 &model) { modelMatrix = model; }

} // namespace simple_application
