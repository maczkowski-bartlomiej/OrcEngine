#include "OrcPch.hpp"

#include "Engine/Engine.hpp"
#include "Engine/GameLayer.hpp"
#include "Graphics/Camera.hpp"
#include "Graphics/Window.hpp"

namespace orc
{

GameLayer::GameLayer()
	: camera(0.0f, 0.0f, (float)Engine::get().getWindow().getWidth(), (float)Engine::get().getWindow().getHeight()),
	audio(Engine::get().getAudio()),
	window(Engine::get().getWindow()),
	renderer(Engine::get().getRenderer()),
	gameLayerManager(Engine::get().getGameLayerManager()),
	fontResources(Engine::get().getFontResources()),
	textureResources(Engine::get().getTextureResources()),
	animationResources(Engine::get().getAnimationResources())
{
}

Camera& GameLayer::getCamera()
{
	return camera;
}

}
