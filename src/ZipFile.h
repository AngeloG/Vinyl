#pragma once

#include "Common.h"

typedef struct mz_zip_archive_tag mz_zip_archive;

VINYL_NS_BEGIN;

enum CompressionLevel
{
	//enum { MZ_NO_COMPRESSION = 0, MZ_BEST_SPEED = 1, MZ_BEST_COMPRESSION = 9, MZ_UBER_COMPRESSION = 10, MZ_DEFAULT_LEVEL = 6, MZ_DEFAULT_COMPRESSION = -1 };
	CL_Stored = 0,
	CL_Fast = 1,
	CL_Best = 9,
	CL_Uber = 10,
	CL_Default = 6
};

class ZipFile;

class ZipEntry
{
	friend class ZipFile;

private:
	ZipFile* m_zip;
	uint32_t m_index;
	size_t m_cachedSize;

public:
	ZipEntry();
	~ZipEntry();

	s::String GetFilePath();
	s::String GetFileName();
	uint32_t GetIndex();
	size_t GetSize();

	void Read(void* buffer, size_t len);
	void Read(s::Stream &strm);
};

class ZipFile
{
	friend class ZipEntry;

private:
	mz_zip_archive* m_archive;
	bool m_opened;

public:
	ZipFile();
	~ZipFile();

private:
	bool EnsureReading();
	bool EnsureWriting();

public:
	void Read(const char* fnm);
	void GetEntries(s::StackArray<ZipEntry> &arr);
	bool GetEntry(const char* name, ZipEntry &entry);
	bool EntryExists(const char* name);

	void Create(const char* fnm);
	void AddFile(const char* entryName, const void* buffer, uint32_t len, CompressionLevel level = CL_Default);
};

VINYL_NS_END;
