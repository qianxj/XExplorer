// �����Ϳ������á�����ࡱ�����ļ�������ɵ� IDispatch ��װ��

//#import "C:\\Program Files (x86)\\Microsoft Office\\Office14\\EXCEL.EXE" no_namespace
// CWorkbooks ��װ��

class CWorkbooks : public CDispatchDriver
{
public:
	CWorkbooks(){} // ���� CDispatchDriver Ĭ�Ϲ��캯��
	CWorkbooks(LPDISPATCH pDispatch) : CDispatchDriver(pDispatch) {}
	CWorkbooks(const CWorkbooks& dispatchSrc) : CDispatchDriver(dispatchSrc) {}

	// ����
public:

	// ����
public:


	// IPivotItemList ����
public:
	/*Application get_Application()
	{
		Application result;
		InvokeHelper(0x94, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	XlCreator get_Creator()
	{
		XlCreator result;
		InvokeHelper(0x95, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}*/
	LPDISPATCH get_Parent()
	{
		LPDISPATCH result;
		InvokeHelper(0x96, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	long get_Count()
	{
		long result;
		InvokeHelper(0x76, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	/*STDMETHOD(Item)(VARIANT& Index, PivotItem * * RHS)
	{
		HRESULT result;
		static BYTE parms[] = VTS_VARIANT VTS_PDISPATCH ;
		InvokeHelper(0xaa, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, &Index, RHS);
		return result;
	}
	PivotItem get__Default(VARIANT& Field)
	{
		PivotItem result;
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms, &Field);
		return result;
	}*/
	STDMETHOD(_NewEnum)(LPUNKNOWN * RHS)
	{
		HRESULT result;
		static BYTE parms[] = VTS_PUNKNOWN ;
		InvokeHelper(0xfffffffc, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, RHS);
		return result;
	}
	
	STDMETHOD(Open)(LPCTSTR fileName)
	{
		HRESULT result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x783, DISPATCH_METHOD, VT_HRESULT, (void*)&result, parms, fileName);
		return result;
	}


	// IPivotItemList ����
public:

};
