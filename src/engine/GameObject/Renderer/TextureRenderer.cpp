#include "TextureRenderer.hpp"
#include "../Cameras/Camera.hpp"
#include "RenderManager.hpp"

/*
 * Stores a texture to be rendered to all applicable cameras.
 *
 * Adds the component to a global list of all TextureRenderers, for rendering
 * purposes. 
 *
 * \param object The game object the component is attached to 
 * \param texture The texture that will be rendered
 * \param startEnabled Whether or not the component is active on creation. `true` by default.
 */
TextureRenderer::TextureRenderer(std::shared_ptr<GameObject> object, std::shared_ptr<LTexture> texture, bool startEnabled)
: ObjectComponent(object, startEnabled), texture(texture)
{
    renderManager.AddRenderer(this);
}

/* Removes the object from global TextureRenderers list */
TextureRenderer::~TextureRenderer()
{
    Destroy();
}

/* Removes the object from global TextureRenderers list */
void TextureRenderer::Destroy()
{
    ObjectComponent::Destroy();
    renderManager.RemoveRenderer(this);
    texture = nullptr;
}

/* 
 * Sets the dimensions of renderer's rect to the object's scale.
 * Centre's the rect on the object's position.
 */
void TextureRenderer::Update()
{
    Vector2Int scale = obj->Scale();
    Vector2Int origin = obj->Position() - (scale/2);
    rect = {origin.x,origin.y,scale.x,scale.y};
}

/* Render the stored texture relative to a camera. Doesn't render is outside the camera's view. */
void TextureRenderer::Render(Camera *camera)
{
    if (texture != nullptr) 
    {
        // camera region
        RectF cam = camera->camera();
        // outside of camera view, don't render
        if (rect.x<cam.x-rect.w || rect.x>cam.x+cam.w ||
            rect.y<cam.y-rect.h || rect.y>cam.y+cam.h) return;
        
        // set up rect for rendering based on the camera's position
        SDL_Rect rend_rect = {rect.x-(int)cam.x, rect.y-(int)cam.y, rect.w, rect.h};
        // render the texture with respect to the rect
        texture->render(&rend_rect);
    }
}