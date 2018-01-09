#ifndef  WPP_RULER_BASE_H
#define  WPP_RULER_BASE_H

#include "vlang3.h"
#include "ruler_path.hpp"
#include "pred.hpp"
#include <vector>
#include <set>
#include <map>
#include <string>
#include "wlexer.hpp"
#include "bitvector.hpp"
#include "werr.hpp"

using namespace wpp::how;

namespace wruler{


//规则语法
class VLANG3_API RulerSym_base
{
public:
	//construct
	RulerSym_base(){initial();}
	void initial(){}
public:
#pragma warning( push )
#pragma warning( disable : 4251 )
	std::vector<CPath> m_Terms;
#pragma warning( pop )
};

struct Token_ref
{
	int token;
	int ref_wtoken;
	int maybeToken;
};

class VLANG3_API RulerEngine_base;
struct VLANG3_API RulerActionParam
{
	std::vector<int>* pStates_;
	std::vector<Token_ref>* pTokens_;
	std::vector<CWord>* pNodes_;
	int first_top;
	int last_top;
	CWord prop_;
	int token;
	bool eval;
	RulerEngine_base * pRulerEngine;
	RulerSym_base* pRulerSym;
	int token_step;
	bool token_reset;
	CWord attach;
	std::vector<Token_ref>* tokens_;
	int token_index;
};

typedef int MoveState_t;
struct MoveState
{
	static const int shift  = 0;
	static const int reduce = 1;
	static const int reduce_reduce_conflict = 2;
	static const int shift_reduce_conflict = 3;
	static const int error = 7;
};

#define decl_action_(id,wclass)		static bool id##(wclass * pAction,CWord &w,RulerActionParam * p);
#define attach_shift_(id, fn,wclass)  attach_shift(id,(fnPathAction)&##wclass##::fn);
#define attach_reduce_(id, fn,wclass) attach_reduce(id,(fnPathAction)&##wclass##::fn);
typedef bool (* fnPathAction)(class RulerAction_base* pAction, CWord &w,RulerActionParam * p) ;
class RulerEngine_base;
class RulerToken_base;
class VLANG3_API RulerAction_base
{
public:
	void attach_reduce(CPath & path,fnPathAction action);
	void attach_shift(CPath & path,fnPathAction action);
public:
	RulerAction_base();
	~RulerAction_base();
public:
	void set_err_ptr(wErr* err_ptr); 
	wErr* get_err_ptr();
private:
	wErr*	err_ptr_;
};

class VLANG3_API RulerToken_base
{
public:
	virtual Token_ref  next_token(RulerActionParam * p) = 0;
	virtual Token_ref  current(RulerActionParam * p) = 0;
	virtual wtoken & get_token(int tokenref, RulerActionParam * p)=0 ;
};

//驱动及执行驱动语义
class VLANG3_API RulerEngine_base
{
	/*
		关于语法分析:
		LL推导方法易于在框架层次上把握推导的层次和推导的进程,而且这样的层次与过程与程序的
	结构比较协调,易于程序的编写、阅读、与调试。
		LL推导的优点在于推导的结构与层次，而细节却不是LL所擅长，并不是说LL不适合于细节的
	推导，比如表达式，而在于细节与总体的要求不同，总体要求的是结构、层次正确，而细节则更
	关注于结果正确，而尽量避免在过细的结构和层次上的纠结。
		LR推导比较适合于机器，LR分析了文法中所有的可用信息，以给于在推导中给出有用的选择，
	LR的程序适合于自动机，不适合于编写推导过程的程序。
		LR主要侧重于推导的结果，推导的结构和过程是隐含的，这样的信息不太利于使用，一个直接
	的后果是不利于错误的恢复。
		在实现时，应该把LL与LR结合起来，才能获得更好的效果，LL侧重于程序的框架和层次，而具
	体细节的实现采用LR方式，具体到语法的编译，名域、类型定义、变量定义、方法定义、句子类别
	、表达式的形式初步确认，通过LL方式实现，具体内容的最终认定则通过LR来分析，也就是说在特
	定的上下文环境下，LL初步确定语法的类型，而LR则最终认定与校验句法成分与具体内容。
		关于语言:
		1、现有语言缺乏一个重要的操作符,Follow操作符(或者叫空格操作符或序列操作符),或者有这个
	操作符号，而没有把它提到应该有的程度。通过Follow操作符，语言的表现方式、表现能力、语义
	能力、扩展性都可以得到比较大的提升。
		2、语言的对组合的解释能力要提高，现有语言通过对象的方法也可以对组合作一定的解释,但这
	样的方式在实际使用时并不直观，也影响了组合的使用和表达，所以从程序语言的本身来说，而是更
	侧重于本体的本身描述与操作以及本体间的结构。
	*/
public:
	static const int token_eof = 0;
	static const int token_option = 1;
	static const int token_error = 2;
public:
	RulerEngine_base();
	~RulerEngine_base();
public:
	void set_err_ptr(wErr* err_ptr){err_ptr_ =  err_ptr;}
	wErr* get_err_ptr(){return err_ptr_;}
private:
	wErr*	err_ptr_;
public:
	struct Ruler
	{
		int lh;	//左项编号
		int rh;	//右项编号
	};

	struct Item
	{
		Ruler r;
		int  indexRulerToken;
		CWord::iterator tr;
		bool simple;
		bool is_option;
		bool reduce;
		bool shift;
		Item(Ruler _r,CWord::iterator _tr,bool _simple):r(_r),tr(_tr),simple(_simple),
			shift(true),reduce(false){
			is_option = false;
			CWord::iterator trRepeat = CWord(tr).begin(pw::sm::turing::repeat);
			if(trRepeat)
			{
				int repeat_val = trRepeat->v.val;
				if(repeat_val=='?'||repeat_val=='*')
					is_option = true;
			}	
		}
		Item(Ruler _r,int _indexRulerToken,CWord::iterator _tr):r(_r),
			indexRulerToken(_indexRulerToken),tr(_tr),
			shift(true),reduce(false){
			is_option = false;
			CWord::iterator trRepeat = CWord(tr).begin(pw::sm::turing::repeat);
			if(trRepeat)
			{
				int repeat_val = trRepeat->v.val;
				if(repeat_val=='?'||repeat_val=='*')
					is_option = true;
			}	
		}
	};
	struct ItemSetState
	{
		int indexRulerToken; /*引入符号*/
		int indexCoreItemSet; /*_rulerCoreItemSet的序号，表示核心项的起始位置*/
		int coreItemSetCount; /*核心集项个数*/
		int nextSateSameToken; /*指向同一引入符号的下一状态*/
		int fromState;
		int reduceCount;
		int indexReduceCoreItemSet;
	};

	struct StateAction
	{
		int nSrcState;
		int nDestState;
	};

	struct StateFollowToken
	{
		std::set<int> indexRulerTokens;
	};
	template<class T>
	struct ListIndex
	{
		int first;
		int last;
		T data;
	};
	template<class T>
	struct ListItem
	{
		int next;
		int piror;
		T data;
	};
#pragma warning( push )
#pragma warning( disable : 4251 )
public:
	//规则符号表
	std::vector<CWord> _rulerTokens;
	//符号归一化表
	std::map<int /* token*/, int /*indexRulertokens*/> _mRulerNonTermination;
	std::map<std::wstring,int /*indexRulertokens*/> _mRulerTermination;
	//规则表
	std::vector<Ruler> _rulers;
	std::map<int /*lh*/,int /*indexRuler*/> _mRulers; 
	//规则细表
	std::vector<Item> _rulerItems;
	std::map<int /*indexRuler*/,int /*nRulerItem*/> _mRulerItems; 
public:
	//工作项引用信息
	std::vector<ListIndex<int>>	  _tokenref;
	std::vector<ListItem<int>> _tokenref_items;
public:
	std::vector<ListItem<int>> _fromStates;
public:
	BitElementVector* ptr_first_set; 
	BitElementVector* ptr_follow_set;
public:	
	//状态表
	std::vector<ItemSetState> _rulerStates;
	std::map<int /*indexRulerToken*/,int /*indexRulerStates*/> _mRulerStates;
	//核心状态集
	std::vector<int /*indexRulerItem*/> _rulerCoreItemSet;
	//状态动作表
	std::vector<StateAction> _rulerActions;
	std::vector<StateFollowToken> _stateFollowToken;
	//状态动作触发表
	std::map<int /*indexRulerToken*/,std::map<int /*indexState*/,int /*indexStateAction*/>> _mRulerActions;
	std::map<int /*indexState*/,int /*indexStateFollowToken*/> _mStateFollowToken;
#pragma warning( pop )
public:
	int ensureRulertoken(CWord::iterator iter);
	/*
	1、创建终结符和非终结符索引及符号数组。
	   _rulerTokens放置符号表，_mRulerNonTermination是非终结符索引，
	   _mRulerTermination是终结符索引。
	2、对所有规则进行编号，同一非终结符的多条规则按顺序放置。
		_rulers放置编号,_mRulers放置索引，。
	3、对规则展开，生成工作项，同一规则的工作项按顺序放置。
	  _rulerItems放置工作项的内容，_mRulerItems放置工作项索引。
	*/
	void initRulerTable(RulerSym_base & r,CPath & path, std::set<int> * as_terminations = 0);
public:
	/*
		计算LR的核心项集结构，需要在调用initRulerTable函数后执行。
		核心项集涉及以下结构:
		_rulerStates 状态表
		_mRulerStates 状态表索引
		_rulerActions 状态转移表
		_mRulerActions 状态转移索引
		_stateFollowToken 状态的后续状态移入符号
		_mStateFollowToken 状态的后续状态移入符号的索引
		_rulerCoreItemSet 状态的核心项集，项序号指向_rulerItems，
						  同一状态的核心项集按顺序放置，起始位置和长度由状态信息指定。
	*/
	void calcCoreItemSet(RulerSym_base & r,CPath path);
	void calcCoreItemSet1(RulerSym_base & r,CPath path);

	//void calcStateClosure(RulerSym_base & r,int indexState);
	void printState();
	void printState(int index,int fromState = -1);
	int conflictState(std::vector<int> & conflictStates,int what=0); 
	int printConflictState(int what=0); 
public:
	int calc_reduce_shift_state(RulerSym_base & r);
	int calcTokenRef(RulerSym_base & r);
public:
	//return count can reduce
	int reduce_count_eat_token(int tk,int state);
	int calc_move_state(int indexToken,int indexState, int & move_state);
	void do_action(RulerSym_base * r,RulerToken_base* rToken,RulerAction_base* action = 0);
public:
	//打印信息
	static void printGrammar(RulerSym_base & r);
	static void printPath(CPath & path);
	static void printPathTerm(CWord & term);
	static void printPathTerm(CWord & term,int nItem);
	static void printTerm(CWord & term);
	static void printTerm(CWord::iterator & iter);
	
	//获取文法多选项
	static CWord::iterator getGrammarSelectIter(CPath & path);
	//获取文法某一多选项中的序列项
	static CWord::iterator getGrammarFollowIter(CWord & term);
	//消除文法左递归
	static void eliminateRecursion(CPath & name, CPath& route);
	static void eliminateRecursion(RulerSym_base & r);
	//公因式提取
	static void extractCommonFactor(RulerSym_base & r);
	//计算First集和Follow集
	void calcFirstSet(RulerSym_base & r);
	void calcFollowSet(RulerSym_base & r);
	//static void calcFirstSet(CPath & path);
	//static void calcFollowSet(CPath & path);
	
	//计算path闭包
	static void calcClosure(RulerSym_base & r,CPath & path);

	//计算可达闭包
	static void calcReachClosure(RulerSym_base & r,CPath & path);

	//优化文法
	static void optimize(RulerSym_base & r);
	//计算终结符
	static int isTerminationTerm(CWord & term);
	static int isTerminationTerm(CWord::iterator & iter);
	static const wchar_t * getTerminationStr(CWord::iterator & iter);
	static CWord calcTerminationTerms(RulerSym_base & r);


	//计算倚赖项
	static wchar_t* getPathID(CWord & w);
	static void calcDependTerms(RulerSym_base & r, CWord & w, std::set<int> &depends);

	//设置动态上下文
	class ruler_dynamic_context_t* dyn_context_;
	void setDynContext(class ruler_dynamic_context_t * p);
	class ruler_dynamic_context_t * getDynContext();

	//static void calcTermClosure(RulerSym_base & r, CWord & w, std::set<int> & except, StateTable & stateTable);

	//获取引导规则
	static CWord getLLRuler(RulerSym_base & r);
		
	//语法编译
	static void parse(RulerSym_base & r, CWord llRuler);

	//解释
	struct interpretation
	{
		template<class Tr, class T0, class T1, class T2,class T3>
		Tr follow(T0 t0,T1 t1,T2 t2,T3 t3);

		template<class Tr, class T0, class T1, class T2>
		Tr follow(T0 t0,T1 t1,T2 t2);

		template<class Tr, class T0, class T1>
		Tr follow(T0 t0,T1 t1);
	};
};

} //namespace wruler

#endif