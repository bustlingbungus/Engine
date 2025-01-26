#include "Rigidbody.hpp"
#include "PhysicsConstants.hpp"
#include "../../TimeTracker.hpp"

/*
 * Component for moving colliding objects apart using elastic collision.
 * 
 * Makes use of a box collider for collision detection. The `BoxCollider` component of 
 * `object` will be used. If none exist, a `BoxCollider` will be added to `object`.
 * 
 * \param object The object that the component is attached to.
 * \param mass The mass of the object.
 * \param frictionCoefficient the coefficient of friction
 * \param velocity The object's starting velocity. `(0,0)` by default.
 * \param isMoveable Whether or not the object may be moved by other rigidbodies. `true` by default
 * \param hasGravity Whether or not the object is affected by gravity. `true` by default.
 * \param startEnabled Whether or not the component is active on creation. `true` by default.
 */
Rigidbody::Rigidbody(std::shared_ptr<GameObject> object, float mass, float frictionCoefficient, Vector2 velocity, bool isMoveable, bool hasGravity, bool startEnabled)
: ObjectComponent(object, startEnabled), 
  mass(mass), velocity(velocity), is_moveable(isMoveable), has_gravity(hasGravity), friction_coeff(frictionCoefficient)
{
    // get a reference to the object's box collider component
    collider = obj->GetComponent<BoxCollider>();
    // if the object has no box collider, add one
    if (collider == nullptr) {
        collider = obj->AddComponent<BoxCollider>(obj, startEnabled);
    }
}

Rigidbody::~Rigidbody()
{
    Destroy();
}

void Rigidbody::Destroy()
{
    ObjectComponent::Destroy();
    collider = nullptr;
}

/*  
 * Performs collision behaviour with all other rigidbodies.
 * Updates velocity based on acceleration, and position based on velocity.
 * 
 * Sets the position of parent object after all checks.
 */
void Rigidbody::Update()
{
    // collide with every object in collision with the box collider
    // perform collision behaviour with all objects the collider is in contact with 
    auto collisions = collider->GetCollisions();
    for (auto& c : collisions) OnCollision(c);

    // update based on velocity and acceleration
    Vector2 pos = obj->Position() + (velocity * gTime.deltaTime());
    obj->SetPosition(pos);
    Vector2 accel = (has_gravity)? acceleration+physicsConstants.gravity : acceleration;
    velocity += accel * gTime.deltaTime();
}

/* If the collision is with another enabled rigidbody, perform elastic collision with respect to the other object. */
void Rigidbody::OnCollision(Collision collision)
{
    // get the rigidbody component from the other object 
    auto other = collision.other->GetComponent<Rigidbody>();

    // valididate the collision
    if ((other != nullptr&&other->Enabled()) && (is_moveable||other->Moveable())) {

        validate_collision(collision.point, other);
    }
}

void Rigidbody::validate_collision(Vector2 p, std::shared_ptr<Rigidbody> other)
{
    // get other required components from object 
    auto other_collider = other->GetCollider();
    RectF other_bbox = other_collider->GetBoundingBox();

    // find the displacement between the point and the bounding box's centre
    Vector2 disp = other->obj->Position() - p;
    
    // find the depth of the collision point into the other object
    Vector2 vec(
        (other_bbox.w/2.0f) - abs(disp.x),
        (other_bbox.h/2.0f) - abs(disp.y)
    );

    // further validate the collision
    if (vec==Vector2_Zero || vec.x<0.0f || vec.y<0.0f) return;
    
    // only collide along one axis by setting one axis to 0
    // modify the collision axis to represent the direction the the objects need to be moved apart
    if (vec.x<vec.y) {
        vec.y = 0.0f; vec.x = (vec.x+physicsConstants.rigidbodySeperation)*sign(disp.x);
    } else {
        vec.x = 0.0f; vec.y = (vec.y+physicsConstants.rigidbodySeperation)*sign(disp.y);
    }

    // perform collision
    rigidbody_collision(other, vec);
}

/* Calls all helper functions for elastic collision, updating both obejcts */
void Rigidbody::rigidbody_collision(std::shared_ptr<Rigidbody> other, Vector2 disp)
{
    // ensure objects are no longer touching
    push_objects_apart(other, disp);

    Vector2 other_vel = other->Velocity();

    // perform elastic collision when both objects are moveable
    if (is_moveable && other->Moveable()) elastic_collision(other_vel, other->Mass());

    // otherwise, cancel the velocity of the moveable object along the appropriate axis
    else {
        Vector2 *v = (is_moveable)? &velocity : &other_vel;
        if (disp.x==0.0f) v->y = 0.0f;
        else v->x = 0.0f;
    }

    // apply friction to objects
    apply_friction(other_vel, other->FrictionCoefficient(), other->Moveable());

    // update other object's velocity
    other->SetVelocity(other_vel);
}

/*
 * Performs elastic collision with another rigidboy's mass and velocity.
 * Assumes perfectly elastic collision, and uses `m1v1i + m2v2i = m1v1f + m2v2f`.
 * Calculates `v1f` and `v2f`, assigning `v1f` to `velocity`, and `v2f` to `other_vel`. 
 */
void Rigidbody::elastic_collision(Vector2& other_vel, float other_mass)
{
    // the starting velocities
    Vector2 v1i = velocity, v2i = other_vel;
    // v2f relative to v1f (v2f = v1f+this)
    Vector2 v2f_relative = v1i - v2i;

    // the constant value on the right of the equation
    Vector2 right_const = v2f_relative * other_mass;
    // the coefficient of v1f on the right of the equation
    Vector2 right_coeff = mass + other_mass;

    // v1f = (left of equation - constant on right) / coefficient on right
    // | is hadamard product as division
    Vector2 v1f = ((v1i*mass)+(v2i*other_mass) - right_const) |right_coeff;
    // find v2f relative to v1f
    Vector2 v2f = v1f + v2f_relative;

    // apply new velocities
    velocity = v1f; other_vel = v2f;
}

/*
 * Applies the other object's friction to own velocity, and own friction to `other_vel`.
 * Does this by lerping between original velocity and the other object's velocity,
 * using the other object's friction coefficient as an interpolator.
 * 
 * \note friction is only applied to an object if it is moveable.
 */
void Rigidbody::apply_friction(Vector2& other_vel, float other_friction, bool other_moveable)
{
    Vector2 temp = velocity;
    if (is_moveable) velocity = lerp(velocity, other_vel, other_friction);
    if (other_moveable) other_vel = lerp(other_vel, temp, friction_coeff);
}

/*
 * Pushes two objects apart by half the displacement vector, by moving each
 * object's position by half the displacement. If one object is immovable,
 * then the other is moved by the full displacement amount.
 */
void Rigidbody::push_objects_apart(std::shared_ptr<Rigidbody> other, Vector2 disp)
{
    Vector2 pos = obj->Position(), other_pos = other->obj->Position();

    // move objects apart
    if (!is_moveable) other_pos += disp;
    else if (!other->Moveable()) pos -= disp;
    else {
        // if both objects are movable, both will 
        // move by half the displacement
        disp /= 2.0f;
        other_pos += disp; pos -= disp;
    }
    // update object positions
    obj->SetPosition(pos); other->obj->SetPosition(other_pos);

    // remove collisions, to avoid repeat rigidbody behaviour
    auto col = other->GetCollider();
    collider->remove_collision(other->obj, col);
    col->remove_collision(obj, collider);
}

/* The collider object that the rigidbody's parent uses for collision detection. */
std::shared_ptr<BoxCollider> Rigidbody::GetCollider() const { return collider; }
/* The mass of the object. */
float Rigidbody::Mass() const { return mass; }
/* Coefficient of friction */
float Rigidbody::FrictionCoefficient() const { return friction_coeff; }
/* Whether or not the object may be moved by other rigidbodies. */
bool Rigidbody::Moveable() const { return is_moveable; }
/* Whether or not the object is affected by gravity. */
bool Rigidbody::GravityEnabled() const { return has_gravity; }
/* Rate of change of position. */
Vector2 Rigidbody::Velocity() const { return velocity; }
/* Rate of change of velocity */
Vector2 Rigidbody::Acceleration() const { return acceleration; }
/* The collider object that the rigidbody's parent uses for collision detection. */
std::shared_ptr<BoxCollider> Rigidbody::Collider() const { return collider; }

/* Assign the rigidbody's mass */
void Rigidbody::SetMass(float newMass) { mass = newMass; }
/* Assign the object's friction coefficient */
void Rigidbody::SetFrictionCoefficient(float frictionCoefficient) { friction_coeff = frictionCoefficient; }
/* Set whether or not the object may be moved by other rigidbodies. */
void Rigidbody::MakeMoveable(bool moveable) { is_moveable = moveable; }
/* Assign object velocity */
void Rigidbody::SetVelocity(Vector2 newVelocity) { velocity = newVelocity; }
/* Assign object Acceleration */
void Rigidbody::SetAcceleration(Vector2 newAcceleration) { acceleration = newAcceleration; }
/* Set whether or not the object is affected by gravity. */
void Rigidbody::EnableGravity(bool gravityEnabled) { has_gravity = gravityEnabled; }