#include "ColliderManager.hpp"
#include "BoxCollider.hpp"

ColliderManager::ColliderManager()
{

}

ColliderManager::~ColliderManager()
{
    colliders.clear();
}

/* Add a collider object to the global vector. */
void ColliderManager::AddCollider(BoxCollider *collider)
{
    colliders.push_back(collider);
}

/* 
 * Removes the BoxCollider from the vector. If the pointer is included multiple
 * times, only the first instance is removed. Does nothing if the BoxCollider
 * is not in the vector.
 */
void ColliderManager::RemoveCollider(BoxCollider *collider)
{
    // iterate through all pointers
    for (auto it=colliders.begin(), e = colliders.end(); it!=e; it++) {
        // when found, remove the matching element and end search
        if (*it == collider) {
            colliders.erase(it);
            break;
        }
    }
}

// create global manager object
ColliderManager colliderManager = ColliderManager();