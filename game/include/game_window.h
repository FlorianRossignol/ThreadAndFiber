#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "game_world.h"
#include "game_building.h"

namespace CityBuilderGame
{
	class window_game
	{
		
	public:
		window_game() = default;
		
		int Create_Window(std::string name);

	private:
			int _height = 1920;
			int _width = 1080;
	};

}
