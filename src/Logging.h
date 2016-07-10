#pragma once

#include "Common.h"

VINYL_NS_BEGIN;

namespace Log
{
	enum LogLevel
	{
		LogLevel_Trace,
		LogLevel_Debug,
		LogLevel_Info,
		LogLevel_Warning,
		LogLevel_Error
	};

	class FormatVar
	{
	public:
		~FormatVar() {}

		virtual const char* GetString() = 0;
	};

	class FormatVarInt : public FormatVar
	{
	public:
		int m_value;
		char* m_cache = nullptr;

	public:
		FormatVarInt(int i);
		virtual ~FormatVarInt();

		virtual const char* GetString();
	};

	class FormatVarString : public FormatVar
	{
	public:
		const char* m_value;

	public:
		FormatVarString(const char* str);

		virtual const char* GetString();
	};

	extern s::String Format(const char* format, va_list vl);

	extern LogLevel Level;

	extern void Trace(const char* format, ...);
	extern void Debug(const char* format, ...);
	extern void Info(const char* format, ...);
	extern void Warning(const char* format, ...);
	extern void Error(const char* format, ...);
	extern void Fatal(const char* format, ...);
}

VINYL_NS_END;

#define FINT(var) v::Log::FormatVarInt(var)
#define FSTR(var) v::Log::FormatVarString(var)
