#ifndef TVM_H
#define  TVM_H


#include "turing.hpp"
#include "pred.hpp"
#include "word.hpp"
#include <stack>
#include "meta.hpp"

using namespace std;
using namespace wpp::how;


struct VLANG3_API TVm
{
	//type vm

	/*
	register   r0...rn		//val register
			   p0...pn      //pointer register
	var 	   v0			//for this
	           v1..vn		//for var
	           x0..xn		//for param 
	code block b0...bn 
	*/

	/*code format
		opcode  ooooooxx
		oprand1 ttrrrrrr
		oprand2 ttrrrrrr
		oprand3 ttrrrrrr

		tt:
		00 r 01 v 10 p 11 b
	*/

	/*run stack
	*/

	struct Register
	{
		int nIndex;
		int nType;
	};

	struct typeT
	{
		//值类型
		static const unsigned char t_undef		= 0x00;
		static const unsigned char t_uint8		= 0x01;
		static const unsigned char t_uint16		= 0x02;
		static const unsigned char t_uint32		= 0x03;
		static const unsigned char t_int8		= 0x04;
		static const unsigned char t_int16		= 0x05;
		static const unsigned char t_int32		= 0x06;
		static const unsigned char t_float		= 0x07;
		static const unsigned char t_double		= 0x08;
		static const unsigned char t_string		= 0x09;
		static const unsigned char t_pointer	= 0x0A;
		static const unsigned char t_array		= 0x0B;
		static const unsigned char t_class		= 0x0C;
		static const unsigned char t_struct		= 0x0D;
		static const unsigned char t_other		= 0x0F;

		static const unsigned char t_int		= t_int32;
		static const unsigned char t_uint		= t_uint32;
		static const unsigned char t_char		= t_int16;
		static const unsigned char t_uchar		= t_uint16;
		static const unsigned char t_bool		= t_uint8;

		//逻辑类型
		static const unsigned char t_term		= 0x20; 
		static const unsigned char t_prop		= 0x21;
		static const unsigned char t_element	= 0x22;
		static const unsigned char t_relation	= 0x23;
		static const unsigned char t_axiom		= 0x24;
		static const unsigned char t_equation	= 0x25;
		static const unsigned char t_word		= 0x26;
		static const unsigned char t_stmt		= 0x27;
		static const unsigned char t_lang		= 0x28;
	};

	struct type
	{
		unsigned char t;
		wchar_t* id;
		int field;
		int size;
	};

	struct field
	{
		unsigned char*	id;
		int ntyp;
		int next;
	};


	struct regItem
	{
		int ntype;
		CWordValue val;
	};

	struct varItemType
	{
		int ntype;
		int next;
		
		inline varItemType(int ntype_,int next_)
		{
			ntype = ntype_;
			next = next_;
		}
	};

	struct varItem
	{
		int ntype;
		void* ptr;
		int nitemtype; 
		int upper;
		int offset;

		inline varItem():ntype(typeT::t_undef),ptr(0),nitemtype(0),offset(0),upper(1)
		{
		}
		template<class T>
		inline varItem(int ntype_,T * ptr_,int offset_)
		{
			ntype = ntype_;
			ptr =  ptr_;
			nitemtype = 0;
			upper = 1;
			offset = offset_;
		}
		template<class T>
		inline varItem(int ntype_,T * ptr_,int offset_,int nitemtype_,int upper_=1)
		{
			ntype = ntype_;
			ptr =  ptr_;
			nitemtype = nitemtype_;
			upper = upper_;
			offset = offset_;
		}
	};

	struct opcode
	{
		static const unsigned char nop   = 0x00;
		static const unsigned char mov   = 0x01;
		static const unsigned char add   = 0x02;
		static const unsigned char sub   = 0x03;
		static const unsigned char mul   = 0x04;
		static const unsigned char div   = 0x05;
		static const unsigned char or    = 0x08;
		static const unsigned char and   = 0x09;
		static const unsigned char xor   = 0x0A;
		static const unsigned char not   = 0x0B;
		static const unsigned char neg   = 0x0C;
		static const unsigned char shl   = 0x0D;
		static const unsigned char shr   = 0x0E;
		static const unsigned char ret   = 0x0F;
		static const unsigned char call  = 0x10;
		static const unsigned char push  = 0x11;
		static const unsigned char pop   = 0x12;
		static const unsigned char inc   = 0x13;
		static const unsigned char dec   = 0x14;
		static const unsigned char jmp   = 0x15;
		static const unsigned char jz	 = 0x16;
		static const unsigned char jnz   = 0x17;
		static const unsigned char jgt	 = 0x18;
		static const unsigned char jge	 = 0x19;
		static const unsigned char jlt   = 0x1A;
		static const unsigned char jle   = 0x1B;
		static const unsigned char jeq   = 0x1C;
		static const unsigned char jne   = 0x1D;
		static const unsigned char zero  = 0x20;
		static const unsigned char nil   = 0x21;
		static const unsigned char gtr   = 0x22;
		static const unsigned char gte   = 0x23;
		static const unsigned char ltr   = 0x24;
		static const unsigned char lte   = 0x25;
		static const unsigned char eq    = 0x26;
		static const unsigned char neq   = 0x27;
		static const unsigned char br	 = 0x30;

		static const unsigned char yadd   = 0x40;
		static const unsigned char ysub   = 0x41;
		static const unsigned char ymul   = 0x42;
		static const unsigned char ydiv   = 0x43;
		static const unsigned char yor    = 0x44;
		static const unsigned char yand   = 0x45;
		static const unsigned char yeq	  = 0x46;
		static const unsigned char yseq	  = 0x47;
	};

	int enterByte(unsigned char data)
	{
		code.push_back(data);
		return 1;
	}

	int setType(int index,int nty)
	{
		if(index >= (int)type_r->size())
		{
			int size = (int)type_r->size();
			type_r->resize(((index + 8)/8)*8);
			for(int i=size;i<(int)type_r->size();i++)
				(*type_r)[i].ntype=0;
		}
		
		(*type_r)[index].ntype = nty;

		return 1;
	}

	int get_type(int index)
	{
		return (*type_r)[index].ntype;
	}


	int addType(unsigned char ntype,wchar_t* id,int nsize)
	{
		type ty;
		ty.t = ntype;
		ty.id = id;
		ty.field = 0;
		ty.size = nsize;
		
		types->push_back(ty);

		return 1;
	}

	type & getDataType(int index)
	{
		return (*types)[index];
	}

	int getTypeSize(int index)
	{
		return (*types)[index].size;
	}

	int getTypeOffset(int index, int nterm)
	{
		return nterm * getTypeSize(index) ;
	}

	#define	_Index(x) (x)
	#define	_IsReg(t) (t==0)
	#define	_IsVar(t) (t==1)
	#define	_IsTerm(t) (t==2)
	#define	_IsParam(t)(t==3)
	#define	_IsIme(t) (t==4)
	#define _ArReg(x) ((*type_r)[_Index(x)])
	#define _ArItem(t,x) (_IsVar(t)? (*type_v)[_Index(x)]: (*type_p)[_Index(x)])
	#define _IsField(t,x) (_IsVar(t) ? ((*type_v)[_Index(x)].offset==-1?false:true): \
				((*type_p)[_Index(x)].offset==-1?false:true))
	#define _Type(tw,x) (_IsIme(tw)? (*types)[x].t :((*types)[_IsReg(tw)?_ArReg(x).ntype:_ArItem(tw,x).ntype].t))
	#define _Code(x) (x>>2);

	#define _Op1(x,v1,v2,v) switch(x){ \
		case opcode::add: v = (v1)+(v2); break; \
		case opcode::sub: v = (v1)-(v2); break; \
		case opcode::mul: v = (v1)*(v2); break; \
		case opcode::div: v = (v1)/(v2); break; \
	}

	#define _Op2(x,v1,v2,v) switch(x){ \
		case opcode::or:  v = (v1)|(v2); break; \
		case opcode::and: v = (v1)&(v2); break; \
		case opcode::xor: v = (v1)^(v2); break; \
		case opcode::shl: v = (v1)<<(v2);break; \
		case opcode::shr: v = (v1)>>(v2);break; \
	}
	#define _Op3(x,v1,v) switch(x){ \
		case opcode::not: v = ~(v1);    break; \
		case opcode::neg: v = -(v1);    break; \
		case opcode::dec: v = (v1) - 1; break; \
		case opcode::inc: v = (v1) + 1; break; \
	}
	#define _Op4(x,v1,v2,v) switch(x){ \
		case opcode::jgt: v = (v1)>(v2); break; \
		case opcode::jge: v = (v1)>=(v2); break; \
		case opcode::jlt: v = (v1)<(v2); break; \
		case opcode::jle: v = (v1)<=(v2); break; \
		case opcode::jeq:  v = (v1)==(v2); break; \
		case opcode::jne: v = (v1)!=(v2); break; \
	}
	#define _Op5(x,v1,v2,v) switch(x){ \
		case opcode::gtr: v = (v1)>(v2); break; \
		case opcode::gte: v = (v1)>=(v2); break; \
		case opcode::ltr: v = (v1)<(v2); break; \
		case opcode::lte: v = (v1)<=(v2); break; \
		case opcode::eq:  v = (v1)==(v2); break; \
		case opcode::neq: v = (v1)!=(v2); break; \
	}

	template<class T>
	void setValue(int t,unsigned char v, int ty,T val,unsigned char vt,void *p=0)
	{
		if(_IsTerm(t))
		{
			if(ty==vt)
			{
				*(T *)p = val;
			}
			else if(ty==typeT::t_double)
				*(double *)p = (double)val;
			else
				*(int *)p = (int)val;
		}
		else if(_IsReg(t))
		{
			setType(v,vt);
			if(vt==typeT::t_double)
				_ArReg(v).val.dval = (double)val;
			else
				_ArReg(v).val.val = (int)val;
		}else
		{
			if(ty==vt)
			{
				*(T *)getPtr(t,v) = val;
			}
			else if(ty==typeT::t_double)
				*(double *)getPtr(t,v) = (double)val;
			else
				*(int *)getPtr(t,v) = (int)val;
		}
	}

	template<class T>
	T getValue(int t, vector<unsigned char>::iterator &tr,int index, unsigned char dt,void* p=0)
	{
		if(_IsTerm(t))
		  return  *(T *)p;
		else if(_IsIme(t))
		{
			return *(T*)(tr._Myptr+ index + 1);
		}else
		{
			return getValue<T>(t, tr[index],dt);
		}
	}

	template<class T>
	T getValue(int t, int v,unsigned char dt)
	{
		if(_IsReg(t))
		{
			if(_Type(t,v)==typeT::t_double)
				return (T)_ArReg(v).val.dval;
			else
				return (T)_ArReg(v).val.val;
		}
		if(_Type(t,v)==dt)
		{
			return *(T*)getPtr(t,v);
		}else
		{
			if(_Type(t,v)==typeT::t_double)
				return (T)*(double*)getPtr(t,v);
			else
				return (T)*(int*)getPtr(t,v);
		}
	}

	void * getPtr(int t,int v)
	{
		if(_ArItem(t,v).ptr) return _ArItem(t,v).ptr;
		return stacks + ebp + _ArItem(t,v).offset;
	}
	
	void getTermInfo(vector<unsigned char>::iterator &tr, int t, int &sz, void * &p,int & ty, int& tyc)
	{
		tyc = 0;
		ty = 0;
		p = 0;
		int nsize = 0;
		int st = 0;
		int offset = 0;

		if(!_IsTerm(t))
		{
			if(_IsReg(t))
			{
				if(type_r->size()<=_Index(tr[st]))
					ty = 0;
				else
					ty = _Type(t,tr[st]);
			}else
			{
				ty = _Type(t,tr[st]);
				if(_IsIme(t) && ty ==typeT::t_double)st+=sizeof(double);
				if(_IsIme(t) && ty!=typeT::t_double)st+=sizeof(int);
			}
		}else
		{
			unsigned ch = tr[st]>>1;
			//var item 
			st++;
			p = this->getPtr(ch>>4,tr[st]);
			tyc = _Type(ch>>4,tr[st]);
			type & tp = (*types)[(*type_tm)[_ArItem(ch>>4,tr[st]).nitemtype].ntype];
			ty = tp.t;
			nsize = tp.size;

			//register item
			if(((ch>>2) & 0x3)==0x1)
			{
				st++;
				offset = getValue<int>(0/*reg*/,tr,st,typeT::t_int32);
			}
			//direct number item
			if((ch&0x3) == 0x1)
			{
				st++;
				offset += (int)tr[st];
			}else if((ch&0x3) == 0x2)
			{
				st++;
				offset += (short &)tr[st];
				st += sizeof(short) - 1;
			}else if((ch&0x3) == 0x3)
			{
				st++;
				offset += (int &)tr[st];
				st += sizeof(int) - 1;
			}
			p = ((unsigned char *)p) + nsize*offset;
		}
		sz = st + 1;
	}

	int throwError(unsigned int  nError)
	{
		return 1;
	}

	int eval(int pc = 0)
	{
		int ebp_ = ebp;
		int esp_ = esp;
		esp += type_v && type_v->size()>0 ? (*type_v)[type_v->size() - 1].offset + 8 : 0;
		int rc= call(pc);
		ebp = ebp_;
		esp = esp_;
		return rc;
	}

	int call(int pc= 0)
	{
		unsigned char cd;
		vector<unsigned char>::iterator tr = code.begin();
		tr += pc;
		while(tr!=code.end())
		{
			cd = _Code(*tr);
			int st = 0;
			switch(cd)
			{
			case opcode::mov:
				{
					unsigned char t1 = ((tr[0]&0x1)<<2)|(tr[1]>>6);
					unsigned char t2 = ((tr[1]>>3)&0x7);
					int ty1=0;
					int ty2=0;
					void * p1 = 0;
					void * p2 = 0;
					int st1 = 0;
					int st2 = 0;
					int sz = 0;
					int tyc = 0;

					st = 2;
					st1 = st;
					getTermInfo(tr + st,t1,sz,p1,ty1,tyc);
					st += sz;

					st2 = st;
					getTermInfo(tr + st,t2,sz,p2,ty2,tyc);
					st += sz;

					int dty = ty2;
					if(dty==typeT::t_double)
					{
						setValue(t1,tr[st1],ty1,getValue<double>(t2,tr,st2,dty,p2),ty2,p1);
					}else
					{
						setValue(t1,tr[st1],ty1,getValue<int>(t2,tr,st2,dty,p2),ty2,p1);
					}
				}
				break;
			case opcode::add:
			case opcode::sub:
			case opcode::mul:
			case opcode::div:
				{
					int nOprand = ((tr[0]>>1)&0x1) ? 3:2;
					unsigned char t1 = ((tr[0]&0x1)<<2)|(tr[1]>>6);
					unsigned char t2 = ((tr[1]>>3)&0x7);
					unsigned char t3 = (tr[1]&0x7);

					int ty1=0;
					int ty2=0;
					void * p1 = 0;
					void * p2 = 0;
					int st1 = 0;
					int st2 = 0;
					int sz = 0;
					int tyc = 0;

					st = 2;
					st1 = st;
					getTermInfo(tr + st,t1,sz,p1,ty1,tyc);
					st += sz;

					st2 = st;
					getTermInfo(tr + st,t2,sz,p2,ty2,tyc);
					st += sz;

					int dty1 = ty1;
					int dty2 = ty2;
					void* dp1 = p1;
					void* dp2 = p2;
					int dst1 = st1;
					int dst2 = st2;
					int dt1 = t1;
					int dt2 = t2;
					if(nOprand==3)
					{
						int ty3=0;
						void * p3 = 0;
						int st3 = 0;
						st3 = st;
						getTermInfo(tr + st,t3,sz,p3,ty3,tyc);
						st += sz;

						dty1 = ty2;
						dty2 = ty3;
						dp1 = p2;
						dp2 = p3;
						dst1 = st2;
						dst2 = st3;
						dt1 = t2;
						dt2 = t3;
					}

					unsigned char dty = typeT::t_int; //default
					if(dty1 == dty2)dty = dty1;
					else if(dty1 == typeT::t_double||dty2==typeT::t_double)dty = typeT::t_double;
					if(dty==typeT::t_double)
					{
						double d = 0;
						_Op1(cd,getValue<double>(dt1,tr,dst1,dty,dp1),getValue<double>(dt2,tr,dst2,dty,dp2),d);
						setValue(t1,tr[st1],ty1,d,dty,p1);
					}
					else if(dty==typeT::t_int)
					{
						int d = 0;
						_Op1(cd,getValue<int>(dt1,tr,dst1,dty,dp1),getValue<int>(dt2,tr,dst2,dty,dp2),d);
						setValue(t1,tr[st1],ty1,d,dty,p1);
					}
				}
				break;
			case opcode::or:
			case opcode::and:
			case opcode::xor:
			case opcode::shl:
			case opcode::shr:
				{
					int nOprand = ((tr[0]>>1)&0x1) ? 3:2;
					unsigned char t1 = ((tr[0]&0x1)<<2)|(tr[1]>>6);
					unsigned char t2 = ((tr[1]>>3)&0x7);
					unsigned char t3 = (tr[1]&0x7);

					int ty1=0;
					int ty2=0;
					void * p1 = 0;
					void * p2 = 0;
					int st1 = 0;
					int st2 = 0;
					int sz = 0;
					int tyc = 0;

					st = 2;
					st1 = st;
					getTermInfo(tr + st,t1,sz,p1,ty1,tyc);
					st += sz;

					st2 = st;
					getTermInfo(tr + st,t2,sz,p2,ty2,tyc);
					st += sz;

					int dty1 = ty1;
					int dty2 = ty2;
					void* dp1 = p1;
					void* dp2 = p2;
					int dst1 = st1;
					int dst2 = st2;
					int dt1 = t1;
					int dt2 = t2;
					if(nOprand==3)
					{
						int ty3=0;
						void * p3 = 0;
						int st3 = 0;
						st3 = st;
						getTermInfo(tr + st,t3,sz,p3,ty3,tyc);
						st += sz;

						dty1 = ty2;
						dty2 = ty3;
						dp1 = p2;
						dp2 = p3;
						dst1 = st2;
						dst2 = st3;
						dt1 = t2;
						dt2 = t3;
					}

					unsigned char dty = typeT::t_int; //default
					if(dty==typeT::t_int)
					{
						int d = 0;
						_Op2(cd,getValue<int>(dt1,tr,dst1,dty,dp1),getValue<int>(dt2,tr,dst2,dty,dp2),d);
						setValue(t1,tr[st1],ty1,d,dty,p1);
					}
				}
			break;
			case opcode::not:
			case opcode::neg:
			case opcode::inc:
			case opcode::dec:
				{
					unsigned char t1 = ((tr[0]&0x1)<<2)|(tr[1]>>6);
					int ty1=0;
					void * p1 = 0;
					int st1 = 0;
					int sz = 0;
					int tyc = 0;

					st = 2;
					st1 = st;
					getTermInfo(tr + st,t1,sz,p1,ty1,tyc);
					st += sz;

					if(cd == opcode::neg && ty1==typeT::t_double)
					{
						double d = - getValue<double>(t1,tr,st1,ty1,p1);
						setValue(t1,tr[st1],ty1,d,ty1,p1);
					}
					else if(ty1==typeT::t_int)
					{
						int d = 0;
						_Op3(cd,getValue<int>(t1,tr,st1,ty1,p1),d);
						setValue(t1,tr[st1],ty1,d,ty1,p1);
					}
				}
				break;
			case opcode::jmp:
			case opcode::jz:
			case opcode::jnz:
			case opcode::jgt:
			case opcode::jge:
			case opcode::jlt:
			case opcode::jle:
			case opcode::jeq:
			case opcode::jne:
				{
					int nOprand = ((tr[0]&0x3)==2 ? 3 : (tr[0]&0x3)==1? 2 : 1);
					bool bjmp = false;
					if(nOprand==1) bjmp = true;
					else if(nOprand==2)
					{
						unsigned char t1 = ((tr[1]>>3)&0x7);
						int ty1=0;
						void * p1 = 0;
						int st1 = 0;
						int sz = 0;
						int tyc = 0;

						st = 2 + 4;
						st1 = st;
						getTermInfo(tr + st,t1,sz,p1,ty1,tyc);
						st += sz;
						bool bzero = true;
						if(ty1==typeT::t_double)
						{
							double d = getValue<double>(t1,tr,st1,ty1,p1);
							if(d)bzero = false;
						}
						else if(ty1==typeT::t_int)
						{
							int d = getValue<int>(t1,tr,st1,ty1,p1);
							if(d)bzero = false;
						}
						if(opcode::jz==cd && bzero) bjmp = true;
						else if(opcode::jnz == cd && !bzero) bjmp = true;
					}
					else if(nOprand==3)
					{
						unsigned char t1 = ((tr[1]>>3)&0x7);
						unsigned char t2 = (tr[1]&0x7);

						int ty1=0;
						int ty2=0;
						void * p1 = 0;
						void * p2 = 0;
						int st1 = 0;
						int st2 = 0;
						int sz = 0;
						int tyc = 0;

						st = 2 + 4;
						st1 = st;
						getTermInfo(tr + st,t1,sz,p1,ty1,tyc);
						st += sz;

						st2 = st;
						getTermInfo(tr + st,t2,sz,p2,ty2,tyc);
						st += sz;

						unsigned char ty = typeT::t_int; //default
						if(ty1 == ty2)ty = ty1;
						else if(ty1 == typeT::t_double||ty2==typeT::t_double)ty = typeT::t_double;

						bool r=false;
						if(ty==typeT::t_double)
						{
							_Op4(cd,getValue<double>(t1,tr,st1,ty,p1),getValue<double>(t2,tr,st2,ty,p2),r);
						}
						else if(ty==typeT::t_int)
						{
							int d1 = getValue<int>(t1,tr,st1,ty,p1);
							int d2 = getValue<int>(t2,tr,st2,ty,p2);
							_Op4(cd,getValue<int>(t1,tr,st1,ty,p1),getValue<int>(t2,tr,st2,ty,p2),r);
						}
						if(r) bjmp = true;
					}
					if(bjmp)
					{
						int addr = *(int*)(tr+2)._Myptr;
						tr = code.begin() + addr;
						pc = addr;
						continue;
					}
				}
				break;
			case opcode::push:
				{
					unsigned char t1 = ((tr[0]&0x1)<<2)|(tr[1]>>6);
					int ty1=0;
					void * p1 = 0;
					int st1 = 0;
					int sz = 0;
					int tyc = 0;

					st = 2;
					st1 = st;
					getTermInfo(tr + st,t1,sz,p1,ty1,tyc);
					st += sz;

					if(ty1==typeT::t_double)
					{
						double d = getValue<double>(t1,tr,st1,ty1,p1);
						*(double *)(stacks + esp) = d;
						stacksType[(esp>>2)] = ty1;
						esp += sizeof(double);
					}
					else /*if(ty1==typeT::t_int)*/
					{
						int d = getValue<int>(t1,tr,st1,ty1,p1);
						*(int *)(stacks + esp) = d;
						stacksType[(esp>>2)] = ty1;
						esp += sizeof(int);
					}
				}
				break;
			case opcode::pop:
				{
					unsigned char t1 = ((tr[0]&0x1)<<2)|(tr[1]>>6);
					int ty1=0;
					void * p1 = 0;
					int st1 = 0;
					int sz = 0;
					int tyc = 0;

					st = 2;
					st1 = st;
					getTermInfo(tr + st,t1,sz,p1,ty1,tyc);
					st += sz;

					int nIndex = (esp - sizeof(int))>>2;
					unsigned char dty = stacksType[nIndex];
					while(dty==0)
					{
						nIndex -= sizeof(int);
						dty = stacksType[nIndex];
					}
					if(dty==typeT::t_double)
					{
						double d = *(double *)(stacks + (nIndex<<2));
						stacksType[nIndex] = 0;
						esp = nIndex<<2;
						setValue(t1,tr[st1],ty1,d,dty,p1);
					}
					else /*if(ty1==typeT::t_int)*/
					{
						double d = *(int *)(stacks + (nIndex<<2));
						stacksType[nIndex] = 0;
						esp = nIndex<<2;
						setValue(t1,tr[st1],ty1,d,dty,p1);
					}
				}
				break;
			case opcode::call:
				{
					int ebp_ = ebp;
					ebp = esp;

					unsigned char t1 = ((tr[0]&0x1)<<2)|(tr[1]>>6);
					int ty1=0;
					void * p1 = 0;
					int st1 = 0;
					int sz = 0;
					int tyc = 0;

					st = 2;
					st1 = st;
					getTermInfo(tr + st,t1,sz,p1,ty1,tyc);
					st += sz;

					int d = getValue<int>(t1,tr,st1,ty1,p1);
					type_fr->push(type_v);
					type_fp->push(type_p);
					CWord fn;
					fn.wIndex.first = d;
					type_v = (vector<varItem>*)metaTable->get_var_frame(fn);
					type_p = (vector<varItem>*)metaTable->get_param_frame(fn);
					
					//free space for var
					esp += type_v && type_v->size()>0 ? (*type_v)[type_v->size() - 1].offset + 8 : 0;
					int addr = metaTable->get_method_addr(fn);
					call(addr);

					type_v = type_fr->top();
					type_fr->pop();
					type_p = type_fp->top();
					type_fp->pop();
					ebp = ebp_;
				}
				break;
			case opcode::ret:
				esp = ebp;
				if(!((tr[0]>>2)&0x1))
				{
					st = 1;
					return 1;
				}else
				{
					unsigned char t1 = ((tr[0]&0x1)<<2)|(tr[1]>>6);
					int ty1=0;
					void * p1 = 0;
					int st1 = 0;
					int sz = 0;
					int tyc = 0;

					st = 2;
					st1 = st;
					getTermInfo(tr + st,t1,sz,p1,ty1,tyc);
					st += sz;

					int d = getValue<int>(t1,tr,st1,ty1,p1);
					if(d>0)
					{
						int nIndex = (ebp - sizeof(int))>>2;
						unsigned char dty = stacksType[nIndex];
						while(d)
						{
							while(dty==0 )
							{
								nIndex -= sizeof(int);
								dty = stacksType[nIndex];
							}
							d--;
							if(d)
							{
								nIndex--;
								dty = stacksType[nIndex];
							}
						}
						esp = nIndex << 2;
					}
					return 1;
				}
				break;
			case opcode::nop:
				st = 1;
				break;
			//logic operate
			case opcode::yadd:
			case opcode::ysub:
			case opcode::ymul:
			case opcode::ydiv:
			case opcode::yor:
			case opcode::yand:
			case opcode::yeq: 
			case opcode::yseq:
				{
					//term can box variant
					int nOprand = ((tr[0]>>1)&0x1) ? 3:2;
					unsigned char t1 = ((tr[0]&0x1)<<2)|(tr[1]>>6);
					unsigned char t2 = ((tr[1]>>3)&0x7);
					unsigned char t3 = (tr[1]&0x7);

					int ty1=0;
					int ty2=0;
					void * p1 = 0;
					void * p2 = 0;
					int st1 = 0;
					int st2 = 0;
					int sz = 0;
					int tyc = 0;

					st = 2;
					st1 = st;
					getTermInfo(tr + st,t1,sz,p1,ty1,tyc);
					st += sz;

					st2 = st;
					getTermInfo(tr + st,t2,sz,p2,ty2,tyc);
					st += sz;

					int dty1 = ty1;
					int dty2 = ty2;
					void* dp1 = p1;
					void* dp2 = p2;
					int dst1 = st1;
					int dst2 = st2;
					int dt1 = t1;
					int dt2 = t2;
					if(nOprand==3)
					{
						int ty3=0;
						void * p3 = 0;
						int st3 = 0;
						st3 = st;
						getTermInfo(tr + st,t3,sz,p3,ty3,tyc);
						st += sz;

						dty1 = ty2;
						dty2 = ty3;
						dp1 = p2;
						dp2 = p3;
						dst1 = st2;
						dst2 = st3;
						dt1 = t2;
						dt2 = t3;
					}

					unsigned char dty = typeT::t_term; //default

					CWord w1,w2;
					w1.wIndex.first = getValue<int>(dt1,tr,dst1,dty,dp1);
					w2.wIndex.first = getValue<int>(dt2,tr,dst2,dty,dp2);
					
					CWord w;
					w << T(L"logic:expr",T(L"ast:op",cd)<<T(w1,w2));
					int d = w.get_index();
					setValue(t1,tr[st1],ty1,d,dty,p1);
				}
				break;
			}
			tr += st;
			pc += st;
		}
		return 1;
	}

	
	#undef	_Index
	#undef	_IsReg
	#undef	_IsVar
	#undef	_IsParam
	#undef	_IsIme
	#undef  _ArItem
	#undef  _IsField
	#undef	_Type
	#undef  _ArReg
	#undef  _IsField
	#undef  _Step
	#undef  _Code

	int initial()
	{
		types   = new vector<type> ;
		type_r  = new vector<regItem> ;
		type_fn = new stack<int> ;
		type_fr = new stack<vector<varItem>*>;
		type_fp = new stack<vector<varItem>*>;

		type_v  = new  vector<varItem>;
		type_p  = new  vector<varItem> ;
		type_tm = new  vector<varItemType> ;

		static const unsigned char t_uint8		= 0x01;
		static const unsigned char t_uint16		= 0x02;
		static const unsigned char t_uint32		= 0x03;
		static const unsigned char t_int8		= 0x04;
		static const unsigned char t_int16		= 0x05;
		static const unsigned char t_int32		= 0x06;
		static const unsigned char t_float		= 0x07;
		static const unsigned char t_double		= 0x08;
		static const unsigned char t_string		= 0x09;
		static const unsigned char t_pointer	= 0x0A;
		static const unsigned char t_array		= 0x0B;
		static const unsigned char t_class		= 0x0C;
		static const unsigned char t_struct		= 0x0D;
		static const unsigned char t_other		= 0x0F;

		static const unsigned char t_term		= 0x20; 
		static const unsigned char t_prop		= 0x21;
		static const unsigned char t_element	= 0x22;
		static const unsigned char t_relation	= 0x23;
		static const unsigned char t_axiom		= 0x24;
		static const unsigned char t_equation	= 0x25;
		static const unsigned char t_word		= 0x26;
		static const unsigned char t_stmt		= 0x27;
		static const unsigned char t_lang		= 0x28;

		addType(typeT::t_undef,L"undef",0);
		addType(typeT::t_uint8,L"uint8",sizeof(unsigned char));
		addType(typeT::t_uint16,L"uint16",sizeof(unsigned short));
		addType(typeT::t_uint32,L"uint32",sizeof(unsigned int));
		addType(typeT::t_int8,L"int8",sizeof(char));
		addType(typeT::t_int16,L"int16",sizeof(short));
		addType(typeT::t_int32,L"int32",sizeof(int));
		addType(typeT::t_float,L"float",sizeof(float));
		addType(typeT::t_double,L"double",sizeof(double));
		addType(typeT::t_string,L"string",sizeof(wchar_t*));
		addType(typeT::t_pointer,L"pointer",sizeof(void*));
		addType(typeT::t_array,L"array",sizeof(void*));
		addType(typeT::t_class,L"class",sizeof(void*));
		addType(typeT::t_struct,L"struct",sizeof(void*));

		addType(typeT::t_term,L"term",sizeof(int));
		addType(typeT::t_prop,L"prop",sizeof(int));
		addType(typeT::t_element,L"element",sizeof(int));
		addType(typeT::t_relation,L"relation",sizeof(int));
		addType(typeT::t_axiom,L"axiom",sizeof(int));
		addType(typeT::t_equation,L"equation",sizeof(int));
		addType(typeT::t_word,L"word",sizeof(int));
		addType(typeT::t_stmt,L"stmt",sizeof(int));
		addType(typeT::t_lang,L"lang",sizeof(int));

		esp = 0;
		ebp = 0;
		stacks = new unsigned char[0x400*0x256];
		stacksType = new unsigned char[0x400*0x256>>2];

		::memset(stacks,0,0x400*0x256);
		::memset(stacksType,0,0x400*0x256>>2);

		metaTable = 0;

		return 1;
	}

	TVm()
	{
		initial();
	}

	~TVm()
	{
		delete stacks;
		delete stacksType;

		delete types   ;
		delete type_r  ;
		delete type_fn ;
		delete type_fr ;
		delete type_fp ;
		delete type_v  ;
		delete type_p  ;
		delete type_tm ;
	}

#pragma warning( push )
#pragma warning( disable : 4251 )
	vector<unsigned char> code;
	vector<type>* types;
	vector<regItem>* type_r;
	vector<varItem>* type_v;
	vector<varItem>* type_p;
	vector<varItemType>* type_tm;
#pragma warning( pop )
	stack<int>* type_fn;
	stack<vector<varItem>*>* type_fr;
	stack<vector<varItem>*>* type_fp;
	unsigned char* stacks;
	unsigned char* stacksType;
	int ebp;
	int esp;
	wMetaTable* metaTable;
};

struct t_Oprand
{
	enum OpType : unsigned char {eReg=0,eVar=1,eTerm=2,eParam=3,eLabel=0,eDirect=4};
	OpType optype;
	t_Oprand(OpType optype_){optype =optype_;}

	virtual int get_index(){return 0;}
};

struct t_Term;
struct t_Reg : t_Oprand
{
	int index;
	t_Reg():index(0),t_Oprand(eReg){}
	t_Reg(int index_):index(index_),t_Oprand(eReg){}

	virtual int get_index(){return index;}
	inline t_Term   operator + (int d);
};

struct t_Var : t_Oprand 
{
	int index;
	t_Var():index(0),t_Oprand(eVar){}
	t_Var(int index_):index(index_),t_Oprand(eVar){}

	virtual int get_index(){return index;}

	inline t_Term & operator [] (t_Term & r);
	inline t_Term   operator [] (int d);
	inline t_Term   operator [] (t_Reg & r);

};

struct t_Param : t_Oprand 
{
	int index;
	t_Param():index(0),t_Oprand(eParam){}
	t_Param(int index_):index(index_),t_Oprand(eParam){}
	virtual int get_index(){return index;}
};

struct t_Term : t_Oprand
{
	t_Var base;
	int scale;
	t_Reg rindex;
	int direct;
	t_Term():base(0),scale(0),rindex(-1),direct(0),t_Oprand(eTerm){}
	
	virtual int get_index(){return base.get_index();}
	unsigned char getTermChar()
	{
		unsigned char ch;
		ch = base.optype<<5;
		if(rindex.get_index()>=0) ch |= (0x01)<<3;
		if(abs(direct)<127) ch |= 0x1<<1;
		else if(abs(direct)< ((1<<15) - 1)) ch |= 0x2<<1;
		else ch |= 0x3<<1;

		return ch;
	}
};


struct t_Label : t_Oprand
{
	t_Label():t_Oprand(eLabel){
		addr = 0;
		initial = false;
	}

	int addr;
	bool initial;
	vector<int> links;

	virtual int get_index(){return addr;}
	template<class T>
	inline T & operator () (T & r)
	{
		r.label = this;
		return r;
	}
};
struct t_Op
{
	unsigned char op;
	t_Op(unsigned char op_):op(op_),used(0),label(0),link(0){}

	int used;
	unsigned char data[32];
	t_Label * label;
	t_Label * link;
	int toCode(struct TVm &vm)
	{
		int top = (int)vm.code.size();
		if(link)
		{
			if(link->addr) 
				*(int *)(data + 2) = link->addr;
			else
				link->links.push_back(top+2);
		}
		if(label)
		{
			label->addr = top;
			for(int i=0;i<(int)label->links.size();i++)
				*(int *)&vm.code[label->links[i]] = top;
		}
		for(int i=0;i<used;i++)vm.enterByte(data[i]);

		return 0;
	}

	void pushByte(unsigned char d)
	{
		data[used] = d;
		used++;
	}
	
	void pushChar(char d)
	{
		*(char*)(data + used) = d;
		used++;
	}

	void pushShort(short d)
	{
		*(short*)(data + used) = d;
		used++;
	}

	void pushInt(int d)
	{
		*(int*)(data + used) = d;
		used += sizeof(int);
	}
	void pushDouble(double d)
	{
		*(double *)(data + used) = d;
		used += sizeof(double);
	}

	void pushOprandCode(t_Oprand & t)
	{
		if(t.optype == t.eTerm)
		{
			t_Term & tm = (t_Term &)t;
			unsigned char ch = tm.getTermChar();
			this->pushByte(ch);
			this->pushByte(tm.base.get_index());
			if(tm.rindex.get_index()>=0)this->pushByte(tm.rindex.get_index());
			if(ch & (0x1<<1))this->pushChar((char)tm.direct);
			else if(ch & (0x2<<1)) this->pushShort((short)tm.direct);
			else if(ch & (0x3<<1)) this->pushInt((int)tm.direct);
		}else
		{
			this->pushByte((unsigned char)t.get_index());
		}
	}

	t_Op & makeCode(t_Oprand & t1,t_Oprand & t2,t_Oprand & t3)
	{
		this->pushByte((op<<2)|0x2|(t1.optype>>2));
		this->pushByte((t1.optype<<6)|(t2.optype<<3)|t3.optype);
		this->pushOprandCode(t1);
		this->pushOprandCode(t2);
		this->pushOprandCode(t3);
		return *this;
	}

	t_Op & makeCode(t_Oprand & t1,t_Oprand & t2,int i)
	{
		this->pushByte((op<<2)|0x2|(t1.optype>>2));
		this->pushByte((t1.optype<<6)|(t2.optype<<3)|t_Oprand::eDirect);
		this->pushOprandCode(t1);
		this->pushOprandCode(t2);
		this->pushByte(TVm::typeT::t_int32);
		this->pushInt(i);
		return *this;
	}

	t_Op & makeCode(t_Oprand & t1,t_Oprand & t2,double d)
	{
		this->pushByte((op<<2)|0x2|(t1.optype>>2));
		this->pushByte((t1.optype<<6)|(t2.optype<<3)|t_Oprand::eDirect);
		this->pushOprandCode(t1);
		this->pushOprandCode(t2);
		this->pushByte(TVm::typeT::t_double);
		this->pushDouble(d);
		return *this;
	}

	t_Op & makeCode(t_Oprand & t1,t_Oprand & t2)
	{
		this->pushByte((op<<2)|0x0|(t1.optype>>2));
		this->pushByte((t1.optype<<6)|(t2.optype<<3));

		this->pushOprandCode(t1);
		this->pushOprandCode(t2);
		return *this;
	}

	t_Op & makeCode(t_Oprand & t1,int i)
	{
		this->pushByte((op<<2)|0x0|(t1.optype>>2));
		this->pushByte((t1.optype<<6)|(t_Oprand::eDirect<<3));

		this->pushOprandCode(t1);
		this->pushByte(TVm::typeT::t_int32);
		this->pushInt(i);
		return *this;
	}

	t_Op & makeCode(t_Oprand & t1,double d)
	{
		this->pushByte((op<<2)|0x0|(t1.optype>>2));
		this->pushByte((t1.optype<<6)|(t_Oprand::eDirect<<3));

		this->pushOprandCode(t1);
		this->pushByte(TVm::typeT::t_double);
		this->pushDouble(d);
		return *this;
	}
	
	t_Op & makeCode(t_Oprand & t1)
	{
		this->pushByte((op<<2)|0x0|(t1.optype>>2));
		this->pushByte((t1.optype<<6));

		this->pushOprandCode(t1);
		return *this;
	}
	inline t_Op & makeCode(int i)
	{
		this->pushByte((op<<2)|0x0|(t_Oprand::eDirect>>2));
		this->pushByte((unsigned char)(t_Oprand::eDirect<<6));
		this->pushByte(TVm::typeT::t_int);
		this->pushInt(i);
		return *this;
	}
	inline t_Op & makeCode(double d)
	{
		this->pushByte((op<<2)|0x0|(t_Oprand::eDirect>>2));
		this->pushByte((unsigned char)(t_Oprand::eDirect<<6));
		this->pushByte(TVm::typeT::t_double);
		this->pushDouble(d);
		return *this;
	}
};

struct t_Ops_mov : t_Op
{
	t_Ops_mov(unsigned char op_):t_Op(op_){}
	inline t_Op & operator () (t_Oprand & t1,t_Oprand & t2)
	{
		return makeCode(t1,t2);
	}
	inline t_Op & operator () (t_Oprand & t1,int i)
	{
		return makeCode(t1,i);
	}
	inline t_Op & operator () (t_Oprand & t1,double d)
	{
		return makeCode(t1,d);
	}
};

struct t_Ops_add : t_Op
{
	t_Ops_add(unsigned char op_):t_Op(op_){}
	inline t_Op & operator () (t_Oprand & t1,t_Oprand & t2,t_Oprand & t3)
	{
		return makeCode(t1,t2,t3);
	}
	inline t_Op & operator () (t_Oprand & t1,t_Oprand & t2,int i)
	{
		return makeCode(t1,t2,i);
	}
	inline t_Op & operator () (t_Oprand & t1,t_Oprand & t2,double d)
	{
		return makeCode(t1,t2,d);
	}

	inline t_Op & operator () (t_Oprand & t1,t_Oprand & t2)
	{
		return makeCode(t1,t2);
	}
	inline t_Op & operator () (t_Oprand & t1,int i)
	{
		return makeCode(t1,i);
	}
	inline t_Op & operator () (t_Oprand & t1,double d)
	{
		return makeCode(t1,d);
	}
};

struct t_Ops_push : t_Op
{
	t_Ops_push(unsigned char op_):t_Op(op_){}
	inline t_Op & operator () (t_Oprand & t1)
	{
		return makeCode(t1);
	}
	inline t_Op & operator () (int i)
	{
		return makeCode(i);
	}
	inline t_Op & operator () (double d)
	{
		return makeCode(d);
	}
};

struct t_Ops_inc : t_Op
{
	t_Ops_inc(unsigned char op_):t_Op(op_){}
	inline t_Op & operator () (t_Oprand & t1)
	{
		return makeCode(t1);
	}
};

struct t_Ops_nop : t_Op
{
	t_Ops_nop(unsigned char op_):t_Op(op_){}
	inline t_Op & operator()()
	{
		this->pushByte((op<<2));
		return *this;
	}
};
struct t_Ops_ret : t_Op
{
	t_Ops_ret(unsigned char op_):t_Op(op_){}
	inline t_Op & operator()()
	{
		this->pushByte(op<<2);
		return *this;
	}
	
	inline t_Op & operator () (t_Oprand & t1)
	{
		this->pushByte((op<<2)|0x2|(t1.optype>>2));
		this->pushByte(t1.optype<<6);
		this->pushOprandCode(t1);
		return *this;
	}
	inline t_Op & operator () (int i)
	{
		this->pushByte((op<<2)|0x2|(t_Oprand::eDirect>>2));
		this->pushByte((unsigned char)(t_Oprand::eDirect<<6));
		this->pushByte(TVm::typeT::t_int32);
		this->pushInt(i);
		return *this;
	}
	inline t_Op & operator () (double d)
	{
		this->pushByte((op<<2)|0x2|(t_Oprand::eDirect>>2));
		this->pushByte((unsigned char)(t_Oprand::eDirect<<6));
		this->pushByte(TVm::typeT::t_double);
		this->pushDouble(d);
		return *this;
	}
};

struct t_Ops_call : t_Op
{
	//last bit 1 not for label flag for two oprand
	t_Ops_call(unsigned char op_):t_Op(op_){}
	inline t_Op & operator () (t_Label & label)
	{
		link = & label;
		this->pushByte((op<<2));
		this->pushByte((label.optype<<6));
		this->pushInt(0);
		return *this;
	}
	inline t_Op & operator () (int i)
	{
		this->pushByte((op<<2)|(t_Oprand::eDirect>>2));
		this->pushByte((unsigned char)(t_Oprand::eDirect<<6));
		this->pushByte(TVm::typeT::t_int32);
		this->pushInt(i);
		return *this;
	}
};

struct t_Ops_jmp1 : t_Op
{
	//last bit 1 not for label flag for two oprand
	t_Ops_jmp1(unsigned char op_):t_Op(op_){}
	inline t_Op & operator () (t_Label & label)
	{
		link = & label;
		this->pushByte((op<<2));
		this->pushByte((label.optype<<6));
		this->pushInt(0);
		return *this;
	}
};

struct t_Ops_jmp2 : t_Op
{
	t_Ops_jmp2(unsigned char op_):t_Op(op_){}
	inline t_Op & operator () (t_Label & label,t_Oprand & t2)
	{
		link = & label;
		this->pushByte((op<<2)|0x1);
		this->pushByte((t2.optype<<3));

		this->pushInt(0);
		this->pushOprandCode(t2);
		return *this;
	}

	inline t_Op & operator () (t_Label & label,int i)
	{
		link = & label;
		this->pushByte((op<<2)|0x1); 
		this->pushByte(t_Oprand::eDirect<<3);

		this->pushInt(0);
		this->pushByte(TVm::typeT::t_int32);
		this->pushInt(i);
		return *this;
	}

	inline t_Op & operator () (t_Label & label,double d)
	{
		link = & label;
		this->pushByte((op<<2)|0x1);
		this->pushByte(t_Oprand::eDirect<<3);

		this->pushInt(0);
		this->pushByte(TVm::typeT::t_double);
		this->pushDouble(d);
		return *this;
	}
};
struct t_Ops_jmp3 : t_Op
{
	t_Ops_jmp3(unsigned char op_):t_Op(op_){}
	inline t_Op & operator () (t_Label & label,t_Oprand & t2,t_Oprand & t3)
	{
		link = & label;
		this->pushByte((op<<2)|0x2|(label.optype>>2));
		this->pushByte((label.optype<<6)|(t2.optype<<3)|t3.optype);
		this->pushInt(0);
		this->pushOprandCode(t2);
		this->pushOprandCode(t3);
		return *this;
	}

	inline t_Op & operator () (t_Label & label,t_Oprand & t2,int i)
	{
		link = & label;
		this->pushByte((op<<2)|0x2|(label.optype>>2));
		this->pushByte((label.optype<<6)|(t2.optype<<3)|t_Oprand::eDirect);
		this->pushInt(0);
		this->pushOprandCode(t2);
		this->pushByte(TVm::typeT::t_int32);
		this->pushInt(i);
		return *this;
	}

	inline t_Op & operator () (t_Label & label,t_Oprand & t2,double d)
	{
		link = & label;
		return this->makeCode(label,t2,d);
	}
};
struct t_Ops_logic : t_Op
{
	t_Ops_logic(unsigned char op_):t_Op(op_){}
	inline t_Op & operator () (t_Oprand & t1,t_Oprand & t2,t_Oprand & t3)
	{
		return makeCode(t1,t2,t3);
	}
};

typedef t_Ops_mov  t_Op_mov;
typedef t_Ops_add  t_Op_add,t_Op_sub,t_Op_div,t_Op_mul;
typedef t_Ops_add  t_Op_or, t_Op_and, t_Op_xor, t_Op_shr,t_Op_shl;
typedef t_Ops_push t_Op_push;
typedef t_Ops_inc  t_Op_pop,t_Op_dec,t_Op_inc;
typedef t_Ops_nop  t_Op_nop;
typedef t_Ops_ret  t_Op_ret;
typedef t_Ops_jmp1  t_Op_jmp;
typedef t_Ops_jmp2  t_Op_jz,t_Op_jnz;
typedef t_Ops_jmp3  t_Op_jgt,t_Op_jlt,t_Op_jge,t_Op_jle,t_Op_jeq,t_Op_jne;
typedef t_Ops_call  t_Op_call;
typedef t_Ops_logic t_Op_yadd,t_Op_ysub,t_Op_ymul,t_Op_ydiv,t_Op_yor,t_Op_yand,t_Op_yeq,t_Op_yseq;


inline t_Term &  t_Var::operator [] (t_Term & r)
{
	r.base = *this;
	return r;
}

inline t_Term   t_Var::operator [] (int d)
{
	t_Term t;
	t.base = *this;
	t.direct = d;
	return t;
}

inline t_Term   t_Var::operator [] (t_Reg & r)
{
	t_Term t;
	t.base = *this;
	t.rindex = r;
	return t;
}


inline t_Term   t_Reg::operator + (int d)
{
	t_Term t;
	t.rindex = *this;
	t.direct = d;
	return t;
}

#define asm_MakeOp(v,x,...) asm_##x, __VA_ARGS__).toCode(v) 
#define asm_op(x)	(t_Op_##x(TVm::opcode::##x))(
#define asm_mov		asm_op(mov)
#define asm_add		asm_op(add)
#define asm_sub		asm_op(sub)
#define asm_mul		asm_op(mul)
#define asm_div		asm_op(div)
#define asm_xor		asm_op(xor)
#define asm_or		asm_op(or ) 
#define asm_and		asm_op(and)
#define asm_shr		asm_op(shr)
#define asm_shl		asm_op(shl) 
#define asm_inc		asm_op(inc)
#define asm_dec		asm_op(dec)
#define asm_push	asm_op(push)
#define asm_pop		asm_op(pop)
#define asm_ret		asm_op(ret) 
#define asm_jmp		asm_op(jmp)
#define asm_jz		asm_op(jz)
#define asm_jnz		asm_op(jnz)
#define asm_jgt		asm_op(jgt)
#define asm_jge		asm_op(jge)
#define asm_jlt		asm_op(jlt)
#define asm_jle		asm_op(jle)
#define asm_jeq		asm_op(jeq)
#define asm_jne		asm_op(jne)
#define asm_call	asm_op(call)

#define asm_yadd   asm_op(yadd)
#define asm_ysub   asm_op(ysub)
#define asm_ymul   asm_op(ymul)
#define asm_ydiv   asm_op(ydiv)
#define asm_yor    asm_op(yor)
#define asm_yand   asm_op(yand)
#define asm_yeq	   asm_op(yeq)
#define asm_yseq   asm_op(yseq)

#endif //TVM_H