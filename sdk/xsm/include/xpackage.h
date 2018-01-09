#pragma once
#include <msxml2.h>
#include "xvmcore.h"
//#include "xssparse.h"

class xsscomplier;
namespace xcode
{
	struct classitem
	{
		tchar* name;
		tchar* src;
		xtuple* psym; 
	};
	class xPackageMgr 
	{

	public:
		struct package
		{
			tchar* src;
			tchar* name;
			std::vector<tchar *> objsrc;
			std::vector<bool > objbuilded;
			std::vector<classitem *> objects;
		};

		std::vector<package *>* m_pPackage;

		int LoadPackages();
		classitem * GetClass(tchar * className);
		tchar * GetClassUrl(tchar * className);
		tchar *  GetClassSource(tchar * className);
		tchar *  GetFileSource(tchar * szFile);
		package*  GetPkg(tchar * PkgName);

		int ClearBuildFlag();
		int ComplierPkg(tchar * pPkg,xsscomplier * pComplier);
		
		xPackageMgr();
		~xPackageMgr();
	};
}