#pragma once

class PacketType
{
public:
	using Type = unsigned char;
	static constexpr const Type Position = 0;
	static constexpr const Type BorderColor = 1;
	static constexpr const Type FillColorDelta = 2;
};
