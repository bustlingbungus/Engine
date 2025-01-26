#include "TextureRenderer.hpp"
#include "../Cameras/Camera.hpp"
#include "RenderManager.hpp"
#include "../../GlobalWindow.hpp"

/*
 * Stores a texture to be rendered to all applicable cameras.
 *
 * Adds the component to a global list of all TextureRenderers, for rendering
 * purposes. 
 *
 * \param object The game object the component is attached to 
 * \param texture The texture that will be rendered.
 * \param z Value used to determine which order objects are rendered in.
 * \param renderRelative Whether or not the object is rendered relative to cameras, or on the window directly.
 * \param startEnabled Whether or not the component is active on creation. `true` by default.
 */
TextureRenderer::TextureRenderer(std::shared_ptr<GameObject> object, std::shared_ptr<LTexture> texture, int z, bool renderRelative, bool startEnabled)
: ObjectComponent(object, startEnabled), texture(texture), z(z), render_relative(renderRelative)
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
    Vector2 scale = obj->Scale();
    Vector2 origin = obj->Position() - (scale/2.0f);
    rect = {origin.x,origin.y,scale.x,scale.y};
}

/* Render the stored texture relative to a camera. Doesn't render is outside the camera's view. */
void TextureRenderer::Render(Camera *camera)
{
    if (texture != nullptr) 
    {
        // camera region
        RectF cam = camera->camera();

        // set up rect for rendering based on the camera's position
        Vector2 origin(rect.x, rect.y), dim(rect.w, rect.h);
        // adjust for zoom
        if (render_relative) 
        {
            float zoom = camera->Zoom();

            Vector2 cp = camera->Position(), d = origin - cp;
            origin = cp + (d * zoom);
            dim *= zoom;
            origin.x -= cam.x;
            origin.y -= cam.y;
        }

        // create the rect
        SDL_Rect rend_rect = {
            int(origin.x), int(origin.y),
            int(dim.x), int(dim.y)
        };

        // outside of camera view, don't render
        if (rend_rect.x<-rend_rect.w || rend_rect.x>cam.w ||
            rend_rect.y<-rend_rect.h || rend_rect.y>cam.h) return;

        // render the texture with respect to the rect
        texture->render(&rend_rect);
    }
}

/* Value used to determine which order objects are rendered in. */
int TextureRenderer::Z() const { return z; }
/* Set the value used to determine which order objects are rendered in. */
void TextureRenderer::SetZ(int newZ) 
{ 
    z = newZ; 
    // remove and reinsert self to global map, to update the sorted order
    renderManager.RemoveRenderer(this);
    renderManager.AddRenderer(this);
}