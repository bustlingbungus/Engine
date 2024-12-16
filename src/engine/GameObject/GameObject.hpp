#pragma once 

#include "../../math/VectorMath.hpp"

#include <vector>
#include <memory>
#include <typeinfo>
#include <type_traits>
#include <utility>

using namespace Math;

// forwards declaration
struct Collision;

/* Baseplate object in game space, contains a vector of components */
class GameObject
{
    public:

        GameObject(Vector2 position = Vector2_Zero, Vector2 scale = Vector2_One, bool startEnabled = true);
        ~GameObject();

        virtual void Destroy();

        virtual void Update();
        void UpdateComponents();

        virtual void SetEnabled(bool enable);
        virtual void SetPosition(Vector2 newPosition);
        virtual void SetScale(Vector2 newScale);

        virtual void AssignComponents(std::shared_ptr<GameObject> self);

        void AddComponent(std::shared_ptr<GameObject> component);

        /* 
         * Removes the specified component type from the object. 
         * If the object has multiple components of the specified type,
         * only the first one found is removed. 
         */
        template <typename T>
        void RemoveComponent()
        {
            // iterate through each item in the components vector
            for (auto it=components.begin(), e=components.end(); it!=e; it++) {
                // once one mathces the specified type, erase it from the vector
                if (typeid(**it) == typeid(T)) {
                    components.erase(it);
                    break;
                }
            }
        }

        /*
         * Finds a component of the specified type attatched to this object.
         * 
         * \returns An `std::shared_ptr` to the component whose type matches the
         * specified type. If the object has multiple components of this type, only
         * the first one found is returned. If no such component exists,
         * `nullptr` is returned. 
         */
        template <typename T>
        std::shared_ptr<T> GetComponent()
        {
            // iterate through all components
            for (auto& component : components) {
                // if a matching type is found, return it
                if (typeid(*component) == typeid(T)) {
                    return std::dynamic_pointer_cast<T>(component);
                }
            }
            // return nullptr for failure
            return nullptr;
        }

        /*
         * Finds all components of the specified type attatched to this object.
         *
         * \returns An `std::vector` of `std::shared_ptr`s of all components of the given
         * type attached to this object
         */
        template <typename T>
        std::vector<std::shared_ptr<T>> GetComponents()
        {
            std::vector<std::shared_ptr<T>> res;
            for (auto& component : components) {
                if (typeid(*component) == typeid(T)) {
                    res.push_back(std::dynamic_pointer_cast<T>(component));
                }
            }
            return res;
        }

        void RemoveComponent(std::shared_ptr<GameObject> obj);

        virtual void OnCollisionEnter(Collision collision);
        virtual void OnCollisionStay(Collision collision);
        virtual void OnCollisionExit(std::shared_ptr<GameObject> other);

        bool Enabled() const;
        Vector2 Position() const;
        Vector2 Scale() const;

    private:

        /* Whether or not the obejct is enabled */
        bool enabled;

        /* Vector of all the object's components */
        std::vector<std::shared_ptr<GameObject>> components;

        /* The object's position in game space */
        Vector2 pos;
        /* The scale, or size of the object */
        Vector2 scale;
};