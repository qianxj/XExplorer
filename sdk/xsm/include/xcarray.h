#pragma once

#include <algorithm>

namespace xcode
{
	const int nxcarrayDelta = 4;
	template< typename A, typename D > class xcarrayIterator;
	template< typename A, typename D > class  xcarray
	{
	public:
		struct DataEntry
		{
			A   nEnd;           // start is end of previous entry + 1
			D   aValue;
			DataEntry() {}  //! uninitialized
		};

		xcarray( A nMaxAccess,
			const D& rValue,
			int nDelta = nxcarrayDelta );
		xcarray( A nMaxAccess,
			const D* pDataArray, int nDataCount );
		virtual                     ~xcarray();
		void                        Resize( int nNewSize );
		void                        Reset( const D& rValue );
		void                        SetValue( A nPos, const D& rValue );
		void                        SetValue( A nStart, A nEnd, const D& rValue );
		const D&                    GetValue( A nPos ) const;

		const D&                    GetValue( A nPos, int& nIndex, A& nEnd ) const;

		const D&                    GetValue( A nPos, int& nIndex, A& nStart, A& nEnd ) const;

		const D&                    GetNextValue( int& nIndex, A& nEnd ) const;

		const D&                    GetPrevValue( int& nIndex, A& nStart ) const;
		void                        FillDataArray( A nStart, A nEnd, D * pArray ) const;

		A                           GetLastUnequalAccess( A nStart, const D& rCompare );

		const D&                    Insert( A nStart, int nCount );

		void                        Remove( A nStart, int nCount );

		void                        CopyFrom( const xcarray& rArray,
			A nStart, A nEnd, long nSourceDy = 0 );

		int                      Search( A nPos ) const;
		int                      GetEntryCount() const;
		const DataEntry&            GetDataEntry( int nIndex ) const;

	protected:

		friend class xcarrayIterator<A,D>;

		int                      nCount;
		int                      nLimit;
		int                      nDelta;
		DataEntry*                  pData;
		A                           nMaxAccess;
	};


	template< typename A, typename D >
	void xcarray<A,D>::Reset( const D& rValue )
	{
		// Create a temporary copy in case we got a reference passed that points to
		// a part of the array to be reallocated.
		D aTmpVal( rValue);
		delete[] pData;
		nCount = nLimit = 1;
		pData = new DataEntry[1];
		pData[0].aValue = aTmpVal;
		pData[0].nEnd = nMaxAccess;
	}


	template< typename A, typename D >
	void xcarray<A,D>::SetValue( A nPos, const D& rValue )
	{
		SetValue( nPos, nPos, rValue);
	}


	template< typename A, typename D >
	const D& xcarray<A,D>::GetValue( A nPos ) const
	{
		int nIndex = Search( nPos);
		return pData[nIndex].aValue;
	}


	template< typename A, typename D >
	const D& xcarray<A,D>::GetValue( A nPos, int& nIndex, A& nEnd ) const
	{
		nIndex = Search( nPos);
		nEnd = pData[nIndex].nEnd;
		return pData[nIndex].aValue;
	}


	template< typename A, typename D >
	const D& xcarray<A,D>::GetValue( A nPos, int& nIndex, A& nStart,
		A& nEnd ) const
	{
		nIndex = Search( nPos);
		nStart = (nIndex > 0 ? pData[nIndex-1].nEnd + 1 : 0);
		nEnd = pData[nIndex].nEnd;
		return pData[nIndex].aValue;
	}


	template< typename A, typename D >
	const D& xcarray<A,D>::GetNextValue( int& nIndex, A& nEnd ) const
	{
		if (nIndex < nCount)
			++nIndex;
		int nEntry = (nIndex < nCount ? nIndex : nCount-1);
		nEnd = pData[nEntry].nEnd;
		return pData[nEntry].aValue;
	}


	template< typename A, typename D >
	const D& xcarray<A,D>::GetPrevValue( int& nIndex, A& nStart ) const
	{
		if (nIndex > 0)
			--nIndex;
		nStart = (nIndex > 0 ? pData[nIndex-1].nEnd + 1 : 0);
		return pData[nIndex].aValue;
	}


	template< typename A, typename D >
	int xcarray<A,D>::GetEntryCount() const
	{
		return nCount;
	}


	template< typename A, typename D >
	const typename xcarray<A,D>::DataEntry&
		xcarray<A,D>::GetDataEntry( int nIndex ) const
	{
		return pData[nIndex];
	}

	template< typename A, typename D > class xcarrayIterator
	{
	public:
		xcarrayIterator(
			const xcarray<A,D> & rArray,
			A nStart, A nEnd );
		/// Set new start and end, position on start.
		void                        NewLimits( A nStart, A nEnd );
		A                           GetIterStart() const;
		A                           GetIterEnd() const;
		/// Advance by a single access point (e.g. row).
		bool                        operator ++();
		A                           GetPos() const;
		operator bool() const;
		const D&                    operator *() const;
		/// Advance an entire range, one entry of the array.
		bool                        NextRange();
		A                           GetRangeStart() const;
		A                           GetRangeEnd() const;
		/// Resync to underlying array, calling Search().
		void                        Resync( A nPos );
		template< typename X >
		void                        Follow( const xcarrayIterator<A,X>& );

	private:
		const xcarray<A,D> &  rArray;
		int                          nIndex;
		A                               nIterStart;
		A                               nIterEnd;
		A                               nCurrent;
		bool                            bEnd;
	};


	template< typename A, typename D >
	xcarrayIterator<A,D>::xcarrayIterator(
		const xcarray<A,D> & rArrayP, A nStart, A nEnd )
		: rArray( rArrayP )
		// other values set in NewLimits()
	{
		NewLimits( nStart, nEnd);
	}


	template< typename A, typename D >
	void xcarrayIterator<A,D>::NewLimits( A nStart, A nEnd )
	{
		nIterStart = nStart;
		nIterEnd = nEnd;
		nIndex = rArray.Search( nStart);
		nCurrent = GetRangeStart();
		bEnd = (nIterEnd < nIterStart);
	}


	template< typename A, typename D >
	A xcarrayIterator<A,D>::GetIterStart() const
	{
		return nIterStart;
	}


	template< typename A, typename D >
	A xcarrayIterator<A,D>::GetIterEnd() const
	{
		return nIterEnd;
	}


	template< typename A, typename D >
	bool xcarrayIterator<A,D>::operator++()
	{
		if (nCurrent < GetRangeEnd())
		{
			++nCurrent;
			return true;
		}
		else
			return NextRange();
	}


	template< typename A, typename D >
	A xcarrayIterator<A,D>::GetPos() const
	{
		return nCurrent;
	}


	template< typename A, typename D >
	bool xcarrayIterator<A,D>::NextRange()
	{
		if (!operator bool())
			return false;

		if (rArray.pData[nIndex].nEnd >= nIterEnd)
			bEnd = true;
		else if (++nIndex >= rArray.GetEntryCount())
		{
			nIndex = rArray.GetEntryCount() - 1;
			bEnd = true;
		}
		nCurrent = bEnd ? nIterEnd : GetRangeStart();
		return operator bool();
	}


	template< typename A, typename D >
	xcarrayIterator<A,D>::operator bool() const
	{
		return !bEnd;
	}


	template< typename A, typename D >
	const D& xcarrayIterator<A,D>::operator*() const
	{
		return rArray.pData[nIndex].aValue;
	}


	template< typename A, typename D >
	A xcarrayIterator<A,D>::GetRangeStart() const
	{
		if (nIndex == 0)
			return nIterStart > 0 ? nIterStart : 0;
		else
			return nIterStart > rArray.pData[nIndex-1].nEnd ? nIterStart :
			rArray.pData[nIndex-1].nEnd + 1;
	}


	template< typename A, typename D >
	A xcarrayIterator<A,D>::GetRangeEnd() const
	{
		return nIterEnd < rArray.pData[nIndex].nEnd ? nIterEnd :
			rArray.pData[nIndex].nEnd;
	}


	template< typename A, typename D >
	void xcarrayIterator<A,D>::Resync( A nPos )
	{
		if (nPos < nIterStart)
			nPos = nIterStart;
		else if (nPos > nIterEnd)
			nPos = nIterEnd;
		nCurrent = nPos;
		bEnd = (nIterEnd < nIterStart);
		nIndex = rArray.Search( nPos);
	}


	template< typename A, typename D > class /*XUTIL_API*/ xuSummableCsArray : public xcarray<A,D>
	{
	public:
		xuSummableCsArray( A nMaxAccessP,
			const D& rValue,
			int nDeltaP = nxcarrayDelta )
			: xcarray<A,D>( nMaxAccessP,
			rValue, nDeltaP)
		{}
		xuSummableCsArray( A nMaxAccessP,
			const D* pDataArray, int nDataCount )
			: xcarray<A,D>( nMaxAccessP,
			pDataArray, nDataCount)
		{}

		unsigned long               SumValues( A nStart, A nEnd ) const;

		unsigned long               SumValuesContinuation( A nStart, A nEnd,
			int& nIndex ) const;

		unsigned long               SumScaledValuesContinuation( A nStart, A nEnd,
			int& nIndex, double fScale ) const;

	};

	template< typename A, typename D > class /*XUTIL_API*/ xuBitMaskCsArray : public xcarray<A,D>
	{
	public:
		xuBitMaskCsArray( A nMaxAccessP,
			const D& rValue,
			int nDeltaP = nxcarrayDelta )
			: xcarray<A,D>( nMaxAccessP, rValue, nDeltaP)
		{}
		xuBitMaskCsArray( A nMaxAccessP,
			const D* pDataArray, int nDataCount )
			: xcarray<A,D>( nMaxAccessP,
			pDataArray, nDataCount)
		{}
		void                        AndValue( A nPos, const D& rValueToAnd );
		void                        OrValue( A nPos, const D& rValueToOr );
		void                        AndValue( A nStart, A nEnd, const D& rValueToAnd );
		void                        OrValue( A nStart, A nEnd, const D& rValueToOr );

		void                        CopyFromAnded(
			const xuBitMaskCsArray& rArray,
			A nStart, A nEnd, const D& rValueToAnd,
			long nSourceDy = 0 );

		void                        CopyFromOred(
			const xuBitMaskCsArray& rArray,
			A nStart, A nEnd, const D& rValueToOr,
			long nSourceDy = 0 );

		A                           GetBitStateStart( A nEnd, const D& rBitMask,
			const D& rMaskedCompare ) const;

		A                           GetBitStateEnd( A nStart, const D& rBitMask,
			const D& rMaskedCompare ) const;

		A                           GetFirstForCondition( A nStart, A nEnd,
			const D& rBitMask,
			const D& rMaskedCompare ) const;

		A                           GetLastForCondition( A nStart, A nEnd,
			const D& rBitMask,
			const D& rMaskedCompare ) const;

		A                           CountForCondition( A nStart, A nEnd,
			const D& rBitMask,
			const D& rMaskedCompare ) const;

		bool                        HasCondition( A nStart, A nEnd,
			const D& rBitMask,
			const D& rMaskedCompare ) const;

		int                      FillArrayForCondition( A nStart, A nEnd,
			const D& rBitMask,
			const D& rMaskedCompare,
			A * pArray, int nArraySize ) const;

		A                           CountForAnyBitCondition( A nStart, A nEnd,
			const D& rBitMask ) const;

		A                           GetLastAnyBitAccess( A nStart,
			const D& rBitMask ) const;

		template< typename S >
		unsigned long               SumCoupledArrayForCondition( A nStart, A nEnd,
			const D& rBitMask, const D& rMaskedCompare,
			const xuSummableCsArray<A,S>& rArray ) const;

		template< typename S >
		unsigned long               SumScaledCoupledArrayForCondition( A nStart, A nEnd,
			const D& rBitMask, const D& rMaskedCompare,
			const xuSummableCsArray<A,S>& rArray,
			double fScale ) const;
	};


	template< typename A, typename D >
	void xuBitMaskCsArray<A,D>::AndValue( A nPos, const D& rValueToAnd )
	{
		const D& rValue = GetValue( nPos);
		if ((rValue & rValueToAnd) != rValue)
			SetValue( nPos, rValue & rValueToAnd);
	}


	template< typename A, typename D >
	void xuBitMaskCsArray<A,D>::OrValue( A nPos, const D& rValueToOr )
	{
		const D& rValue = GetValue( nPos);
		if ((rValue | rValueToOr) != rValue)
			SetValue( nPos, rValue | rValueToOr);
	}



	template< typename A, typename D, typename S >
	class xuCoupledCsArrayIterator
	{
	public:
		~xuCoupledCsArrayIterator();
		xuCoupledCsArrayIterator(
			const xuBitMaskCsArray<A,D> & rArray1,
			A nStart, A nEnd,
			const D& rBitMask,
			const D& rMaskedCompare,
			const xcarray<A,S> & rArray2 );
		void                        NewLimits( A nStart, A nEnd );
		A                           GetIterStart() const;
		A                           GetIterEnd() const;
		bool                        operator ++();
		A                           GetPos() const;
		operator bool() const;
		const S&                    operator *() const;
		bool                        NextRange();
		A                           GetRangeStart() const;
		A                           GetRangeEnd() const;
		void                        Resync( A nPos );

	private:
		//xcarrayIterator<A,D>  aIter1;
		//xcarrayIterator<A,S>  aIter2;
		xcarrayIterator<A,D>*  pIter1;
		xcarrayIterator<A,S>*  pIter2;
		const D&                        rBitMask;
		const D&                        rMaskedCompare;

		void                            InitLimits();
	};


	template< typename A, typename D, typename S >
	A xuCoupledCsArrayIterator<A,D,S>::GetIterStart() const
	{
		return pIter1->GetIterStart();
	}


	template< typename A, typename D, typename S >
	A xuCoupledCsArrayIterator<A,D,S>::GetIterEnd() const
	{
		return pIter1->GetIterEnd();
	}


	template< typename A, typename D, typename S >
	xuCoupledCsArrayIterator<A,D,S>::operator bool() const
	{
		return (*pIter1) && (*pIter2);
	}


	template< typename A, typename D, typename S >
	const S& xuCoupledCsArrayIterator<A,D,S>::operator*() const
	{
		return *(*pIter2);
	}


	template< typename A, typename D, typename S >
	bool xuCoupledCsArrayIterator<A,D,S>::operator ++()
	{
		if (pIter1->GetPos() < pIter1->GetRangeEnd())
		{
			++(*pIter1);
			++(*pIter2);
			return operator bool();
		}
		else
			return NextRange();
	}


	template< typename A, typename D, typename S >
	A xuCoupledCsArrayIterator<A,D,S>::GetPos() const
	{
		return pIter2->GetPos();
	}


	template< typename A, typename D, typename S >
	A xuCoupledCsArrayIterator<A,D,S>::GetRangeStart() const
	{
		return ::std::max(pIter1->GetRangeStart(), pIter2->GetRangeStart());
	}


	template< typename A, typename D, typename S >
	A xuCoupledCsArrayIterator<A,D,S>::GetRangeEnd() const
	{
		return ::std::min( pIter1->GetRangeEnd(), pIter2->GetRangeEnd());
	}
}
