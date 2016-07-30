#include "Common.h"
#include "Logging.h"

#include <stdarg.h>
#include <time.h>

VINYL_NS_BEGIN;

namespace Log
{
	FormatVar::FormatVar(int16_t i)
	{
		m_type = FVT_Integer_16;
		m_data = (void*)i;
	}

	FormatVar::FormatVar(int32_t i)
	{
		m_type = FVT_Integer_32;
		m_data = (void*)i;
	}

	FormatVar::FormatVar(uint16_t i)
	{
		m_type = FVT_UnsignedInteger_16;
		m_data = (void*)i;
	}

	FormatVar::FormatVar(uint32_t i)
	{
		m_type = FVT_UnsignedInteger_32;
		m_data = (void*)i;
	}

	FormatVar::FormatVar(void* p)
	{
		m_type = FVT_Pointer;
		m_data = p;
	}

	FormatVar::FormatVar(const char* p)
	{
		m_type = FVT_String;
		m_data = (void*)p;
	}

	FormatVar::FormatVar(char c)
	{
		m_type = FVT_Char;
		m_data = (void*)c;
	}

	FormatVar::~FormatVar()
	{
		if (m_cache != nullptr) {
			free(m_cache);
		}
	}

	const char* FormatVar::GetString()
	{
		if (m_type >= FVT_Integer_16 && m_type <= FVT_UnsignedInteger_32) {
			if (m_cache == nullptr) {
				m_cache = (char*)malloc(14);
			}
			if (m_type == FVT_UnsignedInteger_16 || m_type == FVT_UnsignedInteger_32) {
#ifdef _LP64
				uint32_t num = (uint32_t)(long int)m_data;
#else
				uint32_t num = (uint32_t)m_data;
#endif
				snprintf(m_cache, 14, "%u", num);
			} else {
#ifdef _LP64
				int32_t num = (int32_t)(long int)m_data;
#else
				int32_t num = (int32_t)m_data;
#endif
				snprintf(m_cache, 14, "%d", num);
			}
			return m_cache;

		} else if (m_type == FVT_String) {
			return (const char*)m_data;

		} else if (m_type == FVT_Pointer) {
			if (m_cache == nullptr) {
				m_cache = (char*)malloc(24);
			}
			snprintf(m_cache, 24, "%p", m_data);
			return m_cache;

		} else if (m_type == FVT_Char) {
			if (m_cache == nullptr) {
				m_cache = (char*)malloc(2);
				m_cache[1] = '\0';
			}
#ifdef _LP64
			m_cache[0] = (char)(long int)m_data;
#else
			m_cache[0] = (char)(int)m_data;
#endif
			return m_cache;
		}

		return "";
	}

	s::String Format(const char* format, va_list vl)
	{
		s::StackArray<FormatVar> vars;
#if !defined(__clang__)
		vars.sa_bOnlyPop = true;
#endif

		int numHighest = 0;

		int bufferLen = 128;
		s::String ret;

		int len = strlen(format);
		for (int i = 0; i < len; i++) {
			char c = format[i];
			if (c == '%') {
				char cn = format[++i];
				char str[5];
				int strc = 0;
				while (cn >= '0' && cn <= '9' && strc < 4 && i < len) {
					str[strc] = cn;
					strc++;
					cn = format[++i];
				}
				i--;
				str[strc] = '\0';
				int num = atoi(str);
				if (num == 0) {
					continue;
				}
				if (num > numHighest) {
					for (int j = numHighest; j < num; j++) {
#if defined(__clang__)
						FormatVar* newvar = va_arg(vl, FormatVar*);
						vars.Push(newvar);
#else
						FormatVar &newvar = va_arg(vl, FormatVar);
						vars.Push(&newvar);
#endif
					}
					numHighest = num;
				}
				FormatVar &var = vars[num - 1];
				ret += var.GetString();
				continue;
			}

			ret += c;
		}

		return ret;
	}

	LogLevel Level = LogLevel_Info;

	static s::Mutex _mutex;

	static s::Stream* _streamCurrent = nullptr;
	static s::Stream* _streamAll = nullptr;

	static void WriteLogLine(const s::String &line)
	{
		if (_streamCurrent != nullptr) {
			_streamCurrent->WriteLine(line);
		}
		if (_streamAll != nullptr) {
			_streamAll->WriteLine(line);
		}
	}

	static void FlushLog()
	{
		if (_streamCurrent != nullptr) {
			_streamCurrent->Flush();
		}
		if (_streamAll != nullptr) {
			_streamAll->Flush();
		}
	}

	static void LogLine(const char* keyword, const s::String &str, int16_t color_windows, const char* color_linux)
	{
		_mutex.Lock();

#define LOG_FORMAT_BEGIN "%7s | %02d:%02d:%02d | "

		time_t t = time(nullptr);
		tm tms;
#if WINDOWS
		localtime_s(&tms, &t);
#else
		localtime_r(&t, &tms);
#endif

		s::String lineBegin = s::strPrintF(LOG_FORMAT_BEGIN, keyword, tms.tm_hour, tms.tm_min, tms.tm_sec);

#if WINDOWS
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, color_windows);
		printf("%s%s\n", (const char*)lineBegin, (const char*)str);
		SetConsoleTextAttribute(hConsole, 7);
#else
		printf("\x1B[%sm%s%s\x1B[0m\n", color_linux, (const char*)lineBegin, (const char*)str);
#endif

		WriteLogLine(lineBegin + str);
		FlushLog();

		_mutex.Unlock();
	}

	void StartLogFile(const char* fnm)
	{
		ASSERT(_streamCurrent == nullptr && _streamAll == nullptr);

		s::String fnmCurrent = s::strPrintF("%s.log", fnm);
		s::String fnmAll = s::strPrintF("%s.All.log", fnm);

		s::FileStream* fsCurrent = new s::FileStream;
		fsCurrent->Open(fnmCurrent, "w+b", true);
		_streamCurrent = fsCurrent;

		s::FileStream* fsAll = new s::FileStream;
		fsAll->Open(fnmAll, "a+b", true);
		_streamAll = fsAll;

		time_t t = time(nullptr);
		tm tms;
#if WINDOWS
		localtime_s(&tms, &t);
#else
		localtime_r(&t, &tms);
#endif

		WriteLogLine("");
		WriteLogLine(s::strPrintF("  --- Start of log file: %d-%02d-%02d %02d:%02d:%02d ---",
			1900 + tms.tm_year, tms.tm_mon, tms.tm_mday,
			tms.tm_hour, tms.tm_min, tms.tm_sec));
		WriteLogLine("");
		FlushLog();
	}

	void EndLogFile()
	{
		ASSERT(_streamCurrent != nullptr && _streamAll != nullptr);

		delete _streamCurrent;
		delete _streamAll;

		_streamCurrent = _streamAll = nullptr;
	}

	void Trace(const char* format, ...)
	{
		if (Level > LogLevel_Trace) {
			return;
		}

		va_list vl;
		va_start(vl, format);
		s::String str = Format(format, vl);
		va_end(vl);

		LogLine("TRACE", str, 8, "02");
	}

	void Debug(const char* format, ...)
	{
		if (Level > LogLevel_Debug) {
			return;
		}

		va_list vl;
		va_start(vl, format);
		s::String str = Format(format, vl);
		va_end(vl);

		LogLine("DEBUG", str, 7, "38");
	}

	void Info(const char* format, ...)
	{
		if (Level > LogLevel_Info) {
			return;
		}

		va_list vl;
		va_start(vl, format);
		s::String str = Format(format, vl);
		va_end(vl);

		LogLine("INFO", str, 15, "37");
	}

	void Warning(const char* format, ...)
	{
		if (Level > LogLevel_Warning) {
			return;
		}

		va_list vl;
		va_start(vl, format);
		s::String str = Format(format, vl);
		va_end(vl);

		LogLine("WARNING", str, 13, "35");
	}

	void Error(const char* format, ...)
	{
		if (Level > LogLevel_Error) {
			return;
		}

		va_list vl;
		va_start(vl, format);
		s::String str = Format(format, vl);
		va_end(vl);

		LogLine("ERROR", str, 14, "33");
	}

	void Fatal(const char* format, ...)
	{
		if (Level > LogLevel_Fatal) {
			return;
		}

		va_list vl;
		va_start(vl, format);
		s::String str = Format(format, vl);
		va_end(vl);

		LogLine("FATAL", str, 12, "31");
	}
}

VINYL_NS_END;
