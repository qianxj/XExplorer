#include "scan_tr.hpp"
#include "parser_tr.hpp"
#include <map>
#include <string>
#include <set>
#include <vector>
#include "debuger.hpp"

VLANG3_API CWord getParserRuler();

namespace parse_r{

struct VLANG3_API StateTable
{
#pragma warning( push )
#pragma warning( disable : 4251 )
	std::map<int /*state*/,std::map<int /*item*/,int /*action*/>> m_states;
	std::map<int/*npath*/,int /*nstate*/> m_pathstate;
#pragma warning( pop )
	
	int getKeywordItem(wchar_t* keyword);
	int getOperateItem(int nOp);
	int getPathItem(CWord &w);
	int getPathItem(wchar_t* path);
	int getNumberItem();
	int getIndentItem();
	int getCharItem();
	int getStringItem();
	int getAction(int nState, int nItem);
	void setAction(int nState, int nItem,int action);
	void setPathState(int nItem,int nState);
	void setPathState(wchar_t* sItem,int nState);
	int getPathState(int nItem);
	int procItem(int ostate, int & nstate, CWord::iterator iter);
	const wchar_t* getItemName(int nItem);
	int process(std::vector<int>& items);
	int printItems(std::vector<int>::iterator& begin,std::vector<int>::iterator& end);
};

} //namespace parse