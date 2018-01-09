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


//�����﷨
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

//������ִ����������
class VLANG3_API RulerEngine_base
{
	/*
		�����﷨����:
		LL�Ƶ����������ڿ�ܲ���ϰ����Ƶ��Ĳ�κ��Ƶ��Ľ���,���������Ĳ�������������
	�ṹ�Ƚ�Э��,���ڳ���ı�д���Ķ�������ԡ�
		LL�Ƶ����ŵ������Ƶ��Ľṹ���Σ���ϸ��ȴ����LL���ó���������˵LL���ʺ���ϸ�ڵ�
	�Ƶ���������ʽ��������ϸ���������Ҫ��ͬ������Ҫ����ǽṹ�������ȷ����ϸ�����
	��ע�ڽ����ȷ�������������ڹ�ϸ�Ľṹ�Ͳ���ϵľ��ᡣ
		LR�Ƶ��Ƚ��ʺ��ڻ�����LR�������ķ������еĿ�����Ϣ���Ը������Ƶ��и������õ�ѡ��
	LR�ĳ����ʺ����Զ��������ʺ��ڱ�д�Ƶ����̵ĳ���
		LR��Ҫ�������Ƶ��Ľ�����Ƶ��Ľṹ�͹����������ģ���������Ϣ��̫����ʹ�ã�һ��ֱ��
	�ĺ���ǲ����ڴ���Ļָ���
		��ʵ��ʱ��Ӧ�ð�LL��LR������������ܻ�ø��õ�Ч����LL�����ڳ���Ŀ�ܺͲ�Σ�����
	��ϸ�ڵ�ʵ�ֲ���LR��ʽ�����嵽�﷨�ı��룬�������Ͷ��塢�������塢�������塢�������
	�����ʽ����ʽ����ȷ�ϣ�ͨ��LL��ʽʵ�֣��������ݵ������϶���ͨ��LR��������Ҳ����˵����
	���������Ļ����£�LL����ȷ���﷨�����ͣ���LR�������϶���У��䷨�ɷ���������ݡ�
		��������:
		1����������ȱ��һ����Ҫ�Ĳ�����,Follow������(���߽пո�����������в�����),���������
	�������ţ���û�а����ᵽӦ���еĳ̶ȡ�ͨ��Follow�����������Եı��ַ�ʽ����������������
	��������չ�Զ����Եõ��Ƚϴ��������
		2�����ԵĶ���ϵĽ�������Ҫ��ߣ���������ͨ������ķ���Ҳ���Զ������һ���Ľ���,����
	���ķ�ʽ��ʵ��ʹ��ʱ����ֱ�ۣ�ҲӰ������ϵ�ʹ�úͱ����Դӳ������Եı�����˵�����Ǹ�
	�����ڱ���ı�������������Լ������Ľṹ��
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
		int lh;	//������
		int rh;	//������
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
		int indexRulerToken; /*�������*/
		int indexCoreItemSet; /*_rulerCoreItemSet����ţ���ʾ���������ʼλ��*/
		int coreItemSetCount; /*���ļ������*/
		int nextSateSameToken; /*ָ��ͬһ������ŵ���һ״̬*/
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
	//������ű�
	std::vector<CWord> _rulerTokens;
	//���Ź�һ����
	std::map<int /* token*/, int /*indexRulertokens*/> _mRulerNonTermination;
	std::map<std::wstring,int /*indexRulertokens*/> _mRulerTermination;
	//�����
	std::vector<Ruler> _rulers;
	std::map<int /*lh*/,int /*indexRuler*/> _mRulers; 
	//����ϸ��
	std::vector<Item> _rulerItems;
	std::map<int /*indexRuler*/,int /*nRulerItem*/> _mRulerItems; 
public:
	//������������Ϣ
	std::vector<ListIndex<int>>	  _tokenref;
	std::vector<ListItem<int>> _tokenref_items;
public:
	std::vector<ListItem<int>> _fromStates;
public:
	BitElementVector* ptr_first_set; 
	BitElementVector* ptr_follow_set;
public:	
	//״̬��
	std::vector<ItemSetState> _rulerStates;
	std::map<int /*indexRulerToken*/,int /*indexRulerStates*/> _mRulerStates;
	//����״̬��
	std::vector<int /*indexRulerItem*/> _rulerCoreItemSet;
	//״̬������
	std::vector<StateAction> _rulerActions;
	std::vector<StateFollowToken> _stateFollowToken;
	//״̬����������
	std::map<int /*indexRulerToken*/,std::map<int /*indexState*/,int /*indexStateAction*/>> _mRulerActions;
	std::map<int /*indexState*/,int /*indexStateFollowToken*/> _mStateFollowToken;
#pragma warning( pop )
public:
	int ensureRulertoken(CWord::iterator iter);
	/*
	1�������ս���ͷ��ս���������������顣
	   _rulerTokens���÷��ű�_mRulerNonTermination�Ƿ��ս��������
	   _mRulerTermination���ս��������
	2�������й�����б�ţ�ͬһ���ս���Ķ�������˳����á�
		_rulers���ñ��,_mRulers������������
	3���Թ���չ�������ɹ����ͬһ����Ĺ����˳����á�
	  _rulerItems���ù���������ݣ�_mRulerItems���ù�����������
	*/
	void initRulerTable(RulerSym_base & r,CPath & path, std::set<int> * as_terminations = 0);
public:
	/*
		����LR�ĺ�����ṹ����Ҫ�ڵ���initRulerTable������ִ�С�
		������漰���½ṹ:
		_rulerStates ״̬��
		_mRulerStates ״̬������
		_rulerActions ״̬ת�Ʊ�
		_mRulerActions ״̬ת������
		_stateFollowToken ״̬�ĺ���״̬�������
		_mStateFollowToken ״̬�ĺ���״̬������ŵ�����
		_rulerCoreItemSet ״̬�ĺ�����������ָ��_rulerItems��
						  ͬһ״̬�ĺ������˳����ã���ʼλ�úͳ�����״̬��Ϣָ����
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
	//��ӡ��Ϣ
	static void printGrammar(RulerSym_base & r);
	static void printPath(CPath & path);
	static void printPathTerm(CWord & term);
	static void printPathTerm(CWord & term,int nItem);
	static void printTerm(CWord & term);
	static void printTerm(CWord::iterator & iter);
	
	//��ȡ�ķ���ѡ��
	static CWord::iterator getGrammarSelectIter(CPath & path);
	//��ȡ�ķ�ĳһ��ѡ���е�������
	static CWord::iterator getGrammarFollowIter(CWord & term);
	//�����ķ���ݹ�
	static void eliminateRecursion(CPath & name, CPath& route);
	static void eliminateRecursion(RulerSym_base & r);
	//����ʽ��ȡ
	static void extractCommonFactor(RulerSym_base & r);
	//����First����Follow��
	void calcFirstSet(RulerSym_base & r);
	void calcFollowSet(RulerSym_base & r);
	//static void calcFirstSet(CPath & path);
	//static void calcFollowSet(CPath & path);
	
	//����path�հ�
	static void calcClosure(RulerSym_base & r,CPath & path);

	//����ɴ�հ�
	static void calcReachClosure(RulerSym_base & r,CPath & path);

	//�Ż��ķ�
	static void optimize(RulerSym_base & r);
	//�����ս��
	static int isTerminationTerm(CWord & term);
	static int isTerminationTerm(CWord::iterator & iter);
	static const wchar_t * getTerminationStr(CWord::iterator & iter);
	static CWord calcTerminationTerms(RulerSym_base & r);


	//����������
	static wchar_t* getPathID(CWord & w);
	static void calcDependTerms(RulerSym_base & r, CWord & w, std::set<int> &depends);

	//���ö�̬������
	class ruler_dynamic_context_t* dyn_context_;
	void setDynContext(class ruler_dynamic_context_t * p);
	class ruler_dynamic_context_t * getDynContext();

	//static void calcTermClosure(RulerSym_base & r, CWord & w, std::set<int> & except, StateTable & stateTable);

	//��ȡ��������
	static CWord getLLRuler(RulerSym_base & r);
		
	//�﷨����
	static void parse(RulerSym_base & r, CWord llRuler);

	//����
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