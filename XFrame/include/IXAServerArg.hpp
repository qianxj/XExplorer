#pragma once

#include "xbxbase.h"
class _variant_t;
namespace Hxsoft{ namespace XFrame
{
	class  XFRAME_API IXAServerArg : public xbObject
	{
	public:
		IXAServerArg(void);
		~IXAServerArg(void);
	public:
		xbXml xml;
		BSTR pValue;
	public:
		int ClearArgs();
		int RemoveArg(LPCTSTR pArgName);

		int SetArg(LPCTSTR pArgName,LPCTSTR pValue,LPCTSTR pTyp = NULL);
		int AddArg(LPCTSTR pArgName,LPCTSTR pValue,LPCTSTR pTyp = NULL);

		int SetArg(LPCTSTR pArgName,int pValue,LPCTSTR pTyp = NULL);
		int AddArg(LPCTSTR pArgName,int pValue,LPCTSTR pTyp = NULL);

		int SetArg(LPCTSTR pArgName,double pValue,LPCTSTR pTyp = NULL);
		int AddArg(LPCTSTR pArgName,double pValue,LPCTSTR pTyp = NULL);

		int SetArg(LPCTSTR pArgName,_variant_t var,LPCTSTR pTyp = NULL);
		int AddArg(LPCTSTR pArgName,_variant_t var,LPCTSTR pTyp = NULL);

		int GetArgInt(LPTSTR pArgName);
		LPTSTR GetArgString(LPTSTR pArgName);
		bool GetArgBool(LPTSTR pArgName);
		double GetArgDouble(LPTSTR pArgName);

		LPCTSTR GetString();
	public:
		static IXAServerArg * CreateInstance();
		static int ReleaseInstance(IXAServerArg * pArgs);

		int SetXml(LPTSTR pStrXml);
	};
}};
