#include "Common.h"
#include "MountpointZip.h"

VINYL_NS_BEGIN;

MountpointZip::MountpointZip(const char* fnm)
{
	m_zip.Read(fnm);
}

bool MountpointZip::IsReadOnly()
{
	return true;
}

bool MountpointZip::FileExists(const char* fnm)
{
	return m_zip.EntryExists(fnm);
}

s::String MountpointZip::FileReadAll(const char* fnm)
{
	ZipEntry entry;
	if (!m_zip.GetEntry(fnm, entry)) {
#ifndef SCRATCH_NO_EXCEPTIONS
		throw s::Exception("File not found.");
#else
		return "";
#endif
	}
	size_t sz = entry.GetSize();
	char* buffer = (char*)malloc(sz + 1);
	entry.Read(buffer, sz);
	buffer[sz] = '\0';
	s::String ret(buffer);
	free(buffer);
	return ret;
}

void MountpointZip::FolderGetIndex(FolderIndex &ret, const char* dir, bool recursive, FolderIndexFilter* filter)
{
	bool curDir = !strcmp(dir, "./");

	s::StackArray<ZipEntry> entries;
	m_zip.GetEntries(entries);

	for (auto &entry : entries) {
		s::Filename path = entry.GetFilePath();
		if (!curDir && !path.StartsWith(dir)) {
			continue;
		}

		s::String dirname = path.Path();
		while (dirname != "") {
			dirname += "/";
			if (dirname == dir) {
				break;
			}

			if (!recursive) {
				const char* szPath = (const char*)dirname;
				if (!curDir) {
					szPath += strlen(dir);
				}
				if (strchr(szPath, '/') != strrchr(szPath, '/')) {
					break;
				}
			}

			if (!ret.m_dirs.Contains(dirname)) {
				if (filter != nullptr && !(*filter)(dirname)) {
					continue;
				}
				ret.m_dirs.Push() = dirname;
			}

			dirname = s::Filename(dirname.TrimRight('/')).Path();
			if (dirname == "") {
				break;
			}
		}

		if (path.EndsWith("/")) {
			continue;
		}

		if (!recursive) {
			const char* szPath = (const char*)path;
			if (!curDir) {
				szPath += strlen(dir);
			}
			if (strchr(szPath, '/') != nullptr) {
				continue;
			}
		}

		if (ret.m_files.Contains(path)) {
			continue;
		}

		if (filter != nullptr && !(*filter)(path)) {
			continue;
		}

		ret.m_files.Push() = path;
	}
}

VINYL_NS_END;
