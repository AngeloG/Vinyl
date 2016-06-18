#pragma once

#include "Common.h"

VINYL_NS_BEGIN;

class Folder;

class FolderIndex
{
	friend class Folder;

public:
	FolderIndex(const char* path, bool recursive);

	int GetFileCount();
	const char* GetFilePath(int i);

	int GetDirCount();
	const char* GetDirPath(int i);

private:
	int m_fileCount;
	const char* m_files;

	int m_dirCount;
	const char* m_dirs;
};

class Folder
{
public:
	Folder(const char* path);
	~Folder();

	FolderIndex GetIndex(bool recursive);

private:
	char* m_path;
};

VINYL_NS_END;
