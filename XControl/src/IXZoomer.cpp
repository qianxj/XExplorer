#pragma once

#include "stdafx.h"
#include "xframe.h"
#include "IXZoomer.hpp"

namespace Hxsoft{ namespace XFrame
{
	IXZoomer::IXZoomer()
	{
	}
	IXZoomer::~IXZoomer()
	{
	}

	int IXZoomer::GetMax()
	{
		return ((xcZoomer *)m_pControl)->m_max;
	}
	int IXZoomer::GetPos()
	{
		return ((xcZoomer *)m_pControl)->m_pos;
	}
	int IXZoomer::SetMax(int nMax)
	{
		int old = ((xcZoomer *)m_pControl)->m_max = nMax;

		((xcZoomer *)m_pControl)->m_max = nMax;
		return old;
	}

	int IXZoomer::SetPos(int nPos)
	{
		int old = ((xcZoomer *)m_pControl)->m_pos;
		((xcZoomer *)m_pControl)->m_pos = nPos;
		return old;
	}
}}