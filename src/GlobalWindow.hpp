#pragma once

#include "sdl/LWindow.hpp"
#include <memory>
#include <string>

#include "Math/VectorMath.hpp"

/* Window dimensions */
#define DEFAULT_WND_WIDTH 1920
#define DEFAULT_WND_HEIGHT 1080

/* Name of the window */
const std::string wndName = "Game Engine";

/* The window used by all objects in the game */
const extern std::shared_ptr<LWindow> gWindow;

void SetWindowTitle(std::string title);

void ToggleFullscreen();

void SetWindowSize(int width, int height, bool changeResolution = true);

Math::Vector2Int GetWindowDimensions();
Math::Vector2Int GetWindowResolution();