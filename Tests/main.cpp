#include <stdio.h>

#include <Folder.h>
#include <Logging.h>

int main()
{
	v::Log::Trace("Hello, world!");
	v::Log::Debug("Hello, world!");
	v::Log::Info("Hello, world!");
	v::Log::Warning("Hello, world!");
	v::Log::Error("Hello, world!");
	v::Log::Fatal("Hello, world!");

	getchar();
	return 0;
}
