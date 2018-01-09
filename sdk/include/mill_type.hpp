#ifndef WPP_MILL_TYPE_H
#define WPP_MILL_TYPE_H

#include "mill_const.hpp"
#include "mill_metatable.hpp"
#include "type.hpp"

namespace mill
{
	struct type :  public wpp::type
	{

	};


	class mill_type_base_ : public wpp::wpp_type
	{
	public:
		bool is_static_spec(int spec) {return spec & wpp::type_spec::static_?true:false;}
		bool is_byref_spec(int spec){ return spec & wpp::type_spec::byref_?true:false;}
	public:
		bool is_class_type(int typ){return typ == type::class_ || typ == type::struct_ ||typ == type::interface_;}
		bool is_struct_type(int typ){return typ == type::struct_;}
	public:
		bool type_can_imp_conv(int ty1,int ty2){return true;}

	public:
		virtual int get_size(int typ);
		virtual int get_static_size(int typ);
	public:
		virtual int calc_class_align_size(int nclass,int & msize, int align = 4);
		virtual int calc_field_offset(int nclass, int nfield,  int align = 4);

	public:
		virtual int find_matched_method(int pl ,int fc , int indent, int nargs,int typs[],int nest=-1);
		virtual bool is_matched_method(int f, int nargs,int  typ[]);

	public:
		virtual void* initial();
	public:
		mill_meta_data_* get_meta_table(){return ptr_meta_table_;}
		void set_meta_table(mill_meta_data_* ptr_meta_table){ptr_meta_table_=ptr_meta_table;}
	public:
		meta_type_ &	get_meta_type(int index_type){return (*ptr_meta_table_->ptr_type)[index_type];}
		meta_class_ &	get_meta_class(int index_class){return (*ptr_meta_table_->ptr_class)[index_class];}
		meta_method_ &  get_meta_method(int index_method){return (*ptr_meta_table_->ptr_method)[index_method];}
		meta_native_ &  get_meta_native(int index_native){return (*ptr_meta_table_->ptr_native)[index_native];}
	private:
		mill_meta_data_* ptr_meta_table_;
	};

} //namespace millative

#endif