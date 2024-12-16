#include "RenderManager.hpp"
#include "TextureRenderer.hpp"

RenderManager::RenderManager()
{

}

RenderManager::~RenderManager()
{
    renderers.clear();
}

/* Add a renderer object to the global vector. */
void RenderManager::AddRenderer(TextureRenderer *renderer)
{
    renderers.push_back(renderer);
}

/* 
 * Removes the TextureRenderer from the vector. If the pointer is included multiple
 * times, only the first instance is removed. Does nothing if the TextureRenderer
 * is not in the vector.
 */
void RenderManager::RemoveRenderer(TextureRenderer *renderer)
{
    // iterate through all pointers
    for (auto it=renderers.begin(), e = renderers.end(); it!=e; it++) {
        // when found, remove the matching element and end search
        if (*it == renderer) {
            renderers.erase(it);
            break;
        }
    }
}

// create global manager object
RenderManager renderManager = RenderManager();