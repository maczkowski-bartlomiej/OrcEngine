#include "OrcPch.hpp"

#include "Audio/Audio.hpp"

#include "Graphics/Window.hpp"
#include "Graphics/Renderer.hpp"

#include "Engine/Engine.hpp"
#include "Engine/GameLayer.hpp"
#include "Engine/ResourceHolder.hpp"
#include "Engine/GameLayerManager.hpp"

namespace orc
{

GameLayer::GameLayer()
	: camera(0.0f, 0.0f, (float)Engine::get().getWindow().getWidth(), (float)Engine::get().getWindow().getHeight()),
	  audio(Engine::get().getAudio()),
	  window(Engine::get().getWindow()),
	  renderer(Engine::get().getRenderer()),
	  gameLayerManager(Engine::get().getGameLayerManager()),
	  fontHolder(Engine::get().getFontHolder()),
	  textureHolder(Engine::get().getTextureHolder()),
	  animationHolder(Engine::get().getAnimationHolder())
{
}

Camera& GameLayer::getCamera()
{
	return camera;
}

}
