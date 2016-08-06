#pragma once

#include "Common.h"

VINYL_NS_BEGIN;

class Mountpoint
{
public:
	virtual bool IsReadOnly() = 0;

	virtual bool FileExists(const char* fnm) = 0;
	virtual bool FileCopy(const char* fnmA, const char* fnmB, bool should_override = false) = 0;
	virtual bool FileMove(const char* fnmA, const char* fnmB) = 0;
	virtual bool FileDelete(const char* fnm) = 0;

	virtual s::String FileReadAll(const char* fnm) = 0;
	virtual void FileWriteAll(const char* fnm, const char* content) = 0;

	virtual FolderIndex FolderGetIndex(const char* dir, bool recursive);
};

namespace Mount
{
	void Add(Mountpoint* point);
	void Remove(Mountpoint* point);
}

VINYL_NS_END;
