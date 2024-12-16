# Bustling Bungus' Game API

This is an open source game engine, or game creation API, rather, that aims to bring the functionality of engines like Unity to an API that is open source and uses C++. This API currently only supports 2D games. 

While there is not currently a GUI to interface with, the API here allows for a game creation experience not too dissimilar from Unity scripting.

The API uses an implementation Simple Directmedia Layer (SDL) libraries for graphics rendering, and uses CMake to build projects. SDL is a cross platform API, so the game API should work across different OS, with some modifications to the `CMakeLists.txt` file. The current CMake file was designed for building on Windows 10 OS. More information is available in `Build Instructions`.

## General Documentation

Following are basic documentation for some of the main functionalities of this API. For more in depth documentation, observe the commenting put in the source code for the various functions.

In general, a scene in the context of this API can be seen as the root of a tree structure, and `GameObject` pointers are added as children of the scene - these are the objects in the game. These game objects can have components and objects added to themselves, that can modify the object and/or interact with other objects. 

### General Function Documentation

- `Instantiate<T>(...)`: Creates and returns an `std::shared_ptr` to a GameObject of type `T`, and adds the object to the scene. 
  - This will only work when using arguments that are suitable for the constructor of class `T`, so be sure to check the constructor for the object you are trying to Instantiate

- `Destroy(std::shared_ptr<GameObject>)`: Finds the specified object in the current scene, and removes it from the scene.
  - This will only work for objects that are the scene's child. i.e, objects that are part of the scene, and not a component or child of any other object. To remove those objects, remove them from their parent object.

- `NewScene(std::string)`: Adds a new scene with the given name, if such a scene does not already exist. Note that the game will be initialised with one scene, which has a camera object.

- `RemoveScene(std::string)`: Finds the desired scene, and if it exists, removes it from the game.

- `EnterScene(std::string)`: Finds the desired scene, and if it exists, switches the current scene to the scene specified.

- `GetObject<T>()`: Finds an object of type `T` in the current scene. Returns `nullptr` if no such object exists, and returns the first match if multiple exist.
  - This will only work for objects that are the scene's child. i.e, objects that are part of the scene, and not a component or child of any other object. To find component objects, search within their parent object.

- `GetObjects<T>()`: Identical functionality to `GetObject<T>()`, but returns a vector of all objects in the scene of type `T`.

### GameObject Function Documentation

- `GameObject::Update()`: Updates the objects members. Doesn't do anything by default, and should be redefined in subclasses for desired object behaviour.
  - No need to call this function anywhere in your code. Assuming the object is a part of the current scene, or a child of any object in the current scene, this will be called automatically by the game.

- `GameObject::UpdateComponents()`: Updates all the objects child objects, calling `Update()` and `UpdateComponents()` on those objects.
  - No need to call this function anywhere in your code. Assuming the object is a part of the current scene, or a child of any object in the current scene, this will be called automatically by the game.

- `GameObject::AddComponent(std::shared_ptr<GameObject>)`: Adds the specified game object as a component of this object.
  - There is no check to ensure the object is not already a component, so avoid adding duplicate components to objects.

- `GameObject::RemoveComponent<T>()`: Removes a component of type `T` from the object. If multiple components of type `T` exist, only the first match is removed. If none exist, nothing is removed.

- `GameObject::GetComponent<T>()`: Finds and returns a pointer to a component of type `T` in the game object. If the object has multiple components of type `T`, only the first pointer found is returned. Returns a `nullptr` if the object has no such component.

- `GameObject::GetComponents<T>()`: Identical functionality to `GameObject::GetComponent<T>()`, but a vector of pointers to all components of type `T` is returned. If no such component exists, an empty vector is returned.

- `GameObject::RemoveComponent(std::shared_ptr<GameObject>)`: Removes a specific component from the object's components. 

## Default Object/Component Types

Below are some included types of game objects and object components. Note that an `ObjectComponent` is just a game object with a pointer to its parent.

To create a game object outright, `Instantiate<ObjectType>(args)` may be used. To make an object that is a child of another game object, use:

``` C++
auto object = std::make_shared<ObjectType>(args);
parentObject->AddComponent(object);
```

Note that for `ObjectComponent`s, this is the only way they should be added to the game, and their `object` constructor parameter should always be set to the object they will be attached to.

Created objects and scenes should be made in the beginning of the `main` function, before the main window loop.

### Camera

This is an object that renders `TextureRenderer` components to the screen. It contains a rect representing the region on the screen it will be rendered to, as well as the position of the camera's origin in game space.

The camera's `Update` function will go through every `TextureRenderer` in the game, and call that object's `Render` function, with reference to itself. 

### TextureRenderer

This is an object component that contains a pointer to an `LTexture` that will be rendered by camera objects. This is accomplished by adding itself to a global vector of Texture Renderers upon creation, and removing itself from said vector when going out of scope.

The `Update` function will centre the rect used for rendering on the parent object, and resize said rect to match the parent's `scale`. Redfining `Update` in a subclass of `TextureRenderer` will allow for more complicated rendering behaviour, without ruining the current functionality, because the base `TextureRenderer` still gets added to the global vector when created via a subclass.

### BoxCollider

This is a component that detects collision with other box colliders. It contains a rect centred its the parent object, with dimensions equivalent to the parent's scale. 

It considers itself in collision with another box colliders when any of it's four corners are within the other collider's bounding box.

Detection is accomplished by adding itself to a global vector of `BoxCollider`s on creation, and removing itself from said vector when going out of scope. Every frame when `Update()` is called, the collider's bounding box is readjusted to represent the parent's size and position, and the collider checks every object in the global vector (other than itself) for collision.

Objects the collider is currently in collision with are stored in an `std::unordered_map` of `Collision` structs. To get these collisions from the collider as a vector, call `BoxCollider::GetCollisions()`.
- `Collision` struct contains an `std::shared_ptr<GameObject>` to the parent of the other collider, and a `Vector2` to the point of collision.

### Rigidbody

This is a component that will move its parent object around based on collisions with other objects. Collision is detected using a `BoxCollider` component. If no such component exists on the parent object during the `Rigidbody`'s creation, the `Rigidbody` will add a `BoxCollider` to its parent.

Rigidbodies contain the following components:
- `mass` for elastic collision calculations. `(float)`
- `velocity` for elastic collision calculations, as well as for changing the position of it's parent every frame. `(Vector2)`
- `acceleration` for changing the velocity vector every frame. `(Vector2)`
- `collider` a pointer its parent's BoxCollider component, for collision detection. `(std::shared_ptr<BoxCollider>)`
- `is_moveable` setting this false means that the rigidbody can move other rigidbody objects, but cannot be moved itself by other rigidbodies. `(bool)`
  - Rigidbodies with `is_moveable` false can still be affected by gravity.
- `has_gravity` setting this false means that the rigidbody's acceleration will no longer be summed with global gravity. `(bool)`
  - Rigidbodies with `has_gravity` false can still be moved by other rigidbodies.

As mentioned, rigidbodies change velocity on collision according to perfectly elastic collision. Since they use box colliders for collision detection, a rigidbody can only collide with another rigidbody along one axis.
- Colliding with another `BoxCollider` from the side will only change motion on the x axis, and colliding with another `Rigidbody` from above or below will only change motion on the y axis.
- Rigidbodies are pushed slightly apart on collision, to avoid infinitely colliding with each other. This seperation diestance is currently `0.0625` units.

## Other Documentation

### Input

User input is tracked with the global `inputHandler` object. It is updated currently in `main` once per frame. It contains a hashmap of booleans representing whether each button is being pressed.

- To see if a specific key is currently being input, use `InputActive` with the desired SDL Keycode/mouse button macro to see if the key or mouse button is currently held down.
- To get the mouse position in the window as a `Vector2Int`, use `MousePosition()`.

### Global Window

`LTexture` objects require an `LWindow` object for creation, for rendering purposes. This API uses one global `LWindow`, names `gWindow`. Use `gWindow` for the creation of all `LTextures`, and other window related purposes. Do not create any `LWindow` objects aside from `gWindow`.

### Framerate and Delta Time

The time elapsed between each frame is calculated and stored in the `gTime` global variable. This is updated once per frame in `main`. Do not change this, as it would likely render delta time inaccurate.

- To get the time elapsed between each frame in `seconds`, delta time, use `gTime.deltaTime()`. 
- To find the inverse of this (the inverse of delta time), use `gTime.Framerate()`.
- By default, the framerate is uncapped. Use `gTime.setMaxFramerate(int)` to cap the framerate at any integer number of frames per second.

The method of calculating delta time, differences in `ctime`'s `clock()` method, is only accurate to 1ms. Therefore, delta time will only be accurate to the nearest millisecond of the true time passed.

## Build Instructions

### Dependencies

- C++ Compiler
- CMake
- Installation of ```SDL2```, ```SDL2_Image```, ```SDL2_ttf```, and ```SDL2_mixer```

### Instructions

Ensure there is a 'build' folder in your directory. Change to this directory with ```cd build```. From the build folder, run these commands:

``` bash
cmake ..
cmake --build . --verbose
```

Then, run ```cd bin``` to go to the output directory, and run ```./project.exe``` to launch the game.

On VSCode, pressing ```ctrl+shift+b``` to run the included default build tasks should also work.

When developing your own projects, be sure to go to ```CMakeLists.txt```, and on line 7, replace 'project' with the name of your project. After doing this, wou will run your project with ```./[yourProjectName].exe```. If using build tasks to automate the build process, be sure to update line 37 in ```tasks.json``` to reflect this name change.

### Build Fix

On some systems, there may be an issue when static linking to the SDL_ttf library. If you are getting a CMake error message about an `undefined reference to '_setjmp'`, there is likely an issue with your installation of the C standard library. Make sure you have a valid C++ compiler for your system , and that it is properly referenced in VSCode and CMake (This process is a huge pain, I'm sorry if this happens to you).

You can also try dynamically linking to SDL_ttf instead, by removing the `-static` property from line 26 of `CMakeLists.txt`. Line 26 should look like this for dynamic linking:

``` CMake
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2_ttf::SDL2_ttf)
```

Note that if you were having the `undefined reference to '_setjmp'` error, This fix might allow you to build the project, but will likely create a new error where calling the `loadFromRenderedText` function more than once will crash your application. If this is happening to you, again, you need to make sure your C++ compiler and the C standard library are both installed properly.

Hopefully this fix works!
