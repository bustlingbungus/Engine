#include "GlobalWindow.hpp"

// create the window pointer 
const std::shared_ptr<LWindow> gWindow = std::make_shared<LWindow>(WND_WIDTH, WND_HEIGHT, wndName);