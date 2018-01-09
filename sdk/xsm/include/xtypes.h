#pragma once
#include "xvmdef.h"
#include "atoms.h"
#include "xtuple.h"

namespace xcode
{
	class xtypes
	{
	public:
		inline static int getSize(int ntyp)
		{
			switch(ntyp)
			{
			case xa_indent:
			case xa_var:
			case xa_type:
				return sizeof(int);
			case xa_float:
				return sizeof(float);
			case xa_double:
				return sizeof(double);
			case - xa_int:
			case xa_int:
				return sizeof(int);
			case - xa_long:
			case xa_long:
				return sizeof(long);
			case - xa_byte:
			case xa_byte:
				return sizeof(char);
			case - xa_word:
			case xa_word:
				return sizeof(WORD);
			case - xa_dword:
			case xa_dword:
				return sizeof(DWORD);
			case xa_pointer:
				return sizeof(void *);
			case xa_string:
				return sizeof(void *);
			case xa_tuple:
				return sizeof(xtuple);
			case - xa_char:
			case xa_char:
				return sizeof(tchar);
			default:
				return sizeof(int);
			}
		}
		
	};
}