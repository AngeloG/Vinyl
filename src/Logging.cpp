#include "Common.h"
#include "Logging.h"

VINYL_NS_BEGIN;

namespace Log
{
	FormatVarInt::FormatVarInt(int i)
	{
		m_value = i;
	}

	FormatVarInt::~FormatVarInt()
	{
		if (m_cache != nullptr) {
			delete m_cache;
		}
	}

	const char* FormatVarInt::GetString()
	{
		if (m_cache != nullptr) {
			delete m_cache;
		}
		m_cache = (char*)malloc(14);
		snprintf(m_cache, 14, "%d", m_value);
		return m_cache;
	}
	
	FormatVarString::FormatVarString(const char* str)
	{
		m_value = str;
	}

	const char* FormatVarString::GetString()
	{
		return m_value;
	}

	s::String Format(const char* format, va_list vl)
	{
		s::StackArray<FormatVar> vars;
		vars.sa_bOnlyPop = true;

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
						FormatVar &newvar = va_arg(vl, FormatVar);
						vars.Push(&newvar);
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

	void Trace(const char* format, ...)
	{
		_mutex.Lock();

		va_list vl;
		va_start(vl, format);
		s::String str = Format(format, vl);
		va_end(vl);

#if WINDOWS
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, 8);
		printf("[TRACE] ");
		SetConsoleTextAttribute(hConsole, 7);
		printf("%s\n", (const char*)str);
#else
		printf("[TRACE] %s\n", (const char*)str);
#endif

		_mutex.Unlock();
	}

	void Debug(const char* format, ...)
	{
		_mutex.Lock();

		va_list vl;
		va_start(vl, format);
		s::String str = Format(format, vl);
		va_end(vl);

		printf("[DEBUG] %s\n", (const char*)str);

		_mutex.Unlock();
	}

	void Info(const char* format, ...)
	{
		_mutex.Lock();

		va_list vl;
		va_start(vl, format);
		s::String str = Format(format, vl);
		va_end(vl);

#if WINDOWS
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, 15);
		printf("[INFO] ");
		SetConsoleTextAttribute(hConsole, 7);
		printf("%s\n", (const char*)str);
#else
		printf("\x1B[01m[INFO]\x1B[0m %s\n", (const char*)str);
#endif

		_mutex.Unlock();
	}

	void Warning(const char* format, ...)
	{
		_mutex.Lock();

		va_list vl;
		va_start(vl, format);
		s::String str = Format(format, vl);
		va_end(vl);

#if WINDOWS
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, 13);
		printf("[WARNING] ");
		SetConsoleTextAttribute(hConsole, 7);
		printf("%s\n", (const char*)str);
#else
		printf("\x1B[35m[WARNING]\x1B[0m %s\n", (const char*)str);
#endif

		_mutex.Unlock();
	}

	void Error(const char* format, ...)
	{
		_mutex.Lock();

		va_list vl;
		va_start(vl, format);
		s::String str = Format(format, vl);
		va_end(vl);

#if WINDOWS
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, 14);
		printf("[ERROR] ");
		SetConsoleTextAttribute(hConsole, 7);
		printf("%s\n", (const char*)str);
#else
		printf("\x1B[33m[ERROR]\x1B[0m %s\n", (const char*)str);
#endif

		_mutex.Unlock();
	}

	void Fatal(const char* format, ...)
	{
		_mutex.Lock();

		va_list vl;
		va_start(vl, format);
		s::String str = Format(format, vl);
		va_end(vl);

#if WINDOWS
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, 12);
		printf("[FATAL] ");
		SetConsoleTextAttribute(hConsole, 7);
		printf("%s\n", (const char*)str);
#else
		printf("\x1B[31m[FATAL]\x1B[0m %s\n", (const char*)str);
#endif

		_mutex.Unlock();
	}
}

VINYL_NS_END;
