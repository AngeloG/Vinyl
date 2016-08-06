#include "Common.h"
#include "MountpointFs.h"

#if !WINDOWS
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#endif

VINYL_NS_BEGIN;

bool MountpointFs::IsReadOnly()
{
	return false;
}

bool MountpointFs::FileExists(const char* fnm)
{
#if WINDOWS
	DWORD dw = GetFileAttributes(fnm);
	return dw != INVALID_FILE_ATTRIBUTES && !(dw & FILE_ATTRIBUTE_DIRECTORY);
#else
	return access(fnm, F_OK) != -1;
#endif
}

bool MountpointFs::FileCopy(const char* fnmA, const char* fnmB, bool should_override)
{
#if WINDOWS
	return CopyFile(fnmA, fnmB, !should_override) == 1;
#else
	if (!FileExists(fnmA)) {
		return false;
	}
	if (FileExists(fnmB)) {
		if (!should_override) {
			return false;
		}
		FileDelete(fnmB);
	}
	FILE* fhA = fopen(fnmA, "rb");
	FILE* fhB = fopen(fnmB, "wb");
	void* buffer = malloc(1024);
	while (!feof(fhA)) {
		size_t szRead = fread(buffer, 1, 1024, fhA);
		fwrite(buffer, 1, szRead, fhB);
	}
	free(buffer);
	fclose(fhB);
	fclose(fhA);
	return true;
#endif
}

bool MountpointFs::FileMove(const char* fnmA, const char* fnmB)
{
#if WINDOWS
	return MoveFile(fnmA, fnmB) == 1;
#else
	return rename(fnmA, fnmB) == 0;
#endif
}

bool MountpointFs::FileDelete(const char* fnm)
{
#if WINDOWS
	return DeleteFile(fnm) == 1;
#else
	return unlink(fnm) == 0;
#endif
}

s::String MountpointFs::FileReadAll(const char* fnm)
{
	s::String ret;

	s::FileStream fs;
	fs.Open(fnm, "r");
	char buffer[1025];
	while (!fs.AtEOF()) {
		int bytesRead = fs.Read(buffer, 1024);
		buffer[bytesRead] = '\0';
		ret += buffer;
	}

	return ret;
}

bool MountpointFs::FileWriteAll(const char* fnm, const char* content)
{
	s::FileStream fs;
	fs.Open(fnm, "w");
	fs.Write(content, strlen(content));

	return true;
}

void MountpointFs::FolderGetIndex(FolderIndex &ret, const char* dir, bool recursive)
{
#if WINDOWS
	ReadDir(ret, nullptr, dir, recursive);
#else
	ReadDir(ret, opendir(dir), dir, recursive);
#endif
}

void MountpointFs::ReadDir(FolderIndex &ret, void* impl, const char* dirname, bool recursive)
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
			ret.m_dirs.Push() = path;
			if (recursive) {
				ReadDir(ret, nullptr, path, true);
			}
		} else {
			ret.m_files.Push() = path;
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
			ret.m_files.Push() = path;
		} else {
			path += "/";
			ret.m_dirs.Push() = path;
			if (recursive) {
				ReadDir(ret, opendir(path), path, true);
			}
		}
	}
#endif
}

VINYL_NS_END;
