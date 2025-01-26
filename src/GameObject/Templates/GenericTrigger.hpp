#pragma once

#include "../GameObject.hpp"
#include "../Colliders/BoxCollider.hpp"

/*
 * Region that will activeate or deactivate upon the prescence (or lack thereof) 
 * of a gameobject of a specific type. 
 * 
 * Must be constructed with respect to a given type `T`. For instance, 
 * `auto trigger = new ToggleWallsTrigger<Player>(...)`
 * 
 * The trigger will only update in response to the prescence or abscence of objects
 * of type `T`.
 * 
 * `EnterFunc` is called when a `T` object enters the region, and `ExitFunc` is 
 * called when it leaves. Redefine these in subclass definitions. 
 * 
 * By default, the trigger will NOT have any rendering. 
 * 
 * \warning For proper functionality, `OnCollisionEnter` and `OnCollisionExit` should
 * NOT be redefined by subclasses. 
 * 
 * \note In your subclass' `AssignComponents` function, be sure to call
 * `GenericTrigger<T>::AssignComponents(self)`. This just adds a `BoxCollider`. 
 * You could also just add a box collider manually.abort
 * 
 * For proper functionality, All `T` objects should have a `BoxCollider` 
 * component.
 */
template <typename T>
class GenericTrigger : public GameObject
{
    public:

        /*
         * Region that will activeate or deactivate upon the prescence (or lack thereof) 
         * of a gameobject of a specific type. 
         * 
         * Must be constructed with respect to a given type `T`. For instance, 
         * `auto trigger = new ToggleWallsTrigger<Player>(...)`
         * 
         * The trigger will only update in response to the prescence or abscence of objects
         * of type `T`.
         * 
         * By default, the trigger will NOT have any rendering.
         * 
         * \param position The trigger's position.
         * \param size The trigger's dimensions.
         */
        GenericTrigger(Vector2 position, Vector2 size)
        : GameObject(position, size)
        {

        }

        ~GenericTrigger()
        {
            Destroy();
        }

        virtual void Destroy()
        {
            GameObject::Destroy();
        }

        /* Adds a box collider */
        virtual void AssignComponents(std::shared_ptr<GameObject> self)
        {
            AddComponent<BoxCollider>(self);
        }

        /* Called when desired object enters. Redefine for meaningful behaviour. */
        virtual void EnterFunc() {}
        /* Called when desired object exits. Redefine for meaningful behaviour. */
        virtual void ExitFunc() {}

        /* Check if the entering object is of the correct type, and update accordingly. */
        virtual void OnCollisionEnter(Collision collision)
        {
            // desired object type enters
            if (typeid(*collision.other) == typeid(T)) {
                // make active and call enter function
                active = true;
                EnterFunc();
            }
        }

        /* Check if the exiting object is of the correct type, and update accordingly. */
        virtual void OnCollisionExit(std::shared_ptr<GameObject> other)
        {
            // object type is leaving
            if (typeid(*other) == typeid(T)) {
                // make inactive and call exit function
                active = false;
                ExitFunc();
            }
        }

        /* Whether or not the desired object type is present. */
        bool Active() const { return active; }

    private:

        /* Whether or not a T object is present in the trigger. */
        bool active = true;
};