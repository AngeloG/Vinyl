#include <stdio.h>

#include <Logging.h>
#include <File.h>
#include <Folder.h>

#include <Mountpoint.h>
#include <MountpointZip.h>

int main()
{
	v::Init();
	v::Log::Level = v::Log::LogLevel_Trace;
	v::Mount::Add(new v::MountpointZip("test1.zip"));
	v::Mount::Add(new v::MountpointZip("Test2.zip"));

	v::Folder folder("test");
	v::FolderIndex index = folder.GetIndex(true);

	for (int i = 0; i < index.GetFileCount(); i++) {
		const char* path = index.GetFilePath(i);
		v::Log::Info("File: %1", FVAR(path));
		v::Log::Info(" ** : %1", FVAR(v::File::ReadAll(path)));
	}
	for (int i = 0; i < index.GetDirCount(); i++) {
		v::Log::Info("Dir: %1", FVAR(index.GetDirPath(i)));
	}

	return 0;
}
