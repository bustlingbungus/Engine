#define SDL_MAIN_HANDLED

// sdl
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "engine/BungusEngine.hpp"

// std
#include <iostream>
#include <memory>

int main()
{
    /* INSTANTIATE OBJECTS */
    auto obj = Instantiate<GameObject>(
        Vector2(-440.0f, 180.0f), 
        Vector2(50.0f, 50.0f)
    );
    auto tex = std::make_shared<LTexture>(gWindow);
    tex->solidColour({255,0,0,255});
    obj->AddComponent<TextureRenderer>(obj, tex);
    obj->AddComponent<Rigidbody>(obj, 1.0f, 0.1f, Vector2(40.0f, -300.0f));


    auto obj2 = Instantiate<GameObject>(
        Vector2(-435.0f, 0.0f), 
        Vector2(50.0f, 50.0f)       
    );
    tex = std::make_shared<LTexture>(gWindow);
    tex->solidColour({0,255,0,255});
    obj2->AddComponent<TextureRenderer>(obj2, tex);
    obj2->AddComponent<Rigidbody>(obj2, 1.0f, 0.1f, Vector2_Zero);


    auto floor = Instantiate<GameObject>(
        Vector2(0.0f, 360.0f), 
        Vector2(1280.0f, 50.0f)
    );
    tex = std::make_shared<LTexture>(gWindow);
    tex->solidColour({0,0,255,255});
    floor->AddComponent<TextureRenderer>(floor, tex);
    floor->AddComponent<Rigidbody>(floor, 1.0f, 0.025f, Vector2_Zero, false, false);


    SDL_Event e;
    bool quit = false;
    // main window loop
    while (!quit)
    {
        // handle events
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT) quit = true;
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