#ifndef WPP_MILL_METATABLE_H
#define WPP_MILL_METATABLE_H

#include <stack>
#include <vector>
#include "mill_const.hpp"
#include "word.hpp"

using namespace wpp::how;

namespace mill {

//mill meta data

typedef struct  meta_extern_package_  meta_extern_package;
typedef struct  meta_extern_class_	  meta_extern_class;
typedef struct  meta_idref_			  meta_idref;
typedef struct  meta_area_			  meta_area;
typedef struct  meta_type_			  meta_type;
typedef struct	meta_array_			  meta_array;
typedef struct  meta_class_			  meta_class;
typedef struct  meta_class_super_	  meta_class_super;
typedef struct  meta_method_		  meta_method;
typedef struct  meta_field_			  meta_field;
typedef struct  meta_param_			  meta_param;
typedef struct  meta_arg_			  meta_arg;
typedef struct  meta_var_			  meta_var;

//mill meta data
typedef class  mill_meta_data_	 mill_meta_data;

struct  meta_extern_package_
{
	int name;	//value of hash name
	int src;    //value of hash src
};

struct meta_extern_class_
{
	int full_name;
	int name;
	int package_index;
};

struct meta_idref_
{
	int  name;
	bool is_extern;
	int  id_ref_dest;
};

struct meta_area_
{	
	int no;
	int name;
	int parent;
};

//data type
struct meta_type_
{
	int no;
	int name;
	int size;	//alloc size
	int staticsize; 
	int area;	
	int nclass; 
	int parent; 
	int param;
	int atype; //atom-type
};

//array define
struct meta_array_
{
	int type;
	int lower;
	int high;
	int next;
};

//class define
struct meta_class_
{
	int spec;
	int first_field;
	int last_field;
	int first_method;
	int last_method;
	int first_attribute;
	int last_attribute;
	int first_prop;
	int last_prop;
	int super_class;
	int native;
};

//super class
struct meta_class_super_
{
	int access;
	int type;
	int next;
};

//method define
struct meta_method_
{
	int no;
	int name;
	int spec;
	int type;
	int size;
	int next;
	int arg_count;
	int var_count;
	int first_arg;
	int last_arg;
	int first_var;
	int last_var;
	int first_attribute;
	int last_attribute;
	int class_;
	int area;
	
	int native;
	bool bole;
};

struct meta_native_
{
	int from;
	int alias;
};

//var define
struct meta_var_
{
	int no;
	int name;
	int spec;
	int type;
	int size;
	int next;
	int offset;
};

struct meta_param_ : public meta_var_{
	bool init;
};
struct meta_field_ : public meta_var_{};
struct meta_arg_   :   public meta_var_{
	bool init;
};

class mill_meta_data_
{
public:
	//global use one hash string
	CHashString * get_hash_string(){ return  & wordSvr.hashString;}
public:
	std::vector<meta_extern_package_>	* ptr_extern_package;
	std::vector<meta_extern_class_>		* ptr_extern_class;
	std::vector<meta_idref_>			* ptr_idref;
	std::vector<meta_area_>				* ptr_area;
	std::vector<meta_type_>				* ptr_type;
	std::vector<meta_array_>			* ptr_array;
	std::vector<meta_class_>			* ptr_class;
	std::vector<meta_class_super_>		* ptr_class_super_;
	std::vector<meta_method_>			* ptr_method;
	std::vector<meta_field_>			* ptr_field;
	std::vector<meta_param_>			* ptr_param;
	std::vector<meta_arg_>				* ptr_arg;
	std::vector<meta_var_>				* ptr_var;
	std::vector<meta_native_>			* ptr_native;
};


} //namespace mill

#endif //WPP_MILL_METATABLE_H