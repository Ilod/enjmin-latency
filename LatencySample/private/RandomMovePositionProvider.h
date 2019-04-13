#pragma once

#include "IPositionProvider.h"
#include "SFML/Network.hpp"
#include <random>

class RandomMovePositionProvider : public IPositionProvider
{
public:
	RandomMovePositionProvider(float _speed, float _directionChangeDuration, float _fillColorDuration, float _borderColorDuration);
	void update(float _dt) override;
	sf::Vector2f getPosition() const override { return m_position; }
	sf::Color getFillColor() const override { return m_fillColor; }
	sf::Color getBorderColor() const override { return m_borderColor; }
private:
	void changeAngle();
	sf::Color getRandomColor();

	sf::Vector2f m_position{ .5f, .5f };
	float m_speed = 0;

	float m_directionChangeDuration = 0;
	float m_angle = 0;
	float m_timeSinceAngleChange = 0;

	sf::Color m_fillColor = sf::Color::White;
	float m_fillColorDuration = 0;
	float m_timeSinceFillColorChange = 0;

	sf::Color m_borderColor = sf::Color::Black;
	float m_borderColorDuration = 0;
	float m_timeSinceBorderColorChange = 0;

	std::random_device m_random;
};