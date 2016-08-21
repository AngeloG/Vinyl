#pragma once

#include "Common.h"
#include "Mountpoint.h"

#include "ZipFile.h"

VINYL_NS_BEGIN;

class MountpointZip : public Mountpoint
{
public:
	ZipFile m_zip;

public:
	MountpointZip(const char* fnm);

	virtual bool IsReadOnly();

	virtual bool FileExists(const char* fnm);

	virtual s::String FileReadAll(const char* fnm);

	virtual void FolderGetIndex(FolderIndex &ret, const char* dir, bool recursive, FolderIndexFilter* filter);
};

VINYL_NS_END;
