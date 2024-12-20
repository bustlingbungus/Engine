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

        /* 
         * Creates a new scene with the given name and adds it to the game. 
         * Does nothing if a scene with the given name already exists.
         * 
         * \param sceneName The name of the scene being added.
         * \param enter Whether or not to enter the scene created.
         * 
         * \note This function only accepts a single string as a scene name, so please ensure that
         * any scene subclass only requires a single string as a constructor argument.
         * 
         * \warning This function will fail if `T` is not a subclass of `Scene`. Be sure to only
         * use this function to add `Scene` objects.
         * 
         * \returns a pointer to the created scene
         */
        template <class T>
        std::shared_ptr<Scene> AddScene(std::string sceneName, bool enter = true)
        {
            // check if the scene already exists in the game
            auto it = scenes.find(sceneName);
            // the scene does not exist
            if (it == scenes.end())
            {
                // create the scene object and add it to the hashmap
                auto newScene = std::make_shared<T>(sceneName);
                scenes.emplace(sceneName, newScene);
                // if there is not a current scene, or if the user requests enter, 
                // switch the current scene to the created scene
                if (current_scene == nullptr || enter) {
                    if (current_scene != nullptr) current_scene->OnSceneExit();
                    current_scene = newScene;
                    current_scene->OnSceneEnter();
                }
                // return created scene
                return newScene;
            }
            // return the already existing scene
            return it->second;
        }

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
            // return created object
            return current_scene->AddComponent<T>(std::forward<Args>(args)...);
        }

        std::shared_ptr<Scene> GetCurrentScene() const;
        std::shared_ptr<Scene> GetScene(std::string sceneName) const;
        std::vector<std::shared_ptr<Scene>> GetScenes() const;

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
        std::shared_ptr<Scene> current_scene = nullptr;
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

void DestroyObject(std::shared_ptr<GameObject> obj);

/* 
 * Creates a new scene with the given name and adds it to the game. 
 * Does nothing if a scene with the given name already exists.
 * 
 * \param sceneName The name of the scene being added.
 * \param enter Whether or not to enter the scene created.
 * 
 * \note This function only accepts a single string as a scene name, so please ensure that
 * any scene subclass only requires a single string as a constructor argument.
 * 
 * \warning This function will fail if `T` is not a subclass of `Scene`. Be sure to only
 * use this function to add `Scene` objects.
 * 
 * \returns a pointer to the created scene
 */
template <class T>
std::shared_ptr<Scene> NewScene(std::string sceneName, bool enter = true) {
    return game.AddScene<T>(sceneName, enter);
}

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

std::shared_ptr<Scene> GetCurrentScene();
std::shared_ptr<Scene> GetScene(std::string sceneName);
std::vector<std::shared_ptr<Scene>> GetScenes();