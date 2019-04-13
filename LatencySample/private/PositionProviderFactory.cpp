#include "PositionProviderFactory.h"
#include "ClientPositionProvider.h"
#include "ServerPositionProvider.h"
#include "RandomMovePositionProvider.h"

std::unique_ptr<IPositionProvider> PositionProviderFactory::createPositionProvider(std::uint16_t _port, bool _server, bool _udp, float _networkUpdateTime)
{
	if (_server)
	{
		auto provider = std::make_unique<RandomMovePositionProvider>(0.5f, 0.5f, 7.f, 5.f);
		if (_udp)
		{
			return std::make_unique<UdpServerPositionProvider>(_port, std::move(provider), _networkUpdateTime);
		}
		else
		{
			return std::make_unique<TcpServerPositionProvider>(_port, std::move(provider), _networkUpdateTime);
		}
	}
	else
	{
		if (_udp)
		{
			return std::make_unique<UdpClientPositionProvider>(_port);
		}
		else
		{
			return std::make_unique<TcpClientPositionProvider>(_port);
		}
	}
}
