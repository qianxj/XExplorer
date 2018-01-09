#ifndef WPP_WLANG_H
#define WPP_WLANG_H

//#include "wparser.hpp"
#include "meta.hpp"
#include "vmravine.hpp"
#include "werr.hpp"
#include "wpp.hpp"

#include <map>
#include <string>
using namespace std;

struct LangType
{
	static const int lang_wpp = 1;
};

namespace wpp {
namespace how {

class VLANG3_API  wlang
{
public:
	static wchar_t* (* GetSource)(wchar_t* purl) ;
	static void (* FreeSource)(wchar_t* purl) ;
	static FILETIME  (* GetSourceDT)(wchar_t* purl) ;
	static FILETIME  (* GetSourceVER)(wchar_t* purl) ;
public:
	wlang();
	~wlang();
private:
	std::map<int,void*>  lang_;
public:
	void* getLang(int lang_type);
	void  registerLang(int lang_type, void * lang);
public:
	//WParser *		parserPtr;
	wMetaTable*		metaPtr;
	vm::VMRavine*	genPtr;
	wErr*			errPtr;
public: 
	map<wstring,FILETIME>* files;
public:
	void setError(wErr* errPtr_); 
	wErr* get_rrror(); 
public:
	bool parse(wchar_t *src); 
public:
	int processFile(const wchar_t* file);
	wchar_t* fetchFile(const wchar_t* file);
};

}} //namespace wpp::how
#endif //WPP_WLANG_H

