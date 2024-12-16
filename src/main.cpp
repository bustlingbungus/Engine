#define SDL_MAIN_HANDLED

// sdl
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "sdl/LWindow.hpp"
#include "sdl/LTexture.hpp"
#include "sdl/LAudio.hpp"

#include "engine/BungusEngine.hpp"

// std
#include <iostream>
#include <memory>

int main()
{
    /* INSTANTIATE OBJECTS */
    Vector2 pos = {200.0f, WND_HEIGHT*0.75f}, scale = {50.0f, 50.0f};
    Vector2 vel(0.0f, -300.0f);
    auto tex = std::make_shared<LTexture>(gWindow);
    tex->solidColour({255,0,0,255});
    auto obj = Instantiate<GameObject>(pos, scale);
    auto rend = std::make_shared<TextureRenderer>(obj, tex);
    auto rigid = std::make_shared<Rigidbody>(obj, 1.0f, vel);
    obj->AddComponent(rend);
    obj->AddComponent(rigid);

    pos = {205.0f, WND_HEIGHT * 0.5f};
    vel = Vector2(0.0f, 0.0f);
    auto obj2 = Instantiate<GameObject>(pos, scale);
    tex = std::make_shared<LTexture>(gWindow);
    tex->solidColour({0,255,0,255});
    rend = std::make_shared<TextureRenderer>(obj2, tex);
    rigid = std::make_shared<Rigidbody>(obj2, 1.0f, vel);
    obj2->AddComponent(rend);
    obj2->AddComponent(rigid);

    pos = {WND_WIDTH/2, WND_HEIGHT}; scale = {WND_WIDTH, 50.0f};
    auto floor = Instantiate<GameObject>(pos, scale);
    tex = std::make_shared<LTexture>(gWindow);
    tex->solidColour({0,0,255,255});
    rend = std::make_shared<TextureRenderer>(floor, tex);
    rigid = std::make_shared<Rigidbody>(floor, 1.0f, Vector2_Zero, false, false);
    floor->AddComponent(rend);
    floor->AddComponent(rigid);


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