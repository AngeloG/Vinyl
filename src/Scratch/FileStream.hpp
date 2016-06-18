/*	libscratch - Multipurpose objective C++ library.

		Copyright (c) 2013 - 2016 Angelo Geels <spansjh@gmail.com>

		Permission is hereby granted, free of charge, to any person
		obtaining a copy of this software and associated documentation
		files (the "Software"), to deal in the Software without
		restriction, including without limitation the rights to use,
		copy, modify, merge, publish, distribute, sublicense, and/or sell
		copies of the Software, and to permit persons to whom the
		Software is furnished to do so, subject to the following
		conditions:

		The above copyright notice and this permission notice shall be
		included in all copies or substantial portions of the Software.

		THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
		EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
		OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
		NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
		HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
		WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
		FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
		OTHER DEALINGS IN THE SOFTWARE.
*/

#pragma once

#include "Common.h"
#include "Stream.hpp"
#include "String.hpp"

SCRATCH_NAMESPACE_BEGIN;

class SCRATCH_EXPORT FileStream : public Stream
{
public:
	String fs_strFileName;
	FILE* fs_pfh;

public:
	FileStream();
	~FileStream();

	uint32_t Size();
	uint32_t Location();
	void Seek(uint32_t ulPos, int32_t iOrigin);
	bool AtEOF();

	bool Open(const char* szFileName, const char* szMode);

	void OpenStdout();
	void OpenStdin();
	void OpenStderr();

	void Close();
	void Write(const void* p, uint32_t iLen);
	int Read(void* pDest, uint32_t iLen);
	const void ReadToEnd(void* pDest);
};

#ifdef SCRATCH_IMPL

FileStream::FileStream()
{
	fs_pfh = nullptr;
}

FileStream::~FileStream()
{
	Close();
}

uint32_t FileStream::Size()
{
	uint32_t ulPos = Location();
	Seek(0, SEEK_END);
	uint32_t ulSize = Location();
	Seek(ulPos, SEEK_SET);
	return ulSize;
}

uint32_t FileStream::Location()
{
	return ftell(fs_pfh);
}

void FileStream::Seek(uint32_t ulPos, int32_t iOrigin)
{
	fseek(fs_pfh, ulPos, iOrigin);
}

bool FileStream::AtEOF()
{
	return feof(fs_pfh) > 0;
}

bool FileStream::Open(const char* szFileName, const char* szMode)
{
	// must not already have a handle open
	ASSERT(fs_pfh == nullptr);

	// open file
#if WINDOWS
	FILE* pfh = nullptr;
	fopen_s(&pfh, szFileName, szMode);
#else
	FILE* pfh = fopen(szFileName, szMode);
#endif

	// it might not exist
	if (pfh == nullptr) {
		return false;
	}

	// remember info
	fs_strFileName = szFileName;
	fs_pfh = pfh;

	// success
	return true;
}

void FileStream::OpenStdout()
{
	fs_strFileName = "stdout";
	fs_pfh = stdout;
}

void FileStream::OpenStdin()
{
	fs_strFileName = "stdin";
	fs_pfh = stdin;
}

void FileStream::OpenStderr()
{
	fs_strFileName = "stderr";
	fs_pfh = stderr;
}

void FileStream::Close()
{
	// close the file handle
	if (fs_pfh != nullptr) {
		fclose(fs_pfh);
		fs_pfh = nullptr;
	}
}

void FileStream::Write(const void* p, uint32_t iLen)
{
	fwrite(p, 1, iLen, fs_pfh);
}

int FileStream::Read(void* pDest, uint32_t iLen)
{
	return fread(pDest, 1, iLen, fs_pfh);
}

#endif

SCRATCH_NAMESPACE_END;
