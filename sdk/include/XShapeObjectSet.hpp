#pragma once

#include "xbxbase.h"
#include <vector>
using namespace std;

namespace Hxsoft{ namespace XFrame{
	class XSHAPE_API XShapeObjectSet :
		public xbObject
	{
	public:
		XShapeObjectSet(void);
		~XShapeObjectSet(void);
	public:
		vector<class XShapeObjectSetItem *> * m_ShapeObjectSetItems;
		int LoadShapeSet(LPTSTR pStrFile);
		int LoadShapeSetXml(LPTSTR pStrXml);
		int LoadShapeSet(IXMLDOMElement * m_pElement);
	public:
		class XShapeObject * Find(LPTSTR strUri,LPTSTR strShape);
	};
}}
