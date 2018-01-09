#ifndef  WPP_VORD_H
#define  WPP_VORD_H

#include "vorddef.hpp"
#include "vordsvr.hpp"
#include "vorditerator.hpp"
#include "vpred.hpp"
#include <vector>

namespace vord {

class VLANG4_API vord_t : public vitem_t
{
public:
	vord_t();
	vord_t(int gid);
	vord_t(const vord_t& v);
	vord_t(const vitem_t& vitem);
	~vord_t();
public:
	vord_t& operator = (const vord_t& w);
	bool operator ==(vord_t& w);
	bool operator !=(vord_t& w);
	operator bool();
public:
	static int askgroup();
	static void freegroup(int gid);
public:
	info_t * getVordInfo() const;
	info_t * getVordInfo(int_t item) const;
	item_t * getVordItem(int_t item) const;
	static info_t * getVordInfo(int_t gid, int_t item);
	static item_t * getVordItem(int_t gid,int_t item);
public:
	static vordsvr_t * getVordSvr();
public:
	int_t  make(int gid);
	void_t release();
	void_t addref();
public:
	int		alloc_item();
	void	free_item(int item);
	bool	equal(vord_t&  w);
private:
	const vord_t & get_nilvord();
public:
	bool is_tempvord(); //template vord
	bool isnil();
public:
	int_t  clone(vord_t& w);
	int_t  append(vord_t& w);
	int_t  merge(vord_t& w);
public:
	void_t reference(vord_t& w) ;
	vord_t get_reference() const;

public:
	int_t append(pred_t pred,const value_t& v) ;
	int_t append(pred_t pred, int_t v);
	int_t append(pred_t pred, wstring_t v);
	int_t append(pred_t pred, double_t v);
	int_t append(pred_t pred, int64_t v);
	int_t append(pred_t pred, ptr_t v);
	int_t append(pred_t pred, vord_t&  v);
	int_t append(pred_t pred,pred_t v);
public:
	void_t remove(int_t item);
	void_t remove(pred_t pred);

protected:
	const value_t * getItem(pred_t pred,bool reference = true) const;
	const value_t * getItem(int_t item) const;
	void_t	setItem(int_t item, const value_t& v); 
	int		getIndex(pred_t pred,bool reference = false,bool cachefind = true)  const;

public:
	void_t setItem(int_t item, int_t v);
	void_t setItem(int_t item, wstring_t v);
	void_t setItem(int_t item, double_t v);
	void_t setItem(int_t item, int64_t v);
	void_t setItem(int_t item, ptr_t v);
	void_t setItem(int_t item, vord_t&  v);
	void_t setItem(int_t item, pred_t v);

	void_t setItem(pred_t pred, const value_t& v); 
	void_t setItem(pred_t pred, int_t v);
	void_t setItem(pred_t pred, wstring_t v);
	void_t setItem(pred_t pred, double_t v);
	void_t setItem(pred_t pred, int64_t v);
	void_t setItem(pred_t pred, ptr_t v);
	void_t setItem(pred_t pred, vord_t&  v);
	void_t setItem(pred_t pred, pred_t v);

public:
	bool_t	   getItemBool(int_t item) const;
	wstring_t  getItemString(int_t item)const;
	int_t	   getItemInt(int_t item) const;
	int64_t    getItemInt64(int_t item) const;
	double_t   getItemDouble(int_t item) const;
	ptr_t      getItemPtr(int_t item) const;
	int_t      getItemHashString(int_t item) const;
	vitem_t	   getItemVord(int_t item) const;

	bool_t	   getItemBool(pred_t pred,bool reference = true);
	wstring_t  getItemString(pred_t pred,bool reference = true);
	int_t	   getItemInt(pred_t pred,bool reference = true);
	int64_t    getItemInt64(pred_t pred,bool reference = true);
	double_t   getItemDouble(pred_t pred,bool reference = true);
	ptr_t      getItemPtr(pred_t pred,bool reference = true);
	int_t      getItemHashString(pred_t pred,bool reference = true);
	vitem_t	   getItemVord(pred_t pred,bool reference = true);
	pred_t	   getItemPred(pred_t pred,bool reference = true);
public:
	template<class T>
	void_t setItem(field_t<T> pred, const T& v)
	{
		setItem((pred_t)pred,(const value_t)(v));
	}

	template<class T>
	const T&  getItem(field_t<T> pred)
	{
		return (const T& )*getItem((pred_t)pred);
	} 

	template<class T>
	const T&  operator()(field_t<T> pred) const
	{
		const value_t * v = getItem((pred_t)pred);
		if(v) return (T& )*v;
		return (const T&)*value_t::nilValue();
	}

	template<class T>
	void  operator()(field_t<T> pred,const value_t&  val) 
	{
		value_t * v = (value_t *)getItem((pred_t)pred);
		if(v)
			v->uval64  = val.uval64;
		else
			append(pred,val);
	}

	template<class T>
	const T&  operator[](field_t<T> pred) const
	{
		const value_t * v = getItem((pred_t)pred);
		if(v) return (T& )*v;
		return (const T&)*value_t::nilValue();
	}

	template<class T>
	T& operator[](field_t<T> pred)
	{
		const value_t * v = getItem((pred_t)pred,false);
		if(v) return (T& )*v;
		int item = append(pred,(int64_t)0);
		v = getItem(item);
		return  (T& ) *v;
	}

public:
	int_t	groupid();
	int_t	id();

//tree
private:
	void_t free_next();
	void_t free_piror();
	void_t free_parent();
	void_t free_child();

private:
	void_t set_next(vord_t& w);
	void_t set_piror(vord_t& w);
	void_t set_parent(vord_t& w);
	void_t set_child(vord_t& w);

public:
	vord_t  get_next();
	vord_t  get_piror();
	vord_t  get_parent();
	vord_t  get_child();

public:
	//append vord_t w as my child
	void_t appendChild(vord_t&  w);
	//remove vord_t w from my childs
	void_t removeChild(vord_t& w);
	//insert first
	void_t insertFirst(vord_t&  w);

	//insert vord_t w before me
	void_t insertBefore(vord_t&  w);
	//insert vord_t w after me
	void_t insertAfter(vord_t& w);

	//split child to two part
	static void_t splitChild(vord_t& newParent,vord_t& from);
	//merge from's child as to's child
	static void_t mergeChild(vord_t& from, vord_t& to);

public:
	bool_t loadXML(wstring_t text);
	wstring_t getXML();
};



} //namespace vord

#endif //WPP_vord_H