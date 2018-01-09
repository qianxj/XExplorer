#include "xvmdef.h"

namespace xcode
{
	#define ARRAYINC 255
	class xstring
	{
	public:
		xstring()
		{
			m_iStrlen = 0;
			m_iSize = 0;
			m_pData = NULL;
		};
		xstring(lptstr str)
		{
			int strlen;
			int	alloclen; 
			strlen = (int)::_tcslen(str);
			if((strlen + 1) % ARRAYINC ==0)
				alloclen = (strlen + 1) / ARRAYINC ;
			else
				alloclen = (strlen + 1) / ARRAYINC  + ARRAYINC;

			m_pData = new tchar[alloclen];
			::_tcscpy_s(m_pData,alloclen,str);
			m_iStrlen = strlen;
			m_iSize = alloclen;
		};

		xstring(tchar ch)
		{
			int strlen;
			int	alloclen; 
			strlen = 1;
			if((strlen + 1) % ARRAYINC ==0)
				alloclen = (strlen + 1) / ARRAYINC ;
			else
				alloclen = (strlen + 1) / ARRAYINC  + ARRAYINC;

			m_pData = new tchar[alloclen];
			m_pData[0] = ch;
			m_pData[1]='\0';
			m_iStrlen = strlen;
			m_iSize = alloclen;
		};

		~xstring()
		{
			if(m_pData)delete m_pData;
		};

		xstring & operator=( const lptstr str )
		{
			if(!str)
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

			int strlen;
			int	alloclen; 
			if(str)
			{
				strlen = (int)::_tcslen(str);
				if((strlen + 1) % ARRAYINC ==0)
					alloclen = (strlen + 1) / ARRAYINC ;
				else
					alloclen = (strlen + 1) / ARRAYINC  + ARRAYINC;
				if(m_pData)
				{
					if(m_iSize >= alloclen && m_iSize - alloclen <=ARRAYINC) //fit to use old buf
					{
						::_tcscpy_s(m_pData,m_iSize,str);
						m_iStrlen = strlen;
						return( *this );
					}
					else
						delete m_pData;
				}
				m_pData = new tchar[alloclen];
				::_tcscpy_s(m_pData,alloclen,str);
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

		xstring & operator=( const int value ) 
		{
			tchar buf[30];
			_itot_s(value,buf,30,10);
			return operator=( buf );
		}

		xstring & operator=( const tchar ch )
		{
			tchar str[2];
			str[0] = ch;
			str[1]='\0';
			xstring::operator =( str);
			return( *this );
		}

		xstring & operator=( const xstring & str )
		{
			xstring::operator =( str.m_pData);
			return( *this );
		}

		xstring & operator+=( const lptstr str )
		{
			int strlen;
			int	alloclen; 
			lptstr pStr;
			if(str==NULL)return (*this);
			strlen = (int)::_tcslen(str);
			strlen += this->m_iStrlen ;
			if(strlen + 1 > this->m_iSize)
			{
				if((strlen + 1) % ARRAYINC ==0)
					alloclen = (strlen + 1) / ARRAYINC * ARRAYINC;
				else
					alloclen = (strlen + 1) / ARRAYINC * ARRAYINC + ARRAYINC;
				pStr = new tchar[alloclen];
				this->m_iSize = alloclen;
				if(m_pData)	::_tcscpy_s(pStr,alloclen,m_pData);
				delete m_pData;
				m_pData = pStr;
			}

			if(str)
			{
				::_tcscpy_s(m_pData + m_iStrlen,_tcslen(str) +1,str);
				m_iStrlen += (int)::_tcslen(str);
			}
			return( *this );
		}

		xstring & operator+=( const tchar ch )
		{
			tchar str[2];
			str[0] = ch;
			str[1]='\0';
			xstring::operator +=( str);
			return( *this );
		}

		xstring & operator+=( const xstring & str )
		{
			xstring::operator +=( str.m_pData);
			return( *this );
		}

		xstring & operator+( const lptstr str )
		{
			xstring *  pStr = new xstring();
			*pStr +=*this;
			*pStr +=(str);
			return *pStr;
		}

		xstring & operator+( const tchar ch )
		{
			xstring *  pStr = new xstring();
			*pStr +=*this;
			*pStr +=(ch);
			return *pStr;
		}

		xstring & operator +( const xstring & str )
		{
			xstring *  pStr = new xstring();
			*pStr +=*this;
			*pStr +=(str);
			return *pStr;
		}

		operator lptstr() const
		{
			return (m_pData);
		}

		friend bool operator==( const xstring & str1 ,const xstring &str2 )
		{
			int result;
			result = ::lstrcmp((lptstr)str1,(lptstr)str2);
			if (result==0) return TRUE;
			return FALSE;
		};

		friend bool operator==( const xstring & str1 ,const lptstr str2 )
		{
			int result;
			result = ::lstrcmp((lptstr)str1,str2);
			if (result==0) return TRUE;
			return FALSE;
		};

		friend bool operator==( const lptstr str1 ,const xstring & str2 )
		{
			int result;
			result = ::lstrcmp(str1,(lptstr)str2);
			if (result==0) return TRUE;
			return FALSE;
		};

		friend bool operator!=( const xstring & str1 ,const xstring &str2 )
		{
			return !(str1==str2);
		};

		friend bool operator!=( const xstring & str1 ,const lptstr str2 )
		{
			return !(str1==str2);
		};

		friend bool operator!=( const lptstr str1 ,const xstring & str2 )
		{
			return !(str1==str2);
		};

		friend bool operator>=( const xstring & str1 ,const xstring str2 )
		{
			int result;
			result = ::lstrcmp((lptstr)str1,(lptstr)str2);
			if (result>=0) return TRUE;
			return FALSE;
		};

		friend bool operator>=( const xstring & str1 ,const lptstr str2 )
		{
			int result;
			result = ::lstrcmp((lptstr)str1,str2);
			if (result>=0) return TRUE;
			return FALSE;
		};
		friend bool operator>=( const lptstr str1 ,const xstring & str2 )
		{
			int result;
			result = ::lstrcmp(str1,(lptstr)str2);
			if (result >=0) return TRUE;
			return FALSE;
		};

		friend bool operator>( const xstring & str1 ,const xstring str2 )
		{
			int result;
			result = ::lstrcmp((lptstr)str1,(lptstr)str2);
			if (result>0) return TRUE;
			return FALSE;
		};
		friend bool operator>( const lptstr str1 ,const xstring & str2 )
		{
			int result;
			result = ::lstrcmp(str1,(lptstr)str2);
			if (result >0) return TRUE;
			return FALSE;
		};

		friend bool operator>( const xstring & str1 ,const lptstr  str2 )
		{
			int result;
			result = ::lstrcmp((lptstr)str1,str2);
			if (result >0) return TRUE;
			return FALSE;
		};


		friend bool operator<=( const xstring & str1 ,const xstring str2 )
		{
			int result;
			result = ::lstrcmp((lptstr)str1,(lptstr)str2);
			if (result<=0) return TRUE;
			return FALSE;
		};
		friend bool operator<=( const xstring & str1 ,const lptstr  str2 )
		{
			int result;
			result = ::lstrcmp((lptstr)str1,str2);
			if (result <=0) return TRUE;
			return FALSE;
		};
		friend bool operator<=( const lptstr str1 ,const xstring & str2 )
		{
			int result;
			result = ::lstrcmp(str1,(lptstr)str2);
			if (result <=0) return TRUE;
			return FALSE;
		};

		friend bool operator<( const xstring & str1 ,const xstring str2 )
		{
			int result;
			result = ::lstrcmp((lptstr)str1,(lptstr)str2);
			if (result<0) return TRUE;
			return FALSE;
		};
		friend bool operator<( const xstring & str1 ,const lptstr  str2 )
		{
			int result;
			result = ::lstrcmp((lptstr)str1,str2);
			if (result < 0) return TRUE;
			return FALSE;
		};
		friend bool operator<( const lptstr str1 ,const xstring & str2 )
		{
			int result;
			result = ::lstrcmp(str1,(lptstr)str2);
			if (result <0) return TRUE;
			return FALSE;
		};


		xstring&  makeLower()
		{
			::_tcslwr_s(m_pData,m_iSize);
			return (*this);
		}

		xstring& makeUpper()
		{
			::_tcsupr_s(m_pData,m_iSize);
			return (*this);
		}

		friend xstring & upper(const xstring & str)
		{
			xstring*  pStr = new xstring(str);
			::_tcsupr_s(pStr->m_pData,_tcslen(pStr->m_pData) + 1);
			return *pStr;
		}
		friend xstring & lower(const xstring & str)
		{
			xstring*  pStr = new xstring(str);
			::_tcslwr_s(pStr->m_pData,_tcslen(pStr->m_pData) + 1);
			return *pStr;
		}

		tchar * m_pData;
		int m_iStrlen;
		int m_iSize;
	};
}