#pragma once

#include "IPositionProvider.h"
#include "SFML/Network.hpp"

class ClientPositionProvider : public IPositionProvider
{
public:
	void update(float _dt) override final;
	sf::Vector2f getPosition() const override final { return m_position; }
	sf::Color getFillColor() const override final { return m_fillColor; }
	sf::Color getBorderColor() const override final { return m_borderColor; }
private:
	virtual void updateClient() = 0;
	virtual bool tryReadPacket(sf::Packet& _packet) = 0;

	sf::Vector2f m_position{ .5f, .5f };
	sf::Color m_fillColor = sf::Color::White;
	sf::Color m_borderColor = sf::Color::Black;
};

class TcpClientPositionProvider : public ClientPositionProvider
{
public:
	TcpClientPositionProvider(std::uint16_t _port);
private:
	void updateClient() override;
	bool tryReadPacket(sf::Packet& _packet) override;

	sf::TcpSocket m_socket;
	bool m_connected = false;
	std::uint16_t m_port;
};

class UdpClientPositionProvider : public ClientPositionProvider
{
public:
	UdpClientPositionProvider(std::uint16_t _port);
private:
	void updateClient() override;
	bool tryReadPacket(sf::Packet& _packet) override;

	sf::UdpSocket m_socket;
};