#ifndef  WPP_VORDTREE_H
#define  WPP_VORDTREE_H

#include "vord.hpp"

namespace vord
{

class VLANG4_API vordex_t : public vord_t
{
private:
	void_t free_next();
	void_t free_piror();
	void_t free_parent();
	void_t free_child();
public:
	vordex_t get_next();
	vordex_t get_piror();
	vordex_t get_parent();
	vordex_t get_child();

	void_t set_next(vordex_t& );
	void_t set_piror(vordex_t& );
	void_t set_parent(vordex_t&  w);

	//w is parent and b is befor node
	void_t insertchild(vordex_t&  parent, vordex_t&  before);
	static void_t removechild(vordex_t&  t);
	static void_t splitchild(vordex_t& pn,vordex_t& t);
	//merge child of w as t child
	static void_t mergechild(vordex_t& w, vordex_t& t);
};

} //namespace vord

#endif