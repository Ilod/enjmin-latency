
////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "PositionProviderFactory.h"
#include "Arguments.h"
#include <random>
#include <string_view>
#include <vector>

class GameArguments : public Arguments
{
public:
	Argument<bool> m_server{ false, this, {}, { { {"--client"}, false}, {{"--server"}, true}} };
	Argument<bool> m_udp{ false, this, {}, { { {"--tcp"}, false}, {{"--udp"}, true}} };
	Argument<std::uint16_t> m_port{ 5789, this, {"--port"},{} };
	Argument<std::uint16_t> m_width{ 800, this, {"--width"},{} };
	Argument<std::uint16_t> m_height{ 600, this, {"--height"},{} };
	Argument<std::uint16_t> m_border{ 10, this, {"--border"},{} };
	Argument<float> m_timeBeforeUpdate{ 0, this, {"--update-time"},{} };
	Argument<float> m_timeFactor{ 1, this, {"--time-factor"},{} };
	Argument<std::uint16_t> m_xPos{ 0, this, {"--x"},{} };
	Argument<std::uint16_t> m_yPos{ 0, this, {"--y"},{} };
	Argument<float> m_networkUpdateRate{ 0, this, {"--network-update-time"},{} };
};

////////////////////////////////////////////////////////////
/// Entry point of application
///
/// \return Application exit code
///
////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
	GameArguments gameArgs;
	gameArgs.parse(argc, argv);

    // Define some constants

	constexpr float pi = 3.1415926535f;
    float ballRadius = 10.f;

	auto provider = PositionProviderFactory::createPositionProvider(gameArgs.m_port, gameArgs.m_server, gameArgs.m_udp, gameArgs.m_networkUpdateRate);

    // Create the window of the application
	sf::RenderWindow window(sf::VideoMode(gameArgs.m_width, gameArgs.m_height, 32), gameArgs.m_server ? "LatencySample Server" : "LatencySample Client",
                            sf::Style::Titlebar | sf::Style::Close);
	window.setPosition(sf::Vector2i(gameArgs.m_xPos, gameArgs.m_yPos));
    window.setVerticalSyncEnabled(true);

    // Create the ball
    sf::CircleShape ball;
    ball.setRadius(ballRadius / 2);
    ball.setOutlineThickness(ballRadius / 2);
    ball.setOutlineColor(sf::Color::Black);
    ball.setFillColor(sf::Color::White);
    ball.setOrigin(ballRadius / 2, ballRadius / 2);

	bool paused = false;

    sf::Clock clock;
    while (window.isOpen())
    {
        // Handle events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Window closed or escape key pressed: exit
            if ((event.type == sf::Event::Closed) ||
               ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape)))
            {
                window.close();
                break;
            }
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
			{
				paused = !paused;
			}
        }

		if (clock.getElapsedTime().asSeconds() >= gameArgs.m_timeBeforeUpdate)
		{
			float deltaTime = clock.restart().asSeconds() * gameArgs.m_timeFactor;

			if (!paused)
				provider->update(deltaTime);
		}

		sf::Vector2f position = provider->getPosition();
		ball.setFillColor(provider->getFillColor());
		ball.setOutlineColor(provider->getBorderColor());
		ball.setPosition(position.x * (gameArgs.m_width - gameArgs.m_border * 2) + gameArgs.m_border, position.y * (gameArgs.m_height - gameArgs.m_border * 2) + gameArgs.m_border);

        // Clear the window
        window.clear(sf::Color(50, 200, 50));
        
		window.draw(ball);

        // Display things on screen
        window.display();
    }

    return EXIT_SUCCESS;
}
