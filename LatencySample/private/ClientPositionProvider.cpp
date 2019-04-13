#include "ClientPositionProvider.h"
#include "SFML/Network.hpp"
#include "PacketType.h"

void ClientPositionProvider::update(float)
{
	updateClient();
	sf::Packet packet;
	while (tryReadPacket(packet))
	{
		PacketType::Type type{};
		packet >> type;
		switch (type)
		{
		case PacketType::Position:
			packet >> m_position.x >> m_position.y;
			break;
		case PacketType::BorderColor:
			packet >> m_borderColor.r >> m_borderColor.g >> m_borderColor.b;
			break;
		case PacketType::FillColorDelta:
			sf::Int16 r, g, b;
			packet >> r >> g >> b;
			m_fillColor.r += r;
			m_fillColor.g += g;
			m_fillColor.b += b;
			break;
		}
		packet >> m_position.x >> m_position.y;
	}
}

TcpClientPositionProvider::TcpClientPositionProvider(std::uint16_t _port)
	: m_port(_port)
{
	m_socket.setBlocking(false);
	updateClient();
}

void TcpClientPositionProvider::updateClient()
{
	if (m_connected)
		return;
	m_socket.connect("127.0.0.1", m_port);
	m_connected = true;
}

bool TcpClientPositionProvider::tryReadPacket(sf::Packet& _packet)
{
	sf::Socket::Status status = m_socket.receive(_packet);
	m_connected = (status != sf::Socket::Status::Disconnected);
	return (status == sf::Socket::Status::Done);
}

UdpClientPositionProvider::UdpClientPositionProvider(std::uint16_t _port)
{
	m_socket.setBlocking(false);
	m_socket.bind(_port, "127.0.0.1");
}

void UdpClientPositionProvider::updateClient() {}
bool UdpClientPositionProvider::tryReadPacket(sf::Packet& _packet)
{
	sf::IpAddress originAddress;
	unsigned short originPort;

	sf::Socket::Status status = m_socket.receive(_packet, originAddress, originPort);
	return status == sf::Socket::Status::Done;
}