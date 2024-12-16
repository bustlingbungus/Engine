#pragma once

#include "../GameObject.hpp"

#include <string>

/* Game object with a name. Intended to serve as a root container for other game objects. */
class Scene : public GameObject
{
    public:

        Scene(std::string name);
        ~Scene();

        virtual void Destroy();

        std::string name;
};