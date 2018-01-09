	#pragma once
	#include "../parser/tokenmgr.hxx"
 	#include "../error/prerror.hpp"
        #include "../error/errobj.hpp"
        #include "../tpp/parser_tpp.hxx"
	#include <string>
	#include <iostream>
	#include <ole2.h>
	using namespace std;

class XSM_API  gener 
{
	
public:
			CompileEnv*		m_pEnv;
			parser_cpp*		m_pParser;
			xsm::errobj*			m_perror;

	
public:
		
	 int Trace(tchar* pstr );
	 int ThrowError(int dwError, tchar* pstr );
	 virtual BSTR Exec();
	 virtual BSTR Exec(lptstr pclass,lptstr pfunction);
	 virtual BSTR  DeComplier();
	 virtual BSTR DeComplier(int hCompileUnit); virtual int DeComplier(int hCompileUnit,sym_object * pobj, tchar sp[] ,wstring & str) =0;
};