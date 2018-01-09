#pragma once

#include "../lexer/lexer.hpp"
#include "vldef.hpp"
#include "vlutils.hpp"
#include "errors.hpp"
#include "../vl/atoms.hpp"
#include "vlsym.hpp"

namespace vl
{

	class vlang3;
	class VL_API vlparser3
	{
	public:
		vlparser3();
		~vlparser3();
	private:
		lexer*	pLexer;
		vlang3 * pVlang;
	public:
		void setLexer(lexer* p);
		lexer* getLexer();
	private:
		bool isPrep(tchar* p); //≈–∂œ «∑Ò «ΩÈ¥ 
		bool isProv(tchar* p); //≈–∂œ «∑Ò «¥˙¥ 
	private:
		void * pInterpretEnv_;
	public:
		void setInterpretEnv(void * p);
		void* getInterpretEnv();
	public:
		bool parseAsm(int pl);
		bool parse();
	public:
		//parse vlang unit
		bool parseUnit();
		//parse vlang stmt
		bool parseStmt();
	public:
		void setVlang(vlang3 * pv);
		vlang3 * getVlang();
	public:
		//error processed
		int error(error::err item,bool bprocess = false);
		int error(error::err item,const tchar * ext,bool bprocess = false);
		void procerror();
	};
}