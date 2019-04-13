#pragma once

#include "IPositionProvider.h"
#include "SFML/Network.hpp"

class ServerPositionProvider : public IPositionProvider
{
public:
	ServerPositionProvider(std::unique_ptr<IPositionProvider> _provider, float _networkUpdateTime);
	void update(float _dt) override final;
	sf::Vector2f getPosition() const override final { return m_provider->getPosition(); }
	sf::Color getFillColor() const override final { return m_provider->getFillColor(); }
	sf::Color getBorderColor() const override final { return m_provider->getBorderColor(); }
private:
	virtual void updateServer() = 0;
	virtual void sendPacket(const sf::Packet& _packet) = 0;

	std::unique_ptr<IPositionProvider> m_provider;
	sf::Color m_previousFillColor;
	sf::Color m_previousBorderColor;
	float m_networkUpdateTime = 0;
	sf::Clock m_networkUpdateClock;
};

class TcpServerPositionProvider : public ServerPositionProvider
{
public:
	TcpServerPositionProvider(std::uint16_t _port, std::unique_ptr<IPositionProvider> _provider, float _networkUpdateTime);
private:
	void updateServer() override;
	void sendPacket(const sf::Packet& _packet) override;

	sf::TcpListener m_listener;
	std::vector<std::unique_ptr<sf::TcpSocket>> m_sockets;
};

class UdpServerPositionProvider : public ServerPositionProvider
{
public:
	UdpServerPositionProvider(std::uint16_t _port, std::unique_ptr<IPositionProvider> _provider, float _networkUpdateTime);
private:
	void updateServer() override;
	void sendPacket(const sf::Packet& _packet) override;

	sf::UdpSocket m_socket;
	std::uint16_t m_port;
};