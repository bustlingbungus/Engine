#ifndef LFONT_HPP
#define LFONT_HPP

#include <SDL.h>
#include <SDL_ttf.h>

#include <string>

class LFont
{
    public:

        LFont(int ptsize = 12, std::string filename = "", SDL_Color colour = {255,255,255,255});
        ~LFont();

        void free(bool clearFilename = true);

        bool load(std::string newFilename, int ptsize = 12);
        bool setPtSize(int ptsize);
        void setColour(SDL_Color col);

        TTF_Font* get() const;
        int getPtSize() const;
        SDL_Color getColour() const;

    private:

        /* Raw pointer to the font object */
        TTF_Font *font = NULL;
        /* The point size of the font */
        int size = -1;

        /* Text colour */
        SDL_Color colour;

        /* The file the font is loaded from */
        std::string filename = "";
};

#endif