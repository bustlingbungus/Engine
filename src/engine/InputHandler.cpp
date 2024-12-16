#include "InputHandler.hpp"

InputHandler inputHandler = InputHandler();

InputHandler::InputHandler()
{
    // set up hash map
    for (auto& code : keys) keysdown.emplace(code, false);
}

/* Deallocates memory */
InputHandler::~InputHandler()
{
    keysdown.clear();
    keys.clear();
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
        case SDL_KEYDOWN:
        {
            // find if the key is being tracked
            auto it = keysdown.find((int)e.key.keysym.sym);
            // set entry to true
            if (it != keysdown.end()) it->second = true;
            break;
        }
        // keyboard button released
        case SDL_KEYUP:
        {
            // find if the key is being tracked
            auto it = keysdown.find((int)e.key.keysym.sym);
            // set entry to false
            if (it != keysdown.end()) it->second = false;
            break;
        }
        // mouse button pressed 
        case SDL_MOUSEBUTTONDOWN:
        {
            // find mouse button 
            auto it = keysdown.find((int)e.button.button);
            // set entry to true
            if (it != keysdown.end()) it->second = true;
            break;
        }
        // mouse button released
        case SDL_MOUSEBUTTONUP:
        {
            // find mouse button 
            auto it = keysdown.find((int)e.button.button);
            // set entry to false
            if (it != keysdown.end()) it->second = false;
            break;
        }
        // mouse moved
        case SDL_MOUSEMOTION:
            // update mouse position 
            SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);
            break;
    }
}

/*
 * Checks if a key is currently held down.
 * 
 * \note `InputHandler` stores mouse input and keyboard input in the same place,
 * so either type of input may be checked with this method.
 * 
 * \param input the `SDL_KeyCode` or mouse button macro to check
 * 
 * \returns `true` if the specified input code is currently being held down.
 * `false` if not. `false` if the specified input code is not beign tracked. 
 */
bool InputHandler::inputActive(int input) const
{
    auto it = keysdown.find(input);
    if (it==keysdown.end()) return false;
    return it->second;
}

/* Returns the x/y coordinate of the mouse in the window */
Vector2Int InputHandler::getMousePos() const
{
    return mouse_pos;
}

/* Returns the x/y coordinate of the mouse in the window */
Vector2Int MousePosition() {
    return inputHandler.getMousePos();
}

/*
 * Checks if a key is currently held down.
 * 
 * \note `InputHandler` stores mouse input and keyboard input in the same place,
 * so either type of input may be checked with this method.
 * 
 * \param input the `SDL_KeyCode` or mouse button macro to check
 * 
 * \returns `true` if the specified input code is currently being held down.
 * `false` if not. `false` if the specified input code is not beign tracked. 
 */
bool InputActive(int input) {
    return inputHandler.inputActive(input);
}