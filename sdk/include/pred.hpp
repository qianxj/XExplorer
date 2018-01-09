#ifndef WPP_PRED_H
#define WPP_PRED_H

#include "word.hpp"

namespace wpp  {
namespace how  {

#define decl_pred_(pred, uri) const static int pred = wpp::how::CWord::get_string_pred(L##uri)

struct pw
{
	typedef int  valuetype;
	static const valuetype nil				= 0x00;
	static const valuetype lang_global		= 0x01;
	static const valuetype lang_item		= 0x02;
	static const valuetype lang_namespace	= 0x09;
	static const valuetype lang_uri			= 0x0A;
	static const valuetype lang_class		= 0x0B;
	static const valuetype lang_atype		= 0x10;
	static const valuetype lang_type		= 0x11;
	static const valuetype lang_indent		= 0x12;
	static const valuetype lang_spec		= 0x13;
	static const valuetype lang_var			= 0x14; //word
	static const valuetype lang_param		= 0x15;	//word
	static const valuetype lang_method		= 0x16; //word
	static const valuetype lang_field		= 0x17; //word
	static const valuetype lang_operator	= 0x18;
	static const valuetype lang_parent		= 0x19;	//word
	static const valuetype lang_scope		= 0x1A;
	static const valuetype lang_super		= 0x1B;	//word
	static const valuetype lang_nest		= 0x20;
	static const valuetype lang_vars		= 0x24;
	static const valuetype about_type		= 0x30;
	static const valuetype meta_pred		= 0x31;
	static const valuetype meta_key			= 0x32;

	static const valuetype exp_const		= 0x60;
	static const valuetype exp_var			= 0x61;
	static const valuetype exp_param		= 0x62;
	static const valuetype exp_this			= 0x63;
	static const valuetype exp_field		= 0x64;
	static const valuetype exp_operate		= 0x65;
	static const valuetype exp_indent		= 0x66;
	static const valuetype exp_oprand		= 0x67;
	static const valuetype exp_call			= 0x68;
	static const valuetype exp_ncall		= 0x69;
	static const valuetype stmt_if			= 0x70;
	static const valuetype stmt_while		= 0x71;
	static const valuetype stmt_for			= 0x72;
	static const valuetype stmt_break		= 0x73;
	static const valuetype stmt_continue	= 0x74;
	static const valuetype stmt_case		= 0x75;
	static const valuetype stmt_switch		= 0x76;
	static const valuetype stmt_label		= 0x77;
	static const valuetype stmt_goto		= 0x78;
	static const valuetype stmt_forinit		= 0x79;
	static const valuetype stmt_forcond		= 0x7A;
	static const valuetype stmt_forstep		= 0x7B;
	static const valuetype stmt_forbody		= 0x7C;
	static const valuetype stmt_ifcond		= 0x7D;
	static const valuetype stmt_iftrue		= 0x7E;
	static const valuetype stmt_iffalse		= 0x7F;
	static const valuetype stmt_whilecond	= 0x80;
	static const valuetype stmt_whilebody	= 0x81;
	static const valuetype stmt_until		= 0x83;
	static const valuetype stmt_untilcond	= 0x84;
	static const valuetype stmt_untilbody	= 0x85;
	static const valuetype stmt_casecond	= 0x86;
	static const valuetype stmt_casebody	= 0x87;
	static const valuetype stmt_switchcond	= 0x88;
	static const valuetype stmt_switchbody	= 0x89;

	struct sm //”Ô“Â
	{
		struct turing
		{
			//match
			static const valuetype semantics	= 0x100;
			static const valuetype ruler		= 0x101;
			static const valuetype match_ruler	= 0x101;
			static const valuetype match		= 0x102;
			static const valuetype match_pred	= 0x103;
			static const valuetype match_value	= 0x104;
			static const valuetype match_word	= 0x105;
			static const valuetype match_wpred	= 0x106;
			static const valuetype match_state	= 0x107;
			static const valuetype match_other	= 0x108;
			static const valuetype match_any	= 0x109;
			static const valuetype match_regex	= 0x10A;
			static const valuetype match_string	= 0x10B;
			static const valuetype match_or		= 0x10C;
			static const valuetype match_and	= 0x10D;
			static const valuetype match_follow	= 0x10E;
			static const valuetype match_select	= 0x10F;
			static const valuetype match_oneof	= 0x110;
			static const valuetype match_not	= 0x111;
			static const valuetype match_refer	= 0x112;

			//turing action
			static const valuetype machine		 = 0x120;
			static const valuetype step_next	 = 0x121;
			static const valuetype step_piror	 = 0x122;
			static const valuetype step_last	 = 0x123;
			static const valuetype step_first	 = 0x124;
			static const valuetype step_into	 = 0x125;
			static const valuetype step_out		 = 0x126;
			static const valuetype step_enter	 = 0x127;
			static const valuetype step_leave	 = 0x128;
			static const valuetype step_tomark	 = 0x129;
			static const valuetype step_backmark = 0x12A;
			static const valuetype step_halt	 = 0x12B;
			static const valuetype tostate		 = 0x12C;
			static const valuetype mark			 = 0x12D;
			static const valuetype push			 = 0x130;
			static const valuetype pop			 = 0x131;
			static const valuetype insert		 = 0x132;
			static const valuetype remove		 = 0x133;
			static const valuetype action		 = 0x134;
			static const valuetype break_		 = 0x135;
			static const valuetype halt			 = 0x136;
			static const valuetype repeat		 = 0x137;
			static const valuetype popmark		 = 0x138;
			static const valuetype pushstate	 = 0x139;
			static const valuetype popstate		 = 0x13A;
			static const valuetype faultaction	 = 0x13B;
			static const valuetype trace		 = 0x13C;
			static const valuetype debugbreak	 = 0x13d;
		};

		struct inter //Ω‚ Õ
		{
			//how to register action pred
			static const valuetype act_value	= 0x111;
			static const valuetype act_iterator	= 0x112;
			static const valuetype act_addr		= 0x113;
			static const valuetype act_call		= 0x114;
			static const valuetype act_callop	= 0x115;
			static const valuetype act_this		= 0x116;
			static const valuetype act_nil		= 0x117;
		};

		struct scan //
		{
			//how to register action pred
			static const valuetype token_op				= 0x421;
			static const valuetype token_string			= 0x422;
			static const valuetype token_char			= 0x423;
			static const valuetype token_ln				= 0x424;
			static const valuetype token_indent			= 0x425;
			static const valuetype token_int			= 0x426;
			static const valuetype token_double			= 0x427;
			static const valuetype token_comment		= 0x428;
			static const valuetype token_blockcomment	= 0x429;
			static const valuetype token_number			= 0x42A;
			static const valuetype token_hex			= 0x42B;
			static const valuetype token_blank			= 0x42C;
			static const valuetype token_bool			= 0x42D;
		};

		struct parser
		{
			struct keyword
			{
				static const valuetype _any_				= 0x901;
				static const valuetype _as_				= 0x902;
				static const valuetype _asm_			= 0x903;
				static const valuetype _auto_			= 0x904;
				static const valuetype _blob_			= 0x905; 
				static const valuetype _bool_			= 0x906; 
				static const valuetype _break_			= 0x907; 
				static const valuetype _byref_			= 0x908; 
				static const valuetype _byte_			= 0x909; 
				static const valuetype _byval_			= 0x90A;
				static const valuetype _case_			= 0x90B; 
				static const valuetype _catch_			= 0x90C; 
				static const valuetype _char_			= 0x90D; 
				static const valuetype _class_			= 0x90E; 
				static const valuetype _const_			= 0x90F; 
				static const valuetype _const_cast_		= 0x910; 
				static const valuetype _continue_		= 0x911; 
				static const valuetype _default_		= 0x912; 
				static const valuetype _delete_			= 0x913; 
				static const valuetype _do_				= 0x914; 
				static const valuetype _double_			= 0x915; 
				static const valuetype _dynamic_cast_	= 0x916; 
				static const valuetype _else_			= 0x917; 
				static const valuetype _enum_			= 0x918; 
				static const valuetype _explicit_		= 0x919; 
				static const valuetype _export_			= 0x91A; 
				static const valuetype _extern_			= 0x91B; 
				static const valuetype _false_			= 0x91C; 
				static const valuetype _float_			= 0x91D; 
				static const valuetype _for_				= 0x91E; 
				static const valuetype _friend_			= 0x91F; 
				static const valuetype _function_		= 0x920; 
				static const valuetype _goto_			= 0x921; 
				static const valuetype _if_				= 0x922; 
				static const valuetype _inline_			= 0x923; 
				static const valuetype _int_				= 0x924; 
				static const valuetype _interface_		= 0x925; 
				static const valuetype _long_			= 0x926; 
				static const valuetype _mutable_			= 0x927; 
				static const valuetype _namespace_		= 0x928; 
				static const valuetype _new_				= 0x929; 
				static const valuetype _nil_				= 0x92A; 
				static const valuetype _oleobject_		= 0x92B; 
				static const valuetype _operator_		= 0x92C; 
				static const valuetype _out_				= 0x92D; 
				static const valuetype _param_			= 0x92E; 
				static const valuetype _private_			= 0x92F; 
				static const valuetype _protected_		= 0x930; 
				static const valuetype _public_			= 0x931; 
				static const valuetype _readonly_		= 0x932; 
				static const valuetype _ref_				= 0x933; 
				static const valuetype _register_		= 0x934; 
				static const valuetype _reinterpret_cast_= 0x935; 
				static const valuetype _return_			= 0x936; 
				static const valuetype _short_			= 0x937; 
				static const valuetype _signed_			= 0x938; 
				static const valuetype _sizeof_			= 0x939;
				static const valuetype _static_			= 0x93A; 
				static const valuetype _static_cast_ 	= 0x93B; 
				static const valuetype _stdcall_			= 0x93C; 
				static const valuetype _string_			= 0x93D; 
				static const valuetype _struct_			= 0x93E; 
				static const valuetype _switch_			= 0x93F; 
				static const valuetype _template_		= 0x940; 
				static const valuetype _this_			= 0x941; 
				static const valuetype _throw_			= 0x942; 
				static const valuetype _true_			= 0x943; 
				static const valuetype _try_				= 0x944; 
				static const valuetype _type_			= 0x945; 
				static const valuetype _typedef_			= 0x946; 
				static const valuetype _typeid_			= 0x947; 
				static const valuetype _typename_		= 0x948; 
				static const valuetype _uint_			= 0x949; 
				static const valuetype _union_			= 0x94A; 
				static const valuetype _unit_			= 0x94B; 
				static const valuetype _unnative_		= 0x94C; 
				static const valuetype _unsigned_		= 0x94D; 
				static const valuetype _using_			= 0x94E; 
				static const valuetype _var_			= 0x950; 
				static const valuetype _virtual_		= 0x951; 
				static const valuetype _void_			= 0x952; 
				static const valuetype _volatile_		= 0x953; 
				static const valuetype _wchar_t_			= 0x954; 
				static const valuetype _while_			= 0x955;
			};

			static const valuetype match_token			= 0x610;
			static const valuetype match_keyword		= 0x611;
			static const valuetype match_operate		= 0x612;
			static const valuetype match_spec			= 0x613;
			static const valuetype match_type			= 0x614;
			static const valuetype match_comptype		= 0x615;
			static const valuetype match_namespace		= 0x616;
			static const valuetype match_item			= 0x617;
			static const valuetype match_var			= 0x618;
			static const valuetype match_field			= 0x619;
			static const valuetype match_param			= 0x61A;
			static const valuetype match_ide			= 0x61B;
			static const valuetype match_string			= 0x61C;
			static const valuetype match_char			= 0x61D;
			static const valuetype match_number			= 0x61E;

			static const valuetype exp_const			= 0x760;
			static const valuetype exp_var				= 0x761;
			static const valuetype exp_param			= 0x762;
			static const valuetype exp_this				= 0x763;
			static const valuetype exp_field			= 0x764;
			static const valuetype exp_operate			= 0x765;
			static const valuetype exp_indent			= 0x766;
			static const valuetype exp_oprand			= 0x767;
			static const valuetype exp_call				= 0x768;
			static const valuetype exp_ncall			= 0x769;
			static const valuetype stmt_if				= 0x770;
			static const valuetype stmt_while			= 0x771;
			static const valuetype stmt_for				= 0x772;
			static const valuetype stmt_break			= 0x773;
			static const valuetype stmt_continue		= 0x774;
			static const valuetype stmt_case			= 0x775;
			static const valuetype stmt_switch			= 0x776;
			static const valuetype stmt_label			= 0x777;
			static const valuetype stmt_goto			= 0x778;
			static const valuetype stmt_forinit			= 0x779;
			static const valuetype stmt_forcond			= 0x77A;
			static const valuetype stmt_forstep			= 0x77B;
			static const valuetype stmt_forbody			= 0x77C;
			static const valuetype stmt_ifcond			= 0x77D;
			static const valuetype stmt_iftrue			= 0x77E;
			static const valuetype stmt_iffalse			= 0x77F;
			static const valuetype stmt_whilecond		= 0x780;
			static const valuetype stmt_whilebody		= 0x781;
			static const valuetype stmt_until			= 0x783;
			static const valuetype stmt_untilcond		= 0x784;
			static const valuetype stmt_untilbody		= 0x785;
			static const valuetype stmt_casecond		= 0x786;
			static const valuetype stmt_casebody		= 0x787;
			static const valuetype stmt_switchcond		= 0x788;
			static const valuetype stmt_switchbody		= 0x789;
			static const valuetype stmt_comp			= 0x78A;
			static const valuetype stmt_decl			= 0x78B;
			static const valuetype stmt_expr			= 0x78C;
			static const valuetype stmt					= 0x78D;
			static const valuetype expr					= 0x78E;
			static const valuetype param				= 0x78F;
			static const valuetype paramlist			= 0x790;
			static const valuetype exp_id				= 0x791;
			static const valuetype stmt_return			= 0x792;

			static const valuetype lang_global			= 0x801;
			static const valuetype lang_item			= 0x802;
			static const valuetype lang_namespace		= 0x809;
			static const valuetype lang_uri				= 0x80A;
			static const valuetype lang_class			= 0x80B;
			static const valuetype lang_atype			= 0x810;
			static const valuetype lang_type			= 0x811;
			static const valuetype lang_indent			= 0x812;
			static const valuetype lang_spec			= 0x813;
			static const valuetype lang_var				= 0x814; //word
			static const valuetype lang_param			= 0x815; //word
			static const valuetype lang_method			= 0x816; //word
			static const valuetype lang_field			= 0x817; //word
			static const valuetype lang_operator		= 0x818;
			static const valuetype lang_parent			= 0x819; //word
			static const valuetype lang_scope			= 0x81A;
			static const valuetype lang_super			= 0x81B; //word
			static const valuetype lang_nest			= 0x820;
			static const valuetype lang_vars			= 0x824;
			static const valuetype lang_enterscope		= 0x826;
			static const valuetype lang_leavescope		= 0x827;
		};

		struct business
		{
			static const valuetype match_state			= 0x610;
			static const valuetype match_role			= 0x611;
			static const valuetype match_action			= 0x612;
			static const valuetype match_cond			= 0x613;
			static const valuetype match_bill			= 0x614;

			static const valuetype tostate				= 0x710;
			static const valuetype wstate				= 0x711;
		};
	};
};

struct op1
{
	typedef int  valuetype;
	static const valuetype add_		= 0x01;
	static const valuetype sub_		= 0x02;
	static const valuetype mul_		= 0x03;
	static const valuetype div_		= 0x04;
	static const valuetype shl_		= 0x05;
	static const valuetype shr_		= 0x06;
	static const valuetype asn_		= 0x07;
	static const valuetype mod_		= 0x08;
	static const valuetype inc_		= 0x09;
	static const valuetype dec_		= 0x10;
	static const valuetype or_		= 0x12;
	static const valuetype and_		= 0x13;
	static const valuetype not_		= 0x14;
	static const valuetype band_	= 0x15;
	static const valuetype bor_		= 0x16;
	static const valuetype bnot_	= 0x17;
	static const valuetype xor_		= 0x18;
	static const valuetype eq_		= 0x20;
	static const valuetype neq_		= 0x21;
	static const valuetype gt_		= 0x22;
	static const valuetype gte_		= 0x23;
	static const valuetype lt_		= 0x24;
	static const valuetype lte_		= 0x25;
};

struct TPredLinkType1
{
	typedef struct Double{}  Double;
	typedef struct VoidPtr{} VoidPtr;
	typedef struct Int64{}	 Int64;
	typedef struct Items{}	 Items;
	typedef struct Point{}	 Point;
	typedef struct Rect{}	 Rect;

	typedef int  valuetype;
	static const valuetype _int		= 0x0;
	static const valuetype _double  = 0x1;
	static const valuetype _lint	= 0x2;
	static const valuetype _blob	= 0x3;
	static const valuetype _class   = 0x4;
	static const valuetype _struct  = 0x5;
	static const valuetype _string  = 0x6;
	static const valuetype _word	= 0x7;
	static const valuetype _items	= 0x8;
	static const valuetype _hstring = 0x9;
	static const valuetype _point   = 0xA;
	static const valuetype _rect    = 0xB;
	static const valuetype _more    = 0xf;
};


namespace pbo
{
	decl_pred_(saleorder ,"pbo::saleorder");
	decl_pred_(client ,	 "pbo::client");
	decl_pred_(agent ,	 "pbo::agent");


	decl_pred_(role,		"pbo:role");
	decl_pred_(position, "pbo:position");
	decl_pred_(org,		"pbo:org");
	decl_pred_(bill,		"pbo:bill");
	decl_pred_(cond,		"pbo:cond");
	decl_pred_(ruler,	"pbo:ruler");
	decl_pred_(action,	"pbo:action");
	decl_pred_(scope,	"pbo:scope");
};

namespace psm //”Ô“Â
{
	decl_pred_(isa,		"psm:isa");
	decl_pred_(havea,	"psm:havea");
	decl_pred_(partof,	"psm:partof");
	decl_pred_(childof,	"psm:childof");
	decl_pred_(parentof, "psm:parentof");
	decl_pred_(ident,	"psm:ident");
	decl_pred_(id,		"psm:id");
};

 
}} //namespace wpp::how

#endif //WPP_PRED_H