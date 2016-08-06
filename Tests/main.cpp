#include <stdio.h>

#include <Logging.h>
#include <File.h>

int main()
{
	v::Log::Level = v::Log::LogLevel_Trace;

	int x, y;
	x = 10;
	y = 20;

	v::Log::Info("This is a test %1 and %2.", FVAR(x), FVAR(y));

	return 0;
}
