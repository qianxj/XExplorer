#ifndef WPP_MILL_CONTEXT_INTER_H
#define WPP_MILL_CONTEXT_INTER_H

#include <vector>
#include "mill_context.hpp"
#include "debuger.hpp"
#include "vill_context.hpp"
namespace mill
{

	
struct mill_register_inter_
{
	union 
	{
		unsigned int uint_val;
		int			 int_val;
		double	     double_val;
		__int64		 int64_val;
		void*		 ptr_val;
	} val;
	int type;
};

class mill_context_inter_ : public mill_context_base_
{
public:
	vill_context * get_vill_context(){return ptr_vill_context_;}
	void set_vill_context(vill_context * ptr_vill_context){ptr_vill_context_ = ptr_vill_context;}
private:
	vill_context * ptr_vill_context_;
	std::vector<mill_register_inter_>* ptr_registers;


#if 0
	//get object memory add
	inline void * get_oprand_ptr(int what_oprand_type, int fn,int index)
	{
		ASSERT(what_oprand_type == oprand_type::var_ ||
					what_oprand_type == oprand_type::arg_ ||
					what_oprand_type == oprand_type::register_);

		if(what_oprand_type == oprand_type::register_)
			return &(*ptr_registers)[index].val;
		if(what_oprand_type == oprand_type::var_)
			return get_var_ptr(fn,index);
		else if(what_oprand_type == oprand_type::arg_)
			return get_arg_ptr(fn,index);
	}

	inline void * get_arg_ptr(int fn,int index_arg)
	{
		return ((unsigned char *)ptr_runtime->ptr_active_recorder->ptr_arg) +
			ptr_meta_data->ptr_arg[ptr_meta_data->ptr_method[fn].first_arg].offset;
	}

	inline void * get_var_ptr(int fn,int index_var)
	{
		return ((unsigned char *)ptr_runtime->ptr_active_recorder->ptr_arg) +
			ptr_meta_data->ptr_var[ptr_meta_data->ptr_method[fn].first_var].offset;
	}

	//get object type
	inline int get_arg_type(int fn, int index_arg)
	{
		return ptr_meta_data->ptr_arg[ptr_meta_data->ptr_method[fn].first_arg].type;
	}

	inline int get_var_type(int fn,int index_var)
	{
		return ptr_meta_data->ptr_var[ptr_meta_data->ptr_method[fn].first_var].type;
	}

	//get object value
	template<class T>  inline T get_arg_value(int fn,int index_arg)
	{
		return *(T *)get_arg_ptr(fn,index_arg);
	}

	template<class T>  inline T get_var_value(int fn,int index_var)
	{
		return *(T *)get_var_ptr(fn,index_var);
	}
	
	//set object value
	template<class T>  inline void  set_arg_value(int fn,int index_arg, T val)
	{
		*(T *)get_arg_ptr(fn,index_arg) = val;
	}

	template<class T>  inline void  set_var_value(int fn,int index_var, T val)
	{
		*(T *)get_var_ptr(fn,index_var) = val;
	}
#endif
};

} //namespace mill

#endif //WPP_MILL_CONTEXT_INTER_H