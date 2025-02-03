#ifndef LWINDOW_HPP
#define LWINDOW_HPP

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <sstream>
#include <stdio.h>
#include <string>
#include <memory>

#include "LFont.hpp"

/* SDL Init object */
class LWindow
{
public:
  /* The window renderer */
  SDL_Renderer *gRenderer = NULL;

  /* Globally used font */
  std::shared_ptr<LFont> gFont;

  /* Handles window events */
  void handleEvent(SDL_Event &e);

  /* Window width in pixels */
  int getWidth();
  /* Window height in pixels */
  int getHeight();

  /* The X render scale of the window */  
  float getScaleX();
  /* The Y render scale of the window */  
  float getScaleY();

  /* Window focii */
  bool hasMouseFocus();
  bool hasKeyboardFocus();
  bool isMinimized();

  /* toggles fullscreen */
  bool toggleFullscreen();

  /* changes window dimensions */
  void setDimensions(int width, int height, bool maintainResolution = false);
  /* Sets the position of the window's origin on the monitor. leave as defaults for the window to be centred */
  void setPosition(int x = SDL_WINDOWPOS_CENTERED, int y = SDL_WINDOWPOS_CENTERED);

  /* Initialises variables */
  LWindow(int width = 1280, int height = 720, std::string name = "Window");

  /* deallocates memory */
  ~LWindow();

  /* Changes window name */
  void setName(std::string newName);

private:
  /* The window being rendered to */
  SDL_Window *gWindow = NULL;

  /* Window dimensions */
  int wWidth;
  int wHeight;

  /* Scaling factor */
  float scaleX = 1.0f;
  float scaleY = 1.0f;
  bool rescale = false;

  /* Window focus */
  bool wMouseFocus;
  bool wKeyboardFocus;
  bool wFullScreen;
  bool wMinimized;

  /* The name of the window */
  std::string windowName= "";
};

#endif