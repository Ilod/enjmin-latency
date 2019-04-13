#pragma once

#include <vector>
#include <string_view>
#include <unordered_map>
#include <charconv>
#include <optional>

class Arguments
{
public:
	template<typename ArgType> void registerArgument(const std::vector<std::string_view>& _names, ArgType& _argument)
	{
		registerArgumentParser(_names, std::make_unique<ArgumentParser<ArgType>>(_argument));
	}

	template<typename ArgType> void registerFlag(const std::vector<std::string_view>& _names, ArgType& _argument, ArgType _valueToSet)
	{
		registerArgumentParser(_names, std::make_unique<FlagParser<ArgType>>(_argument, _valueToSet));
	}

	void parse(int argc, char* argv[]);
	void parse(const std::vector<std::string_view>& _arguments);
private:
	class IArgumentParser
	{
	public:
		virtual ~IArgumentParser() = default;
		virtual bool parse(std::optional<std::string_view> _str) = 0;
	};

	template<typename ArgType>
	static void parseString(std::string_view _str, ArgType& _argument) { std::from_chars(_str.data(), _str.data() + _str.size(), _argument); }

	template<typename ArgType>
	class ArgumentParser : public IArgumentParser
	{
	public:
		ArgumentParser(ArgType& _argument) : m_argument(_argument) {}
		bool parse(std::optional<std::string_view> _str)
		{
			if (!_str)
				return false;
			Arguments::parseString(*_str, m_argument);
			return true;
		}
	private:
		ArgType& m_argument;
	};

	template<typename ArgType>
	class FlagParser : public IArgumentParser
	{
	public:
		FlagParser(ArgType& _argument, ArgType _valueToSet) : m_argument(_argument), m_valueToSet(std::move(_valueToSet)) {}
		bool parse(std::optional<std::string_view>)
		{
			m_argument = m_valueToSet;
			return false;
		}
	private:
		ArgType& m_argument;
		ArgType m_valueToSet;
	};

	void registerArgumentParser(const std::vector<std::string_view>& _names, std::unique_ptr<IArgumentParser> _argument);

	std::unordered_map<std::string, std::shared_ptr<IArgumentParser>> m_arguments;
};

template<typename ArgType>
class Argument final
{
public:
	Argument(ArgType _defaultValue, Arguments* _parent, std::vector<std::string_view> _names, std::vector<std::pair<std::vector<std::string_view>, ArgType>> _flags)
		: m_value(std::move(_defaultValue))
	{
		if (_parent)
		{
			_parent->registerArgument(_names, m_value);
			for (auto& flagValue : _flags)
				_parent->registerFlag(flagValue.first, m_value, flagValue.second);
		}
	}

	operator const ArgType&() const { return m_value; }
	const ArgType& operator*() const { return m_value; }
	ArgType& operator=(ArgType _value) { m_value = std::move(_value); }
private:
	ArgType m_value;
};

extern template void Arguments::parseString<char>(std::string_view, char&);
extern template void Arguments::parseString<bool>(std::string_view, bool&);
extern template void Arguments::parseString<std::string>(std::string_view, std::string&);