#define SCRATCH_IMPL
#include "Common.h"
#include "Mountpoint.h"
#include "MountpointFs.h"

VINYL_NS_BEGIN;

void Init()
{
	Mount::Add(new MountpointFs());
}

VINYL_NS_END;
