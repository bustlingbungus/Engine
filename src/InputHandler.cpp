#include "InputHandler.hpp"
#include "GameObject/Scene/Game.hpp"
#include "GameObject/Cameras/Camera.hpp"

// initalise global input handler
InputHandler inputHandler = InputHandler();

InputHandler::InputHandler()
{
    
}

/* Deallocates memory */
InputHandler::~InputHandler()
{
    keysheld.clear();
    keysdown.clear();
    keysup.clear();
}

/* 
 * Checks input from given the SDL_Event, and updates
 * key tracking variables by assigning hasmap entries
 */
void InputHandler::getInput(SDL_Event& e)
{
    // check input type
    switch (e.type)
    {
        // keyboard button pressed
        // add the key to the keysdown and keysheld sets
        case SDL_KEYDOWN:
        {
            auto it = keysheld.find((int)e.key.keysym.sym);
            if (it == keysheld.end()) {
                keysdown.emplace((int)e.key.keysym.sym);
                keysheld.emplace((int)e.key.keysym.sym);
            }
            break;
        }

        // keyboard button released
        // add the key to the keysup set, and remove it from keysheld
        case SDL_KEYUP:
        {
            keysup.emplace((int)e.key.keysym.sym);
            auto it = keysheld.find((int)e.key.keysym.sym);
            if (it != keysheld.end()) keysheld.erase(it);
            break;
        }

        // mouse button pressed
        // add the button to the keysdown and keysheld sets
        case SDL_MOUSEBUTTONDOWN:
        {
            auto it = keysheld.find(e.button.button);
            if (it == keysheld.end()) {
                keysdown.emplace(e.button.button);
                keysheld.emplace(e.button.button);
            }
            break;
        }

        // mouse button released 
        // add the key to the keysup set, and remove it from keysheld
        case SDL_MOUSEBUTTONUP:
        {
            keysup.emplace(e.button.button);
            auto it = keysheld.find(e.button.button);
            if (it != keysheld.end()) keysheld.erase(it);
            break;
        }

        // mouse moved
        case SDL_MOUSEMOTION:
            SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);
            break;
    }
}

/*
 * Returns if the given input was activated on this frame. I.e., if `w` was pressed on this frame,
 * `inputDown(SDLK_w)` will return `true` on this frame, and `false` on the next frame (even
 * if w is not released).
 * 
 * \param input The input being queried. This may be an SDL keycode or an SDL mouse button
 * macro. Both input types can be tracked with this function.
 * 
 * \note An input is considered "held" as soon as it is pressed. Therefore, if `inputDown(x)`
 * returns `true`, so will `inputHeld(x)`, though the inverse will not always be true. 
 */
bool InputHandler::inputDown(int& input) const {
    return keysdown.find(input) != keysdown.end();
}

/*
 * Returns if the given input was released on this frame. I.e., if `w` was being held on
 * the previous frame, and was released on this frame, `inputUp(SDLK_w)` will return `true` 
 * on this frame, and `false` on the next frame.
 * 
 * \param input The input being queried. This may be an SDL keycode or an SDL mouse button
 * macro. Both input types can be tracked with this function.
 * 
 * \note An input is no longer considered "held" as soon as it is released. Therefore, 
 * if `inputUp(x)` returns `true`, `inputHeld(x)` will return false, though the inverse 
 * will not always be true. 
 */
bool InputHandler::inputUp(int& input) const {
    return keysup.find(input) != keysup.end();
}

/*
 * Returns if the given input is currently being held. I.e., if the user is currently holding down
 * the `w` key, `inputHeld(w)` will return `true`, and will do so until `w` is released.
 * 
 * \param input The input being queried. This may be an SDL keycode or an SDL mouse button
 * macro. Both input types can be tracked with this function.
 * 
 * \note An input is considered "held" from the frame the key is pressed (inclusive) to the 
 * frame the key is released (not inclusive).
 */
bool InputHandler::inputHeld(int& input) const {
    return keysheld.find(input) != keysheld.end();
}

/* clears keydown and keyup inputs. */
void InputHandler::Update()
{
    // keydown and keyup inputs should only activate on the frame on input, 
    // so clear these sets every frame
    keysdown.clear();
    keysup.clear();
}


/* Returns the x/y coordinate of the mouse in the window */
Vector2Int InputHandler::getMousePos() const
{
    return mouse_pos;
}

/* Returns the x/y coordinate of the mouse in game */
Vector2 MousePosition(std::shared_ptr<Camera> cam) 
{
    // get mouse camera on the window
    Vector2 res = inputHandler.getMousePos(); 

    // convert to game space based on camera, if possible
    if (cam == nullptr) cam = GetObject<Camera>();
    if (cam != nullptr) {
        Vector2 cpos(cam->camera().x, cam->camera().y);
        res += cpos;
    }
    return res;
}

/* Returns the x/y coordinates of the mouse in the window */
Vector2Int MouseWindowPosition() {
    return inputHandler.getMousePos();
}

/*
 * Returns if the given input was activated on this frame. I.e., if `w` was pressed on this frame,
 * `InputDown(SDLK_w)` will return `true` on this frame, and `false` on the next frame (even
 * if w is not released).
 * 
 * \param input The input being queried. This may be an SDL keycode or an SDL mouse button
 * macro. Both input types can be tracked with this function.
 * 
 * \note An input is considered "held" as soon as it is pressed. Therefore, if `InputDown(x)`
 * returns `true`, so will `InputHeld(x)`, though the inverse will not always be true. 
 */
bool InputDown(int input) {
    return inputHandler.inputDown(input);
}

/*
 * Returns if the given input was released on this frame. I.e., if `w` was being held on
 * the previous frame, and was released on this frame, `InputUp(SDLK_w)` will return `true` 
 * on this frame, and `false` on the next frame.
 * 
 * \param input The input being queried. This may be an SDL keycode or an SDL mouse button
 * macro. Both input types can be tracked with this function.
 * 
 * \note An input is no longer considered "held" as soon as it is released. Therefore, 
 * if `InputUp(x)` returns `true`, `InputHeld(x)` will return false, though the inverse 
 * will not always be true. 
 */
bool InputUp(int input) {
    return inputHandler.inputUp(input);
}

/*
 * Returns if the given input is currently being held. I.e., if the user is currently holding down
 * the `w` key, `InputHeld(w)` will return `true`, and will do so until `w` is released.
 * 
 * \param input The input being queried. This may be an SDL keycode or an SDL mouse button
 * macro. Both input types can be tracked with this function.
 * 
 * \note An input is considered "held" from the frame the key is pressed (inclusive) to the 
 * frame the key is released (not inclusive).
 */
bool InputHeld(int input) {
    return inputHandler.inputHeld(input);
}