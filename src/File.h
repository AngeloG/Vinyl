#pragma once

#include "Common.h"

VINYL_NS_BEGIN;

namespace File
{
	bool Exists(const char* fnm);
	bool Copy(const char* fnmA, const char* fnmB, bool should_override = false);
	bool Move(const char* fnmA, const char* fnmB);
	bool Delete(const char* fnm);
}

VINYL_NS_END;
