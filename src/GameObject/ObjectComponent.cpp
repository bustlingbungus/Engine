#include "ObjectComponent.hpp"

/*
 * A component added to a game object that can modify the object.
 *
 * \param object The object that the component is attached to.
 * \param startEnabled Whether or not the component is active on creation. `true` by default.
 * 
 * \note creation of an object component for some object `obj` should be met with
 * calling `obj->AddComponent(newComponent)`, so that the new component actually 
 * gets updated alongside the object
 */
ObjectComponent::ObjectComponent(std::shared_ptr<GameObject> object, bool startEnabled)
: GameObject(Vector2_Zero, Vector2_One, startEnabled), obj(object)
{

}

ObjectComponent::~ObjectComponent()
{
    Destroy();
}

void ObjectComponent::Destroy()
{
    GameObject::Destroy();
    obj = nullptr;
}