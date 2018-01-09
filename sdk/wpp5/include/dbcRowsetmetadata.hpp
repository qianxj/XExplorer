#ifndef WPP_DBCROWSETMETADATA_H
#define WPP_DBCROWSETMETADATA_H

class dbcRowsetMetaData
{
public:
	virtual int  getColumnCount() = 0;
public:
	virtual wstring_t getName(int columnindex) = 0;
	virtual wstring_t getTypeName(int columnindex) = 0;
	virtual int		  getType(int columnindex) = 0;
	virtual int		  getColumnSize(int columnindex) = 0;
	virtual int		  getPrecision(int columnindex) = 0;
	virtual int		  getScale(int columnindex) = 0;
};

#endif //WPP_DBCROWSETMETADATA_H