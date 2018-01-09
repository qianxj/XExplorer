#pragma once

namespace Hxsoft{ namespace XUtil
{
	enum TSPAQuantum {SPASmall, SPALarge};   //{ Section size }

	template<class T>
	class XUTIL_API xuSpArray
	{
	public:
		xuSpArray(void);
		virtual ~xuSpArray(void);

	public:
		void Create(TSPAQuantum Quantum = SPALarge);
		void Clear();

	private:
		typedef  T*	TSlot;
		TSlot* TSecDir;

		int	NewHighBound;
		WORD	indexMask, secShift;
		WORD	FSectionSize;
		int		FHighBound;
		int		FSecDirSize;
	public:
		const T GetAt(int Index) const;
		T GetAt(int Index);
		T&  MakeAt(int Index);
		void PutAt(int Index, T Item);
		void ResetHighBound();
		inline int  GetHighBound(){return FHighBound; };
		inline WORD GetSectionSize(){return FSectionSize;};

		inline const T   operator[ ](int Index) const
		{
			return GetAt(Index);
		};
		inline T & operator[ ](int Index)
		{
			return MakeAt(Index);
		};
	};

	template<class T>
	class XUTIL_API xuSpList
	{
	public:
		xuSpList(); 
		virtual ~xuSpList(); 

	private:
		xuSpArray<T>  FList;
		TSPAQuantum FQuantum;
		void NewList(TSPAQuantum Quantum);
	public:
		void Error() const; //virtual;
		const T Get(int Index) const;
		T Get(int Index);
		void Put(int Index, T Item);
	public:
		void Create(TSPAQuantum Quantum);
		inline void Clear() {FList.Clear();};
		void Delete(int Index);
		void Exchange(int Index1, int Index2);
		void Insert(int Index, T Item);
		void Move(int CurIndex, int NewIndex);
		int  GetCount(){return GetHighBound() + 1;};
		int	 GetHighBound(){return FList.GetHighBound();};

		inline const T   operator[ ](int Index) const {return FList[Index];};
		inline T &   operator[ ](int Index){return FList[Index];};
	};
}}
