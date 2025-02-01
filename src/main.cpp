#define SDL_MAIN_HANDLED

// sdl
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "BungusEngine.hpp"

int main()
{
    // call user initialisation
    Init();

    SDL_Event e;
    // main window loop
    while (!quit)
    {
        // handle events
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT) Exit();
            gWindow->handleEvent(e);
            inputHandler.getInput(e);
        }

        gTime.findDeltaTime();

        // render when not minimised
        if (!gWindow->isMinimized())
        {
            // clear the window
            SDL_SetRenderDrawColor(gWindow->gRenderer, 0, 0, 0, 0xFF);
            SDL_RenderClear(gWindow->gRenderer);

            game.Update();

            // update the window
            SDL_RenderPresent(gWindow->gRenderer);
        }
    }

    return 0;
}