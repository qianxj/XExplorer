#ifndef WPP_POOLSTRING_H
#define WPP_POOLSTRING_H

#define  STR_POOL_SIZE (4096 *256)

namespace wpp  {
namespace how  {

class  CPoolString
{
public:
	CPoolString(void);
	~CPoolString(void);
private:
	wchar_t * pBuffer;
	int used;
	int alloced;
public:
	 wchar_t * GetStringByPos(int hString);
	 int AppendString(const wchar_t * pStr);
	 int SetUsed(int val);
	 int GetUsed();
	 const wchar_t * GetStartString();
	 const wchar_t * GetBuffer();
};

} } // namespace wpp::how

#endif //WPP_POOLSTRING_H