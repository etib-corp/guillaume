#include <cstdlib>
#include <string>

#include "guillaume/context.hpp"
#include "guillaume/events/keyboard_event.hpp"
#include "guillaume/events/mouse_event.hpp"
#include "guillaume/renderer.hpp"

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

class SDL3Renderer : public guillaume::Renderer {

private:
    SDL_Window* _window;
    SDL_Renderer* _renderer;
    TTF_TextEngine* _text_engine;

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

    void draw_rectangle(const guillaume::RectanglePrimitive& rectangle) override
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

    void draw_text(const guillaume::TextPrimitive& text, const guillaume::Font& font) override
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

    guillaume::Vector get_text_size(const guillaume::Font& font,
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

        return guillaume::Vector(static_cast<float>(width), static_cast<float>(height));
    }

    void set_clip_rect(const guillaume::Rectangle& rectangle) override
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

    void clear(const guillaume::Color& color) override
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

        while (SDL_PollEvent(&sdl_event)) {
            switch (sdl_event.type) {
            case SDL_EVENT_QUIT:
                _set_running(false);
                break;
            case SDL_EVENT_KEY_DOWN:
                // _push_event(guillaume::KeyboardEvent(
                //     guillaume::KeyboardEvent::KeyType::KEY_DOWN,
                //     static_cast<guillaume::KeyboardEvent::KeyCode>(sdl_event.key.keysym.sym),
                //     static_cast<guillaume::KeyboardEvent::KeyModifiers>(sdl_event.key.mod)));
                break;
            case SDL_EVENT_KEY_UP:
                // _push_event(guillaume::KeyboardEvent(
                //     guillaume::KeyboardEvent::KeyType::KEY_UP,
                //     static_cast<guillaume::KeyboardEvent::KeyCode>(sdl_event.key.keysym.sym),
                //     static_cast<guillaume::KeyboardEvent::KeyModifiers>(sdl_event.key.mod)));
                break;
            default:
                break;
            }
        }
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
    std::unique_ptr<guillaume::Renderer> renderer = nullptr;
    std::unique_ptr<guillaume::Context> context = nullptr;

    try {
        renderer = std::make_unique<SDL3Renderer>();
    } catch (const std::runtime_error& exception) {
        fprintf(stderr, "Error initializing renderer: %s\n", exception.what());
        return EXIT_FAILURE;
    }

    try {
        context = std::make_unique<guillaume::Context>(std::move(renderer));
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