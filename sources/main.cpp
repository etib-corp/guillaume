#include <cstdlib>
#include <string>

#include "guillaume/context.hpp"
#include "guillaume/events/keyboard_event.hpp"
#include "guillaume/events/mouse_event.hpp"
#include "guillaume/events/quit_event.hpp"
#include "guillaume/events/unknown_event.hpp"
#include "guillaume/renderer.hpp"

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

class SDL3Renderer : public guigui::Renderer {

private:
    SDL_Window* _window;
    SDL_Renderer* _renderer;
    TTF_TextEngine* _text_engine;

    std::map<SDL_Keycode, guigui::KeyboardEvent::KeyCode> _key_code_map = {
        { SDLK_A, guigui::KeyboardEvent::KeyCode::A },
        { SDLK_B, guigui::KeyboardEvent::KeyCode::B },
        { SDLK_C, guigui::KeyboardEvent::KeyCode::C },
        { SDLK_D, guigui::KeyboardEvent::KeyCode::D },
        { SDLK_E, guigui::KeyboardEvent::KeyCode::E },
        { SDLK_F, guigui::KeyboardEvent::KeyCode::F },
        { SDLK_G, guigui::KeyboardEvent::KeyCode::G },
        { SDLK_H, guigui::KeyboardEvent::KeyCode::H },
        { SDLK_I, guigui::KeyboardEvent::KeyCode::I },
        { SDLK_J, guigui::KeyboardEvent::KeyCode::J },
        { SDLK_K, guigui::KeyboardEvent::KeyCode::K },
        { SDLK_L, guigui::KeyboardEvent::KeyCode::L },
        { SDLK_M, guigui::KeyboardEvent::KeyCode::M },
        { SDLK_N, guigui::KeyboardEvent::KeyCode::N },
        { SDLK_O, guigui::KeyboardEvent::KeyCode::O },
        { SDLK_P, guigui::KeyboardEvent::KeyCode::P },
        { SDLK_Q, guigui::KeyboardEvent::KeyCode::Q },
        { SDLK_R, guigui::KeyboardEvent::KeyCode::R },
        { SDLK_S, guigui::KeyboardEvent::KeyCode::S },
        { SDLK_T, guigui::KeyboardEvent::KeyCode::T },
        { SDLK_U, guigui::KeyboardEvent::KeyCode::U },
        { SDLK_V, guigui::KeyboardEvent::KeyCode::V },
        { SDLK_W, guigui::KeyboardEvent::KeyCode::W },
        { SDLK_X, guigui::KeyboardEvent::KeyCode::X },
        { SDLK_Y, guigui::KeyboardEvent::KeyCode::Y },
        { SDLK_Z, guigui::KeyboardEvent::KeyCode::Z },
        { SDLK_0, guigui::KeyboardEvent::KeyCode::NUM_0 },
        { SDLK_1, guigui::KeyboardEvent::KeyCode::NUM_1 },
        { SDLK_2, guigui::KeyboardEvent::KeyCode::NUM_2 },
        { SDLK_3, guigui::KeyboardEvent::KeyCode::NUM_3 },
        { SDLK_4, guigui::KeyboardEvent::KeyCode::NUM_4 },
        { SDLK_5, guigui::KeyboardEvent::KeyCode::NUM_5 },
        { SDLK_6, guigui::KeyboardEvent::KeyCode::NUM_6 },
        { SDLK_7, guigui::KeyboardEvent::KeyCode::NUM_7 },
        { SDLK_8, guigui::KeyboardEvent::KeyCode::NUM_8 },
        { SDLK_9, guigui::KeyboardEvent::KeyCode::NUM_9 },
        { SDLK_F1, guigui::KeyboardEvent::KeyCode::F1 },
        { SDLK_F2, guigui::KeyboardEvent::KeyCode::F2 },
        { SDLK_F3, guigui::KeyboardEvent::KeyCode::F3 },
        { SDLK_F4, guigui::KeyboardEvent::KeyCode::F4 },
        { SDLK_F5, guigui::KeyboardEvent::KeyCode::F5 },
        { SDLK_F6, guigui::KeyboardEvent::KeyCode::F6 },
        { SDLK_F7, guigui::KeyboardEvent::KeyCode::F7 },
        { SDLK_F8, guigui::KeyboardEvent::KeyCode::F8 },
        { SDLK_F9, guigui::KeyboardEvent::KeyCode::F9 },
        { SDLK_F10, guigui::KeyboardEvent::KeyCode::F10 },
        { SDLK_F11, guigui::KeyboardEvent::KeyCode::F11 },
        { SDLK_F12, guigui::KeyboardEvent::KeyCode::F12 },
        { SDLK_UP, guigui::KeyboardEvent::KeyCode::UP },
        { SDLK_DOWN, guigui::KeyboardEvent::KeyCode::DOWN },
        { SDLK_LEFT, guigui::KeyboardEvent::KeyCode::LEFT },
        { SDLK_RIGHT, guigui::KeyboardEvent::KeyCode::RIGHT },
        { SDLK_SPACE, guigui::KeyboardEvent::KeyCode::SPACE },
        { SDLK_RETURN, guigui::KeyboardEvent::KeyCode::ENTER },
        { SDLK_ESCAPE, guigui::KeyboardEvent::KeyCode::ESC },
        { SDLK_TAB, guigui::KeyboardEvent::KeyCode::TAB },
        { SDLK_BACKSPACE, guigui::KeyboardEvent::KeyCode::BACKSPACE },
        { SDLK_INSERT, guigui::KeyboardEvent::KeyCode::INSERT },
        { SDLK_DELETE, guigui::KeyboardEvent::KeyCode::DELETE },
        { SDLK_HOME, guigui::KeyboardEvent::KeyCode::HOME },
        { SDLK_END, guigui::KeyboardEvent::KeyCode::END },
        { SDLK_PAGEUP, guigui::KeyboardEvent::KeyCode::PAGE_UP },
        { SDLK_PAGEDOWN, guigui::KeyboardEvent::KeyCode::PAGE_DOWN },
        { SDLK_KP_0, guigui::KeyboardEvent::KeyCode::NP_0 },
        { SDLK_KP_1, guigui::KeyboardEvent::KeyCode::NP_1 },
        { SDLK_KP_2, guigui::KeyboardEvent::KeyCode::NP_2 },
        { SDLK_KP_3, guigui::KeyboardEvent::KeyCode::NP_3 },
        { SDLK_KP_4, guigui::KeyboardEvent::KeyCode::NP_4 },
        { SDLK_KP_5, guigui::KeyboardEvent::KeyCode::NP_5 },
        { SDLK_KP_6, guigui::KeyboardEvent::KeyCode::NP_6 },
        { SDLK_KP_7, guigui::KeyboardEvent::KeyCode::NP_7 },
        { SDLK_KP_8, guigui::KeyboardEvent::KeyCode::NP_8 },
        { SDLK_KP_9, guigui::KeyboardEvent::KeyCode::NP_9 },
        { SDLK_KP_PLUS, guigui::KeyboardEvent::KeyCode::NP_ADD },
        { SDLK_KP_MINUS, guigui::KeyboardEvent::KeyCode::NP_SUBTRACT },
        { SDLK_KP_MULTIPLY, guigui::KeyboardEvent::KeyCode::NP_MULTIPLY },
        { SDLK_KP_DIVIDE, guigui::KeyboardEvent::KeyCode::NP_DIVIDE },
        { SDLK_KP_PERIOD, guigui::KeyboardEvent::KeyCode::NP_DECIMAL },
        { SDLK_KP_ENTER, guigui::KeyboardEvent::KeyCode::NP_ENTER },
    };

    std::map<SDL_Keymod, guigui::KeyboardEvent::KeyModifiers> _key_modifiers_map = {
        { SDL_KMOD_CTRL, guigui::KeyboardEvent::KeyModifiers::CTRL },
        { SDL_KMOD_ALT, guigui::KeyboardEvent::KeyModifiers::ALT },
        { SDL_KMOD_SHIFT, guigui::KeyboardEvent::KeyModifiers::SHIFT },
        { SDL_KMOD_GUI, guigui::KeyboardEvent::KeyModifiers::SUPER }
    };

    std::unique_ptr<guigui::KeyboardEvent> _translate_keyboard_event(const SDL_Event& sdl_event)
    {
        // Translate SDL keyboard events to guigui keyboard events
        guigui::KeyboardEvent::KeyType key_type = (sdl_event.type == SDL_EVENT_KEY_DOWN)
            ? guigui::KeyboardEvent::KeyType::KEY_PRESS
            : guigui::KeyboardEvent::KeyType::KEY_RELEASE;

        guigui::KeyboardEvent::KeyCode key_code = _key_code_map[sdl_event.key.key];

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

    std::map<SDL_MouseButtonFlags, guigui::MouseEvent::MouseButton> _mouse_button_map = {
        { SDL_BUTTON_LEFT, guigui::MouseEvent::MouseButton::LEFT },
        { SDL_BUTTON_RIGHT, guigui::MouseEvent::MouseButton::RIGHT },
        { SDL_BUTTON_MIDDLE, guigui::MouseEvent::MouseButton::MIDDLE },
        { SDL_BUTTON_X1, guigui::MouseEvent::MouseButton::SIDE_1 },
        { SDL_BUTTON_X2, guigui::MouseEvent::MouseButton::SIDE_2 }
    };

    std::unique_ptr<guigui::MouseEvent> _translate_mouse_event(const SDL_Event& sdl_event)
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

    std::unique_ptr<guigui::Eventable> _translate_event(const SDL_Event& sdl_event)
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

public:
    void init(std::string name) override
    {
        SDL_DisplayID display_id = 0;
        SDL_Rect display_bounds;

        if (!SDL_Init(SDL_INIT_VIDEO))
            throw std::runtime_error("SDL could not initialize.");

        if (!TTF_Init())
            throw std::runtime_error("SDL_ttf could not initialize.");

        display_id = SDL_GetPrimaryDisplay();

        if (!SDL_GetDisplayBounds(display_id, &display_bounds))
            throw std::runtime_error("Could not get display bounds.");

        _window = SDL_CreateWindow(name.c_str(),
            display_bounds.w, display_bounds.h,
            SDL_WINDOW_FULLSCREEN);
        if (!_window)
            throw std::runtime_error("Window could not be created!");

        _renderer = SDL_CreateRenderer(_window, NULL);
        if (!_renderer)
            throw std::runtime_error("Renderer could not be created!");

        _text_engine = TTF_CreateSurfaceTextEngine();
        if (!_text_engine)
            throw std::runtime_error("Text engine could not be created!");
    }

    void draw_rectangle(const guigui::RectanglePrimitive& rectangle) override
    {

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
            throw std::runtime_error("Failed to set render draw color.");
        }

        if (!SDL_RenderFillRect(_renderer, &sdl_rect)) {
            throw std::runtime_error("Failed to render rectangle.");
        }
    }

    void draw_text(const guigui::TextPrimitive& text, const guigui::Font& font) override
    {
        TTF_Font* ttf_font = TTF_OpenFont(font.get_path().c_str(), font.get_size());
        if (!ttf_font) {
            throw std::runtime_error("Failed to load font: " + font.get_path());
        }

        SDL_Color sdl_color = { text.get_color().get_red(), text.get_color().get_green(), text.get_color().get_blue(), text.get_color().get_alpha() };

        SDL_Surface* surface = TTF_RenderText_Solid(ttf_font, text.getText().c_str(), text.getText().length(), sdl_color);
        if (!surface) {
            TTF_CloseFont(ttf_font);
            throw std::runtime_error("Failed to render text: " + text.getText());
        }

        SDL_Texture* texture = SDL_CreateTextureFromSurface(_renderer, surface);
        if (!texture) {
            SDL_DestroySurface(surface);
            TTF_CloseFont(ttf_font);
            throw std::runtime_error("Failed to create texture from surface");
        }

        SDL_FRect dst_rect = {
            text.getPosition().get_x(),
            text.getPosition().get_y(),
            static_cast<float>(surface->w),
            static_cast<float>(surface->h)
        };

        if (!SDL_RenderTexture(_renderer, texture, NULL, &dst_rect)) {
            SDL_DestroyTexture(texture);
            SDL_DestroySurface(surface);
            TTF_CloseFont(ttf_font);
            throw std::runtime_error("Failed to render text texture.");
        }

        SDL_DestroyTexture(texture);
        SDL_DestroySurface(surface);
        TTF_CloseFont(ttf_font);
    }

    guigui::Vector get_text_size(const guigui::Font& font,
        const std::string& content) override
    {
        int width = 0, height = 0;

        TTF_Font* ttf_font = TTF_OpenFont(font.get_path().c_str(), font.get_size());
        if (!ttf_font) {
            throw std::runtime_error("Failed to load font: " + font.get_path());
        }

        TTF_Text* ttf_text = TTF_CreateText(_text_engine, ttf_font, content.c_str(), content.length());
        if (!ttf_text) {
            TTF_CloseFont(ttf_font);
            throw std::runtime_error("Failed to create text: " + content);
        }

        if (!TTF_GetTextSize(ttf_text, &width, &height)) {
            TTF_DestroyText(ttf_text);
            TTF_CloseFont(ttf_font);
            throw std::runtime_error("Failed to get text size for: " + content);
        }

        TTF_DestroyText(ttf_text);
        TTF_CloseFont(ttf_font);

        return guigui::Vector(static_cast<float>(width), static_cast<float>(height));
    }

    void set_clip_rect(const guigui::Rectangle& rectangle) override
    {
        SDL_Rect sdl_rect = {
            rectangle.get_x(),
            rectangle.get_y(),
            rectangle.get_width(),
            rectangle.get_height()
        };
        if (!SDL_SetRenderClipRect(_renderer, &sdl_rect))
            throw std::runtime_error("Failed to set clip rectangle.");
    }

    void clear(const guigui::Color& color) override
    {
        if (!SDL_RenderClear(_renderer))
            throw std::runtime_error("Failed to clear renderer.");
    }

    void present() override
    {
        if (!SDL_RenderPresent(_renderer))
            throw std::runtime_error("Failed to present renderer.");
    }

    void poll_events() override
    {
        SDL_Event sdl_event = { 0 };

        while (SDL_PollEvent(&sdl_event))
            _push_event(_translate_event(sdl_event));
    }

    ~SDL3Renderer()
    {
        TTF_DestroySurfaceTextEngine(_text_engine);
        SDL_DestroyRenderer(_renderer);
        SDL_DestroyWindow(_window);
        TTF_Quit();
        SDL_Quit();
    }
};

int main(int argc, char* const argv[], char* const envp[])
{
    std::unique_ptr<guigui::Renderer> renderer = nullptr;
    std::unique_ptr<guigui::Context> context = nullptr;

    try {
        renderer = std::make_unique<SDL3Renderer>();
    } catch (const std::runtime_error& exception) {
        fprintf(stderr, "Error initializing renderer: %s\n", exception.what());
        return EXIT_FAILURE;
    }

    try {
        context = std::make_unique<guigui::Context>(std::move(renderer));
    } catch (const std::runtime_error& exception) {
        fprintf(stderr, "Error initializing context: %s\n", exception.what());
        return EXIT_FAILURE;
    }

    try {
        context->run();
    } catch (const std::runtime_error& exception) {
        fprintf(stderr, "Error running context: %s\n", exception.what());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}