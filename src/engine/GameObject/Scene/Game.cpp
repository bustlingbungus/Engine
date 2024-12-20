#include "Game.hpp"

/* The game that holds all scenes. Creates a single `Default Scene` on creation */
Game::Game()
{
    current_scene = AddScene<Scene>("Default Scene");
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

/* Removes a scene with the given name from the game. */
void Game::RemoveScene(std::string sceneName)
{
    // find the scene in the hashmap
    auto it = scenes.find(sceneName);
    // if the scene exists, remove it
    if (it != scenes.end()) {
        it->second->OnSceneExit();
        scenes.erase(it);
        // if the current scene is the one being removed, reassign the current scene
        if (current_scene==nullptr || current_scene->name==sceneName) {
            if (scenes.empty()) current_scene = nullptr;
            else {
                current_scene = scenes.begin()->second;
                current_scene->OnSceneEnter();
            }
        }
    }
}

/* Changes the current scene to the specified scene. */
void Game::EnterScene(std::string sceneName) 
{
    // find the scene
    auto it = scenes.find(sceneName);
    // if the scene exists, switch to it
    if (it != scenes.end()) {
        // call exit behaviour for old scene
        if (current_scene != nullptr) current_scene->OnSceneExit();
        // switch to new scene and call enter behaviour
        current_scene = it->second;
        current_scene->OnSceneEnter();
    }
}

/* Get a pointer to the current scene. */
std::shared_ptr<Scene> Game::GetCurrentScene() const {
    return current_scene;
}

/* Get a pointer to a scene with the given name. `nullptr` if no such scene exists. */
std::shared_ptr<Scene> Game::GetScene(std::string sceneName) const 
{
    // find the scene
    auto it = scenes.find(sceneName);
    // if it exists, return it. else return nullptr
    return (it==scenes.end())? nullptr : it->second;
}

/* Get a vector of pointer to all scenes in the game */
std::vector<std::shared_ptr<Scene>> Game::GetScenes() const
{
    std::vector<std::shared_ptr<Scene>> res;
    for (auto& p : scenes) res.push_back(p.second);
    return res;
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

/* Removes a scene with the given name from the game. */
void RemoveScene(std::string sceneName) {
    game.RemoveScene(sceneName);
}

/* Changes the current scene to the specified scene. */
void EnterScene(std::string sceneName) {
    game.EnterScene(sceneName);
}

/* Get a pointer to the current scene. */
std::shared_ptr<Scene> GetCurrentScene() {
    return game.GetCurrentScene();
}

/* Get a pointer to a scene with the given name. `nullptr` if no such scene exists. */
std::shared_ptr<Scene> GetScene(std::string sceneName) {
    return game.GetScene(sceneName);
}

/* Get a vector of pointer to all scenes in the game */
std::vector<std::shared_ptr<Scene>> GetScenes() {
    return game.GetScenes();
}