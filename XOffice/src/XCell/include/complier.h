#pragma once



#define ARRAYINC 255
#define ARRAYMANINC 255
#define STRINC 255
#define SCCONSTINIT	32768

enum{sv_int,sv_char,sv_float,sv_str,sv_bool};

class CStr
{
public:
	CStr()
	{
		m_iStrlen = 0;
		m_iSize = 0;
		m_pData = NULL;
	};
	CStr(LPTSTR str)
	{
		int strlen;
		int	alloclen; 
		strlen = (int)::_tcslen(str);
		if((strlen + 1) % ARRAYINC ==0)
			alloclen = (strlen + 1) / ARRAYINC ;
		else
			alloclen = (strlen + 1) / ARRAYINC  + ARRAYINC;
		
		m_pData = new TCHAR[alloclen];
		::_tcscpy(m_pData,str);
		m_iStrlen = strlen;
		m_iSize = alloclen;
	};

	CStr(TCHAR ch)
	{
		int strlen;
		int	alloclen; 
		strlen = 1;
		if((strlen + 1) % ARRAYINC ==0)
			alloclen = (strlen + 1) / ARRAYINC ;
		else
			alloclen = (strlen + 1) / ARRAYINC  + ARRAYINC;
		
		m_pData = new TCHAR[alloclen];
		m_pData[0] = ch;
		m_pData[1]='\0';
		m_iStrlen = strlen;
		m_iSize = alloclen;
	};

	~CStr()
	{
		if(m_pData)delete m_pData;
	};

	CStr & operator=( const LPTSTR str )
	{
		int strlen;
		int	alloclen; 
		if(str)
		{
			strlen = ::_tcslen(str);
			if((strlen + 1) % ARRAYINC ==0)
				alloclen = (strlen + 1) / ARRAYINC ;
			else
				alloclen = (strlen + 1) / ARRAYINC  + ARRAYINC;
			if(m_pData)
			{
				if(m_iSize >= alloclen && m_iSize - alloclen <=ARRAYINC) //fit to use old buf
				{
					::_tcscpy(m_pData,str);
					m_iStrlen = strlen;
					return( *this );
				}
				else
					delete m_pData;
			}
			m_pData = new TCHAR[alloclen];
			::_tcscpy(m_pData,str);
			m_iStrlen = strlen;
			m_iSize = alloclen;
		}else
		{
			if(m_pData) delete m_pData;
			m_pData = NULL;
			m_iStrlen = 0;
			m_iSize = 0;
		}

		return( *this );
	}

	CStr & operator=( const int nullstr ) 
	{
		if(m_pData)
		{
			delete m_pData;
			m_pData = NULL;
		}
		m_iStrlen = 0;
		m_iSize = 0;

		return( *this );
	}

	CStr & operator=( const TCHAR ch )
	{
		TCHAR str[2];
		str[0] = ch;
		str[1]='\0';
		CStr::operator =( str);
		return( *this );
	}

	CStr & operator=( const CStr & str )
	{
		CStr::operator =( str.m_pData);
		return( *this );
	}

	CStr & operator+=( const LPTSTR str )
	{
		int strlen;
		int	alloclen; 
		LPTSTR pStr;
		if(str==NULL)return (*this);
		strlen = ::_tcslen(str);
		strlen += this->m_iStrlen ;
		if(strlen + 1 > this->m_iSize)
		{
			if((strlen + 1) % ARRAYINC ==0)
				alloclen = (strlen + 1) / ARRAYINC ;
			else
				alloclen = (strlen + 1) / ARRAYINC  + ARRAYINC;
			pStr = new TCHAR[alloclen];
			this->m_iSize = alloclen;
			if(m_pData)	::_tcscpy(pStr,m_pData);
			delete m_pData;
			m_pData = pStr;
		}

		if(str)
		{
			::_tcscpy(m_pData + m_iStrlen,str);
			m_iStrlen += ::_tcslen(str);
		}
		return( *this );
	}

	CStr & operator+=( const TCHAR ch )
	{
		TCHAR str[2];
		str[0] = ch;
		str[1]='\0';
		CStr::operator +=( str);
		return( *this );
	}

	CStr & operator+=( const CStr & str )
	{
		CStr::operator +=( str.m_pData);
		return( *this );
	}

	CStr & operator+( const LPTSTR str )
	{
		CStr *  pStr = new CStr();
		*pStr +=*this;
		*pStr +=(str);
		return *pStr;
	}

	CStr & operator+( const TCHAR ch )
	{
		CStr *  pStr = new CStr();
		*pStr +=*this;
		*pStr +=(ch);
		return *pStr;
	}

	CStr & operator +( const CStr & str )
	{
		CStr *  pStr = new CStr();
		*pStr +=*this;
		*pStr +=(str);
		return *pStr;
	}

	operator LPTSTR() const
	{
		return (m_pData);
	}

	friend bool operator==( const CStr & str1 ,const CStr &str2 )
	{
		int result;
		result = ::lstrcmp((LPTSTR)str1,(LPTSTR)str2);
		if (result==0) return TRUE;
		return FALSE;
	};

	friend bool operator==( const CStr & str1 ,const LPTSTR str2 )
	{
		int result;
		result = ::lstrcmp((LPTSTR)str1,str2);
		if (result==0) return TRUE;
		return FALSE;
	};

	friend bool operator==( const LPTSTR str1 ,const CStr & str2 )
	{
		int result;
		result = ::lstrcmp(str1,(LPTSTR)str2);
		if (result==0) return TRUE;
		return FALSE;
	};

	friend bool operator!=( const CStr & str1 ,const CStr &str2 )
	{
		return !(str1==str2);
	};

	friend bool operator!=( const CStr & str1 ,const LPTSTR str2 )
	{
		return !(str1==str2);
	};

	friend bool operator!=( const LPTSTR str1 ,const CStr & str2 )
	{
		return !(str1==str2);
	};

	friend bool operator>=( const CStr & str1 ,const CStr str2 )
	{
		int result;
		result = ::lstrcmp((LPTSTR)str1,(LPTSTR)str2);
		if (result>=0) return TRUE;
		return FALSE;
	};

	friend bool operator>=( const CStr & str1 ,const LPTSTR str2 )
	{
		int result;
		result = ::lstrcmp((LPTSTR)str1,str2);
		if (result>=0) return TRUE;
		return FALSE;
	};
	friend bool operator>=( const LPTSTR str1 ,const CStr & str2 )
	{
		int result;
		result = ::lstrcmp(str1,(LPTSTR)str2);
		if (result >=0) return TRUE;
		return FALSE;
	};

	friend bool operator>( const CStr & str1 ,const CStr str2 )
	{
		int result;
		result = ::lstrcmp((LPTSTR)str1,(LPTSTR)str2);
		if (result>0) return TRUE;
		return FALSE;
	};
	friend bool operator>( const LPTSTR str1 ,const CStr & str2 )
	{
		int result;
		result = ::lstrcmp(str1,(LPTSTR)str2);
		if (result >0) return TRUE;
		return FALSE;
	};

	friend bool operator>( const CStr & str1 ,const LPTSTR  str2 )
	{
		int result;
		result = ::lstrcmp((LPTSTR)str1,str2);
		if (result >0) return TRUE;
		return FALSE;
	};


	friend bool operator<=( const CStr & str1 ,const CStr str2 )
	{
		int result;
		result = ::lstrcmp((LPTSTR)str1,(LPTSTR)str2);
		if (result<=0) return TRUE;
		return FALSE;
	};
	friend bool operator<=( const CStr & str1 ,const LPTSTR  str2 )
	{
		int result;
		result = ::lstrcmp((LPTSTR)str1,str2);
		if (result <=0) return TRUE;
		return FALSE;
	};
	friend bool operator<=( const LPTSTR str1 ,const CStr & str2 )
	{
		int result;
		result = ::lstrcmp(str1,(LPTSTR)str2);
		if (result <=0) return TRUE;
		return FALSE;
	};

	friend bool operator<( const CStr & str1 ,const CStr str2 )
	{
		int result;
		result = ::lstrcmp((LPTSTR)str1,(LPTSTR)str2);
		if (result<0) return TRUE;
		return FALSE;
	};
	friend bool operator<( const CStr & str1 ,const LPTSTR  str2 )
	{
		int result;
		result = ::lstrcmp((LPTSTR)str1,str2);
		if (result < 0) return TRUE;
		return FALSE;
	};
	friend bool operator<( const LPTSTR str1 ,const CStr & str2 )
	{
		int result;
		result = ::lstrcmp(str1,(LPTSTR)str2);
		if (result <0) return TRUE;
		return FALSE;
	};


	CStr&  makeLower()
	{
		::_tcslwr(m_pData);
		return (*this);
	}

	CStr& makeUpper()
	{
		::_tcsupr(m_pData);
		return (*this);
	}

	friend CStr & upper(const CStr & str)
	{
		CStr*  pStr = new CStr(str);
		::_tcsupr(pStr->m_pData);
		return *pStr;
	}
	friend CStr & lower(const CStr & str)
	{
		CStr*  pStr = new CStr(str);
		::_tcslwr(pStr->m_pData);
		return *pStr;
	}
	

	TCHAR * m_pData;
	int m_iStrlen;
	int m_iSize;


};

class CArrayMan
{
	public:
		CArrayMan(void)
		{
			m_pItem = new LPVOID[ARRAYMANINC];
			m_pItemUpperBound = new int[ARRAYMANINC];
			m_pItemSize = new int[ARRAYMANINC];
			m_pItemTypeSize = new int[ARRAYMANINC];
			m_iSize = ARRAYMANINC;
			 m_iUpperBound = -1;
		};
	public:
		~CArrayMan(void)
		{
			delete m_pItem;
			delete m_pItemUpperBound;
			delete m_pItemSize;
			delete m_pItemTypeSize;
		};
	LPVOID * m_pItem;
	int * m_pItemUpperBound;
	int * m_pItemSize;
	int * m_pItemTypeSize;
	int m_iSize;
	int m_iUpperBound;
		int AddItem(LPVOID &Item,int typeSize,int initSize =0,int upperBound = -1)
	{
		int i;
		for(i=0;i<=m_iUpperBound;i++)
		{
			if( &Item == m_pItem[i])break;
		}
		if(i <= m_iUpperBound) return i; //已进入项管理
		for(i=0;i<=m_iUpperBound;i++)
		{
			if(NULL == m_pItem[i])break;
		}
		if( i>m_iUpperBound)
		{
			m_iUpperBound ++;
			if(m_iUpperBound == m_iSize)
			{
				LPVOID * pItem;
				m_iSize += ARRAYMANINC;
				pItem = new LPVOID[m_iSize];
				::memcpy(m_pItem,pItem,sizeof(LPVOID)*(m_iUpperBound  - 1));
				delete m_pItem;
				m_pItem = pItem;
			}
			i = m_iUpperBound;
		}
		m_pItem[i] = &Item;
		if(initSize==0) initSize = ARRAYINC;
		if(upperBound >=0 )
		{
			m_pItemUpperBound[i] = upperBound;
			m_pItemSize[i] =upperBound + 1;
		}else
		{
			Item = new BYTE[typeSize * ARRAYINC];
			::memset(Item,0,typeSize * ARRAYINC);
			m_pItemUpperBound[i] = -1;
			m_pItemSize[i] =initSize;
		}
		m_pItemTypeSize[i] = typeSize;
		return i;
	};

	int RemoveItem(LPVOID &Item)
	{
		int i;
		for(i=0;i<=m_iUpperBound;i++)
		{
			if( &Item == m_pItem[i])break;
		}
		if(i > m_iUpperBound) return -1; //未进入项管理
		m_pItem[i] = NULL;
	}

	int GetUpperBound(LPVOID &Item)
	{
		int i;
		for(i=0;i<=m_iUpperBound;i++)
		{
			if(&Item == m_pItem[i])break;
		}
		if(i > m_iUpperBound) return -1; //未进入项管理
		return m_pItemUpperBound[i];
	}

	int AddItemData(LPVOID &Item)
	{
		int i;
		for(i=0;i<=m_iUpperBound;i++)
		{
			if(&Item == m_pItem[i])break;
		}
		if(i > m_iUpperBound) return -1; //未进入项管理
		m_pItemUpperBound[i]++;
		if(m_pItemUpperBound[i] == m_pItemSize[i])
		{
			LPBYTE pByte;
			m_pItemSize[i] += ARRAYINC;
			pByte = new BYTE[m_pItemTypeSize[i] * m_pItemSize[i]];
			::memset(pByte,0,m_pItemTypeSize[i] * m_pItemSize[i]);
			::memcpy(pByte,*(LPBYTE *)(m_pItem[i]),m_pItemTypeSize[i] * (m_pItemSize[i] - ARRAYINC));
			delete *(LPBYTE *)(m_pItem[i]);
			*(LPBYTE *)(m_pItem[i]) = pByte;
		}
		return m_pItemUpperBound[i];
	};

};


struct var
{
	union
	{
		int i;
		bool b;
		TCHAR ch;
	};
	float f;
	CStr* m_pStr;
	int sv_type;
	var()
	{
		i=0;
		f=0;
		sv_type=0;
		m_pStr = new CStr();
	};

	~var()
	{
		if(m_pStr)delete m_pStr;
	};
	float toFloat()
	{
		if(sv_type==sv_int) f=i;
		else if( sv_type == sv_char) f = ch;
		else if(sv_type==sv_bool) f = b;
		else if(sv_type==sv_str)
		{
			if(m_pStr->m_pData)
			{
				f  = ::_tstof((m_pStr->m_pData));
				//delete m_pStr;
				//m_pStr = NULL;
			}
			else 
				f=0;
		}
		this->sv_type=sv_float;
		return f;
	}
	int toInt()
	{
		if(sv_type==sv_float) i = f;
		else if(sv_type==sv_str)
		{
			if(m_pStr->m_pData)
			{
				i  = ::_tstoi((m_pStr->m_pData));
				//delete m_pStr;
				//m_pStr = NULL;
			}
			else 
				i=0;
		}
		this->sv_type=sv_int;
		return i;
	}
	LPTSTR toString()
	{
		if(!this->m_pStr)
			this->m_pStr = new CStr();
		TCHAR buf[60];
		if(sv_type==sv_float)
		{
			::_stprintf(buf,"%f",f);
			*this->m_pStr = buf;
		}
		else if(sv_type==sv_int)
		{
			::_stprintf(buf,"%i",i);
			*this->m_pStr = buf;
		}
		else if(sv_type==sv_char)
		{
			::_stprintf(buf,"%c",ch);
			*this->m_pStr = buf;
		}
		else if(sv_type==sv_bool)
		{
			if(b)
				*m_pStr=_T("true");
			else
				*m_pStr=_T("false");
		}
		this->sv_type=sv_str;
		return m_pStr->m_pData;
	}
	var & operator=(const int value)
	{
		if(this->sv_type = sv_int && m_pStr)
		{
			delete m_pStr;
			m_pStr = NULL;
		}
		this->i = value;
		this->sv_type = sv_int;
		return (*this);
	}
	var & operator=(const var & var1)
	{
		i = var1.i;
		f = var1.f;
		sv_type = var1.sv_type;
		if(var1.m_pStr->m_pData) *m_pStr = var1.m_pStr->m_pData;
		return (*this);
	}
	var & operator=(const float value)
	{
		if(this->sv_type = sv_int && m_pStr)
		{
			delete m_pStr;
			m_pStr = NULL;
		}
		this->f = value;
		this->sv_type = sv_float;
		return (*this);
	}
	var & operator=(const LPTSTR value)
	{
		if(this->sv_type = sv_int && m_pStr)
		{
			delete m_pStr;
			m_pStr = NULL;
		}
		if(!this->m_pStr)
			this->m_pStr = new CStr();
		*this->m_pStr = value;
		this->sv_type = sv_str;
		return (*this);
	}
	var & operator=(const bool value)
	{
		if(this->sv_type = sv_int && m_pStr)
		{
			delete m_pStr;
			m_pStr = NULL;
		}
		this->b = value;
		this->sv_type = sv_bool;
		return (*this);
	}
	var & operator=(const TCHAR value)
	{
		if(this->sv_type = sv_int && m_pStr)
		{
			delete m_pStr;
			m_pStr = NULL;
		}
		this->ch = value;
		this->sv_type = sv_char;
		return (*this);
	}

	operator int() 
	{
		toInt();
		return this->i;
	}
	operator float() 
	{
		toFloat();
		return this->f;
	}
	operator TCHAR() 
	{
		this->ch = toInt() % 65536;
		return this->ch;
	}
	operator LPSTR() 
	{
		if(!m_pStr) return NULL;
		return this->m_pStr->m_pData;
	}

	operator bool() 
	{
		return this->b;
	}
	friend bool operator==(const var &r1 ,const var &r2)
	{
		if(r1.sv_type == sv_int)
		{
			if(r2.sv_type == sv_int) return r1.i == r2.i;
			if(r2.sv_type == sv_float) return (float)r1.i == r2.f;
			if(r2.sv_type == sv_bool)
			{
				if(r2.b && r1.i !=0)
					return true;
				else
					return false;

			}
			if(r2.sv_type == sv_char) return r1.i == r2.ch;
			return false;
		}
		if(r1.sv_type == sv_float)
		{
			if(r2.sv_type == sv_int) return r1.f ==(float) r2.i;
			if(r2.sv_type == sv_float) return (float)r1.f == r2.f;
			if(r2.sv_type == sv_bool)
			{
				if(r2.b && r1.f !=0)
					return true;
				else
					return false;

			}
			if(r2.sv_type == sv_char) return r1.f == r2.ch;
			return false;
		}
		if(r1.sv_type == sv_bool)
		{
			if(r2.sv_type == sv_int)
			{
				if(r1.b && r2.i !=0)
					return true;
				else
					return false;
			}
			if(r2.sv_type == sv_float)
			{
				if(r1.b && r2.f !=0)
					return true;
				else
					return false;
			}
			if(r2.sv_type == sv_bool)
			{
				if(r1.b == r2.b)
					return true;
				else
					return false;

			}
			if(r2.sv_type == sv_char)
			{
				if(r1.b && r2.ch !=0)
					return true;
				else
					return false;
			}
			return false;
		}
		if(r1.sv_type == sv_char)
		{
			if(r2.sv_type == sv_int) return r1.ch ==(float) r2.i;
			if(r2.sv_type == sv_float) return (float)r1.ch == r2.f;
			if(r2.sv_type == sv_bool)
			{
				if(r2.b && r1.ch !=0)
					return true;
				else
					return false;

			}
			if(r2.sv_type == sv_char) return r1.ch == r2.ch;
			return false;
		}
		if(r1.sv_type == sv_str)
		{
			if(r2.sv_type == sv_str)
				if (*r1.m_pStr == *r2.m_pStr )
					return true;
				else
					return false;
			else
				return false;
		}
		return false;
	}
};

class CStringGrid;
class __declspec(dllexport) CComplier
{
public:
	CComplier(void);
public:
	~CComplier(void);

	CStringGrid*	m_pGrid;
	struct tagtab
	{
		CStr		name;
		short		link;
		CStr		obj;
		CStr		typ;
		short		refe;
		boolean		normal;
		short		lev;
		short		adr;
	};

	struct item
	{
		CStr		typ;
		int		refe;
	};

	struct order
	{
		int		f;
		int		x;
		int		y;
	};

	struct tagbtab 
	{
		int		lastadr;
		int		lastptr;
		int		psize;
		int		vsize;
	};

	struct tagatab 
	{
		CStr		inxtyp;
		CStr		eltyp;
		int		elref;
		int		low;
		int		high;
		int		elsize;
		int		size;
	};

	struct tagcols 
	{
		CStr		name;
		CStr		desc;
		int		index;
		int		colid;
		CStr		typ;
	};

	class stack
	{
		var * m_pvar;
		int m_size;
		int m_upperBound;
	public:
		stack()
		{
			m_pvar = NULL;
			m_pvar = new var[8192];
			m_size = 8192;
			m_upperBound = -1;
		};
		~stack()
		{
			if(m_pvar) delete[] m_pvar;
		};
		
		var & operator[ ](int Index) 
		{
			if(Index >=m_size)
			{
				m_size += 8192;
				var * pvar = new var[m_size];
				::memcpy(pvar,m_pvar,sizeof(var)*m_size);
				delete m_pvar;
				m_pvar = pvar;
				m_upperBound = Index;
			}
			if(Index>m_upperBound)m_upperBound = Index;
			return m_pvar[Index];
		};
		var operator[ ](int Index)  const
		{
			return m_pvar[Index];
		};
	};

	struct conrec 
	{
		CStr		tp;
		var		val;
	};

protected:
	CStr	source;
	int		sourcelen;

	int 	cc;
	TCHAR	ch;
	int 	irow;

	CStr	sy;
	float	rnum;
	int	inum;
	CStr	snum;

	//字符串常数
	LPTSTR psconst;
	int isconstpos;
	LPTSTR*	sconst;
	int	sc1,sc2;

	
	//LPTSTR*	symbol;
	//LPTSTR*	key;
	//LPTSTR*	ksy;
	LPTSTR*	sps;
	//LPTSTR*	objs;
	//LPTSTR*	types;
	
	int*	display;

	tagtab*	tab;
	tagatab*	atab;
	tagbtab*	btab;
	LPTSTR*	stab;

	int it,ia,ib,is; 

	order*	code;
	int	lc;

	float*	rconst;
	int	c2,c1;


	LPTSTR	typ;

	tagcols*	cols;

	CArrayMan* m_pArrayMan;

public:
	int of_setsource (LPTSTR as_source);
	void enter (LPTSTR x0, LPTSTR x1, LPTSTR x2, short x3);
	short insymbol ();
	int expression (LPTSTR * &fsys, item &x);
	int standfct (LPTSTR * &fsys, int n, int i);
	int in (LPTSTR aa_args[], int count , LPTSTR value);
	int variableeddeclaration ();
	int loc (LPTSTR as_id);
	int factor (LPTSTR * &fsys, item &x);
	int simplesexpression (LPTSTR * &fsys, item &x);
	LPTSTR resulttype (LPTSTR t1, LPTSTR t2);
	void emit1 (int fct, int b);
	void emit (int fct);
	void emit2 (int fct, int a, int b);
	int term (LPTSTR* & fsys,  item &x);
	int enterreal (float x);
	int interpret (var &value);
	int of_calcexpression (LPTSTR source1,var &value);
	int of_comppcode (LPTSTR source1);
	int of_execpcode ();
	LPTSTR printtable ();
	int  of_enterstr (float x);
	int enterstr (LPTSTR x);
	//int of_setdslist (ref datastore ads_objs[], boolean ab_cols);
	int of_setcols (LPTSTR as_name[],  LPTSTR as_desc[], int al_index[], int al_order[], LPTSTR as_type[]);
	int getdsindex (LPTSTR as_name, int &al_index, int &al_colid, LPTSTR &as_type);
	int constex (LPTSTR fsys[], conrec &c);
	int arraytyp (int &aref, int &arsz);
	int of_dsreset ();
	int construct();
	int constdesc();
private:
	int readscale ();
	void nextch ();
	void  error (LPTSTR as_errcode);
	void adjustscale (int e);

};
