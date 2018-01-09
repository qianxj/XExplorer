#ifndef WMWPP_H
#define WMWPP_H

//���������
class TVM{};
//���ͽӿ�
class TVMwpp : public TVM{};

//������������ӿ�
template<class HData>
class IVMwpp : HData
{

};

//�������״̬�ӿ�
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
//�����״̬
class DVMwpp
{
	int x;
	int y;
};

//���������
template<
	class T,
	template<class T> class I,
	template<class T> class H, 
	class D
	>
class VMwpp : public I< H <D> >,public T{};

typedef VMwpp<TVMwpp, IVMwpp,HVMwpp,DVMwpp> CVMwpp;


#endif //WMWPP_H