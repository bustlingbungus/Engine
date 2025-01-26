#include "LFont.hpp"

/*
 * Font management object.
 *
 * \param size the point size
 * \param filename the file to load the font from
 * \param colour RGBS text colour
 */
LFont::LFont(int size, std::string filename, SDL_Color colour)
: size(size), filename(filename), colour(colour)
{
    // initalise font
    if (!load(filename, size)) {
        printf("Failed to initialise font!");
    }
}

/* Deallocate resources */
LFont::~LFont()
{
    free();
}

/* Deallocates the font, and clears member variables */
void LFont::free(bool clearFilename)
{
    if (font!=NULL) TTF_CloseFont(font);
    font = NULL;
    size = -1;
    colour = {255,255,255,255};
    if (clearFilename) filename = "";
}

/*
 * Load's the a font from file with the given filename.
 * Does nothing and returns `true` when trying to reload an already loaded font with the same size.
 *
 * \param newFilename the file to load the font from 
 * \param ptsize the point size for the loaded font. must be positive. 12 by default
 * \returns `true` if the creation was successful, or if nothing needed to be done
 */
bool LFont::load(std::string newFilename, int ptSize)
{
    // don't load the same file with the same size while the font exists
    if (font==NULL || (ptSize>0&&ptSize!=size) || (newFilename!=""&&filename!=newFilename))
    {
        // clear existing font
        free();

        // update members 
        size = ptSize;
        filename = newFilename;

        // load font from file 
        font = TTF_OpenFont(filename.c_str(), size);
        // error handling
        if (font == NULL) {
            printf("Failed to load %s! TTF Error: %s", filename.c_str(), TTF_GetError());
            free();
        }
    }
    // return success
    return font != NULL;
}

/*
 * Set the font's point size by reloading the font from file with the desired size.
 *
 * \param ptsize the new point size. must be positve
 * \returns `true` for successful font creation. `false` if the function fails 
 */
bool LFont::setPtSize(int ptsize)
{
    // don't load if the stored file name is invalid, or if there is no size change
    if (filename != "" && (ptsize>0&&ptsize!=size))
    {
        // clear existing font
        free(false); // don't wipe filename

        // update size member
        size = ptsize;

        // load the new font size
        font = TTF_OpenFont(filename.c_str(), size);
        // error handling 
        if (font == NULL) {
            printf("Failed to change font size! TTF Error: %s", TTF_GetError());
            free();
        }
    }
    // return success
    return font != NULL;
}

/* Assign font colour */
void LFont::setColour(SDL_Color col) { colour = col; }

/* A raw pointer to the TTF_Font */
TTF_Font* LFont::get() const { return font; }
/* The point size */
int LFont::getPtSize() const { return size; }
/* Text colour */
SDL_Color LFont::getColour() const { return colour; }