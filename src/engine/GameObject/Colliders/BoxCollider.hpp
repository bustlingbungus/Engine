#pragma once

#include "../ObjectComponent.hpp"
#include <unordered_set>
#include <unordered_map>
#include <vector>

/* Container for information about a collision */
struct Collision {
    /* The other object being collided with */
    std::shared_ptr<GameObject> other;
    /* The point of collision */
    Vector2 point;
};

/* 
 * Component that checks collisions with other BoxColliders,
 * and tracks which objects it is currently in collision with.
 */
class BoxCollider : public ObjectComponent
{
    public:

        BoxCollider(std::shared_ptr<GameObject> object, bool startEnabled = true);
        ~BoxCollider();

        virtual void Update();

        RectF GetBoundingBox() const;

        bool CheckCollision(BoxCollider *other, Vector2 *p);

        std::vector<Collision> GetCollisions();

        void add_collision(BoxCollider *other, Vector2 p);
        void remove_collision(BoxCollider *other);
        
    private:

        /* The box used for collision detection, centred on the object's position */
        RectF bounding_box;
        /* Set of all objects currently being collided with */
        std::unordered_map<std::shared_ptr<GameObject>,Collision> collisions;
};