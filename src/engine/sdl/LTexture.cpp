#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <sstream>
#include <stdio.h>
#include <string>

#include "LTexture.hpp"

LTexture::LTexture(std::shared_ptr<LWindow> gHolder) : gHolder(gHolder)
{
  mTexture = NULL;
  gHolder = NULL;
  mWidth = 0;
  mHeight = 0;
}

LTexture::~LTexture()
{
  // Deallocate
  free();
}

#if defined(SDL_TTF_MAJOR_VERSION)
bool LTexture::loadFromRenderedText(std::string textureText,
                                    SDL_Color textColor, TTF_Font *font)
{
  // Get rid of preexisting texture
  free();

  if (font == NULL) font = gHolder->gFont->get();

  // Render text surface
  SDL_Surface *textSurface =
      TTF_RenderText_Solid(font, textureText.c_str(), textColor);
  if (textSurface != NULL)
  {
    // Create texture from surface pixels
    mTexture = SDL_CreateTextureFromSurface(gHolder->gRenderer, textSurface);
    if (mTexture == NULL)
    {
      printf("Unable to create texture from rendered text! SDL Error: %s\n",
             SDL_GetError());
    }
    else
    {
      // Get image dimensions
      mWidth = textSurface->w;
      mHeight = textSurface->h;
    }

    // Get rid of old surface
    SDL_FreeSurface(textSurface);
  }
  else
  {
    printf("Unable to render text surface! SDL_ttf Error: %s\n",
           TTF_GetError());
  }

  // Return success
  return mTexture != NULL;
}
#endif

/* 
 * Creates a texture of a uniform colour
 *
 * \note created texture will have pixel format `SDL_PIXELFORMAT_RGBA8888`
 * and texture access `SDL_TEXTUREACCESS_TARGET`
 * 
 * \param colour the RGBA colour
 * \param width, height the dimensions of the created texture
 * 
 * \return `true` for successful texture creation.
 * \return `false` for failure.
 */
bool LTexture::solidColour(SDL_Color colour, int width, int height)
{
  // free pre-existing texture
  free();
  // create empty texture
  mTexture = SDL_CreateTexture(gHolder->gRenderer, 
              SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 
              width, height);
  // error handling
  if (mTexture == NULL) {
    printf("Failed to create solid colour! SDL_Error: %s", SDL_GetError());
  } else {
    // prepare texture and renderer
    SDL_SetTextureBlendMode(mTexture, SDL_BLENDMODE_BLEND);
    SDL_SetRenderTarget(gHolder->gRenderer, mTexture);
    SDL_SetRenderDrawBlendMode(gHolder->gRenderer, SDL_BLENDMODE_NONE);
    SDL_SetRenderDrawColor(gHolder->gRenderer, colour.r, colour.g, colour.b, colour.a);
    // fill texture with colour
    SDL_RenderFillRect(gHolder->gRenderer, NULL);
    // reset renderer for rendering 
    SDL_SetRenderTarget(gHolder->gRenderer, NULL);
    SDL_SetRenderDrawBlendMode(gHolder->gRenderer, SDL_BLENDMODE_BLEND);
    // set width and height members
    mWidth = width;
    mHeight = height;
  }
  // return success
  return mTexture != NULL;
}

bool LTexture::loadFromFile(std::string path)
{
  // Get rid of preexisting texture
  free();

  // The final texture
  SDL_Texture *newTexture = NULL;

  // Load image at specified path
  SDL_Surface *loadedSurface = IMG_Load(path.c_str());
  if (loadedSurface == NULL)
  {
    printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(),
           IMG_GetError());
  }
  else
  {
    // Color key image
    SDL_SetColorKey(loadedSurface, SDL_TRUE,
                    SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

    // Create texture from surface pixels
    newTexture =
        SDL_CreateTextureFromSurface(gHolder->gRenderer, loadedSurface);
    if (newTexture == NULL)
    {
      printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(),
             SDL_GetError());
    }
    else
    {
      // Get image dimensions
      mWidth = loadedSurface->w;
      mHeight = loadedSurface->h;
    }

    // Get rid of old loaded surface
    SDL_FreeSurface(loadedSurface);
  }

  // Return success
  mTexture = newTexture;
  return mTexture != NULL;
}

void LTexture::free()
{
  // Free texture if it exists
  if (mTexture != NULL)
  {
    SDL_DestroyTexture(mTexture);
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
  }
}

void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
  // Modulate texture rgb
  SDL_SetTextureColorMod(mTexture, red, green, blue);
}

void LTexture::setBlendMode(SDL_BlendMode blending)
{
  // Set blending function
  SDL_SetTextureBlendMode(mTexture, blending);
}

void LTexture::setAlpha(Uint8 alpha)
{
  // Modulate texture alpha
  SDL_SetTextureAlphaMod(mTexture, alpha);
}

void LTexture::renderAsBackground(SDL_Rect *clip, double angle,
                                  SDL_Point *center, SDL_RendererFlip flip)
{
  // Set rendering space and render to screen
  SDL_Rect renderQuad = {0, 0, gHolder->getWidth(), gHolder->getHeight()};

  // Render to screen
  SDL_RenderCopyEx(gHolder->gRenderer, mTexture, clip, &renderQuad, angle,
                   center, flip);
}

void LTexture::render(int x, int y, SDL_Rect *dest, SDL_Rect *clip, double angle,
                      SDL_Point *center, SDL_RendererFlip flip)
{
  // Set rendering space and render to screen
  SDL_Rect renderQuad = {x, y, mWidth, mHeight};

  // Set dest rendering dimensions
  if (dest != NULL)
  {
    renderQuad.w = dest->w;
    renderQuad.h = dest->h;
  }

  // Render to screen
  SDL_RenderCopyEx(gHolder->gRenderer, mTexture, clip, &renderQuad, angle,
                   center, flip);
}

void LTexture::render(SDL_Rect *dest, SDL_Rect *clip, double angle,
                      SDL_Point *center, SDL_RendererFlip flip)
{
  // Render to screen
  SDL_RenderCopyEx(gHolder->gRenderer, mTexture, clip, dest, angle,
                   center, flip);
}

// renders to another texture
void LTexture::render_toTexture(LTexture *target, int x, int y, SDL_Rect *dest, SDL_Rect *clip, double angle,
            SDL_Point *center, SDL_RendererFlip flip)
{
  // set the renderer to target the other's mtexture
  if (SDL_SetRenderTarget(gHolder->gRenderer, target->mTexture) < 0) {
    printf("Failed to render to texture! SDL Error: %s\n", SDL_GetError());
    return;
  }
  render(x, y, dest, clip, angle, center, flip);

  // reset the render target
  if (SDL_SetRenderTarget(gHolder->gRenderer, NULL) < 0) {
    printf("Failed to reset render target! SDL Error: %s\n", SDL_GetError());
  }
}
// renders to another texture
void LTexture::render_toTexture(LTexture *target, SDL_Rect *dest, SDL_Rect *clip, double angle,
            SDL_Point *center, SDL_RendererFlip flip)
{
  // set the renderer to target the other's mtexture
  if (SDL_SetRenderTarget(gHolder->gRenderer, target->mTexture) < 0) {
    printf("Failed to render to texture! SDL Error: %s\n", SDL_GetError());
    return;
  }
  render(dest, clip, angle, center, flip);

  // reset the render target
  if (SDL_SetRenderTarget(gHolder->gRenderer, NULL) < 0) {
    printf("Failed to reset render target! SDL Error: %s\n", SDL_GetError());
  }
}

int LTexture::getWidth() { return mWidth; }

int LTexture::getHeight() { return mHeight; }


void renderText( std::string txt, int x, int y, std::shared_ptr<LWindow> window, SDL_Color colour, TTF_Font *font, TextOrientation orientation )
{
  // vector of strings to be rendered
  std::vector<std::string> strings;

  int n = txt.size(), last = 0, i;

  for (i = 0; i < n; i++) 
  {
    if (txt[i] == '\n') {
      strings.push_back(txt.substr(last, i-last));
      last = i+1;
    }
  }
  strings.push_back(txt.substr(last, i-last));

  n = strings.size();
  for (i = 0; i < n; i++) 
  {
    if (strings[i].size() == 0) continue;
    auto rend = std::make_unique<LTexture>(window);
    if (!rend->loadFromRenderedText(strings[i], colour, font)) {
      std::cerr << "failed to load rendered text!";
    }
    int X = (orientation == Centred)? x - (rend->getWidth()/2) : x;
    rend->render(X, y);
    y += rend->getHeight()+3;
    rend->free();
  }
}