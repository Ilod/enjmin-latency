#include "ServerPositionProvider.h"
#include "PacketType.h"

ServerPositionProvider::ServerPositionProvider(std::unique_ptr<IPositionProvider> _provider, float _networkUpdateTime)
	: m_provider(std::move(_provider))
	, m_networkUpdateTime(_networkUpdateTime)
{
	m_previousBorderColor = getBorderColor();
	m_previousFillColor = getFillColor();
}

void ServerPositionProvider::update(float _dt)
{
	m_provider->update(_dt);

	updateServer();
	
	if (m_networkUpdateClock.getElapsedTime().asSeconds() < m_networkUpdateTime)
		return;

	m_networkUpdateClock.restart();

	{
		auto position = m_provider->getPosition();
		sf::Packet packet;
		packet << PacketType::Position << position.x << position.y;
		sendPacket(packet);
	}

	if (getBorderColor() != m_previousBorderColor)
	{
		m_previousBorderColor = getBorderColor();

		{
			sf::Packet packet;
			packet << PacketType::BorderColor << (128 + m_previousBorderColor.r) << (128 + m_previousBorderColor.g) << (128 + m_previousBorderColor.b);
			sendPacket(packet);
		}
		{
			sf::Packet packet;
			packet << PacketType::BorderColor << m_previousBorderColor.r << m_previousBorderColor.g << m_previousBorderColor.b;
			sendPacket(packet);
		}
	}

	if (getFillColor() != m_previousFillColor)
	{
		sf::Color nextColor = getFillColor();
		sf::Int16 r = (sf::Int16)nextColor.r - m_previousFillColor.r;
		sf::Int16 g = (sf::Int16)nextColor.g - m_previousFillColor.g;
		sf::Int16 b = (sf::Int16)nextColor.b - m_previousFillColor.b;

		m_previousFillColor = nextColor;

		sf::Packet packet;
		packet << PacketType::FillColorDelta << r << g << b;
		sendPacket(packet);
	}
}

TcpServerPositionProvider::TcpServerPositionProvider(std::uint16_t _port, std::unique_ptr<IPositionProvider> _provider, float _networkUpdateTime)
	: ServerPositionProvider(std::move(_provider), _networkUpdateTime)
{
	m_listener.listen(_port, "127.0.0.1");
	m_listener.setBlocking(false);
}

void TcpServerPositionProvider::updateServer()
{
	sf::TcpSocket socket;
	if (m_listener.accept(socket) == sf::Socket::Done)
	{
		m_sockets.emplace_back(std::make_unique<sf::TcpSocket>(std::move(socket)));
	}
}

void TcpServerPositionProvider::sendPacket(const sf::Packet& _packet)
{
	for (auto& socket : m_sockets)
	{
		sf::Packet packet = _packet;
		sf::Socket::Status status;
		do
		{
			status = socket->send(packet);
		} while (status == sf::Socket::Status::Partial);
	}
}

UdpServerPositionProvider::UdpServerPositionProvider(std::uint16_t _port, std::unique_ptr<IPositionProvider> _provider, float _networkUpdateTime)
	: ServerPositionProvider(std::move(_provider), _networkUpdateTime)
	, m_port(_port)
{
	m_socket.setBlocking(false);
}

void UdpServerPositionProvider::updateServer()
{}

void UdpServerPositionProvider::sendPacket(const sf::Packet& _packet)
{
	sf::Packet packet = _packet;
	m_socket.send(packet, "127.0.0.1", m_port);
}