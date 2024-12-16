#include "Game.hpp"

/* The game that holds all scenes. Creates a single `Default Scene` on creation */
Game::Game()
{
    auto default_scene = std::make_shared<Scene>("Default Scene");
    scenes.emplace(default_scene->name, default_scene);
    current_scene = default_scene;
}

/* Clear scenes */
Game::~Game()
{
    scenes.clear();
    current_scene = nullptr;
}

/* Update the scenes and all its component objects. */
void Game::Update()
{
    if (current_scene != nullptr) {
        current_scene->Update();
        current_scene->UpdateComponents();
    }
}

/* 
 * Creates a new scene with the given name and adds it to the game. 
 * Does nothing if a scene with the given name already exists.
 * 
 * \returns a pointer to the created scene
 */
std::shared_ptr<Scene> Game::AddScene(std::string sceneName)
{
    // check if the scene already exists in the game 
    auto it = scenes.find(sceneName);
    // the scene does not exist
    if (it == scenes.end()) 
    {
        // create the scene object and add it to the hashmap
        auto newScene = std::make_shared<Scene>(sceneName);
        scenes.emplace(sceneName, newScene);
        // if there is not a current scene, make the new scene the current one
        if (current_scene==nullptr) current_scene = newScene;
        // return created scene
        return newScene;
    }
    // return the already existing scene
    return it->second;
}

/* Removes a scene with the given name from the game. */
void Game::RemoveScene(std::string sceneName)
{
    // find the scene in the hashmap
    auto it = scenes.find(sceneName);
    // if the scene exists, remove it
    if (it != scenes.end()) {
        scenes.erase(it);
        // if the current scene is the one being removed, reassign the current scene
        if (current_scene==nullptr || current_scene->name==sceneName) {
            if (scenes.empty()) current_scene = nullptr;
            else current_scene = scenes.begin()->second;
        }
    }
}

/* Changes the current scene to the specified scene. */
void Game::EnterScene(std::string sceneName) 
{
    // find the scene
    auto it = scenes.find(sceneName);
    // if the scene exists, switch to it
    if (it != scenes.end()) current_scene = it->second;
}

/* Removes the object pointer from the current scene. */
void Game::RemoveFromScene(std::shared_ptr<GameObject> obj) {
    if (current_scene != nullptr) current_scene->RemoveComponent(obj);
}

/* Create global game object. */
Game game = Game();


/* Removes the object pointer from the current scene. */
void DestroyObject(std::shared_ptr<GameObject> obj) {
    game.RemoveFromScene(obj);
}

/* 
 * Creates a new scene with the given name and adds it to the game. 
 * Does nothing if a scene with the given name already exists.
 * 
 * \returns a pointer to the created scene
 */
std::shared_ptr<Scene> NewScene(std::string sceneName) {
    return game.AddScene(sceneName);
}

/* Removes a scene with the given name from the game. */
void RemoveScene(std::string sceneName) {
    game.RemoveScene(sceneName);
}

/* Changes the current scene to the specified scene. */
void EnterScene(std::string sceneName) {
    game.EnterScene(sceneName);
}