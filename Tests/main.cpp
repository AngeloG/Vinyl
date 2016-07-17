#include <stdio.h>

#include <Logging.h>
#include <File.h>

int main()
{
	v::Log::Level = v::Log::LogLevel_Trace;

	bool ok = v::File::Copy("test.txt", "test2.txt", true);
	v::Log::Info("Copy: %1", FVAR(ok ? "OK" : "ERROR"));

	return 0;
}
