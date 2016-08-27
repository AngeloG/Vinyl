#pragma once

#include "Common.h"
#include "Mountpoint.h"

VINYL_NS_BEGIN;

class MountpointFs : public Mountpoint
{
public:
	virtual bool IsReadOnly();

	virtual bool FileExists(const char* fnm);
	virtual bool FileCopy(const char* fnmA, const char* fnmB, bool should_override = false);
	virtual bool FileMove(const char* fnmA, const char* fnmB);
	virtual bool FileDelete(const char* fnm);

	virtual s::String FileReadAll(const char* fnm);
	virtual bool FileWriteAll(const char* fnm, const char* content);

	virtual void FolderGetIndex(FolderIndex &ret, const char* dir, bool recursive, const FolderIndexFilter &filter);

private:
	void ReadDir(FolderIndex &ret, void* impl, const char* dirname, bool recursive, const FolderIndexFilter &filter);
};

VINYL_NS_END;
