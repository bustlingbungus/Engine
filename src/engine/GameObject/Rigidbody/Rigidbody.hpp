#pragma once

#include "../ObjectComponent.hpp"
#include "../Colliders/BoxCollider.hpp"

/* Component for moving colliding objects apart using elastic collision. */
class Rigidbody : public ObjectComponent
{
    public:

        Rigidbody(std::shared_ptr<GameObject> object, float mass, float frictionCoefficient, Vector2 velocity = Vector2_Zero, bool isMoveable = true, bool hasGravity = true, bool startEnabled = true);
        ~Rigidbody();

        virtual void Update();

        virtual void OnCollision(Collision collision);

        std::shared_ptr<BoxCollider> GetCollider() const;
        float Mass() const;
        float FrictionCoefficient() const;
        bool Moveable() const;
        bool GravityEnabled() const;
        Vector2 Velocity() const;
        Vector2 Acceleration() const;

        void SetMass(float newMass);
        void SetFrictionCoefficient(float frictionCoefficient);
        void MakeMoveable(bool moveable);
        void SetVelocity(Vector2 newVelocity);
        void SetAcceleration(Vector2 newAcceleration);
        void EnableGravity(bool gravityEnabled);

    private:

        /* The mass of the object. */
        float mass;
        /* Coefficient of friction */
        float friction_coeff;

        /* Whether or not the object may be moved by other rigidbodies. */
        bool is_moveable;
        /* Whether or not the object is affected by gravity. */
        bool has_gravity;

        /* Rate of change of position. */
        Vector2 velocity;
        /* Rate of change of velocity. */
        Vector2 acceleration;

        /* The collider object that the rigidbody's parent uses for collision detection. */
        std::shared_ptr<BoxCollider> collider;

        void collision_on_y_axis(std::shared_ptr<Rigidbody> other, float dy);
        void collision_on_x_axis(std::shared_ptr<Rigidbody> other, float dx);
};