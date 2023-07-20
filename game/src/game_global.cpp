#include "game_global.h"

namespace CityBuilderGame
{
	sf::Vector2f& GetMouse()
	{
		sf::Mouse _mouseRef;
		sf::Vector2f _mousePos = static_cast<sf::Vector2f>(_mouseRef.getPosition());
		sf::Vector2f _result = sf::Vector2f{ _mousePos.x,_mousePos.y };
		return _result;
	}
}

