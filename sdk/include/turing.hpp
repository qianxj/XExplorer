#ifndef WPP_TURING_H
#define WPP_TURING_H

#include "turingIterator.hpp"
#include "word.hpp"
#include "windows.h"

namespace wpp  {
namespace how  {

template <class T>
struct Turing
{
public:
	typedef T TuringTape;
public:
	virtual  bool matchPred(TuringTape &tape,int v)
	{
		return false;
	}

	virtual  bool matchValue(TuringTape &tape,int v)
	{
		return false;
	}

	virtual  bool matchString(TuringTape &tape,wchar_t * v)
	{
		return false;
	}

	virtual  bool matchRegex(TuringTape &tape,wchar_t * regex)
	{
		return false;
	}

	virtual bool isTapeEnd(TuringTape &tape)
	{
		return true;
	}

public:
	virtual  bool MyMatch(TuringTape &tape,CWord & t, CWord::iterator iter)
	{
		return false;
	}

	virtual  int MyAction(TuringTape &tape,CWord & t, CWord::iterator & action,int &halt)
	{
		return  1;
	}

public:
	bool m_trace;
private:
	bool _step;
public:
	Turing() : _step(false),m_trace(true),_state(0)
	{

	}

	bool getStep()
	{
		return _step;
	}
	void setStep(bool step)
	{
		_step = step;
	}

private:
	TuringTape* _tapePtr;
	CWord _ruler;
	int _state;
public:
	Turing & setTapePtr(TuringTape* ptr)  {_tapePtr = ptr; return *this;}
	TuringTape* getTapePtr(){return _tapePtr;}
	Turing &  setRuler(const CWord & ruler){_ruler = ruler; return *this;}
	CWord & getRuler()  {return _ruler;}
	Turing &  setState(int state){_state = state; return *this;}
	int  getState()  {return _state;}
public:
	int step()
	{
		return  step(*getTapePtr(),getRuler(),getState());
	}
	int step(TuringTape& tape, CWord & t,int state)
	{
		bool oldStep = _step ;
		_step = true;
		int ret = run(tape,t,state);
		_step = oldStep;
		return ret;
	}
private:
	 int getRepeat(CWord::iterator & iter)
	{
		int rIndex = iter.get_word()->find_pred_index(iter.get_index(),pw::sm::turing::repeat);
		if(rIndex) return iter.get_word()->get_word_item(rIndex)->v.val;
		return 0;
	}


public:
	 int procAction(TuringTape &tape,CWord & t, CWord::iterator & tp,int & halt,bool faultAction=false)
	{
		CWord::iterator action(*tp.get_word(),tp.get_word()->find_pred_index(tp.get_index(),
			faultAction?pw::sm::turing::faultaction : pw::sm::turing::action),0);
		/*CWord::iterator action;
		int p = faultAction?pw::sm::turing::faultaction : pw::sm::turing::action;
		CWord::iterator act = ++tp;
		if(act.get_index())
		{
			int pred = act.get_word()->get_item_pred(act.get_word()->get_word_item(act.get_index()));
			if(pred == p)
				action = act;
			else if(pred == pw::sm::turing::action||pred == pw::sm::turing::faultaction)
			{
				act++;
				if(act.get_index())
				{
					int pred = act.get_word()->get_item_pred(act.get_word()->get_word_item(act.get_index()));
					if(pred == p)
						action = act;
				}
			}
		}*/
		while(action.get_index())
		{
			CWord::iterator actions(CWord(action),action->v.wi.first,0);
			while(actions.get_index())
			{
				int pred = actions.get_word()->get_item_pred(&(*actions));
				int val  = actions->v.val;

				switch(pred)
				{
				case pw::sm::turing::mark:
					tape.mark();
					break;
				case pw::sm::turing::popmark:
					tape.popmark();
					break;
				case pw::sm::turing::step_tomark:
					tape = (TuringTape &)tape.tomark(val);
					break;
				case pw::sm::turing::step_next:
					if(!val)val++;
					while(val){tape++;val--;}
					break;
				case pw::sm::turing::step_piror:
					if(!val)val++;
					while(val){tape--;val--;}
					break;				/*
				case pw::sm::turing::step_first:
					//tape.first();
					break;
				case pw::sm::turing::step_last:
					//tape.last();
					break;
				case pw::sm::turing::step_into:
					//tapes.push(iter);
					//iter = CWord::iterator((*iter).val,0);
					break;
				case pw::sm::turing::step_out:
					//iter = tapes.top();
					//tapes.pop();
					break;
				case pw::sm::turing::step_enter:
					//tapes.push(iter);
					//iter = CWord::iterator(CWord::getPredIndex(iter.get_index(),pw::exp_oprand),pw::exp_oprand);
					break;
				case pw::sm::turing::step_leave:
					//iter = tapes.top();
					//tapes.pop();
					break;
				*/
				case pw::sm::turing::trace:
					printf("%S",actions->v.sval);
					break;
				case pw::sm::turing::debugbreak:
					DebugBreak();
					break;
				case pw::sm::turing::halt:
					halt = true;
					break;
				case pw::sm::turing::tostate:
					setState(val);
					break;
				default:
					MyAction(tape,t,actions,halt);
					break;
				}
				actions++;
			}
			action++;
		}
		return 1;
	}

	 bool procItemEx(TuringTape &tape,CWord & t,CWord::iterator & iter,int state,int & halt)
	{
		if(isTapeEnd(tape))
		{
			int pred = iter.get_word()->get_item_pred(&(*iter));
			//if(pred!=pw::sm::turing::match_any && 
			//	pred!=pw::sm::turing::action )return false;
			return false;
		}

		TuringTape tape1 = tape;

		int  nrepeat = getRepeat(iter);
		bool bmatch = procItem(tape,t,iter,state,halt);
		if(halt) return bmatch;

		if(bmatch)
		{
			if(nrepeat==L'+' || nrepeat==L'*')
				while(procItem(tape,t,iter,state,halt));
			if(nrepeat>>16)
			{
				int low = (nrepeat >> 16)	   - 1;
				int high = (nrepeat & 0xffff ) - 1;
				int count = 1;
				if(low>0)
				{
					while(count < low  && procItem(tape,t,iter,state,halt))count++;
					if(count < low) return false;
				}
				if(high>0)
				{
					while(count <= high  && procItem(tape,t,iter,state,halt))count++;
				}
			}
		}
		//if(!bmatch && nrepeat=='*') bmatch = true;
		if(!bmatch)	tape = tape1;
		return bmatch;
	}

	 bool procItem(TuringTape &tape,CWord & t,CWord::iterator & iter,int state,int & halt)
	{
		bool bmatch = false;

		//判断是否匹配
		wchar_t* ruler =0;
		int pred = iter.get_word()->get_item_pred(&(*iter));
		if(pred== CWord::get_string_pred(L"meta:schema") || 
				pred== CWord::get_string_pred(L"path:id") )
		{
			if(pred== CWord::get_string_pred(L"path:id")) ruler = iter->v.sval;

			iter++;
			if(!&(*iter))return bmatch;
			pred = iter.get_word()->get_item_pred(&(*iter));
		}
		switch(pred)
		{
		/*case pw::sm::turing::action:
			bmatch = true;
			break;*/
		case pw::sm::turing::ruler:
			{
				CWord::iterator r(CWord((*iter).v.wi),(*iter).v.wi.first,0);
				bmatch = procItemEx(tape,t,r,state,halt);
				break;
			}
		case pw::sm::turing::match_pred:
			bmatch = false;
			break;
		case pw::sm::turing::match_value:
			bmatch =  matchValue(tape,(*iter).v.val);
			break;
		case pw::sm::turing::match_word:
		case pw::sm::turing::match_wpred:
			bmatch = false;
			break;
		case pw::sm::turing::match_state:
			bmatch = ( getState()==(*iter).v.val);
			break;
		case pw::sm::turing::match_not:
			{
				CWord::iterator niter(CWord(iter),iter->v.wi.first,0);
				bmatch = !procItemEx(tape,t,niter,state,halt);
			}
			break;
		case pw::sm::turing::match_other:
		case pw::sm::turing::match_any:
			bmatch = true;
			break;
		case pw::sm::turing::match_regex:
			bmatch =  matchRegex(tape,(*iter).v.sval);
			break;
		case pw::sm::turing::match_string:
			bmatch =  matchString(tape,(*iter).v.sval);
			break;
		case pw::sm::turing::match_refer:
			{

				CWord::iterator r(CWord((*iter).v.wi),(*iter).v.wi.first,0);
				bmatch = procItemEx(tape,t,r,state,halt);
				bool bmatchHere = bmatch;
				int repeat = getRepeat(iter);
				while(bmatch && repeat=='*')
				{
					bmatch = procItemEx(tape,t,r,state,halt);
				}
				bmatch = bmatchHere;
			}
			break;
		case pw::sm::turing::match_and:
		case pw::sm::turing::match_follow:
			{
				CWord::iterator niter(CWord(iter),iter->v.wi.first,0);
				bool bmatchHere = false;
				bool bmatchHere1 = false;
				while(niter.get_index())
				{
					CWord::iterator it(CWord(niter),niter->v.wi.first,0);
					bmatchHere1 = procItemEx(tape,t,it,state,halt);
					if(!bmatchHere1)
					{
						if(getRepeat(it)!='*' && getRepeat(it)!='?')
						{
							bmatchHere = bmatchHere1;
							break;
						}
					}else
						bmatchHere = true;
					niter++;
				}
				bmatch = bmatchHere;
			}
			break;
		case pw::sm::turing::match_or:
		case pw::sm::turing::match_select:
			{
				CWord::iterator niter(CWord(iter),iter->v.wi.first,0);
				bool bmatchHere = false;
				while(niter.get_index())
				{
					CWord::iterator it(CWord(niter),niter->v.wi.first,0);
					bmatchHere = procItemEx(tape,t,it,state,halt);
					if(bmatchHere)break;
					niter++;
				}
				bmatch = bmatchHere;
			}
			break;
		default:
			bmatch = MyMatch(tape,t,iter);
			break;
		}

		//匹配则执行相应动作
		if(bmatch)
		{
			//if(ruler)printf("use ruler: %S\r\n",ruler);
			if(ruler)printf("->: %S\r\n",ruler);
			procAction(tape,t,iter,halt);
		}
		else
			procAction(tape,t,iter,halt,true); //action faultaction

		return bmatch;
	}

	 int process(TuringTape &tape, CWord & t,CWord::iterator tp, int state,int & halt)
	{
		CWord::iterator iter = tp;
		while(!isTapeEnd(tape) && !halt)
		{
			bool bmatch = false;
			while(tp != t.end(tp.getPred()) && !bmatch)
			{
				CWord::iterator iter(CWord(tp),tp->v.wi.first,0);
				bmatch = procItemEx(tape,t,iter,state,halt);

				if(halt)break;
				if(!bmatch)
					tp++;
			}
			if(!bmatch)
			{
				if(isTapeEnd(tape)||halt) return 1;
				printf("\r\nnot have a match\r\n");
				tape++;
			}
			tp = iter;
		}
		return 1;
	}

	 int run(TuringTape& tape, CWord & t,int state)
	{
		int halt = 0;
		CWord::iterator tp = t.begin(pw::sm::turing::ruler);
		return process(tape,t,tp,state,halt);
	}
};

}} // namespace wpp::how

#endif //WPP_TURING_H