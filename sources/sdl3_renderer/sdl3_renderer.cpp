#include <string>

#include "sdl3_renderer/sdl3_renderer.hpp"
#include "guillaume/logger.hpp"
#include "guillaume/config.hpp"

#include "guillaume/events/keyboard_event.hpp"
#include "guillaume/events/mouse_event.hpp"
#include "guillaume/events/quit_event.hpp"
#include "guillaume/events/unknown_event.hpp"
#include "guillaume/renderer.hpp"

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

std::unique_ptr<guigui::KeyboardEvent> SDL3Renderer::_translate_keyboard_event(const SDL_Event& sdl_event)
{
    // Translate SDL keyboard events to guigui keyboard events
    guigui::KeyboardEvent::KeyType key_type = (sdl_event.type == SDL_EVENT_KEY_DOWN)
        ? guigui::KeyboardEvent::KeyType::KEY_PRESS
        : guigui::KeyboardEvent::KeyType::KEY_RELEASE;

    guigui::KeyboardEvent::KeyCode key_code;

    if (_key_code_map.find(sdl_event.key.key) != _key_code_map.end()) {
        key_code = _key_code_map[sdl_event.key.key];
    } else {
        key_code = guigui::KeyboardEvent::KeyCode::NONE;
    }

    std::vector<guigui::KeyboardEvent::KeyModifiers> key_modifiers;

    // Check for modifiers
    for (const auto& [sdl_mod, gui_mod] : _key_modifiers_map) {
        if (sdl_event.key.mod & sdl_mod) {
            key_modifiers.push_back(gui_mod);
        }
    }

    return std::make_unique<guigui::KeyboardEvent>(
        key_type, key_code, key_modifiers);
}

std::unique_ptr<guigui::MouseEvent> SDL3Renderer::_translate_mouse_event(const SDL_Event& sdl_event)
{
    // Translate SDL mouse events to guigui mouse events
    guigui::MouseEvent::MouseEventType mouse_event_type;
    guigui::MouseEvent::MouseButton mouse_button;
    guigui::Vector position;

    switch (sdl_event.type) {
    case SDL_EVENT_MOUSE_BUTTON_DOWN:
        mouse_event_type = guigui::MouseEvent::MouseEventType::MOUSE_BUTTON_PRESS;
        mouse_button = _mouse_button_map[sdl_event.button.button];
        position = { static_cast<float>(sdl_event.button.x), static_cast<float>(sdl_event.button.y) };
        if (guigui::Config::get_instance().is_debug_mouse_coordinates()) {
            LOG_DEBUG_F("Mouse button press at ({}, {})", position.get_x(), position.get_y());
        }
        break;
    case SDL_EVENT_MOUSE_BUTTON_UP:
        mouse_event_type = guigui::MouseEvent::MouseEventType::MOUSE_BUTTON_RELEASE;
        mouse_button = _mouse_button_map[sdl_event.button.button];
        position = { static_cast<float>(sdl_event.button.x), static_cast<float>(sdl_event.button.y) };
        if (guigui::Config::get_instance().is_debug_mouse_coordinates()) {
            LOG_DEBUG_F("Mouse button release at ({}, {})", position.get_x(), position.get_y());
        }
        break;
    case SDL_EVENT_MOUSE_MOTION:
    default:
        mouse_event_type = guigui::MouseEvent::MouseEventType::MOUSE_MOTION;
        mouse_button = guigui::MouseEvent::MouseButton::NONE; // No button for motion events
        position = { static_cast<float>(sdl_event.motion.x), static_cast<float>(sdl_event.motion.y) };
        // Only log every 10th motion event to avoid spam, and only if debugging is enabled
        if (guigui::Config::get_instance().is_debug_mouse_coordinates()) {
            static int motion_counter = 0;
            if (++motion_counter % 10 == 0) {
                LOG_DEBUG_F("Mouse motion at ({}, {}) [every 10th event]", position.get_x(), position.get_y());
            }
        }
        break;
    }

    return std::make_unique<guigui::MouseEvent>(
        mouse_event_type, mouse_button, position);
}

std::unique_ptr<guigui::Eventable> SDL3Renderer::_translate_event(const SDL_Event& sdl_event)
{
    switch (sdl_event.type) {
    case SDL_EVENT_QUIT:
        return std::make_unique<guigui::QuitEvent>();
    case SDL_EVENT_KEY_DOWN:
    case SDL_EVENT_KEY_UP:
        return _translate_keyboard_event(sdl_event);
    case SDL_EVENT_MOUSE_BUTTON_DOWN:
    case SDL_EVENT_MOUSE_BUTTON_UP:
    case SDL_EVENT_MOUSE_MOTION:
        return _translate_mouse_event(sdl_event);
    default:
        return std::make_unique<guigui::UnknownEvent>();
    }
}

SDL3Renderer::SDL3Renderer()
{
}

SDL3Renderer::~SDL3Renderer()
{
    clear_cache(); // Clear all cached textures
    TTF_DestroySurfaceTextEngine(_text_engine);
    SDL_DestroyRenderer(_renderer);
    SDL_DestroyWindow(_window);
    TTF_Quit();
    SDL_Quit();
}

void SDL3Renderer::init(std::string name)
{
    LOG_INFO_F("Initializing SDL3 renderer: {}", name);
    SDL_DisplayID display_id = 0;
    SDL_Rect display_bounds;

    LOG_DEBUG("Initializing SDL video subsystem");
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        LOG_ERROR("SDL video initialization failed");
        throw std::runtime_error("SDL could not initialize.");
    }
    LOG_DEBUG("SDL video subsystem initialized successfully");

    LOG_DEBUG("Initializing SDL_ttf");
    if (!TTF_Init()) {
        LOG_ERROR("SDL_ttf initialization failed");
        throw std::runtime_error("SDL_ttf could not initialize.");
    }
    LOG_DEBUG("SDL_ttf initialized successfully");

    display_id = SDL_GetPrimaryDisplay();
    LOG_DEBUG_F("Primary display ID: {}", display_id);

    if (!SDL_GetDisplayBounds(display_id, &display_bounds)) {
        LOG_ERROR_F("Failed to get display bounds for display {}", display_id);
        throw std::runtime_error("Could not get display bounds.");
    }
    LOG_DEBUG_F("Display bounds: {}x{}", display_bounds.w, display_bounds.h);

    // Get configuration settings
    auto& config = guigui::Config::get_instance();
    
    if (config.get_window_mode() == guigui::Config::WindowMode::FULLSCREEN) {
        LOG_DEBUG_F("Creating fullscreen window: {}", name);
        _window = SDL_CreateWindow(name.c_str(),
            display_bounds.w, display_bounds.h,
            SDL_WINDOW_FULLSCREEN);
    } else {
        // Create a windowed mode - use configured size or default to 80% of screen
        int window_width = config.get_window_width();
        int window_height = config.get_window_height();
        
        // Ensure window fits on screen
        if (window_width > display_bounds.w) window_width = static_cast<int>(display_bounds.w * 0.8);
        if (window_height > display_bounds.h) window_height = static_cast<int>(display_bounds.h * 0.8);
        
        SDL_WindowFlags flags = 0;
        if (config.is_window_resizable()) {
            flags |= SDL_WINDOW_RESIZABLE;
        }
        
        LOG_DEBUG_F("Creating windowed window: {} ({}x{})", name, window_width, window_height);
        _window = SDL_CreateWindow(name.c_str(),
            window_width, window_height,
            flags);
    }
    if (!_window) {
        LOG_ERROR_F("Failed to create window: {}", SDL_GetError());
        throw std::runtime_error("Window could not be created!");
    }
    LOG_DEBUG("Window created successfully");

    LOG_DEBUG("Creating SDL renderer");
    _renderer = SDL_CreateRenderer(_window, NULL);
    if (!_renderer) {
        LOG_ERROR_F("Failed to create renderer: {}", SDL_GetError());
        throw std::runtime_error("Renderer could not be created!");
    }
    LOG_DEBUG("SDL renderer created successfully");

    LOG_DEBUG("Creating text engine");
    _text_engine = TTF_CreateSurfaceTextEngine();
    if (!_text_engine) {
        LOG_ERROR_F("Failed to create text engine: {}", SDL_GetError());
        throw std::runtime_error("Text engine could not be created!");
    }
    LOG_DEBUG("Text engine created successfully");
    
    LOG_INFO("SDL3 renderer initialization complete");
}

void SDL3Renderer::draw_rectangle(const guigui::RectanglePrimitive& rectangle)
{
    LOG_TRACE_F("Drawing rectangle at ({}, {}) with size {}x{}", 
                rectangle.get_rectangle().get_x(), 
                rectangle.get_rectangle().get_y(),
                rectangle.get_rectangle().get_width(), 
                rectangle.get_rectangle().get_height());

    // Track primitive request (rectangles are always rendered directly, no caching yet)
    _total_primitive_requests++;
    _new_primitive_creations++;

    SDL_FRect sdl_rect = {
        static_cast<float>(rectangle.get_rectangle().get_x()),
        static_cast<float>(rectangle.get_rectangle().get_y()),
        static_cast<float>(rectangle.get_rectangle().get_width()),
        static_cast<float>(rectangle.get_rectangle().get_height())
    };

    SDL_Color sdl_color = {
        rectangle.get_color().get_red(),
        rectangle.get_color().get_green(),
        rectangle.get_color().get_blue(),
        rectangle.get_color().get_alpha()
    };

    if (!SDL_SetRenderDrawColor(_renderer, sdl_color.r, sdl_color.g, sdl_color.b, sdl_color.a)) {
        LOG_ERROR_F("Failed to set render draw color: {}", SDL_GetError());
        throw std::runtime_error("Failed to set render draw color.");
    }

    if (!SDL_RenderFillRect(_renderer, &sdl_rect)) {
        LOG_ERROR_F("Failed to render rectangle: {}", SDL_GetError());
        throw std::runtime_error("Failed to render rectangle.");
    }
    
    LOG_TRACE("Rectangle drawn successfully");
}

void SDL3Renderer::draw_text(const guigui::TextPrimitive& text)
{
    LOG_TRACE_F("Drawing text '{}' at position ({}, {}) with font {}", 
                text.get_text(), 
                text.get_position().get_x(), 
                text.get_position().get_y(),
                text.get_font().get_path());

    // Skip rendering if text is empty
    if (text.get_text().empty()) {
        LOG_TRACE("Skipping rendering of empty text");
        return;
    }

    // Track primitive request
    _total_primitive_requests++;

    // Generate cache key
    std::string cache_key = _generate_text_cache_key(
        text.get_component_id(),
        text.get_primitive_name(),
        text.get_text(),
        text.get_font().get_path(),
        text.get_font().get_size()
    );

    // Check if texture is cached
    auto cache_it = _text_cache.find(cache_key);
    SDL_Texture* texture = nullptr;
    int width = 0, height = 0;
    
    if (cache_it != _text_cache.end()) {
        // Use cached texture
        texture = cache_it->second.texture;
        width = cache_it->second.width;
        height = cache_it->second.height;
        _cached_primitive_uses++;
        LOG_TRACE_F("Using cached texture for text '{}'", text.get_text());
    } else {
        // Create new texture and cache it
        _new_primitive_creations++;
        LOG_TRACE_F("Creating new texture for text '{}'", text.get_text());
        
        TTF_Font* ttf_font = TTF_OpenFont(text.get_font().get_path().c_str(), text.get_font().get_size());
        if (!ttf_font) {
            LOG_ERROR_F("Failed to load font: {} ({})", text.get_font().get_path(), SDL_GetError());
            throw std::runtime_error("Failed to load font: " + text.get_font().get_path());
        }

        SDL_Color sdl_color = { text.get_color().get_red(), text.get_color().get_green(), text.get_color().get_blue(), text.get_color().get_alpha() };

        SDL_Surface* surface = TTF_RenderText_Solid(ttf_font, text.get_text().c_str(), text.get_text().length(), sdl_color);
        if (!surface) {
            TTF_CloseFont(ttf_font);
            LOG_ERROR_F("Failed to render text '{}': {}", text.get_text(), SDL_GetError());
            throw std::runtime_error("Failed to render text: " + text.get_text());
        }

        texture = SDL_CreateTextureFromSurface(_renderer, surface);
        if (!texture) {
            SDL_DestroySurface(surface);
            TTF_CloseFont(ttf_font);
            LOG_ERROR_F("Failed to create texture from surface: {}", SDL_GetError());
            throw std::runtime_error("Failed to create texture from surface");
        }

        width = surface->w;
        height = surface->h;

        // Cache the texture
        TextCache& cache_entry = _text_cache[cache_key];
        cache_entry.texture = texture;
        cache_entry.width = width;
        cache_entry.height = height;
        cache_entry.content = text.get_text();
        cache_entry.font_path = text.get_font().get_path();
        cache_entry.font_size = text.get_font().get_size();

        SDL_DestroySurface(surface);
        TTF_CloseFont(ttf_font);
    }

    // Render the texture
    SDL_FRect dst_rect = {
        text.get_position().get_x(),
        text.get_position().get_y(),
        static_cast<float>(width),
        static_cast<float>(height)
    };

    if (!SDL_RenderTexture(_renderer, texture, NULL, &dst_rect)) {
        LOG_ERROR_F("Failed to render text texture: {}", SDL_GetError());
        throw std::runtime_error("Failed to render text texture.");
    }
    
    LOG_TRACE_F("Text '{}' rendered successfully", text.get_text());
}

guigui::Vector SDL3Renderer::get_text_size(const guigui::Font& font,
    const std::string& content)
{
    LOG_TRACE_F("Getting text size for '{}' with font {}", content, font.get_path());
    
    // Return zero size for empty text
    if (content.empty()) {
        LOG_TRACE("Returning zero size for empty text");
        return guigui::Vector(0.0f, 0.0f);
    }
    
    int width = 0, height = 0;

    TTF_Font* ttf_font = TTF_OpenFont(font.get_path().c_str(), font.get_size());
    if (!ttf_font) {
        LOG_ERROR_F("Failed to load font for text size calculation: {} ({})", font.get_path(), SDL_GetError());
        throw std::runtime_error("Failed to load font: " + font.get_path());
    }

    TTF_Text* ttf_text = TTF_CreateText(_text_engine, ttf_font, content.c_str(), content.length());
    if (!ttf_text) {
        TTF_CloseFont(ttf_font);
        LOG_ERROR_F("Failed to create text for size calculation: {} ({})", content, SDL_GetError());
        throw std::runtime_error("Failed to create text: " + content);
    }

    if (!TTF_GetTextSize(ttf_text, &width, &height)) {
        TTF_DestroyText(ttf_text);
        TTF_CloseFont(ttf_font);
        LOG_ERROR_F("Failed to get text size for '{}': {}", content, SDL_GetError());
        throw std::runtime_error("Failed to get text size for: " + content);
    }

    TTF_DestroyText(ttf_text);
    TTF_CloseFont(ttf_font);

    LOG_TRACE_F("Text '{}' size: {}x{}", content, width, height);
    return guigui::Vector(static_cast<float>(width), static_cast<float>(height));
}

void SDL3Renderer::set_clip_rect(const guigui::Rectangle& rectangle)
{
    LOG_TRACE_F("Setting clip rectangle: ({}, {}) {}x{}", 
                rectangle.get_x(), rectangle.get_y(), 
                rectangle.get_width(), rectangle.get_height());
    
    SDL_Rect sdl_rect = {
        rectangle.get_x(),
        rectangle.get_y(),
        rectangle.get_width(),
        rectangle.get_height()
    };
    if (!SDL_SetRenderClipRect(_renderer, &sdl_rect)) {
        LOG_ERROR_F("Failed to set clip rectangle: {}", SDL_GetError());
        throw std::runtime_error("Failed to set clip rectangle.");
    }
    
    LOG_TRACE("Clip rectangle set successfully");
}

void SDL3Renderer::clear(const guigui::Color& color)
{
    LOG_TRACE_F("Clearing renderer with {}", color.to_string());
    
    if (!SDL_RenderClear(_renderer)) {
        LOG_ERROR_F("Failed to clear renderer: {}", SDL_GetError());
        throw std::runtime_error("Failed to clear renderer.");
    }
    
    LOG_TRACE("Renderer cleared successfully");
}

void SDL3Renderer::present()
{
    LOG_TRACE("Presenting rendered frame");
    
    if (!SDL_RenderPresent(_renderer)) {
        LOG_ERROR_F("Failed to present renderer: {}", SDL_GetError());
        throw std::runtime_error("Failed to present renderer.");
    }
    
    LOG_TRACE("Frame presented successfully");
}

void SDL3Renderer::poll_events()
{
    LOG_TRACE("Polling for SDL events");
    SDL_Event sdl_event = { 0 };
    int event_count = 0;

    while (SDL_PollEvent(&sdl_event)) {
        event_count++;
        _push_event(_translate_event(sdl_event));
    }
    
    if (event_count > 0) {
        LOG_TRACE_F("Processed {} SDL events", event_count);
    }
}

std::string SDL3Renderer::_generate_text_cache_key(const std::string& component_id, 
                                                   const std::string& primitive_name,
                                                   const std::string& content,
                                                   const std::string& font_path,
                                                   int font_size) const
{
    return component_id + "|" + primitive_name + "|" + content + "|" + font_path + "|" + std::to_string(font_size);
}

void SDL3Renderer::clear_cache()
{
    LOG_DEBUG_F("Clearing text cache with {} entries", _text_cache.size());
    _text_cache.clear();
}

void SDL3Renderer::clear_component_cache(const std::string& component_id)
{
    LOG_DEBUG_F("Clearing cache for component: {}", component_id);
    
    auto it = _text_cache.begin();
    while (it != _text_cache.end()) {
        if (it->first.find(component_id + "|") == 0) {
            LOG_TRACE_F("Removing cached entry: {}", it->first);
            it = _text_cache.erase(it);
        } else {
            ++it;
        }
    }
}

void SDL3Renderer::log_primitive_statistics() const
{
    if (_total_primitive_requests > 0) {
        double cache_hit_ratio = 100.0 * _cached_primitive_uses / _total_primitive_requests;
        LOG_DEBUG_F("Primitive stats: {} total requests ({} cached, {} new) - {}% cache hit ratio", 
                   _total_primitive_requests, _cached_primitive_uses, _new_primitive_creations, cache_hit_ratio);
    }
}

void SDL3Renderer::reset_primitive_statistics()
{
    _total_primitive_requests = 0;
    _cached_primitive_uses = 0;
    _new_primitive_creations = 0;
}
