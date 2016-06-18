#include "Common.h"
#include "Folder.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>

VINYL_NS_BEGIN;

FolderIndex::FolderIndex(const char* path, bool recursive)
{
	s::String pathname = path;
	if (!pathname.EndsWith("/")) {
		pathname += "/";
	}
	ReadDir(opendir(pathname), pathname, recursive);
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
