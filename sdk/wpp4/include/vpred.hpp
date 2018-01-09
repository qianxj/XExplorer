#ifndef  WPP_VPRED_H
#define  WPP_VPRED_H

#include "vlang4.h"

class pred_t
{
public:
	int pred_;
	pred_t(int pred):pred_(pred){}

	inline int id(){return pred_;}
	inline bool operator ==(const pred_t&  right) {return pred_==right.pred_;}
	inline bool operator !=(const pred_t&  right) {return pred_!=right.pred_;}
}; 

template <typename T>
struct field_t : pred_t
{
	typedef  T type;
	field_t(int pred):pred_t(pred){}
};

#endif WPP_VPRED_H