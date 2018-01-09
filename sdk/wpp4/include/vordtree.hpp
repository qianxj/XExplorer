#ifndef  WPP_VORDTREE_H
#define  WPP_VORDTREE_H

#include "vord.hpp"

namespace vord
{

class VLANG4_API vordtree_t : public vord_t
{
private:
	void_t free_next();
	void_t free_piror();
	void_t free_parent();
	void_t free_child();
public:
	vord_t get_next();
	vord_t get_piror();
	vord_t get_parent();
	vord_t get_child();

	void_t set_next(vord_t& );
	void_t set_piror(vord_t& );
	void_t set_parent(vord_t&  w);

	//w is parent and b is befor node
	void_t insertchild(vord_t&  parent, vord_t&  before);
	static void_t removechild(vord_t&  t);
	static void_t splitchild(vord_t& pn,vord_t& t);
	//merge child of w as t child
	static void_t mergechild(vord_t& w, vord_t& t);
};

} //namespace vord

#endif