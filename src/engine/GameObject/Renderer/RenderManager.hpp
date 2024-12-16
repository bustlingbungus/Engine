#pragma once

#include <vector>
#include <memory>

// forwards declaration
class TextureRenderer;

/* Stores all `TextureRenderer` components, to be used by camera objects when rendering. */
class RenderManager
{
    public:

        RenderManager();
        ~RenderManager();

        void AddRenderer(TextureRenderer *renderer);
        void RemoveRenderer(TextureRenderer *renderer);

        /* Vector of pointers to all TextureRenderers. */
        std::vector<TextureRenderer*> renderers;
};

// create global object
extern RenderManager renderManager;