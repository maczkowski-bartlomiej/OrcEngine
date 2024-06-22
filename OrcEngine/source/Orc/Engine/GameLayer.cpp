#include "OrcPch.hpp"

#include "Engine/Engine.hpp"
#include "Engine/GameLayer.hpp"
#include "Engine/GameLayerManager.hpp"

#include "Graphics/Window.hpp"
#include "Graphics/Renderer.hpp"

#include "Audio/Audio.hpp"

namespace orc
{

GameLayer::GameLayer()
	: camera(0.0f, (float)Engine::get().getWindow().getWidth(), (float)Engine::get().getWindow().getHeight(), 0.0f),
	  audio(Engine::get().getAudio()),
	  window(Engine::get().getWindow()),
	  renderer(Engine::get().getRenderer()),
	  gameLayerManager(Engine::get().getGameLayerManager()),
	  fontHolder(Engine::get().getFontHolder()),
	  shaderHolder(Engine::get().getShaderHolder()),
	  textureHolder(Engine::get().getTextureHolder()),
	  animationHolder(Engine::get().getAnimationHolder()),
	  soundBufferHolder(Engine::get().getSoundBufferHolder())
{
}

Camera& GameLayer::getCamera()
{
	return camera;
}

}
