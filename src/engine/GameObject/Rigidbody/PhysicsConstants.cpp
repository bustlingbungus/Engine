#include "PhysicsConstants.hpp"

// create global variable
PhysicsConstants physicsConstants = PhysicsConstants();

/* Set the global direction and magnitude of gravity */
void SetGravity(Vector2 newGravity) {
    physicsConstants.gravity = newGravity;
}