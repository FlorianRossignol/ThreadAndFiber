#include "game_building.h"

namespace CityBuilderGame
{
	Building::Building(float cost) : _cost(cost)
	{
		
	}

	void Building::Init()
	{
		if (!_houseTexture.loadFromFile("../game/Data/House.png"))
		{
			 throw std::runtime_error("Cant load an file");
		}
		_houseTexture.setSmooth(true);
		_houseSprite.setTexture(_houseTexture);
		//auto _mousePos = GetMouse();
		_houseSprite.setPosition(900.0f,625.0f);
		
	}

	void Building::DrawHouse(sf::RenderWindow& window)
	{
		
		window.draw(_houseSprite);
		
		if(_cost <= 1000.0f)
		{
			std::cout << "Dont have Money for build the house !";
		}
	
	}

	void Building::MultipleDraw(sf::RenderWindow& windows)
	{
		for (int i = 0; i < 100;i++)
		{
			windows.draw(_houseSprite);
		}
		
		if (_cost <= 1000.0f)
		{
			std::cout << "Dont have Money for build the house !";
		}
	}


	void Building::Update()
	{
	}

	sf::Sprite Building::GetSprite()
	{
		return _houseSprite;
	}

	
}
