#pragma once

#include "Common.h"

VINYL_NS_BEGIN;

typedef s::Function<bool(const s::Filename &)> FolderIndexFilter;

const uint32_t FolderIndexFlags_Recursive = (1 << 0);
const uint32_t FolderIndexFlags_Sorted = (1 << 1);

class FolderIndex
{
public:
	FolderIndex();
	FolderIndex(const char* path, uint32_t flags);
	FolderIndex(const char* path, uint32_t flags, const FolderIndexFilter &filter);
	~FolderIndex();

	void Populate(uint32_t flags);
	void Populate(uint32_t flags, const FolderIndexFilter &filter);
private:
	void AfterPopulate(uint32_t flags);

public:
	void Clear();

	const char* GetPath();

	int GetFileCount();
	const char* GetFilePath(int i);

	int GetDirCount();
	const char* GetDirPath(int i);

public:
	s::String m_path;
	s::StackArray<s::String> m_files;
	s::StackArray<s::String> m_dirs;
};

class Folder
{
public:
	Folder(const char* path);
	~Folder();

	const char* GetPath();

	FolderIndex GetIndex(uint32_t flags = 0);
	FolderIndex GetIndex(const FolderIndexFilter &filter, uint32_t flags = 0);

private:
	s::String m_path;
};

VINYL_NS_END;
