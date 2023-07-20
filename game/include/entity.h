#pragma once
#include <SFML/Graphics.hpp>
#include <Windows.h>

#include "job_system.h"

namespace CityBuilderGame
{
	class Entity
	{
	public:

		void Init();

		void Update(sf::Sprite houseSprite,sf::RenderWindow& window);

		void Draw(sf::RenderWindow& window);

		void MultipleDraw(sf::RenderWindow& window);

		LPVOID MultipleDrawFiber(sf::RenderWindow& window);

		LPFIBER_START_ROUTINE DrawFiber(sf::RenderWindow& window);
	private:
		sf::Sprite _entitySprite;
		sf::Texture _entityTexture;
	};
	
}
