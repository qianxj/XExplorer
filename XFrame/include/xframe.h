#pragma once

#ifdef XFRAME_EXPORTS
#define XFRAME_API __declspec(dllexport)
#else
#define XFRAME_API __declspec(dllimport)
#endif

#include "xfAreaObject.hpp"
#include "xfnode.hpp"
#include "xfnodebox.hpp"
#include "xfnodelayers.hpp"
#include "xfnodelayer.hpp"
#include "xfnoderesource.hpp"
#include "xfnodewin.hpp"
#include "xfnodecontrol.hpp"
#include "xfxpage.hpp"
#include "xfResourceMgr.hpp"

#include "xfCacheMgr.hpp"

#include "xfwinbase.hpp"
#include "xfwin.hpp"
#include "xfmdiframe.hpp"
#include "xfapp.hpp"
#include "xfcontrol.hpp"
#include "xfUriLoader.hpp"
#include "xfConnect.hpp"

#include "ITrans.hpp"
#include "IXAServer.hpp"
#include "IEntitySvr.hpp"
#include "ISvrTree.hpp"
#include "IXNode.hpp"
#include "IXPage.hpp"
#include "IXFControl.hpp"
#include "IXFWin.hpp"
#include "IXAServerArg.hpp"
#include "IXNode.hpp"



