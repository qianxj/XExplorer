#pragma once

#include "xframe.h"
#include "xccontrol.h"
#include "xoffice.h"

using namespace Hxsoft::XFrame;

class EXTEXPLORER_API EntityMaint :
	public xfWin
{
public:
	EntityMaint(void);
	~EntityMaint(void);

public:
	virtual int OnXCommand(LPCTSTR	pStrID, class xfControl * pControl);
public:
	virtual int OnOpen();
public:
	BSTR m_bstrData;
};
