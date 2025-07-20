#include <string>

#include "sdl3_renderer/sdl3_renderer.hpp"
#include "guillaume/logger.hpp"

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
    guigui::Vector position = { static_cast<float>(sdl_event.button.x), static_cast<float>(sdl_event.button.y) };

    switch (sdl_event.type) {
    case SDL_EVENT_MOUSE_BUTTON_DOWN:
        mouse_event_type = guigui::MouseEvent::MouseEventType::MOUSE_BUTTON_PRESS;
        mouse_button = _mouse_button_map[sdl_event.button.button];
        break;
    case SDL_EVENT_MOUSE_BUTTON_UP:
        mouse_event_type = guigui::MouseEvent::MouseEventType::MOUSE_BUTTON_RELEASE;
        mouse_button = _mouse_button_map[sdl_event.button.button];
        break;
    case SDL_EVENT_MOUSE_MOTION:
    default:
        mouse_event_type = guigui::MouseEvent::MouseEventType::MOUSE_MOTION;
        mouse_button = guigui::MouseEvent::MouseButton::NONE; // No button for motion events
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

    LOG_DEBUG("Initializing SDL video subsystem...");
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        LOG_ERROR("SDL video initialization failed");
        throw std::runtime_error("SDL could not initialize.");
    }
    LOG_DEBUG("SDL video subsystem initialized successfully");

    LOG_DEBUG("Initializing SDL_ttf...");
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

    LOG_DEBUG_F("Creating fullscreen window: {}", name);
    _window = SDL_CreateWindow(name.c_str(),
        display_bounds.w, display_bounds.h,
        SDL_WINDOW_FULLSCREEN);
    if (!_window) {
        LOG_ERROR_F("Failed to create window: {}", SDL_GetError());
        throw std::runtime_error("Window could not be created!");
    }
    LOG_DEBUG("Window created successfully");

    LOG_DEBUG("Creating SDL renderer...");
    _renderer = SDL_CreateRenderer(_window, NULL);
    if (!_renderer) {
        LOG_ERROR_F("Failed to create renderer: {}", SDL_GetError());
        throw std::runtime_error("Renderer could not be created!");
    }
    LOG_DEBUG("SDL renderer created successfully");

    LOG_DEBUG("Creating text engine...");
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

    SDL_Texture* texture = SDL_CreateTextureFromSurface(_renderer, surface);
    if (!texture) {
        SDL_DestroySurface(surface);
        TTF_CloseFont(ttf_font);
        LOG_ERROR_F("Failed to create texture from surface: {}", SDL_GetError());
        throw std::runtime_error("Failed to create texture from surface");
    }

    SDL_FRect dst_rect = {
        text.get_position().get_x(),
        text.get_position().get_y(),
        static_cast<float>(surface->w),
        static_cast<float>(surface->h)
    };

    if (!SDL_RenderTexture(_renderer, texture, NULL, &dst_rect)) {
        SDL_DestroyTexture(texture);
        SDL_DestroySurface(surface);
        TTF_CloseFont(ttf_font);
        LOG_ERROR_F("Failed to render text texture: {}", SDL_GetError());
        throw std::runtime_error("Failed to render text texture.");
    }

    SDL_DestroyTexture(texture);
    SDL_DestroySurface(surface);
    TTF_CloseFont(ttf_font);
    
    LOG_TRACE_F("Text '{}' rendered successfully", text.get_text());
}

guigui::Vector SDL3Renderer::get_text_size(const guigui::Font& font,
    const std::string& content)
{
    LOG_TRACE_F("Getting text size for '{}' with font {}", content, font.get_path());
    
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
    LOG_TRACE_F("Clearing renderer with color RGBA({}, {}, {}, {})", 
                color.get_red(), color.get_green(), color.get_blue(), color.get_alpha());
    
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
