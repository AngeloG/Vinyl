#include "Common.h"
#include "File.h"
#include "Mountpoint.h"

VINYL_NS_BEGIN;

namespace File
{
	bool Exists(const char* fnm)
	{
		for (auto &point : Mount::Points) {
			if (point.FileExists(fnm)) {
				return true;
			}
		}
		return false;
	}

	bool ExistsForWriting(const char* fnm)
	{
		for (auto &point : Mount::Points) {
			if (point.IsReadOnly()) {
				continue;
			}
			if (point.FileExists(fnm)) {
				return true;
			}
		}
		return false;
	}

	bool Copy(const char* fnmA, const char* fnmB, bool should_override)
	{
		for (auto &point : Mount::Points) {
			if (point.IsReadOnly()) {
				continue;
			}
			if (point.FileCopy(fnmA, fnmB, should_override)) {
				return true;
			}
		}
		return false;
	}

	bool Move(const char* fnmA, const char* fnmB)
	{
		for (auto &point : Mount::Points) {
			if (point.IsReadOnly()) {
				continue;
			}
			if (point.FileMove(fnmA, fnmB)) {
				return true;
			}
		}
		return false;
	}

	bool Delete(const char* fnm)
	{
		for (auto &point : Mount::Points) {
			if (point.IsReadOnly()) {
				continue;
			}
			if (point.FileDelete(fnm)) {
				return true;
			}
		}
		return false;
	}

	s::String ReadAll(const char* fnm)
	{
		for (auto &point : Mount::Points) {
			if (!point.FileExists(fnm)) {
				continue;
			}
			return point.FileReadAll(fnm);
		}
#ifndef SCRATCH_NO_EXCEPTIONS
		throw s::Exception("File not found.");
#else
		return "";
#endif
	}

	bool WriteAll(const char* fnm, const char* content)
	{
		for (auto &point : Mount::Points) {
			if (point.IsReadOnly()) {
				continue;
			}
			if (point.FileWriteAll(fnm, content)) {
				return true;
			}
		}
		return false;
	}
}

VINYL_NS_END;
