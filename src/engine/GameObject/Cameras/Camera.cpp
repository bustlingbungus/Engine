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
 */
Camera::Camera(Vector2 position, Vector2 scale, bool startEnabled)
: GameObject(position, scale, startEnabled)
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

/* Renders every enabled TextureRenderer in the global rendererManager vector */
void Camera::Update()
{
    // centre camera rect on object position
    Vector2 origin = Position()-(Scale()/2.0f);
    cam = {origin.x, origin.y, (float)WND_WIDTH, (float)WND_HEIGHT};

    // iterate through each renderer
    for (auto& renderer : renderManager.renderers) {
        // if the renderer is enabled, render it with reference to this camera 
        if (renderer->Enabled()) {
            renderer->Render(this);
        }
    }
}