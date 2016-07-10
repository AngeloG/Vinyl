#pragma once

#include "Common.h"

VINYL_NS_BEGIN;

namespace Log
{
	enum LogLevel : int
	{
		LogLevel_Trace,
		LogLevel_Debug,
		LogLevel_Info,
		LogLevel_Warning,
		LogLevel_Error,
		LogLevel_Fatal
	};

	extern s::String Format(const char* format, va_list vl);

	extern LogLevel Level;

	extern void Trace(const char* format, ...);
	extern void Debug(const char* format, ...);
	extern void Info(const char* format, ...);
	extern void Warning(const char* format, ...);
	extern void Error(const char* format, ...);
	extern void Fatal(const char* format, ...);

	enum FormatVarType
	{
		FVT_Integer,
		FVT_String,
		FVT_Char
	};

	class FormatVar
	{
	public:
		void* m_data;
		FormatVarType m_type;
		char* m_cache = nullptr;

	public:
		FormatVar(int i);
		FormatVar(const char* str);
		FormatVar(char c);
		~FormatVar();

		const char* GetString();
	};
}

VINYL_NS_END;

#define FVAR(var) v::Log::FormatVar(var)
