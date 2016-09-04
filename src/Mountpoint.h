#pragma once

#include "Common.h"
#include "Folder.h"

VINYL_NS_BEGIN;

class Mountpoint
{
public:
	virtual bool IsReadOnly() = 0;

	virtual bool FileExists(const char* fnm);
	virtual bool FileCopy(const char* fnmA, const char* fnmB, bool should_override = false);
	virtual bool FileMove(const char* fnmA, const char* fnmB);
	virtual bool FileDelete(const char* fnm);

	virtual s::String FileReadAll(const char* fnm);
	virtual bool FileWriteAll(const char* fnm, const char* content);

	virtual void FolderGetIndex(FolderIndex &ret, const char* dir, uint32_t flags, const FolderIndexFilter &filter);
};

namespace Mount
{
	// Mountpoints.
	// NOTE: It's generally a good idea to only have 1 writable mountpoint!
	//       Having multiple writable mountpoints means the first one to succeed will win.
	extern s::StackArray<Mountpoint> Points;

	void Add(Mountpoint* point);
	void Remove(Mountpoint* point);
}

VINYL_NS_END;
