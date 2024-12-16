#pragma once

#include "../../../math/VectorMath.hpp"

using namespace Math;

/* Container for global constant values related to physics */
struct PhysicsConstants {
    Vector2 gravity = {0.0f, 1200.0f};
    /* The distance put between two rigidbodies upon collision. */
    float rigidbodySeperation = 0.0625f;
};

extern PhysicsConstants physicsConstants;

void SetGravity(Vector2 newGravity);