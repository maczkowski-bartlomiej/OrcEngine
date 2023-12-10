#include "OrcPch.hpp"

#include "Engine/GameLayer.hpp"

#include "Engine/Engine.hpp"

namespace orc
{

GameLayer::GameLayer()
	: audio(Engine::get().getAudio())
	, window(Engine::get().getWindow())
	, renderer(Engine::get().getRenderer())
{
}

}
