#ifndef WPP_VILL_RUNTIME_H
#define WPP_VILL_RUNTIME_H

#include <vector>
#include "wlexer.hpp"
#include "werr.hpp"
#include "errors.hpp"

using namespace wpp;
using namespace wpp::how;

namespace mill{

struct vill_type
{
	typedef unsigned char		int_u8;
	typedef unsigned short		int_u16;
	typedef unsigned int		int_u32;
	typedef unsigned __int64	int_u64;
	typedef char				int_i8;
	typedef short				int_i16;
	typedef int					int_i32;
	typedef __int64				int_i64;
	typedef float				float32;
	typedef double				float64;
	typedef long double			float80;
	typedef char*				astring;
	typedef wchar_t*			string;
	typedef unsigned char		ubyte;
	typedef char				byte;
	typedef char				achar;
	typedef wchar_t				wchar;
	typedef void*				obj_ptr;
	typedef bool				boolean;
};

struct vill_register
{
	union 
	{
		vill_type::int_u8		val_u8;
		vill_type::int_u16		val_u16;
		vill_type::int_u32		val_u32;
		vill_type::int_u64		val_u64;
		vill_type::int_i8		val_i8;
		vill_type::int_i16		val_i16;
		vill_type::int_i32		val_i32;
		vill_type::int_i64		val_i64;
		vill_type::float32		val_f32;
		vill_type::float64		val_f64;
		vill_type::float80		val_f80;
		vill_type::boolean		val_bool;

		vill_type::obj_ptr		val_ptr;
		vill_type::string		val_string;
		vill_type::astring		val_astring;
	};
};

struct vill_flag_register
{
	unsigned int C : 1; //carry
	unsigned int Z : 1; //zero
	unsigned int S : 1; //sign
	unsigned int O : 1; //overflow
};

class vill_runtime
{
public:
	static const int void_ptr_size = 4;
public:
	std::vector<vill_register>*  ptr_registeres;
	vill_flag_register flag_register;
	int* ptr_stackes;
	int* stack_bp;
	int* stack_sp;
public:
	void set_err_ptr(wErr* err_ptr);
	wErr* get_err_ptr();
	void error(error::err nError, const wchar_t* str, wtoken& tk);
private:
	wErr*	err_ptr_;
public:
	//std::map<std::wstring /*dll*/,std::map<std::wstring /*alias*/, int>> * ptr_extern_method_;
	vill_runtime();
	~vill_runtime();
public:
	void reset();
	//int step_next();
	//int step_continue();
};

} //namespace mill

#endif //WPP_VILL_RUNTIME

