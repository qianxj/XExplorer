#include <ole2.h>
#include <vector>

#define VTS_I2              "\x02"      // a 'short'
#define VTS_I4              "\x03"      // a 'long'
#define VTS_R4              "\x04"      // a 'float'
#define VTS_R8              "\x05"      // a 'double'
#define VTS_CY              "\x06"      // a 'CY' or 'CY*'
#define VTS_DATE            "\x07"      // a 'DATE'
#define VTS_WBSTR           "\x08"      // an 'LPCOLESTR'
#define VTS_DISPATCH        "\x09"      // an 'IDispatch*'
#define VTS_SCODE           "\x0A"      // an 'SCODE'
#define VTS_BOOL            "\x0B"      // a 'VARIANT_BOOL'
#define VTS_VARIANT         "\x0C"      // a 'const VARIANT&' or 'VARIANT*'
#define VTS_UNKNOWN         "\x0D"      // an 'IUnknown*'
#if defined(_UNICODE)
	#define VTS_BSTR            VTS_WBSTR// an 'LPCOLESTR'
	#define VT_BSTRT            VT_BSTR
#else
	#define VTS_BSTR            "\x0E"  // an 'LPCSTR'
	#define VT_BSTRA            14
	#define VT_BSTRT            VT_BSTRA
#endif
#define VTS_I1              "\x10"      // a 'signed char'
#define VTS_UI1             "\x11"      // a 'BYTE'
#define VTS_UI2             "\x12"      // a 'WORD'
#define VTS_UI4             "\x13"      // a 'DWORD'
#define VTS_I8              "\x14"      // a 'LONGLONG'
#define VTS_UI8             "\x15"      // a 'ULONGLONG'

// parameter types: by reference VTs
#define VTS_PI2             "\x42"      // a 'short*'
#define VTS_PI4             "\x43"      // a 'long*'
#define VTS_PR4             "\x44"      // a 'float*'
#define VTS_PR8             "\x45"      // a 'double*'
#define VTS_PCY             "\x46"      // a 'CY*'
#define VTS_PDATE           "\x47"      // a 'DATE*'
#define VTS_PBSTR           "\x48"      // a 'BSTR*'
#define VTS_PDISPATCH       "\x49"      // an 'IDispatch**'
#define VTS_PSCODE          "\x4A"      // an 'SCODE*'
#define VTS_PBOOL           "\x4B"      // a 'VARIANT_BOOL*'
#define VTS_PVARIANT        "\x4C"      // a 'VARIANT*'
#define VTS_PUNKNOWN        "\x4D"      // an 'IUnknown**'
#define VTS_PI1             "\x50"      // a 'signed char*'
#define VTS_PUI1            "\x51"      // a 'BYTE*'
#define VTS_PUI2            "\x52"      // a 'WORD*'
#define VTS_PUI4            "\x53"      // a 'DWORD*'
#define VTS_PI8             "\x54"      // a 'LONGLONG*'
#define VTS_PUI8            "\x55"      // a 'ULONGLONG*'

// special VT_ and VTS_ values
#define VTS_NONE            NULL        // used for members with 0 params
#define VT_MFCVALUE         0xFFF       // special value for DISPID_VALUE
#define VT_MFCBYREF         0x40        // indicates VT_BYREF type
#define VT_MFCMARKER        0xFF        // delimits named parameters (INTERNAL USE)

// variant handling (use V_BSTRT when you have ANSI BSTRs, as in DAO)
#ifndef _UNICODE
	#define V_BSTRT(b)  (LPSTR)V_BSTR(b)
#else
	#define V_BSTRT(b)  V_BSTR(b)
#endif



class CVariantBoolPair
{
public:
	CVariantBoolPair()
	: m_pbool(NULL),m_pvarbool(NULL) 
	{
	}
	CVariantBoolPair(BOOL* pbool,VARIANT_BOOL* pvarbool,BOOL bOwnBOOLMem = TRUE)
	: m_pbool(pbool),m_pvarbool(pvarbool),m_bOwnBOOLMem(bOwnBOOLMem)
	{
		//ENSURE(m_pbool!=NULL && m_pvarbool!=NULL);
	}
	~CVariantBoolPair()
	{
		if (m_bOwnBOOLMem)
		{
			delete m_pbool;		
		}
#ifdef _DEBUG
		m_pbool = NULL;
		m_pvarbool=NULL;
#endif
		
	}
	CVariantBoolPair& operator =(const CVariantBoolPair& rhs)
	{
		if (this != &rhs ) 
		{ 
			m_pbool	      = rhs.Detach();
			m_pvarbool    = rhs.m_pvarbool;
			m_bOwnBOOLMem = rhs.m_bOwnBOOLMem;
		}
		return *this; 
	}

	CVariantBoolPair(const CVariantBoolPair& rhs)
	{		
		m_pbool	      = rhs.Detach();
		m_pvarbool    = rhs.m_pvarbool;
		m_bOwnBOOLMem = rhs.m_bOwnBOOLMem;
	}
	BOOL* Detach() const
	{
		BOOL* pb=m_pbool;
		if (m_bOwnBOOLMem)
		{
			m_pbool=NULL;
		}
		return pb;
	}
	mutable BOOL* m_pbool;
	VARIANT_BOOL* m_pvarbool;
	BOOL m_bOwnBOOLMem;
};

class CVariantBoolConverter 
{
protected:
	std::vector<CVariantBoolPair> m_boolArgs;
public:
	void AddPair(const CVariantBoolPair& newPair)
	{
		m_boolArgs.push_back(newPair);
	}
	void CopyBOOLsIntoVarBools()
	{
		for (int i=0 ; i < (int)m_boolArgs.size() ; ++i)
		{
			//ENSURE(m_boolArgs[i].m_pbool!=NULL && m_boolArgs[i].m_pvarbool!=NULL);
			*m_boolArgs[i].m_pvarbool = *m_boolArgs[i].m_pbool ? VARIANT_TRUE : VARIANT_FALSE;
		}
	}

	void CopyVarBoolsIntoBOOLs()
	{
		for (int i=0 ; i < (int)m_boolArgs.size(); ++i)
		{
			//ENSURE(m_boolArgs[i].m_pbool!=NULL && m_boolArgs[i].m_pvarbool!=NULL);
			*m_boolArgs[i].m_pbool = (*m_boolArgs[i].m_pvarbool == 0)  ? FALSE : TRUE;
		}
	}
};

class COleException;
class XCONTROL_API CDispatchDriver
{
// Constructors
public:
	CDispatchDriver();
	CDispatchDriver(LPDISPATCH lpDispatch, BOOL bAutoRelease = TRUE);
	CDispatchDriver(const CDispatchDriver& dispatchSrc);

// Attributes
	LPDISPATCH m_lpDispatch;
	BOOL m_bAutoRelease;

// Operations
	BOOL CreateDispatch(REFCLSID clsid, COleException* pError = NULL);
	BOOL CreateDispatch(LPCTSTR lpszProgID, COleException* pError = NULL);

	void AttachDispatch(LPDISPATCH lpDispatch, BOOL bAutoRelease = TRUE);
	LPDISPATCH DetachDispatch();
		// detach and get ownership of m_lpDispatch
	void ReleaseDispatch();

	// helpers for IDispatch::Invoke
	void InvokeHelper(DISPID dwDispID, WORD wFlags,
		VARTYPE vtRet, void* pvRet, const BYTE* pbParamInfo, ...);
	void SetProperty(DISPID dwDispID, VARTYPE vtProp, ...);
	void GetProperty(DISPID dwDispID, VARTYPE vtProp, void* pvProp) const;

	// special operators
	operator LPDISPATCH();
	const CDispatchDriver& operator=(const CDispatchDriver& dispatchSrc);

// Implementation
public:
	~CDispatchDriver();
	void InvokeHelperV(DISPID dwDispID, WORD wFlags, VARTYPE vtRet,
		void* pvRet, const BYTE* pbParamInfo, va_list argList);
};
