#include "StdAfx.h"
#include "CDispatchDriver.hpp"
#include "CExcelApplication.h"
#include "CExcelWorkbooks.h"
#include "CExcelWorkSheet.h"
#include "CExcelShapes.h"
#include "CExcelShape.h"

CExcelApplication* CExcelApplication::CreateInstance()
{
	CExcelApplication* p = new CExcelApplication();
	p->CreateDispatch(L"excel.application");
	return p;
}

void CExcelApplication::Release(CExcelApplication * p)
{
	if(p) delete p;
}

void CExcelApplication::OpenDocument(const LPTSTR FileName)
{
	CWorkbooks books(get_Workbooks());
	books.Open(FileName);
}

BOOL CExcelApplication::get_Visible()
{
	BOOL result;
	InvokeHelper(0x22e, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void CExcelApplication::put_Visible(BOOL newValue)
{
	static BYTE parms[] = VTS_BOOL ;
	InvokeHelper(0x22e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);

}

LPDISPATCH CExcelApplication::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0x94, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);

	GUID IID__Application = {0x000208d5,0x0000,0x0000,{0xC0,0,0,0,0,0,0,0x46}};
	LPDISPATCH app;
	result->QueryInterface(IID__Application,(void**)&app);
	return app;
}

void CExcelApplication::TransShape()
{
		// TODO: 在此处为应用程序的行为编写代码。
	CWorksheet sheet(get_ActiveSheet());
	CExcelShapes shapes(sheet.get_Shapes());
	long cnt = shapes.get_Count();
	VARIANT v;
	v.vt=VT_UI4;
	for(int i=1;i<=cnt;i++)
	{
		v.uintVal = i;
		CExcelShape shape(shapes.Item(v));
		int left = shape.get_Left();
		int top = shape.get_Top();
		
		//shape.Copy();
		shape.InvokeHelper(0xd5, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
		
		//sheet Paste();
		sheet.InvokeHelper(0xd3, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);

		sheet.put_EnableCalculation(true);
		v.vt=VT_UI4;
		//v.uintVal = i + cnt;
		v.uintVal = cnt + 1;
		CExcelShape newShape(shapes.Item(v));
		newShape.put_Left(left);
		newShape.put_Top(top);
		shape.Delete();
	}
	/*
	for(int i = cnt; i>=1 ; i--)
	{
		v.uintVal = i;
		v.vt=VT_UI4;
		CExcelShape shape(shapes.Item(v));
		shape.Delete();
	}*/
	
}


