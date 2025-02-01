#include "GlobalWindow.hpp"
#include "GameObject/Scene/Game.hpp"

// create the window pointer 
const std::shared_ptr<LWindow> gWindow = std::make_shared<LWindow>(DEFAULT_WND_WIDTH, DEFAULT_WND_HEIGHT, wndName);

/* Sets the title of the global window */
void SetWindowTitle(std::string title) {
    gWindow->setName(title);
}

/* Toggles window fullscreen */
void ToggleFullscreen() {
    gWindow->toggleFullscreen();
}

void SetWindowSize(int width, int height, bool changeResolution) {
    gWindow->setDimensions(width, height, !changeResolution);
    // auto sc = GetCurrentScene();
    // if (sc!=nullptr) EnterScene(sc->name);
}

Math::Vector2Int GetWindowDimensions() {
    return Math::Vector2Int(gWindow->getWidth(), gWindow->getHeight());
}

Math::Vector2Int GetWindowResolution() {
    Vector2 sc(gWindow->getScaleX(), gWindow->getScaleY());
    return Vector2(gWindow->getWidth(), gWindow->getHeight())|sc;
}