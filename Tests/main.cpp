#include <stdio.h>

#include <Folder.h>
#include <Logging.h>

int main()
{
	v::Log::Level = v::Log::LogLevel_Trace;
	v::Log::Trace("Hello, world: %1 %2 %3", FVAR('>'), FVAR("Vinyl"), FVAR(10));
	v::Log::Debug("Hello, world: %1", FVAR("Hey"));
	v::Log::Info("Hello, world!");
	v::Log::Warning("Hello, world!");
	v::Log::Error("Hello, world!");
	v::Log::Fatal("Hello, world!");

	getchar();
	return 0;
}
