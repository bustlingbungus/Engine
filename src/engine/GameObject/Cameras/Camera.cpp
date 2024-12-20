#include "Camera.hpp"
#include "../../GlobalWindow.hpp"
#include "../Renderer/RenderManager.hpp"
#include "../Renderer/TextureRenderer.hpp"
#include <iostream>

/*
 * Camera that renders all `TextureRenderer`s to an area on the screen.
 *
 * \param position The position of the camera in game
 * \param scale The scale, or size, of the rendering region 
 * \param zoom How big/small the camera renders things.
 * \param startEnabled Whether or not the object is enabled on creation. `true` by default.
 */
Camera::Camera(Vector2 position, Vector2 scale, float zoom, bool startEnabled)
: GameObject(position, scale, startEnabled), zoom(zoom)
{
    // set up the in game rect for the camera
    Vector2 view = scale & Vector2(gWindow->getWidth(), gWindow->getHeight()); // hadamard product
    Vector2 origin = position-(view/2.0f);
    cam = {origin.x, origin.y, view.x, view.y};
}

Camera::~Camera() 
{
    Destroy();
}

void Camera::Destroy()
{
    GameObject::Destroy();
}

/* The region and position of the region viewed by the camera. */
RectF Camera::camera() const { return cam; }
/* How big/small the camera renders things. */
float Camera::Zoom() const { return zoom; }
/* Set how big/small the camera renders things. */
void Camera::SetZoom(float newZoom) { zoom = newZoom; }

/* Renders every enabled TextureRenderer in the global rendererManager vector */
void Camera::Update()
{
    // centre camera rect on object position
    Vector2 view = Scale() & Vector2(gWindow->getWidth(), gWindow->getHeight()), 
            origin = Position()-(view/2.0f);
    cam = {origin.x, origin.y, view.x, view.y};

    // iterate through each renderer
    for (auto& renderer : renderManager.renderers) {
        // if the renderer is enabled, render it with reference to this camera 
        if (renderer->Enabled()) {
            renderer->Render(this);
        }
    }
}