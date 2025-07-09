// sdl3_renderer.hpp

#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

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

    std::unique_ptr<guigui::KeyboardEvent> _translate_keyboard_event(const SDL_Event& sdl_event);

    std::map<SDL_MouseButtonFlags, guigui::MouseEvent::MouseButton> _mouse_button_map = {
        { SDL_BUTTON_LEFT, guigui::MouseEvent::MouseButton::LEFT },
        { SDL_BUTTON_RIGHT, guigui::MouseEvent::MouseButton::RIGHT },
        { SDL_BUTTON_MIDDLE, guigui::MouseEvent::MouseButton::MIDDLE },
        { SDL_BUTTON_X1, guigui::MouseEvent::MouseButton::SIDE_1 },
        { SDL_BUTTON_X2, guigui::MouseEvent::MouseButton::SIDE_2 }
    };

    std::unique_ptr<guigui::MouseEvent> _translate_mouse_event(const SDL_Event& sdl_event);

    std::unique_ptr<guigui::Eventable> _translate_event(const SDL_Event& sdl_event);

public:
    SDL3Renderer();
    ~SDL3Renderer();

    void init(std::string name) override;
    void draw_rectangle(const guigui::RectanglePrimitive& rectangle) override;
    void draw_text(const guigui::TextPrimitive& text) override;
    guigui::Vector get_text_size(const guigui::Font& font, const std::string& content) override;
    void set_clip_rect(const guigui::Rectangle& rectangle) override;
    void clear(const guigui::Color& color) override;
    void present() override;
    void poll_events() override;
};
