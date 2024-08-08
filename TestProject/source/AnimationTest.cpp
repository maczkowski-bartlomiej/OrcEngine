#include "AnimationTest.hpp"

AnimationTest::AnimationTest()
{
	orc::Ref<orc::Texture> soldierTexture = textureHolder.getResource("soldier_texture");
	m_sprite = orc::Sprite(soldierTexture, orc::Vector2f(100.0f, 75.0f));
	m_sprite.setScale(0.5f);
	m_sprite.setOrigin(m_sprite.getGlobalRect().getSize() / 2.0f);

	//orc::Ref<orc::Animation> animation = animationHolder.getResource("player_animation");
	//m_sprite.getAnimator().addAnimation("idle", animation);
}

AnimationTest::~AnimationTest()
{
	ORC_LOG_INFO("AnimationTest shutting down...");
}

void AnimationTest::onAttach()
{
	window.setTitle("AnimationTest");
}

void AnimationTest::onDetach()
{
}

void AnimationTest::onUpdate(float deltaTime)
{
	renderer.setClearColor(orc::Color(25, 25, 25, 255));
	renderer.clear();

	renderer.begin(camera);

	renderer.end();
}

void AnimationTest::onEvent(orc::Event& event)
{
	if (event.getType() == orc::Event::Type::KeyboardKeyPressed)
	{
		auto newEvent = (orc::KeyboardKeyPressedEvent*)&event;
		if (newEvent->key == orc::Keyboard::Key::Right)
		{
			gameLayerManager.setActiveGameLayer("circles_test");
		}
		else if (newEvent->key == orc::Keyboard::Key::Left)
		{
			gameLayerManager.setActiveGameLayer("game");
		}
	}
}
