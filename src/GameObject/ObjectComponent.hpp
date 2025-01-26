#pragma once

#include "GameObject.hpp"

/* A component added to a game object that can modify the object */
class ObjectComponent : public GameObject
{
    public:

        ObjectComponent(std::shared_ptr<GameObject> object, bool startEnabled = true);
        ~ObjectComponent();

        virtual void Destroy();

        /* Reference to the object the component modifies */
        std::shared_ptr<GameObject> obj;
};