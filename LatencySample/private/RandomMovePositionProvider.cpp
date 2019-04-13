#include "RandomMovePositionProvider.h"

RandomMovePositionProvider::RandomMovePositionProvider(float _speed, float _directionChangeDuration, float _fillColorDuration, float _borderColorDuration)
	: m_speed(_speed)
	, m_directionChangeDuration(_directionChangeDuration)
	, m_fillColorDuration(_fillColorDuration)
	, m_borderColorDuration(_borderColorDuration)
{
	changeAngle();
}

void RandomMovePositionProvider::changeAngle()
{
	const float pi = 3.1415926535f;
	std::uniform_real_distribution<float> rnd{ 0, 2 * pi };
	m_angle = rnd(m_random);
	m_timeSinceAngleChange = 0;
}

void RandomMovePositionProvider::update(float _dt)
{
	float factor = _dt * m_speed;
	m_position.x = std::clamp(m_position.x + std::cos(m_angle) * factor, 0.f, 1.f);
	m_position.y = std::clamp(m_position.y + std::sin(m_angle) * factor, 0.f, 1.f);

	m_timeSinceAngleChange += _dt;
	if (m_timeSinceAngleChange >= m_directionChangeDuration)
		changeAngle();

	m_timeSinceFillColorChange += _dt;
	if (m_timeSinceFillColorChange >= m_fillColorDuration)
	{
		m_fillColor = getRandomColor();
		m_timeSinceFillColorChange = 0;
	}

	m_timeSinceBorderColorChange += _dt;
	if (m_timeSinceBorderColorChange >= m_borderColorDuration)
	{
		m_borderColor = getRandomColor();
		m_timeSinceBorderColorChange = 0;
	}
}

sf::Color RandomMovePositionProvider::getRandomColor()
{
	std::uniform_int_distribution<sf::Uint16> rnd{ 0, 255 };
	return sf::Color{ (sf::Uint8)rnd(m_random), (sf::Uint8)rnd(m_random), (sf::Uint8)rnd(m_random) };
}
