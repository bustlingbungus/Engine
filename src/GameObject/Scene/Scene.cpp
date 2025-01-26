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
    AddComponent<Camera>(Vector2_Zero);
}

Scene::~Scene()
{
    Destroy();
}

void Scene::Destroy()
{
    GameObject::Destroy();
}

/* What happens when the scene is entered. Redefine in subclass definitions. */
void Scene::OnSceneEnter()
{

}

/* What happens when the scene is exited. Redefine in subclass definitions. */
void Scene::OnSceneExit()
{
    
}