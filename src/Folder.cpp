#include "Common.h"
#include "Folder.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>

VINYL_NS_BEGIN;

FolderIndex::FolderIndex(const char* path, bool recursive)
{
	DIR* d = opendir(".");
	if (!d) {
		return;
	}

	struct dirent* dir;
	while ((dir = readdir(d)) != nullptr) {
		if (!strcmp(dir->d_name, ".") || !strcmp(dir->d_name, "..")) {
			continue;
		}

		struct stat fst;
		stat(dir->d_name, &fst);
		bool isFile = S_ISREG(fst.st_mode);
		if (isFile) {
			printf("File: %s\n", dir->d_name);
		} else {
			printf("Dir: %s\n", dir->d_name);
		}
	}
	closedir(d);
}

int FolderIndex::GetFileCount()
{
}

const char* FolderIndex::GetFilePath(int i)
{
}

int FolderIndex::GetDirCount()
{
}

const char* FolderIndex::GetDirPath(int i)
{
}

Folder::Folder(const char* path)
{
	int len = strlen(path);
	m_path = (char*)malloc(len + 1);
	strcpy(m_path, path);
}

Folder::~Folder()
{
	free(m_path);
}

FolderIndex Folder::GetIndex(bool recursive)
{
	return FolderIndex(m_path, recursive);
}

VINYL_NS_END;
