#include "Common.h"
#include "Mountpoint.h"
#include "MountpointFs.h"

VINYL_NS_BEGIN;

bool Mountpoint::FileExists(const char* fnm)
{
	return false;
}

bool Mountpoint::FileCopy(const char* fnmA, const char* fnmB, bool should_override)
{
	return false;
}

bool Mountpoint::FileMove(const char* fnmA, const char* fnmB)
{
	return false;
}

bool Mountpoint::FileDelete(const char* fnm)
{
	return false;
}

s::String Mountpoint::FileReadAll(const char* fnm)
{
	return "";
}

bool Mountpoint::FileWriteAll(const char* fnm, const char* content)
{
	return false;
}

void Mountpoint::FolderGetIndex(FolderIndex &ret, const char* dir, uint32_t flags, const FolderIndexFilter &filter)
{
}

namespace Mount
{
	s::StackArray<Mountpoint> Points;

	void Add(Mountpoint* point)
	{
		Points.Push(point);
	}

	void Remove(Mountpoint* point)
	{
		Points.PopAt(Points.FindPointer(point));
	}
}

VINYL_NS_END;
