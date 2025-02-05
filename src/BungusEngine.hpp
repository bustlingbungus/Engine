#pragma once

// include all engine headers 

#include "sdl/LWindow.hpp"
#include "sdl/LTexture.hpp"
#include "sdl/LAudio.hpp"
#include "sdl/LFont.hpp"

#include "GlobalWindow.hpp"
#include "InputHandler.hpp"
#include "TimeTracker.hpp"

#include "GameObject/GameObject.hpp"
#include "GameObject/ObjectComponent.hpp"

#include "GameObject/Cameras/Camera.hpp"

#include "GameObject/Renderer/RenderManager.hpp"
#include "GameObject/Renderer/TextureRenderer.hpp"
#include "GameObject/Renderer/AnimationRenderer.hpp"
#include "GameObject/Renderer/TextRenderer.hpp"

#include "GameObject/Colliders/ColliderManager.hpp"
#include "GameObject/Colliders/BoxCollider.hpp"

#include "GameObject/Rigidbody/PhysicsConstants.hpp"
#include "GameObject/Rigidbody/Rigidbody.hpp"

#include "GameObject/Scene/Game.hpp"
#include "GameObject/Scene/Scene.hpp"

#include "GameObject/Audio/AudioPlayer.hpp"

#include "GameObject/Templates/GenericTrigger.hpp"

/// @brief  declaration for user initialiser function
void Init();

inline bool quit = false;
/* Closes the window */
inline void Exit() { quit = true; }