#pragma once

#include "Common.h"

VINYL_NS_BEGIN;

typedef s::Function<bool(const s::Filename &)> FolderIndexFilter;

class FolderIndex
{
public:
	FolderIndex();
	FolderIndex(const char* path, bool recursive);
	FolderIndex(const char* path, bool recursive, const FolderIndexFilter &filter);
	~FolderIndex();

	void Populate(bool recursive);
	void Populate(bool recursive, const FolderIndexFilter &filter);
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

	FolderIndex GetIndex(bool recursive = false);
	FolderIndex GetIndex(bool recursive, const FolderIndexFilter &filter);

private:
	s::String m_path;
};

VINYL_NS_END;
