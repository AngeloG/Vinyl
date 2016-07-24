#include "Common.h"
#include "ZipFile.h"

#include "ThirdParty/miniz.h"

VINYL_NS_BEGIN;

ZipEntry::ZipEntry()
{
	m_zip = nullptr;
	m_index = 0;
	m_cachedSize = 0;
}

ZipEntry::~ZipEntry()
{
}

s::String ZipEntry::GetFilePath()
{
	char entryName[256];
	mz_zip_reader_get_filename(m_zip->m_archive, m_index, entryName, 256);
	return s::String(entryName);
}

s::String ZipEntry::GetFileName()
{
	char entryName[256];
	mz_zip_reader_get_filename(m_zip->m_archive, m_index, entryName, 256);
	char* entryFilename = strchr(entryName, '/');
	if (entryFilename != nullptr) {
		return s::String(entryFilename);
	}
	return s::String(entryName);
}

uint32_t ZipEntry::GetIndex()
{
	return m_index;
}

size_t ZipEntry::GetSize()
{
	if (m_cachedSize == 0) {
		mz_zip_archive_file_stat stat;
		memset(&stat, 0, sizeof(mz_zip_archive_file_stat));
		mz_zip_reader_file_stat(m_zip->m_archive, m_index, &stat);

		m_cachedSize = (size_t)stat.m_uncomp_size;
	}
	return m_cachedSize;
}

void ZipEntry::Read(void* buffer, size_t len)
{
	mz_zip_reader_extract_to_mem(m_zip->m_archive, m_index, buffer, len, 0);
}

static size_t ZipEntryWriter(void* pOpaque, mz_uint64 file_ofs, const void* pBuf, size_t n)
{
	s::Stream &strm = *(s::Stream*)pOpaque;
	strm.Write(pBuf, n);
	return n;
}

void ZipEntry::Read(s::Stream &strm)
{
	uint64_t size = GetSize();
	mz_zip_reader_extract_to_callback(m_zip->m_archive, m_index, &ZipEntryWriter, &strm, 0);
}

ZipFile::ZipFile()
{
	m_archive = new mz_zip_archive;
	memset(m_archive, 0, sizeof(mz_zip_archive));

	m_opened = false;
}

ZipFile::~ZipFile()
{
	if (m_opened) {
		if (m_archive->m_zip_mode == MZ_ZIP_MODE_WRITING) {
			mz_zip_writer_finalize_archive(m_archive);
			mz_zip_writer_end(m_archive);
		} else if (m_archive->m_zip_mode == MZ_ZIP_MODE_READING) {
			mz_zip_reader_end(m_archive);
		}
	}
	delete m_archive;
}

void ZipFile::Read(const char* fnm)
{
	if (!mz_zip_reader_init_file(m_archive, fnm, 0)) {
#ifndef SCRATCH_NO_EXCEPTIONS
		throw s::Exception("Failed to open archive for reading.");
#endif
	}
	m_opened = true;
}

void ZipFile::GetEntries(s::StackArray<ZipEntry> &arr)
{
	int num = mz_zip_reader_get_num_files(m_archive);
	for (int i = 0; i < num; i++) {
		ZipEntry &entry = arr.Push();
		entry.m_zip = this;
		entry.m_index = i;
	}
}

bool ZipFile::GetEntry(const char* name, ZipEntry &entry)
{
	int index = mz_zip_reader_locate_file(m_archive, name, nullptr, 0);
	if (index == -1) {
		return false;
	}
	entry.m_zip = this;
	entry.m_index = index;
	return true;
}

void ZipFile::Create(const char* fnm)
{
	if (!mz_zip_writer_init_file(m_archive, fnm, 0)) {
#ifndef SCRATCH_NO_EXCEPTIONS
		throw s::Exception("Failed to create archive.");
#endif
	}
	m_opened = true;
}

void ZipFile::AddFile(const char* entryName, const void* buffer, uint32_t len, CompressionLevel level)
{
	if (!mz_zip_writer_add_mem(m_archive, entryName, buffer, len, (mz_uint)level)) {
#ifndef SCRATCH_NO_EXCEPTIONS
		throw s::Exception("Failed to write entry '%s'.", entryName);
#endif
	}
}

VINYL_NS_END;
