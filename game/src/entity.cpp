#include "entity.h"
#include "game_global.h"
#include <tracy/Tracy.hpp>
#include "job_system.h"



namespace CityBuilderGame
{
	void Entity::Init()
	{
		
		if (!_entityTexture.loadFromFile("../game/Data/Entity.png"))
		{
			throw std::runtime_error("Cant load an file");
		}
		_entityTexture.setSmooth(true);
		_entitySprite.setTexture(_entityTexture);
		_entitySprite.setPosition(0.2f, 740.0f);
		_entitySprite.scale(0.2F, 0.2F);
	}

	void Entity::Update(sf::Sprite houseSprite,sf::RenderWindow& window)
	{
		ZoneScopedN("UpdateEntity");
		if (houseSprite.getGlobalBounds().contains(houseSprite.getPosition()))
		{
			deletedSprite = false;
			_entitySprite.move(1.0f, 0.0f);
			window.clear();
		}
		else
		{
			deletedSprite = true;
		}
		if (_entitySprite.getPosition().x >= houseSprite.getPosition().x)
		{
			deletedSprite = true;
		}
	}

	void Entity::Draw(sf::RenderWindow& window)
	{
		window.draw(_entitySprite);
	}

	void  Entity::MultipleDraw(sf::RenderWindow& window)
	{
		ZoneScopedN("Draw multiple");
		
		for (int i = 0; i < 30000; i++)
		{
			window.draw(_entitySprite);
		}
	}

	LPVOID Entity::MultipleDrawFiber(sf::RenderWindow& window)
	{
		ZoneScopedN("Draw multiple fiber");

		for (int i = 0; i < 30000; i++)
		{
			window.draw(_entitySprite);
		}
		return 0;
	}


	LPFIBER_START_ROUTINE Entity::DrawFiber(sf::RenderWindow& window)
	{
		ZoneScopedN("Draw multiple fiber");

		for (int i = 0; i < 30000; i++)
		{
			 window.draw(_entitySprite);
		}
		return nullptr;
	}
}
