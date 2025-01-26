#pragma once

#include <SDL.h>
#include "Math/VectorMath.hpp"

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <memory>

using namespace Math;

/* Tracks input currently being held down, as well as mouse position */
class InputHandler
{
    public:

        InputHandler();
        ~InputHandler();

        void getInput(SDL_Event& e);

        bool inputDown(int& input) const;
        bool inputUp(int& input) const;
        bool inputHeld(int& input) const;

        void Update();
        
        Vector2Int getMousePos() const;

    private:

        /* hash sets representing input keys */
        std::unordered_set<int> keysheld, // the keys that are currently held down
                                keysdown, // the keys that were pressed on the current frame
                                keysup; // the keys that were released on the current frame

        /* The position of the cursor in the window */
        Vector2Int mouse_pos = Vector2Int_Zero;
};

extern InputHandler inputHandler;

// forwards declaration
class Camera;
Vector2 MousePosition(std::shared_ptr<Camera> cam = nullptr);
Vector2Int MouseWindowPosition();

bool InputDown(int input);
bool InputUp(int input);
bool InputHeld(int input);