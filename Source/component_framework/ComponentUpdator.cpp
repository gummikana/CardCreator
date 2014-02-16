#include "ComponentUpdator.h"

namespace {
	bool always_refresh = false;
}

void SetComponentRefresh( bool r )
{	
	always_refresh = r;
}

bool GetComponentRefresh()
{
	return always_refresh;
}
