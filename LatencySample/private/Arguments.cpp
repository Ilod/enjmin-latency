#include "Arguments.h"
#include <string>
#include <charconv>
#include <cctype>

template<> void Arguments::parseString<char>(std::string_view _str, char& _val)
{
	if (!_str.empty())
		_val = *_str.begin();
	else
		_val = '\0';
}

template<> void Arguments::parseString<bool>(std::string_view _str, bool& _val)
{
	if (_str == "1")
	{
		_val = true;
		return;
	}
	std::string_view trueStr{ "true" };
	_val = (_str.size() == trueStr.size() && std::equal(_str.begin(), _str.end(), trueStr.begin(), [](auto c1, auto c2) { return c1 == c2 || std::toupper(c1) == std::toupper(c2); }));
}

template<> void Arguments::parseString<std::string>(std::string_view _str, std::string& _val)
{
	_val = _str;
}

void Arguments::registerArgumentParser(const std::vector<std::string_view>& _names, std::unique_ptr<IArgumentParser> _argument)
{
	std::shared_ptr<IArgumentParser> argument{ _argument.release() };
	for (auto& name : _names)
		m_arguments.emplace(name, argument);
}

void Arguments::parse(int argc, char* argv[])
{
	std::vector<std::string_view> args;
	args.reserve(argc);
	for (int i = 0; i < argc; ++i)
		args.emplace_back(argv[i]);
	parse(args);
}

void Arguments::parse(const std::vector<std::string_view>& args)
{
	for (std::size_t idx = 0; idx < args.size(); ++idx)
	{
		auto it = m_arguments.find(std::string{ args[idx] });
		if (it == m_arguments.end())
			continue;
		std::optional<std::string_view> argStr;
		if (idx + 1 < args.size())
			argStr.emplace(args[idx + 1]);
		if (it->second->parse(argStr))
			++idx;
	}
}