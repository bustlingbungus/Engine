#pragma once

#include "TextureRenderer.hpp"

std::vector<std::string> GetStrings(std::string original);

/* A Texture renderer whose texture is a rendered string. */
class TextRenderer : public TextureRenderer
{
    public:

        TextRenderer(std::shared_ptr<GameObject> object, std::string text, std::shared_ptr<LFont> font = nullptr, int z = 0, bool use_zoom = true, bool startEnabled = true);
        ~TextRenderer();

        virtual void Destroy();

        std::string Text() const;
        std::shared_ptr<LFont> Font() const;

        virtual void SetText(std::string newText);
        virtual void SetFont(std::shared_ptr<LFont> newFont);

        virtual void create_texture();

    private:

        /* The string that gets rendered. */
        std::string text = "";
        /* The font used for text rendering. */
        std::shared_ptr<LFont> fnt;
};