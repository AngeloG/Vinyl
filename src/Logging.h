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

	enum FormatVarType : int
	{
		FVT_Unknown,
		FVT_Integer_16,
		FVT_Integer_32,
		FVT_UnsignedInteger_16,
		FVT_UnsignedInteger_32,
		FVT_String,
		FVT_Pointer,
		FVT_Char
	};

	class FormatVar
	{
	public:
		void* m_data;
		FormatVarType m_type;
		char* m_cache = nullptr;

	public:
		FormatVar(int16_t i);
		FormatVar(int32_t i);
		FormatVar(uint16_t i);
		FormatVar(uint32_t i);
		FormatVar(void* p);
		FormatVar(const char* p);
		FormatVar(char c);
		~FormatVar();

		const char* GetString();
	};
}

VINYL_NS_END;

#if defined(__clang__)
#define FVAR(var) new v::Log::FormatVar(var)
#else
#define FVAR(var) v::Log::FormatVar(var)
#endif
