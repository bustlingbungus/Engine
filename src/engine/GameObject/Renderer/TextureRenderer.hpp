#include "../ObjectComponent.hpp"

#include "../../../sdl/LTexture.hpp"

// forwards declaration 
class Camera;

/* Stores a texture to be rendered to all applicable cameras */
class TextureRenderer : public ObjectComponent
{
    public:

        TextureRenderer(std::shared_ptr<GameObject> obj, std::shared_ptr<LTexture> texture, bool startEnabled = true);
        ~TextureRenderer();

        virtual void Update();

        virtual void Render(Camera *camera);

        /* The texture being rendered */
        std::shared_ptr<LTexture> texture = nullptr;

        /* Integer rect representing the object's position and size. */
        SDL_Rect rect;
};