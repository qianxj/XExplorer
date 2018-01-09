#pragma once

#include "xbxbase.h"
#include <map>
#include <string>
#include <vector>
#include <set>
using namespace std;

namespace Hxsoft{ namespace XFrame{
	typedef struct tagShapeMgrItem
	{
		LPTSTR	m_pStrFile;
		LPTSTR	m_pStrUri;
		std::set<std::wstring>  m_setShape;
		tagShapeMgrItem():m_pStrFile(NULL),m_pStrUri(NULL){};
		~tagShapeMgrItem()
		{
			if(m_pStrFile) delete m_pStrFile;
			if(m_pStrUri) delete m_pStrUri;
			m_setShape.clear();
		}
	}TShapeMgrItem;

	class XSHAPE_API XShapeMgr :
		public xbObject
	{
	public:
		XShapeMgr(void);
		~XShapeMgr(void);
	public:
		multimap<std::wstring, TShapeMgrItem *>*   m_pmapShapeMgrItem;
		map<std::wstring, class XShapeObjectSet *>*   m_pmapShapeObjectSet;
	public:
		virtual XShapeObjectSet * GetShapeSet(LPTSTR pStrUri); 
		virtual bool CreateShape(LPTSTR pStrUri, LPTSTR pStrObject,class XShapeObject * &pShape,bool bSetClassName = true);
	public:
		void RegisterShapesByFile(LPTSTR pStrFile );
	};
}}
