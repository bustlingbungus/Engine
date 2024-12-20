#pragma once

#include <SDL.h>
#include "Math/VectorMath.hpp"

#include <unordered_map>
#include <vector>
#include <memory>

using namespace Math;

/* Tracks input current being held down, as well as mouse position */
class InputHandler
{
    public:

        InputHandler();
        ~InputHandler();

        void getInput(SDL_Event& e);
        
        bool inputActive(int input) const;
        Vector2Int getMousePos() const;

    private:

        /* boolean of each individual input */
        std::unordered_map<int,bool> keysdown;

        /* The position of the cursor in the window */
        Vector2Int mouse_pos = Vector2Int_Zero;

        /* The keys that will be tracked */
        std::vector<int> keys = {
            SDLK_LCTRL,
            SDLK_LSHIFT,
            SDLK_LALT,
            SDLK_RCTRL,
            SDLK_RSHIFT,
            SDLK_RALT,
            SDLK_KP_PERIOD,
            SDLK_KP_1,
            SDLK_KP_2,
            SDLK_KP_3,
            SDLK_KP_4,
            SDLK_KP_5,
            SDLK_KP_6,
            SDLK_KP_7,
            SDLK_KP_8,
            SDLK_KP_9,
            SDLK_KP_0,
            SDLK_KP_DIVIDE,
            SDLK_KP_MULTIPLY,
            SDLK_KP_MINUS,
            SDLK_KP_PLUS,
            SDLK_KP_ENTER,
            SDLK_PRINTSCREEN,
            SDLK_SCROLLLOCK,
            SDLK_PAUSE,
            SDLK_INSERT,
            SDLK_HOME,
            SDLK_PAGEUP,
            SDLK_DELETE,
            SDLK_END,
            SDLK_PAGEDOWN,
            SDLK_RIGHT,
            SDLK_LEFT,
            SDLK_DOWN,
            SDLK_UP,
            SDLK_F1,
            SDLK_F2,
            SDLK_F3,
            SDLK_F4,
            SDLK_F5,
            SDLK_F6,
            SDLK_F7,
            SDLK_F8,
            SDLK_F9,
            SDLK_F10,
            SDLK_F11,
            SDLK_F12,
            SDLK_CAPSLOCK,
            SDLK_LEFTBRACKET,
            SDLK_BACKSLASH,
            SDLK_RIGHTBRACKET,
            SDLK_CARET,
            SDLK_UNDERSCORE,
            SDLK_BACKQUOTE,
            SDLK_a,
            SDLK_b,
            SDLK_c,
            SDLK_d,
            SDLK_e,
            SDLK_f,
            SDLK_g,
            SDLK_h,
            SDLK_i,
            SDLK_j,
            SDLK_k,
            SDLK_l,
            SDLK_m,
            SDLK_n,
            SDLK_o,
            SDLK_p,
            SDLK_q,
            SDLK_r,
            SDLK_s,
            SDLK_t,
            SDLK_u,
            SDLK_v,
            SDLK_w,
            SDLK_x,
            SDLK_y,
            SDLK_z,
            SDLK_RETURN,
            SDLK_ESCAPE,
            SDLK_BACKSPACE,
            SDLK_TAB,
            SDLK_SPACE,
            SDLK_EXCLAIM,
            SDLK_QUOTEDBL,
            SDLK_HASH,
            SDLK_PERCENT,
            SDLK_DOLLAR,
            SDLK_AMPERSAND,
            SDLK_QUOTE,
            SDLK_LEFTPAREN,
            SDLK_RIGHTPAREN,
            SDLK_ASTERISK,
            SDLK_PLUS,
            SDLK_COMMA,
            SDLK_MINUS,
            SDLK_PERIOD,
            SDLK_SLASH,
            SDLK_0,
            SDLK_1,
            SDLK_2,
            SDLK_3,
            SDLK_4,
            SDLK_5,
            SDLK_6,
            SDLK_7,
            SDLK_8,
            SDLK_9,
            SDLK_COLON,
            SDLK_SEMICOLON,
            SDLK_LESS,
            SDLK_EQUALS,
            SDLK_GREATER,
            SDLK_QUESTION,
            SDLK_AT,
            SDL_BUTTON_LEFT,
            SDL_BUTTON_RIGHT,
            SDL_BUTTON_MIDDLE,
            SDL_BUTTON_X1,
            SDL_BUTTON_X2,
        };
};

extern InputHandler inputHandler;

// forwards declaration
class Camera;
Vector2 MousePosition(std::shared_ptr<Camera> cam = nullptr);
Vector2Int MouseWindowPosition();
bool InputActive(int input);