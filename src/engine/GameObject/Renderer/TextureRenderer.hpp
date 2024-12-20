#pragma once

#include "../ObjectComponent.hpp"

#include "../../sdl/LTexture.hpp"

// forwards declaration 
class Camera;

/* Stores a texture to be rendered to all applicable cameras */
class TextureRenderer : public ObjectComponent
{
    public:

        TextureRenderer(std::shared_ptr<GameObject> obj, std::shared_ptr<LTexture> texture, int z = 0, bool renderRelative = true, bool startEnabled = true);
        ~TextureRenderer();

        virtual void Destroy();

        virtual void Update();

        virtual void Render(Camera *camera);

        /* The texture being rendered */
        std::shared_ptr<LTexture> texture = nullptr;

        /* Integer rect representing the object's position and size. */
        RectF rect;

        int Z() const;
        void SetZ(int newZ);

    private:

        /* Value used to determine which order objects are rendered in. */
        int z;
        /* Whether or not the object is rendered relative to cameras, or on the window directly. */
        bool render_relative;
};