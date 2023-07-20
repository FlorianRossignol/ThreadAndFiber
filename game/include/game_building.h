#pragma once
#include <SFML/Graphics.hpp>
#include "game_global.h"

namespace CityBuilderGame
{
	class Building
	{
	public:

		Building(float cost);

		void Init();

		void DrawHouse(sf::RenderWindow& window);

		void MultipleDraw(sf::RenderWindow& windows);

		void Update();

		sf::Sprite GetSprite();

	private:
		sf::Texture _houseTexture;
		sf::Sprite _houseSprite;
		float _cost;
	};
}
