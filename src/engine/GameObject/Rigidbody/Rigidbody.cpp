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
    if (Enabled()) 
    {
        // perform collision behaviour with all objects the collider is in contact with 
        auto collisions = collider->GetCollisions();
        for (auto& c : collisions) OnCollision(c);

        // update based on velocity and acceleration
        Vector2 pos = obj->Position() + (velocity * gTime.deltaTime());
        obj->SetPosition(pos);
        Vector2 accel = (has_gravity)? acceleration+physicsConstants.gravity : acceleration;
        velocity += accel * gTime.deltaTime();
    }
}

/* If the collision is with another enabled rigidbody, perform elastic collision with respect to the other object. */
void Rigidbody::OnCollision(Collision collision)
{
    // get the rigidbody component from the other object 
    auto other = collision.other->GetComponent<Rigidbody>();

    // valid collision, proceed with collision behaviour
    if (other != nullptr && other->Enabled())
    {
        if (is_moveable || other->Moveable())
        {
            // get other required components form object 
            auto other_collider = other->GetCollider();
            RectF other_bbox = other_collider->GetBoundingBox();

            // find the displacement between the point and the bounding box's centre
            Vector2 disp = collision.other->Position() - collision.point;
            // find the magnitude of this displacement along both axes
            float dx = (other_bbox.w/2.0f) - abs(disp.x);
            float dy = (other_bbox.h/2.0f) - abs(disp.y);

            // move the objects orthogonally along the axis with the least displacement
            if (dy<dx && dy>=0.0f) collision_on_y_axis(other, 
                (dy+physicsConstants.rigidbodySeperation) * sign(disp.y)
            );
            else if (dx>=0.0f && dy>=0.0f) collision_on_x_axis(other, 
                (dx+physicsConstants.rigidbodySeperation) * sign(disp.x)
            );
        }
    }
}

/* Performs elastic collision along the y axis based on the objects' velocities */
void Rigidbody::collision_on_y_axis(std::shared_ptr<Rigidbody> other, float dy)
{
    Vector2 pos = obj->Position(), other_pos = other->obj->Position();

    // move objects apart
    if (!is_moveable) other_pos.y += dy;
    else if (!other->Moveable()) pos.y -= dy;
    else {
        // if both objects are movable, both will 
        // move by half the displacement
        dy /= 2.0f;
        other_pos.y += dy; pos.y -= dy;
    }
    obj->SetPosition(pos); other->obj->SetPosition(other_pos);

    Vector2 other_vel = other->Velocity();

    // elastic collision
    if (is_moveable && other->Moveable())
    {
        // the starting velocities on the appropriate axis
        float v1i = velocity.y, v2i = other_vel.y;
        // v2f relative to v1f (v2f = v1f+this)
        float v2f_relative = v1i - v2i;

        // the constant value of the right of the equation
        float right_const = v2f_relative * other->Mass();
        // the coefficient of v1f on the right of the equation
        float right_coeff = mass + other->Mass();

        // v1f = (left of equation - constant on right) / coefficient on right
        float v1f = ((v1i*mass)+(v2i*other->Mass()) - right_const)/right_coeff;
        // find v2f relative to v1f
        float v2f = v1f + v2f_relative;
        velocity.y = v1f; other_vel.y = v2f;

        // alter x velocities according to friction
        float temp = other_vel.x;
        other_vel.x = other_vel.x*(1.0f-friction_coeff) + velocity.x*(friction_coeff);
        velocity.x = velocity.x*(1.0f-other->FrictionCoefficient()) + temp*other->FrictionCoefficient();
    }
    else {
        // if an object is not moveable, just cancel their velocity and alter movement with friction
        if (is_moveable) {
            velocity.y = 0.0f;
            velocity.x = velocity.x*(1.0f-other->FrictionCoefficient()) + other_vel.x*other->FrictionCoefficient();
        } else {
            other_vel.y = 0.0f;
            other_vel.x = other_vel.x*(1.0f-friction_coeff) + velocity.x*(friction_coeff);
        } 
    }
    other->SetVelocity(other_vel);
}

/* Performs elastic collision along the x axis based on the objects' velocities */
void Rigidbody::collision_on_x_axis(std::shared_ptr<Rigidbody> other, float dx)
{
    Vector2 pos = obj->Position(), other_pos = other->obj->Position();

    // move objects apart
    if (!is_moveable) other_pos.x += dx;
    else if (!other->Moveable()) pos.x -= dx;
    else {
        // if both objects are movable, both will 
        // move by half the displacement
        dx /= 2.0f;
        other_pos.x += dx; pos.x -= dx;
    }
    obj->SetPosition(pos); other->obj->SetPosition(other_pos);

    Vector2 other_vel = other->Velocity();

    // elastic collision
    if (is_moveable && other->Moveable())
    {
        // the starting velocities on the appropriate axis
        float v1i = velocity.x, v2i = other_vel.x;
        // v2f relative to v1f (v2f = v1f+this)
        float v2f_relative = v1i - v2i;

        // the constant value of the right of the equation
        float right_const = v2f_relative * other->Mass();
        // the coefficient of v1f on the right of the equation
        float right_coeff = mass + other->Mass();

        // v1f = (left of equation - constant on right) / coefficient on right
        float v1f = ((v1i*mass)+(v2i*other->Mass()) - right_const)/right_coeff;
        // find v2f relative to v1f
        float v2f = v1f + v2f_relative;
        velocity.x = v1f; other_vel.x = v2f;

        // alter y velocities according to friction
        float temp = other_vel.y;
        other_vel.y = other_vel.y*(1.0f-friction_coeff) + velocity.y*(friction_coeff);
        velocity.y = velocity.y*(1.0f-other->FrictionCoefficient()) + temp*other->FrictionCoefficient();
    }
    else {
        // if an object is not moveable, just cancel their velocity and alter movement with friction
        if (is_moveable) {
            velocity.x = 0.0f;
            velocity.y = velocity.y*(1.0f-other->FrictionCoefficient()) + other_vel.y*other->FrictionCoefficient();
        } else {
            other_vel.x = 0.0f;
            other_vel.y = other_vel.y*(1.0f-friction_coeff) + velocity.y*(friction_coeff);
        }
    }
    other->SetVelocity(other_vel);
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