#ifndef WMWPP_H
#define WMWPP_H

//虚拟机类型
class TVM{};
//类型接口
class TVMwpp : public TVM{};

//虚拟机的能力接口
template<class HData>
class IVMwpp : HData
{

};

//虚拟机的状态接口
template <class Data>
class HVMwpp : protected Data
{
	int getX()
	{
		return x;
	}
	void setX(int _x)
	{
		return x = _x;
	}
	int getY()
	{
		return y;
	}
	void setY(int _y)
	{
		return y = _y;
	}
};
//虚拟机状态
class DVMwpp
{
	int x;
	int y;
};

//虚拟机构造
template<
	class T,
	template<class T> class I,
	template<class T> class H, 
	class D
	>
class VMwpp : public I< H <D> >,public T{};

typedef VMwpp<TVMwpp, IVMwpp,HVMwpp,DVMwpp> CVMwpp;


#endif //WMWPP_H