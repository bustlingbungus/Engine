#include "Scene.hpp"
#include "../Cameras/Camera.hpp"

/*
 * Game object with a name. Intended to serve as a root container for other game objects.
 *
 * \param name The name of the scene.
 */
Scene::Scene(std::string name)
: GameObject(), name(name)
{
    // add a camera to the scene by default
    auto cam = std::make_shared<Camera>(Vector2_Zero);
    AddComponent(cam);
}

Scene::~Scene()
{
    Destroy();
}

void Scene::Destroy()
{
    GameObject::Destroy();
}