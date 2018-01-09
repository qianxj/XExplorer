#ifndef  WPP_RULER_CONTEXT_H
#define  WPP_RULER_CONTEXT_H

#include "ruler_base.hpp"
#include <vector>
using namespace std;

namespace wruler {

//������Ҫʵ���﷨�Զ�̬�ķ�ʽʵ�־�̬չ��������˵�Ǹ�����������
//���ڲ��������ȫ������ʽ�ķ����޷�ʶ���ڴ�Ҳ���Բ������������
//����Ӱ��ԭ�е��﷨��ʽ��

//����������Ŀ����ʹ����ȷ���������淺�������ʹ�������������ǿ��
typedef int state_node_hitem_t;
typedef int state_context_hitem_t;
typedef int path_node_hitem_t;
typedef int state_path_hitem_t;
struct t_state_context{};
struct t_path_node{};
struct t_state_path{};
struct t_state_node{};
struct t_node_tokens{};
struct t_path_childs{};

//״̬������
struct state_context_t
{
	std::vector<int>		states_;
	std::vector<Token_ref>	tokens_;
};

struct path_link_t
{
	int from;
	int to;
};

//path node
struct path_node_t
{	
	path_node_hitem_t  forward;		//ref forward node
	path_node_hitem_t  child_first;	//ref child
	int  child_count;	//count of child
	int reduce_height;
	state_node_hitem_t  state_node;   //ref state node
	
	path_node_t():forward(-1),child_first(0),child_count(0),state_node(-1),
		reduce_height(-1){}
};

//�������е�state_node����state_path
struct state_path_t
{
	path_node_hitem_t root_path_node;
	//ָ����token��state_node
	set<path_node_hitem_t> nodeTokens; 
	state_path_t():root_path_node(-1){}
};


//state node
struct state_node_t
{
	int  state; 
	state_context_hitem_t  state_context;
	path_node_hitem_t  path_node;
	state_path_hitem_t state_path;
	int  token_follow; //ref token set

	state_node_t(): state(-1),state_context(-1),path_node(-1),token_follow(-1),state_path(-1){}
};

struct t_maybe_token {};
struct t_maybe_state {};
struct t_maybe_path  {};

//��̬������
class ruler_dynamic_context_t
{
private:
	//����ڵ�����
	vector<int>	  path_childs;	//not used
	//·���ڵ㼯
	vector<path_node_t>		path_nodes;
	//״̬�ڵ㼯
	vector<state_node_t>	state_nodes;
	//������
	vector<state_context_t>	state_contexts;	
	//״̬·��
	vector<state_path_t>	state_pathes;

public:
	void push(state_context_t &ctx); 
	void push(std::vector<int>& states,
		std::vector<Token_ref>& tokens);
	void push(path_node_t & path_node);
	void push(state_node_t & state_node);
	void push(state_path_t & state_path);

	int  count(t_state_node);
	int  count(t_path_node);
	int  count(t_path_childs);
	int  count(t_node_tokens);
	int  count(t_state_context);
	int	 count(t_state_path);

	path_node_t&     item(t_path_node,	  int index);
	state_node_t&    item(t_state_node,	  int index);
	state_context_t& item(t_state_context,int index);
	int&			 item(t_path_childs,  int index);
	state_path_t&    item(t_state_path,	  int index);

public:
	state_node_hitem_t find_state_node(RulerSym_base & rSym,
		RulerEngine_base& rEngine,
		int state,
		int token_first,
		int token_last,
		vector<int>& states,
		vector<Token_ref>& tokens);

	state_node_hitem_t calc_state_context(RulerSym_base & rSym,
		RulerEngine_base& rEngine,
		int state);

	state_node_hitem_t calc_state_context(RulerSym_base & rSym,
		RulerEngine_base& rEngine,
		int state,
		vector<int>& states,
		vector<Token_ref>& tokens);

	void calc_alones_state_node_tokens(RulerSym_base & rSym,
		RulerEngine_base& rEngine,
		vector<int>&  state_node_alones);
	void calc_alones_state_node_tokens(RulerSym_base & rSym,
		RulerEngine_base& rEngine,
		state_node_hitem_t state_node_alone);

	int find_context_path(RulerSym_base & rSym,
		RulerEngine_base& rEngine,
		int state_node,
		int TokenIndex,
		vector<path_link_t> & items,
		int path_from = -1);
public:
	virtual void * getAction();
	virtual void setAction(void *p);
public:
	virtual int guess(t_maybe_path,RulerToken_base * rToken, RulerActionParam *  param);
	virtual int guess_path(vector<int> &p,	RulerToken_base * rToken, RulerActionParam *  param);
	virtual int ensure(RulerSym_base & rSym,RulerEngine_base& rEngine,int state_node,int TokenIndex,vector<path_link_t> & items);
public:
	virtual int register_maybe(t_maybe_token,int from,int to);
	virtual int register_maybe(t_maybe_state,int from,int to);
public:
	virtual bool maybe(t_maybe_token,int from ,int to,
		RulerToken_base * rToken, RulerActionParam *  param);
	virtual bool maybe(t_maybe_state,int from, int to,
		RulerToken_base * rToken, RulerActionParam *  param);
	virtual bool nomaybe(t_maybe_token,int from ,int to,
		RulerToken_base * rToken, RulerActionParam *  param);
	virtual bool nomaybe(t_maybe_state,int from, int to,
		RulerToken_base * rToken, RulerActionParam *  param);
	virtual bool ensure(t_maybe_token,int from ,int to,
		RulerToken_base * rToken, RulerActionParam *  param);
	virtual bool ensure(t_maybe_state,int from, int to,
		RulerToken_base * rToken, RulerActionParam *  param);
};


} //namespace

#endif