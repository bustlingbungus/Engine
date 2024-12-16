#pragma once

#include "../sdl/LWindow.hpp"
#include <memory>
#include <string>

/* Window dimensions */
#define WND_WIDTH 1280
#define WND_HEIGHT 720

/* Name of the window */
const std::string wndName = "Game Engine";

/* The window used by all objects in the game */
const extern std::shared_ptr<LWindow> gWindow;