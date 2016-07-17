#include "Common.h"
#include "File.h"

VINYL_NS_BEGIN;

namespace File
{
	bool Exists(const char* fnm)
	{
#if WINDOWS
		DWORD dw = GetFileAttributes(fnm);
		return dw != INVALID_FILE_ATTRIBUTES && !(dw & FILE_ATTRIBUTE_DIRECTORY);
#else
		return access(fnm, F_OK) != -1;
#endif
	}

	bool Copy(const char* fnmA, const char* fnmB, bool should_override)
	{
#if WINDOWS
		return CopyFile(fnmA, fnmB, !should_override) == 1;
#else
		if (!Exists(fnmA)) {
			return false;
		}
		if (Exists(fnmB)) {
			if (!should_override) {
				return false;
			}
			Delete(fnmB);
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

	bool Move(const char* fnmA, const char* fnmB)
	{
#if WINDOWS
		return MoveFile(fnmA, fnmB) == 1;
#else
		return rename(fnmA, fnmB) == 0;
#endif
	}

	bool Delete(const char* fnm)
	{
#if WINDOWS
		return DeleteFile(fnm) == 1;
#else
		return unlink(fnm) == 0;
#endif
	}
}

VINYL_NS_END;
