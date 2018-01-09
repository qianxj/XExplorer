#include "StdAfx.h"
#include "..\include\xfNetwork.hpp"

namespace Hxsoft{ namespace XFrame
{
	xfNetwork::xfNetwork(void)
	{
		SysInitLibrary();
	}

	xfNetwork::~xfNetwork(void)
	{
		SysCleanupLibrary();
	}

}}
