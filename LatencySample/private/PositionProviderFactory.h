#pragma once

#include "IPositionProvider.h"
#include "SFML/Network.hpp"

class PositionProviderFactory
{
public:
	static std::unique_ptr<IPositionProvider> createPositionProvider(std::uint16_t _port, bool _server, bool _udp, float _networkUpdateTime);
};