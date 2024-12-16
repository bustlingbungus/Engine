#pragma once

#include <vector>
#include <memory>

// forwards declaration
class BoxCollider;

/* Stores all `BoxCollider` components, to be used for collision detection. */
class ColliderManager
{
    public:

        ColliderManager();
        ~ColliderManager();

        void AddCollider(BoxCollider *collider);
        void RemoveCollider(BoxCollider *collider);

        /* Vector of pointers to all BoxColliders. */
        std::vector<BoxCollider*> colliders;
};

// create global object
extern ColliderManager colliderManager;