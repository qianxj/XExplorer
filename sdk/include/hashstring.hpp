#ifndef WPP_HASHSTRING_H
#define WPP_HASHSTRING_H

#include "string_pool.hpp"
#include "vlang3.h"

#include <vector>
#include <deque>

#define  STR_BUCKET_SIZE 4096

namespace wpp {
namespace how {

using namespace std;

struct CHashStringItem
{
	wchar_t * hItem;
	int next;
};

class  VLANG3_API CHashString
{
public:
	CHashString(void);
	~CHashString(void);
private:
	string_pool_t * pool_string_;
	int _bucketsize;
private:
#pragma warning( push )
#pragma warning( disable : 4251 )
	std::deque<CHashStringItem> _bucket;
#pragma warning( pop )
	int _count;
public:
	int get_count();
public:
	int hash_value(const wchar_t* str);
	int hash_value(const wchar_t* str,int size);
public:
	int hash_token(const wchar_t* str,int size);
	int hash_token(const wchar_t* str);
	int find_token(const wchar_t* str,int size);
	int find_token(const wchar_t* str);
public:
	wchar_t* get_string(int nIndex);
};

}}  // namespace wpp::how

#endif //WPP_HASHSTRING_H
