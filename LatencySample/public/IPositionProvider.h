#pragma once

#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"

class IPositionProvider
{
public:
	virtual ~IPositionProvider() = default;
	virtual sf::Vector2f getPosition() const = 0;
	virtual sf::Color getFillColor() const = 0;
	virtual sf::Color getBorderColor() const = 0;
	virtual void update(float _dt) = 0;
};