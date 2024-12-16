#pragma once

#include "Scene.hpp"
#include "../GameObject.hpp"

#include <unordered_map>
#include <iostream>

/* The game that holds all scenes. */
class Game
{
    public:

        Game();
        ~Game();

        void Update();

        std::shared_ptr<Scene> AddScene(std::string sceneName);
        void RemoveScene(std::string sceneName);
        void EnterScene(std::string sceneName);

        /*
         * Adds an object into the current scene using the information provided.
         *
         * \returns A pointer to the object added.
         * 
         * \warning This function will fail if the arguments provided are not suitable for the
         * desired type. Please read the documentation for class `T`.
         * 
         * \note The created object will be added to the scene with no parents. Hence, `ObjectComponents`
         * should not be created this way, nor any `GameObject` intended to be a child object. To create
         * those, attatch them to their intended parent, instead of using this.
         */
        template <class T, class... Args>
        std::shared_ptr<T> AddToScene(Args&&... args)
        {
            // no valid scene
            if (current_scene==nullptr) {
                std::cerr << "No existing scene!\n";
                return nullptr;
            }
            // create the object
            auto obj = std::make_shared<T>(std::forward<Args>(args)...);
            obj->AssignComponents(obj);
            // add object to current scene
            current_scene->AddComponent(obj);
            // return created object
            return obj;
        }

        /*
         * Returns a pointer to an object from the current scene whose type matches the one given.
         * If multiple objects of type `T` exist, the first one found is returned.
         * If no such object exists, or no scene is currently active, `nullptr` is returned.
         */
        template <typename T>
        std::shared_ptr<T> GetObject()
        {
            if (current_scene==nullptr) {
                std::cerr << "Not in a scene!\n";
                return nullptr;
            }
            return current_scene->GetComponent<T>();
        }

        /*
         * Returns a vector of pointer to all objects from the current scene whose type
         * match the one given. If no such object exists,
         * or if no scene is currently active, an empty vector is returned.
         */
        template <typename T>
        std::vector<std::shared_ptr<T>> GetObjects()
        {
            if (current_scene==nullptr) {
                std::cerr << "Not in a scene!\n";
                return {};
            }
            return current_scene->GetComponents<T>();
        }

        void RemoveFromScene(std::shared_ptr<GameObject> obj);

    private:

        /* All the scenes currently in the game. */
        std::unordered_map<std::string,std::shared_ptr<Scene>> scenes;

        /* The scene currently being handled. */
        std::shared_ptr<Scene> current_scene;
};

/* Global game object */
extern Game game;

/*
 * Adds an object into the current scene using the information provided.
 *
 * \returns A pointer to the object added.
 * 
 * \warning This function will fail if the arguments provided are not suitable for the
 * desired type. Please read the documentation for class `T`.
 * 
 * \note The created object will be added to the scene with no parents. Hence, `ObjectComponents`
 * should not be created this way, nor any `GameObject` intended to be a child object. To create
 * those, attatch them to their intended parent, instead of using this.
 */
template <class T, class... Args>
std::shared_ptr<T> Instantiate(Args&&... args) {
    return game.AddToScene<T>(std::forward<Args>(args)...);
}

void Destroy(std::shared_ptr<GameObject> obj);

std::shared_ptr<Scene> NewScene(std::string sceneName);
void RemoveScene(std::string sceneName);

void EnterScene(std::string sceneName);

/*
 * Returns a pointer to an object from the current scene whose type matches the one given.
 * If multiple objects of type `T` exist, the first one found is returned.
 * If no such object exists, or no scene is currently active, `nullptr` is returned.
 */
template <typename T>
std::shared_ptr<T> GetObject() {
    return game.GetObject<T>();
}

/*
 * Returns a vector of pointer to all objects from the current scene whose type
 * match the one given. If no such object exists,
 * or if no scene is currently active, an empty vector is returned.
 */
template <typename T>
std::vector<std::shared_ptr<T>> GetObjects() {
    return game.GetObjects<T>();
}