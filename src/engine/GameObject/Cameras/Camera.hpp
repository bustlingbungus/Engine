#pragma once

#include "../GameObject.hpp"

/* Camera that renders all `TextureRenderer`s to an area on the screen */
class Camera : public GameObject
{
    public:

        Camera(Vector2 position, Vector2 scale = Vector2_One, bool startEnabled = true);
        ~Camera();

        virtual void Destroy();

        virtual void Update();

        RectF camera() const;
        
    private:

        /* Position of the camera in game, and dimensions of the render region. */
        RectF cam;
};