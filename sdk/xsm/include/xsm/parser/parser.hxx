	#pragma once

	#include "../lexer/lexer.hxx" 
	#include "types.hxx"
	#include "../error/prerror.hpp"
        #include "../error/errobj.hpp"
	#include <set>
	#include <map>
	
  //
	#define	__atom_cp_parse_ok	         		0x0000
	#define	__atom_cp_parse_error		   		0x0001
	#define	__atom_cp_parse_unprocessed		0x0002    //this is   indicate current process not match this stmt
	#define	__atom_cp_parse_processed	   		0x0003    //this is   indicate current process match this stmt
	#define	__atom_cp_parse_skip				0x0004    //this is indicate should skip this stmt
	#define	__atom_cp_parse_process_skip			0x0005   //this is before continue process should process skip  
 //parse state
	#define	__atom_parse_state_undef				0x0000
	#define	__atom_parse_state_processed			0x0001
	#define	__atom_parse_state_integer	     		0x0002
	#define	__atom_parse_state_class	        		0x0008
	#define	__atom_parse_state_const		 		0x0010

	#define	__atom_parse_state_method	    		0x0020
	#define	__atom_parse_state_method_static	          0x0040
	
	#define	__atom_parse_state_declare				0x0080
	#define	__atom_parse_state_declare_block			0x0100
	#define	__atom_parse_state_expr				0x0200
	
	#define	__atom_parse_state_class_member			0x0400
	#define	__atom_parse_state_method_param			0x0004
	#define	__atom_parse_state_method_var			0x0800
	
	#define	__atom_parse_state_can_this		(__atom_parse_state_class && __atom_parse_state_method )
class XSM_API parse 
{
//main start

public:
		lexer* 				cp;     		//lexer 
        	dword				state;        	//parse state
 
		xsm::errobj*			m_perror;
	//var std::set<std::wstring> 	m_files;
		std::map<std::wstring, FILETIME> m_ftfiles;

        	static lptstr (* GetSource)(lptstr purl) ;
        	static void (* FreeSource)(lptstr purl) ;
         	static FILETIME  (* GetSourceDT)(lptstr purl) ;
        	static FILETIME  (* GetSourceVER)(lptstr purl) ;

	//parse error
	
	 int error(atom errcode,bool prev);
	 int errorex(atom errcode, tchar arg,bool prev);
	 int errorex(atom errcode, tchar* arg,bool prev);
	 int addtip(tchar* ptip );
	 int addfct(tchar* pfct,int srow,int erow );
	 parse();
	 ~parse();
	  virtual atom parse_trans_unit();};