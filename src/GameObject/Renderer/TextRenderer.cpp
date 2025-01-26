#include "TextRenderer.hpp"
#include "../../GlobalWindow.hpp"
#include <deque>

/* 
 * Returns the original string as a vector of substrings seperated by 
 * newline characters in the original string.
 */
std::vector<std::string> GetStrings(std::string original)
{
    std::vector<std::string> res;
    std::string substr = "";
    // check each character in the original string
    for (char& ch : original)
    {
        // when a newline is encountered, add the current substring to the output
        if (ch == '\n') {
            // ensure strings are not of size 0 (for converting to texture later)
            if (substr.size()==0) substr = " ";
            res.push_back(substr);
            // reset substring 
            substr = "";
        // for all other characters, add them to the current substring until a newline is found
        } else substr += ch;
    }
    // add whatever was not handled in the loop
    if (substr.size()!=0) res.push_back(substr);

    // return list of individual substrings
    return res;
}

/*
 * A Texture renderer whose texture is a rendered string.
 * The string will be split into multiple different strings based on `newline` characters,
 * Where each newline will result in the text's strings being rendered on a different line.
 * 
 * \param object The game object the component is attached to.
 * \param text The text that will be rendered.
 * \param font The font that will be used for rendering. Leave as `nullptr` to use global font.
 * \param z Value used to determine which order objects are rendered in.
 * \param useZoom Whether or not the texture is affected by camera zoom.
 * \param startEnabled Whether or not the component is active on creation. `true` by default.
 */
TextRenderer::TextRenderer(std::shared_ptr<GameObject> object, std::string text, std::shared_ptr<LFont> font, int z, bool useZoom, bool startEnabled)
: TextureRenderer(object, std::make_shared<LTexture>(gWindow), z, useZoom, startEnabled),
  text(text), fnt((font==nullptr)?gWindow->gFont:font), native_scale(object->Scale())
{
    create_texture();
}

TextRenderer::~TextRenderer()
{
    Destroy();
}

void TextRenderer::Destroy()
{
    TextureRenderer::Destroy();
    fnt = nullptr;
}

/* The string that gets rendered. */
std::string TextRenderer::Text() const { return text; }
/* The font used for text rendering. */
std::shared_ptr<LFont> TextRenderer::Font() const { return fnt; }

/* Change what string is rendered. Recreates the texture using the same font, but new string. */
void TextRenderer::SetText(std::string newText)
{
    text = newText;
    create_texture();
}

/* 
 * Change what font is used for rendering. 
 * Recreates the texture using the same text, but new font. 
 * If a `nullptr` is provided, the default global font will be used.
 */
void TextRenderer::SetFont(std::shared_ptr<LFont> newFont)
{
    fnt = (newFont==nullptr)? gWindow->gFont : newFont;
    create_texture();
}

/* 
 * Creates the object's texture using the stored font and string text. 
 * Resizes the parent object to match the dimensions of the new texture.
 */
void TextRenderer::create_texture()
{
    auto strings = GetStrings(text);

    // turn all strings into textures, and store in deque
    std::deque<std::shared_ptr<LTexture>> textures;
    // track the width and height required to store all the textures
    int w = 0, h = 0;
    for (auto& str : strings)
    {
        // create texture
        auto tex = std::make_shared<LTexture>(gWindow);
        tex->loadFromRenderedText(str, fnt->getColour(), fnt->get());
        // add texture to deque
        textures.push_front(tex);
        // readjust width and height
        h += tex->getHeight();
        w = max(w, tex->getWidth());
    }

    // create a blank texture with the dimensions calculated
    texture->solidColour({0,0,0,0}, w, h);

    // render each string texture onto the base, centred
    int y = 0;
    while (!textures.empty())
    {
        // get texture from back
        auto tex = textures.back();
        textures.pop_back();
        // find the centre point
        int x = (w-tex->getWidth())/2;
        // render string in place
        tex->render_toTexture(texture.get(), x, y);
        // increment y coordinate
        y += tex->getHeight();
    }

    Vector2 scale(texture->getWidth(), texture->getHeight());
    // resize object based on new texture dimensions
    obj->SetScale(scale & native_scale);
}