#include "Common.h"
#include "Folder.h"
#include "Mountpoint.h"

VINYL_NS_BEGIN;

FolderIndex::FolderIndex()
{
}

FolderIndex::FolderIndex(const char* path, uint32_t flags)
{
	m_path = path;
	Populate(flags);
}

FolderIndex::FolderIndex(const char* path, uint32_t flags, const FolderIndexFilter &filter)
{
	m_path = path;
	Populate(flags, filter);
}

FolderIndex::~FolderIndex()
{
}

void FolderIndex::Populate(uint32_t flags)
{
	for (auto &point : Mount::Points) {
		point.FolderGetIndex(*this, m_path, flags, nullptr);
	}
	AfterPopulate(flags);
}

void FolderIndex::Populate(uint32_t flags, const FolderIndexFilter &filter)
{
	for (auto &point : Mount::Points) {
		point.FolderGetIndex(*this, m_path, flags, filter);
	}
	AfterPopulate(flags);
}

void FolderIndex::AfterPopulate(uint32_t flags)
{
	if (flags & FolderIndexFlags_Sorted) {
		auto func = [](const s::String &a, const s::String &b) -> int {
			return a.CaseCompare(b);
		};
		m_dirs.Sort(func);
		m_files.Sort(func);
	}
}

void FolderIndex::Clear()
{
	m_path = "";
	m_files.Clear();
	m_dirs.Clear();
}

const char* FolderIndex::GetPath()
{
	return m_path;
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

FolderIndex Folder::GetIndex(uint32_t flags)
{
	FolderIndex ret;
	s::String pathname = m_path;
	if (!pathname.EndsWith("/")) {
		pathname += "/";
	}
	ret.m_path = pathname;
	ret.Populate(flags);
	return ret;
}

FolderIndex Folder::GetIndex(const FolderIndexFilter &filter, uint32_t flags)
{
	FolderIndex ret;
	s::String pathname = m_path;
	if (!pathname.EndsWith("/")) {
		pathname += "/";
	}
	ret.m_path = pathname;
	ret.Populate(flags, filter);
	return ret;
}

VINYL_NS_END;
