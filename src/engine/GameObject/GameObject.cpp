#include "GameObject.hpp"
#include "Colliders/BoxCollider.hpp"

/*
 * Baseplate object in game space, contains a vector of components.
 * 
 * \param position The initial position of the object. 
 * \param scale The scale, or size, of the object.
 * \param startEnabled Whether or not the object is enabled on creation. `true` by default.
 */
GameObject::GameObject(Vector2 position, Vector2 scale, bool startEnabled)
: pos(position), scale(scale), enabled(startEnabled)
{

}

GameObject::~GameObject()
{
    Destroy();
}

/* Removes all components, and components' components */
void GameObject::Destroy() {
    for (auto& comp : components) comp->Destroy();
    components.clear();
}

/* Update's the game object. Does nothing by default, redefine in subclass definitions. */
void GameObject::Update()
{
    
}

/* Calls `Update` for all object components. Calls `UpdateComponents` for all components as well. */
void GameObject::UpdateComponents()
{
    if (enabled) {
        for (int i=0; i<components.size(); i++) {
            auto comp = components[i];
            if (comp!=nullptr && comp->Enabled()) comp->Update();
            if (comp!=nullptr && comp->Enabled()) comp->UpdateComponents();
        }
    }
}

/* Turn the object on/off. Sets enabled for all component objects. */
void GameObject::SetEnabled(bool enable) 
{
    // assign for self
    enabled = enable;
    // set enabled for all component objects 
    for (auto& component : components) {
        component->SetEnabled(enable);
    }
}

/* Assign the object's position. Moves all component objects. */
void GameObject::SetPosition(Vector2 newPosition) 
{
    Vector2 disp = newPosition - pos;
    // assign position for self
    pos = newPosition;
    // move components according to displacement
    for (auto& component : components) {
        component->SetPosition(component->Position() + disp);
    }
}

/* Sets this object's scale. Resizes all component objects accoridngly. */
void GameObject::SetScale(Vector2 newScale) 
{
    Vector2 diff = {newScale.x/scale.x, newScale.y/scale.y};
    // resize for self
    scale = newScale;
    // resize all components accordingly
    for (auto& component : components) {
        component->SetScale(component->Scale()*diff);
    }
}

/* Assigns components given a shared pointer to itself. Does nothing, shoudl be redefined in subclass definitiions. */
void GameObject::AssignComponents(std::shared_ptr<GameObject> self)
{

}

/* Adds a new component to the object. */
void GameObject::AddComponent(std::shared_ptr<GameObject> component) {
    components.push_back(component);
}

/* Removes the specified object as a component of the game object */
void GameObject::RemoveComponent(std::shared_ptr<GameObject> obj)
{
    // search each component object
    for (auto it = components.begin(); it != components.end(); it++) {
        // if the object is found, remove it and end the search
        if (*it == obj) {
            (*it)->Destroy();
            components.erase(it);
            break;
        }
    }
}

/* Behaviour for first collision with an object. Redefine in subclass definitions */
void GameObject::OnCollisionEnter(Collision collision)
{

}

/* Behaviour for ongoing collision with an object. Redefine in subclass definitions */
void GameObject::OnCollisionStay(Collision collision)
{

}

/* Behaviour for breaking collision with an object. Redefine in subclass definitions */
void GameObject::OnCollisionExit(std::shared_ptr<GameObject> other)
{

}

/* Whether or not the object is currently active. */
bool GameObject::Enabled() const { return enabled; }
/* The current position of the object. */
Vector2 GameObject::Position() const { return pos; }
/* The current scale, or size, of the object. */
Vector2 GameObject::Scale() const { return scale; }