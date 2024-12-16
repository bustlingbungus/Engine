#include "BoxCollider.hpp"
#include "ColliderManager.hpp"

/* 
 * Component that checks collisions with other BoxColliders,
 * and tracks which objects it is currently in collision with.
 * 
 * Adds this object to the globals colliders vector.
 * 
 * \param object The object that the component is attached to.
 * \param startEnabled Whether or not the component is active on creation. `true` by default.
 */
BoxCollider::BoxCollider(std::shared_ptr<GameObject> object, bool startEnabled)
: ObjectComponent(object, startEnabled)
{
    // set up bounding box
    Vector2 s = obj->Scale(), origin = obj->Position()-(s/2.0f);
    bounding_box = {origin.x, origin.y, s.x, s.y};
    // add self to global colliders vector
    colliderManager.AddCollider(this);
}

/* Remove self from global colliders vector */
BoxCollider::~BoxCollider()
{
    colliderManager.RemoveCollider(this);
    collisions.clear();
}

/*
 * Updates bounding box based on object position.
 * Checks for collision with all other BoxColliders. After `Update` is called,
 * `GetCollisions` will successfully return all objects currently in collision with.
 */
void BoxCollider::Update()
{
    // update bounding box
    Vector2 s = obj->Scale(), origin = obj->Position()-(s/2.0f);
    bounding_box = {origin.x, origin.y, s.x, s.y};
    
    // check other box colliders for collision
    if (Enabled()) {
        for (auto& other : colliderManager.colliders) {
            // don't collide with self or disabled objects
            if (other != this && other->Enabled()) {
                Vector2 p; // track point of collision
                if (CheckCollision(other, &p)) add_collision(other, p);
                else remove_collision(other);
            }
        }
    }
}

/* The rectangular bounding box used for collision detection */
RectF BoxCollider::GetBoundingBox() const { return bounding_box; }

/*
 * Checks collision with another BoxCollider by checking if any of the corners
 * of this object's bounding box are within the other object's bounding box.
 * 
 * Stores the point of collision in a given pointer.
 * 
 * \param other The BoxCollider to check collision with.
 * \param p The pointer to store the point of collision in, if any.
 * 
 * \returns `true` for collision. `false` for no collision.
 */
bool BoxCollider::CheckCollision(BoxCollider *other, Vector2 *p)
{
    // check if any of the bounding box's corners are within the other's bounding box

    // top left
    *p = Vector2(bounding_box.x, bounding_box.y);
    if (isInRect(*p, other->bounding_box)) return true;
    // top right
    p->x += bounding_box.w;
    if (isInRect(*p, other->bounding_box)) return true;
    // bottom right
    p->y += bounding_box.h;
    if (isInRect(*p, other->bounding_box)) return true;
    // bottom left
    p->x -= bounding_box.w;
    if (isInRect(*p, other->bounding_box)) return true;

    // not within the other object
    return false;
}

/* Returns a vector of all objects the collider is currently colliding with */
std::vector<Collision> BoxCollider::GetCollisions()
{
    // create and return a vector from the set
    std::vector<Collision> res;
    for (auto& p : collisions) res.push_back(p.second);
    return res;
}

/* Adds parent objects to both collider sets */
void BoxCollider::add_collision(BoxCollider *other, Vector2 p)
{
    Collision col = {other->obj, p};
    auto it = collisions.find(other->obj);
    if (it == collisions.end()) {
        // engage collision behaviour
        obj->OnCollisionEnter(col);
        // add objects to both collider sets
        collisions.emplace(other->obj, col);
        col.other = obj;
        other->collisions.emplace(obj, col);
        other->obj->OnCollisionEnter(col);
    } else {
        // engage collision stay behaviour
        obj->OnCollisionStay(col);
        col.other = obj;
        other->obj->OnCollisionStay(col);
    }
}

/* Removes game objects from respective collider sets */
void BoxCollider::remove_collision(BoxCollider *other)
{
    // remove item from collisions set
    auto it = collisions.find(other->obj);
    if (it != collisions.end()) {
        collisions.erase(it);
        obj->OnCollisionExit(other->obj);
    }

    // remove self from other collider's set
    it = other->collisions.find(obj);
    if (it != other->collisions.end()) {
        other->collisions.erase(it);
        other->obj->OnCollisionExit(obj);
    }
}