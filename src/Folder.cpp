#include "Common.h"
#include "Folder.h"

#if !WINDOWS
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#endif

VINYL_NS_BEGIN;

FolderIndex::FolderIndex(const char* path, bool recursive)
{
	s::String pathname = path;
	if (!pathname.EndsWith("/")) {
		pathname += "/";
	}
	m_path = pathname;
#if WINDOWS
	ReadDir(nullptr, pathname, recursive);
#else
	ReadDir(opendir(pathname), pathname, recursive);
#endif
}

FolderIndex::~FolderIndex()
{
}

const char* FolderIndex::GetPath()
{
	return m_path;
}

void FolderIndex::ReadDir(void* impl, const char* dirname, bool recursive)
{
#if WINDOWS
	WIN32_FIND_DATA findData;

	s::String fnmDir = dirname;
	fnmDir += "*";

	HANDLE findHandle = FindFirstFile(fnmDir, &findData);
	if (findHandle == INVALID_HANDLE_VALUE) {
		return;
	}

	do {
		if (!strcmp(findData.cFileName, ".") || !strcmp(findData.cFileName, "..")) {
			continue;
		}

		s::String path = dirname;
		path += findData.cFileName;

		if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			path += "/";
			m_dirs.Push() = path;
			if (recursive) {
				ReadDir(nullptr, path, true);
			}
		} else {
			m_files.Push() = path;
		}
	} while (FindNextFile(findHandle, &findData));
#else
	DIR* d = (DIR*)impl;
	if (!d) { return; }

	struct dirent* dir;
	while ((dir = readdir(d)) != nullptr) {
		if (!strcmp(dir->d_name, ".") || !strcmp(dir->d_name, "..")) {
			continue;
		}

		s::String path = dirname;
		path += dir->d_name;

		struct stat fst;
		stat(path, &fst);
		if (S_ISREG(fst.st_mode)) {
			m_files.Push() = path;
		} else {
			path += "/";
			m_dirs.Push() = path;
			if (recursive) {
				ReadDir(opendir(path), path, true);
			}
		}
	}
#endif
}

int FolderIndex::GetFileCount()
{
	return m_files.Count();
}

const char* FolderIndex::GetFilePath(int i)
{
	return m_files[i];
}

int FolderIndex::GetDirCount()
{
	return m_dirs.Count();
}

const char* FolderIndex::GetDirPath(int i)
{
	return m_dirs[i];
}

Folder::Folder(const char* path)
{
	m_path = path;
}

Folder::~Folder()
{
}

const char* Folder::GetPath()
{
	return m_path;
}

FolderIndex Folder::GetIndex(bool recursive)
{
	return FolderIndex(m_path, recursive);
}

VINYL_NS_END;
