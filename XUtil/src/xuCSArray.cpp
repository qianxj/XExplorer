#include "stdafx.h"
#pragma push_macro("max")
#pragma push_macro("min")
#undef max
#undef min

#include "xucsarray.hpp"
#include <limits>

namespace Hxsoft{ namespace XUtil
{
	template< typename A, typename D >
	xuCsArray<A,D>::xuCsArray( A nMaxAccessP, const D& rValue,
		int nDeltaP )
		: nCount(1)
		, nLimit(1)
		, nDelta( nDeltaP > 0 ? nDeltaP : 1)
		, pData( new DataEntry[1])
		, nMaxAccess( nMaxAccessP)
	{
		pData[0].aValue = rValue;
		pData[0].nEnd = nMaxAccess; 
	}


	template< typename A, typename D >
	xuCsArray<A,D>::xuCsArray( A nMaxAccessP, const D* pDataArray,
		int nDataCount )
		: nCount(0)
		, nLimit( nDataCount)
		, nDelta( nxuCsArrayDelta)
		, pData( new DataEntry[nDataCount])
		, nMaxAccess( nMaxAccessP)
	{
		D aValue = pDataArray[0];
		for (int j=0; j<nDataCount; ++j)
		{
			if (!(aValue == pDataArray[j]))
			{
				pData[nCount].aValue = aValue;
				pData[nCount].nEnd = j-1;
				++nCount;
				aValue = pDataArray[j];
			}
		}
		pData[nCount].aValue = aValue;
		pData[nCount].nEnd = nMaxAccess;
		++nCount;
		Resize( nCount);
	}


	template< typename A, typename D >
	xuCsArray<A,D>::~xuCsArray()
	{
		delete[] pData;
	}


	template< typename A, typename D >
	void xuCsArray<A,D>::Resize( int nNewLimit)
	{
		if ((nCount <= nNewLimit && nNewLimit < nLimit) || nLimit < nNewLimit)
		{
			nLimit = nNewLimit;
			DataEntry* pNewData = new DataEntry[nLimit];
			memcpy( pNewData, pData, nCount*sizeof(DataEntry));
			delete[] pData;
			pData = pNewData;
		}
	}


	template< typename A, typename D >
	int xuCsArray<A,D>::Search( A nAccess ) const
	{
		if (nAccess == 0)
			return 0;

		long nLo    = 0;
		long nHi    = static_cast<long>(nCount) - 1;
		long nStart = 0;
		long nEnd   = 0;
		long i      = 0;
		bool bFound = (nCount == 1);
		while (!bFound && nLo <= nHi)
		{
			i = (nLo + nHi) / 2;
			if (i > 0)
				nStart = (long) pData[i - 1].nEnd;
			else
				nStart = -1;
			nEnd = (long) pData[i].nEnd;
			if (nEnd < (long) nAccess)
				nLo = ++i;
			else
				if (nStart >= (long) nAccess)
					nHi = --i;
				else
					bFound = true;
		}
		return (bFound ? static_cast<int>(i) : (nAccess < 0 ? 0 : nCount-1));
	}


	template< typename A, typename D >
	void xuCsArray<A,D>::SetValue( A nStart, A nEnd, const D& rValue )
	{
		if (0 <= nStart && nStart <= nMaxAccess && 0 <= nEnd && nEnd <= nMaxAccess
			&& nStart <= nEnd)
		{
			if ((nStart == 0) && (nEnd == nMaxAccess))
				Reset( rValue);
			else
			{
				// Create a temporary copy in case we got a reference passed that
				// points to a part of the array to be reallocated.
				D aNewVal( rValue);
				int nNeeded = nCount + 2;
				if (nLimit < nNeeded)
				{
					nLimit += nDelta;
					if (nLimit < nNeeded)
						nLimit = nNeeded;
					DataEntry* pNewData = new DataEntry[nLimit];
					memcpy( pNewData, pData, nCount*sizeof(DataEntry));
					delete[] pData;
					pData = pNewData;
				}

				int ni;          // number of leading entries
				int nInsert;     // insert position (nMaxAccess+1 := no insert)
				bool bCombined = false;
				bool bSplit = false;
				if (nStart > 0)
				{
					// skip leading
					ni = Search( nStart);

					nInsert = nMaxAccess+1;
					if (!(pData[ni].aValue == aNewVal))
					{
						if (ni == 0 || (pData[ni-1].nEnd < nStart - 1))
						{   // may be a split or a simple insert or just a shrink,
							// row adjustment is done further down
							if (pData[ni].nEnd > nEnd)
								bSplit = true;
							ni++;
							nInsert = ni;
						}
						else if (ni > 0 && pData[ni-1].nEnd == nStart - 1)
							nInsert = ni;
					}
					if (ni > 0 && pData[ni-1].aValue == aNewVal)
					{   // combine
						pData[ni-1].nEnd = nEnd;
						nInsert = nMaxAccess+1;
						bCombined = true;
					}
				}
				else
				{
					nInsert = 0;
					ni = 0;
				}

				int nj = ni;     // stop position of range to replace
				while (nj < nCount && pData[nj].nEnd <= nEnd)
					nj++;
				if (!bSplit)
				{
					if (nj < nCount && pData[nj].aValue == aNewVal)
					{   // combine
						if (ni > 0)
						{
							if (pData[ni-1].aValue == aNewVal)
							{   // adjacent entries
								pData[ni-1].nEnd = pData[nj].nEnd;
								nj++;
							}
							else if (ni == nInsert)
								pData[ni-1].nEnd = nStart - 1;   // shrink
						}
						nInsert = nMaxAccess+1;
						bCombined = true;
					}
					else if (ni > 0 && ni == nInsert)
						pData[ni-1].nEnd = nStart - 1;   // shrink
				}
				if (ni < nj)
				{   // remove middle entries
					if (!bCombined)
					{   // replace one entry
						pData[ni].nEnd = nEnd;
						pData[ni].aValue = aNewVal;
						ni++;
						nInsert = nMaxAccess+1;
					}
					if (ni < nj)
					{   // remove entries
						memmove( pData + ni, pData + nj,
							(nCount - nj) * sizeof(DataEntry));
						nCount -= nj - ni;
					}
				}

				if (nInsert < static_cast<int>(nMaxAccess+1))
				{   // insert or append new entry
					if (nInsert <= nCount)
					{
						if (!bSplit)
							memmove( pData + nInsert + 1, pData + nInsert,
							(nCount - nInsert) * sizeof(DataEntry));
						else
						{
							memmove( pData + nInsert + 2, pData + nInsert,
								(nCount - nInsert) * sizeof(DataEntry));
							pData[nInsert+1] = pData[nInsert-1];
							nCount++;
						}
					}
					if (nInsert)
						pData[nInsert-1].nEnd = nStart - 1;
					pData[nInsert].nEnd = nEnd;
					pData[nInsert].aValue = aNewVal;
					nCount++;
				}
			}
		}
	}


	template< typename A, typename D >
	void xuCsArray<A,D>::CopyFrom( const xuCsArray<A,D>& rArray, A nStart,
		A nEnd, long nSourceDy )
	{
		int nIndex;
		A nRegionEnd;
		for (A j=nStart; j<=nEnd; ++j)
		{
			const D& rValue = (j==nStart ?
				rArray.GetValue( j+nSourceDy, nIndex, nRegionEnd) :
			rArray.GetNextValue( nIndex, nRegionEnd));
			nRegionEnd -= nSourceDy;
			if (nRegionEnd > nEnd)
				nRegionEnd = nEnd;
			SetValue( j, nRegionEnd, rValue);
			j = nRegionEnd;
		}
	}


	template< typename A, typename D >
	const D& xuCsArray<A,D>::Insert( A nStart, int nAccessCount )
	{
		int nIndex = Search( nStart);
		// No real insertion is needed, simply extend the one entry and adapt all
		// following. In case nStart points to the start row of an entry, extend
		// the previous entry (inserting before nStart).
		if (nIndex > 0 && pData[nIndex-1].nEnd+1 == nStart)
			--nIndex;
		const D& rValue = pData[nIndex].aValue; // the value "copied"
		do
		{
			pData[nIndex].nEnd += nAccessCount;
			if (pData[nIndex].nEnd >= nMaxAccess)
			{
				pData[nIndex].nEnd = nMaxAccess;
				nCount = nIndex + 1;    // discard trailing entries
			}
		} while (++nIndex < nCount);
		return rValue;
	}


	template< typename A, typename D >
	void xuCsArray<A,D>::Remove( A nStart, int nAccessCount )
	{
		A nEnd = nStart + nAccessCount - 1;
		int nIndex = Search( nStart);
		if (nEnd > pData[nIndex].nEnd)
			SetValue( nStart, nEnd, pData[nIndex].aValue);
		if ((nStart == 0 || (nIndex > 0 && nStart == pData[nIndex-1].nEnd+1)) &&
			pData[nIndex].nEnd == nEnd && nIndex < nCount-1)
		{
			int nRemove;
			if (pData[nIndex-1].aValue == pData[nIndex+1].aValue)
			{
				nRemove = 2;
				--nIndex;
			}
			else
				nRemove = 1;
			memmove( pData + nIndex, pData + nIndex + nRemove, (nCount - (nIndex +
				nRemove)) * sizeof(DataEntry));
			nCount -= nRemove;
		}
		do
		{
			pData[nIndex].nEnd -= nAccessCount;
		} while (++nIndex < nCount);
		pData[nCount-1].nEnd = nMaxAccess;
	}


	template< typename A, typename D >
	A xuCsArray<A,D>::GetLastUnequalAccess( A nStart, const D& rCompare )
	{
		A nEnd = ::std::numeric_limits<A>::max();
		int nIndex = nCount-1;
		while (1)
		{
			if (pData[nIndex].aValue != rCompare)
			{
				nEnd = pData[nIndex].nEnd;
				break; 
			}
			else
			{
				if (nIndex > 0)
				{
					--nIndex;
					if (pData[nIndex].nEnd < nStart)
						break;  
				}
				else
					break;  
			}
		}
		return nEnd;
	}


	template< typename A, typename D >
	void xuCsArray<A,D>::FillDataArray( A nStart, A nEnd, D * pArray ) const
	{
		int nUsed = 0;
		int nIndex = Search( nStart);
		A nS = ::std::max( (nIndex>0 ? pData[nIndex-1].nEnd+1 : 0), nStart);
		do
		{
			A nE = ::std::min( pData[nIndex].nEnd, nEnd);
			while (nS <= nE)
			{
				pArray[nUsed++] = pData[nIndex].aValue;
				++nS;
			}
			if (pData[nIndex].nEnd >= nEnd)
				break;  
			++nIndex;
		} while (nIndex < nCount);
	}


	template< typename A, typename D >
	unsigned long xuSummableCsArray<A,D>::SumValues( A nStart, A nEnd ) const
	{
		int nIndex = Search( nStart);
		unsigned long nSum = SumValuesContinuation( nStart, nEnd, nIndex);
		if (nEnd > this->nMaxAccess)
			nSum += this->pData[this->nCount-1].aValue * (nEnd - this->nMaxAccess);
		return nSum;
	}


	template< typename A, typename D >
	unsigned long xuSummableCsArray<A,D>::SumValuesContinuation(
		A nStart, A nEnd, int& nIndex ) const
	{
		unsigned long nSum = 0;
		A nS = nStart;
		while (nIndex < this->nCount && nS <= nEnd)
		{
			A nE = ::std::min( this->pData[nIndex].nEnd, nEnd);
			unsigned long nNew = (unsigned long) this->pData[nIndex].aValue * (nE - nS + 1);
			nSum += nNew;
			if (nSum < nNew)
				return ::std::numeric_limits<unsigned long>::max();
			nS = nE + 1;
			if (nS <= nEnd)
				++nIndex;
		}
		return nSum;
	}


	template< typename A, typename D >
	unsigned long xuSummableCsArray<A,D>::SumScaledValuesContinuation(
		A nStart, A nEnd, int& nIndex, double fScale ) const
	{
		unsigned long nSum = 0;
		A nS = nStart;
		while (nIndex < this->nCount && nS <= nEnd)
		{
			A nE = ::std::min( this->pData[nIndex].nEnd, nEnd);
			unsigned long nScaledVal = (unsigned long) (this->pData[nIndex].aValue * fScale);
			unsigned long nNew = nScaledVal * (nE - nS + 1);
			nSum += nNew;
			if (nSum < nNew)
				return ::std::numeric_limits<unsigned long>::max();
			nS = nE + 1;
			if (nS <= nEnd)
				++nIndex;
		}
		return nSum;
	}


	template< typename A, typename D >
	void xuBitMaskCsArray<A,D>::AndValue( A nStart, A nEnd,
		const D& rValueToAnd )
	{
		if (nStart > nEnd)
			return;

		int nIndex = Search( nStart);
		do
		{
			if ((this->pData[nIndex].aValue & rValueToAnd) != this->pData[nIndex].aValue)
			{
				A nS = ::std::max( (nIndex>0 ? this->pData[nIndex-1].nEnd+1 : 0), nStart);
				A nE = ::std::min( this->pData[nIndex].nEnd, nEnd);
				SetValue( nS, nE, this->pData[nIndex].aValue & rValueToAnd);
				if (nE >= nEnd)
					break;  
				nIndex = Search( nE + 1);
			}
			else if (this->pData[nIndex].nEnd >= nEnd)
				break;  
			else
				++nIndex;
		} while (nIndex < this->nCount);
	}


	template< typename A, typename D >
	void xuBitMaskCsArray<A,D>::OrValue( A nStart, A nEnd,
		const D& rValueToOr )
	{
		if (nStart > nEnd)
			return;

		int nIndex = Search( nStart);
		do
		{
			if ((this->pData[nIndex].aValue | rValueToOr) != this->pData[nIndex].aValue)
			{
				A nS = ::std::max( (nIndex>0 ? this->pData[nIndex-1].nEnd+1 : 0), nStart);
				A nE = ::std::min( this->pData[nIndex].nEnd, nEnd);
				SetValue( nS, nE, this->pData[nIndex].aValue | rValueToOr);
				if (nE >= nEnd)
					break;  
				nIndex = Search( nE + 1);
			}
			else if (this->pData[nIndex].nEnd >= nEnd)
				break;  
			else
				++nIndex;
		} while (nIndex < this->nCount);
	}


	template< typename A, typename D >
	void xuBitMaskCsArray<A,D>::CopyFromAnded(
		const xuBitMaskCsArray<A,D>& rArray, A nStart, A nEnd,
		const D& rValueToAnd, long nSourceDy )
	{
		int nIndex;
		A nRegionEnd;
		for (A j=nStart; j<=nEnd; ++j)
		{
			const D& rValue = (j==nStart ?
				rArray.GetValue( j+nSourceDy, nIndex, nRegionEnd) :
			rArray.GetNextValue( nIndex, nRegionEnd));
			nRegionEnd -= nSourceDy;
			if (nRegionEnd > nEnd)
				nRegionEnd = nEnd;
			SetValue( j, nRegionEnd, rValue & rValueToAnd);
			j = nRegionEnd;
		}
	}


	template< typename A, typename D >
	void xuBitMaskCsArray<A,D>::CopyFromOred(
		const xuBitMaskCsArray<A,D>& rArray, A nStart, A nEnd,
		const D& rValueToOr, long nSourceDy )
	{
		int nIndex;
		A nRegionEnd;
		for (A j=nStart; j<=nEnd; ++j)
		{
			const D& rValue = (j==nStart ?
				rArray.GetValue( j+nSourceDy, nIndex, nRegionEnd) :
			rArray.GetNextValue( nIndex, nRegionEnd));
			nRegionEnd -= nSourceDy;
			if (nRegionEnd > nEnd)
				nRegionEnd = nEnd;
			SetValue( j, nRegionEnd, rValue | rValueToOr);
			j = nRegionEnd;
		}
	}


	template< typename A, typename D >
	A xuBitMaskCsArray<A,D>::GetBitStateStart( A nEnd,
		const D& rBitMask, const D& rMaskedCompare ) const
	{
		A nStart = ::std::numeric_limits<A>::max();
		int nIndex = Search( nEnd);
		while ((this->pData[nIndex].aValue & rBitMask) == rMaskedCompare)
		{
			if (nIndex > 0)
			{
				--nIndex;
				nStart = this->pData[nIndex].nEnd + 1;
			}
			else
			{
				nStart = 0;
				break;  
			}
		}
		return nStart;
	}


	template< typename A, typename D >
	A xuBitMaskCsArray<A,D>::GetBitStateEnd( A nStart,
		const D& rBitMask, const D& rMaskedCompare ) const
	{
		A nEnd = ::std::numeric_limits<A>::max();
		int nIndex = Search( nStart);
		while (nIndex < this->nCount && (this->pData[nIndex].aValue & rBitMask) ==
			rMaskedCompare)
		{
			nEnd = this->pData[nIndex].nEnd;
			++nIndex;
		}
		return nEnd;
	}


	template< typename A, typename D >
	A xuBitMaskCsArray<A,D>::GetFirstForCondition( A nStart, A nEnd,
		const D& rBitMask, const D& rMaskedCompare ) const
	{
		int nIndex = Search( nStart);
		do
		{
			if ((this->pData[nIndex].aValue & rBitMask) == rMaskedCompare)
			{
				A nFound = nIndex > 0 ? this->pData[nIndex-1].nEnd + 1 : 0;
				return ::std::max( nFound, nStart);
			}
			if (this->pData[nIndex].nEnd >= nEnd)
				break;  
			++nIndex;
		} while (nIndex < this->nCount);
		return ::std::numeric_limits<A>::max();
	}


	template< typename A, typename D >
	A xuBitMaskCsArray<A,D>::GetLastForCondition( A nStart, A nEnd,
		const D& rBitMask, const D& rMaskedCompare ) const
	{
		int nIndex = Search( nEnd);
		while (1)
		{
			if ((this->pData[nIndex].aValue & rBitMask) == rMaskedCompare)
				return ::std::min( this->pData[nIndex].nEnd, nEnd);

			if (nIndex > 0)
			{
				--nIndex;
				if (this->pData[nIndex].nEnd < nStart)
					break;  
			}
			else
				break;  
		}
		return ::std::numeric_limits<A>::max();
	}


	template< typename A, typename D >
	A xuBitMaskCsArray<A,D>::CountForCondition( A nStart, A nEnd,
		const D& rBitMask, const D& rMaskedCompare ) const
	{
		A nRet = 0;
		int nIndex = Search( nStart);
		do
		{
			if ((this->pData[nIndex].aValue & rBitMask) == rMaskedCompare)
			{
				A nS = ::std::max( (nIndex>0 ? this->pData[nIndex-1].nEnd+1 : 0), nStart);
				A nE = ::std::min( this->pData[nIndex].nEnd, nEnd);
				nRet += nE - nS + 1;
			}
			if (this->pData[nIndex].nEnd >= nEnd)
				break;  
			++nIndex;
		} while (nIndex < this->nCount);
		return nRet;
	}


	template< typename A, typename D >
	int xuBitMaskCsArray<A,D>::FillArrayForCondition( A nStart, A nEnd,
		const D& rBitMask, const D& rMaskedCompare,
		A * pArray, int nArraySize ) const
	{
		int nUsed = 0;
		int nIndex = Search( nStart);
		while (nIndex < this->nCount && nUsed < nArraySize)
		{
			if ((this->pData[nIndex].aValue & rBitMask) == rMaskedCompare)
			{
				A nS = ::std::max( (nIndex>0 ? this->pData[nIndex-1].nEnd+1 : 0), nStart);
				A nE = ::std::min( this->pData[nIndex].nEnd, nEnd);
				while (nS <= nE && nUsed < nArraySize)
					pArray[nUsed++] = nS++;
			}
			if (this->pData[nIndex].nEnd >= nEnd)
				break;  
			++nIndex;
		}
		return nUsed;
	}


	template< typename A, typename D >
	bool xuBitMaskCsArray<A,D>::HasCondition( A nStart, A nEnd,
		const D& rBitMask, const D& rMaskedCompare ) const
	{
		int nIndex = Search( nStart);
		do
		{
			if ((this->pData[nIndex].aValue & rBitMask) == rMaskedCompare)
				return true;
			if (this->pData[nIndex].nEnd >= nEnd)
				break;  
			++nIndex;
		}  while (nIndex < this->nCount);
		return false;
	}


	template< typename A, typename D >
	A xuBitMaskCsArray<A,D>::CountForAnyBitCondition( A nStart, A nEnd,
		const D& rBitMask ) const
	{
		A nRet = 0;
		int nIndex = Search( nStart);
		do
		{
			if ((this->pData[nIndex].aValue & rBitMask) != 0)
			{
				A nS = ::std::max( (nIndex>0 ? this->pData[nIndex-1].nEnd+1 : 0), nStart);
				A nE = ::std::min( this->pData[nIndex].nEnd, nEnd);
				nRet += nE - nS + 1;
			}
			if (this->pData[nIndex].nEnd >= nEnd)
				break;  
			++nIndex;
		} while (nIndex < this->nCount);
		return nRet;
	}


	template< typename A, typename D >
	A xuBitMaskCsArray<A,D>::GetLastAnyBitAccess( A nStart,
		const D& rBitMask ) const
	{
		A nEnd = ::std::numeric_limits<A>::max();
		int nIndex = this->nCount-1;
		while (1)
		{
			if ((this->pData[nIndex].aValue & rBitMask) != 0)
			{
				nEnd = this->pData[nIndex].nEnd;
				break;  
			}
			else
			{
				if (nIndex > 0)
				{
					--nIndex;
					if (this->pData[nIndex].nEnd < nStart)
						break;  
				}
				else
					break;  
			}
		}
		return nEnd;
	}


	template< typename A, typename D >
	template< typename S >
	unsigned long xuBitMaskCsArray<A,D>::SumCoupledArrayForCondition(
		A nStart, A nEnd, const D& rBitMask, const D& rMaskedCompare,
		const xuSummableCsArray<A,S>& rArray ) const
	{
		unsigned long nSum = 0;
		A nS = nStart;
		int nIndex1 = Search( nStart);
		int nIndex2 = rArray.Search( nStart);
		do
		{
			if ((this->pData[nIndex1].aValue & rBitMask) == rMaskedCompare)
			{
				while (nIndex2 < rArray.GetEntryCount() &&
					rArray.GetDataEntry(nIndex2).nEnd < nS)
					++nIndex2;
				unsigned long nNew = rArray.SumValuesContinuation( nS,
					::std::min( this->pData[nIndex1].nEnd, nEnd), nIndex2);
				nSum += nNew;
				if (nSum < nNew)
					return ::std::numeric_limits<unsigned long>::max();
			}
			nS = this->pData[nIndex1].nEnd + 1;
			++nIndex1;
		} while (nIndex1 < this->nCount && nS <= nEnd);
		if (nEnd > this->nMaxAccess &&
			(this->pData[this->GetEntryCount()-1].aValue & rBitMask) == rMaskedCompare)
			nSum += rArray.GetDataEntry(rArray.GetEntryCount()-1).aValue * (nEnd -
			this->nMaxAccess);
		return nSum;
	}


	template< typename A, typename D >
	template< typename S >
	unsigned long xuBitMaskCsArray<A,D>::SumScaledCoupledArrayForCondition(
		A nStart, A nEnd, const D& rBitMask, const D& rMaskedCompare,
		const xuSummableCsArray<A,S>& rArray, double fScale ) const
	{
		unsigned long nSum = 0;
		A nS = nStart;
		int nIndex1 = Search( nStart);
		int nIndex2 = rArray.Search( nStart);
		do
		{
			if ((this->pData[nIndex1].aValue & rBitMask) == rMaskedCompare)
			{
				while (nIndex2 < rArray.GetEntryCount() &&
					rArray.GetDataEntry(nIndex2).nEnd < nS)
					++nIndex2;
				unsigned long nNew = rArray.SumScaledValuesContinuation( nS,
					::std::min( this->pData[nIndex1].nEnd, nEnd), nIndex2, fScale);
				nSum += nNew;
				if (nSum < nNew)
					return ::std::numeric_limits<unsigned long>::max();
			}
			nS = this->pData[nIndex1].nEnd + 1;
			++nIndex1;
		} while (nIndex1 < this->nCount && nS <= nEnd);
		if (nEnd > this->nMaxAccess &&
			(this->pData[this->GetEntryCount()-1].aValue & rBitMask) == rMaskedCompare)
			nSum += (unsigned long)
			(rArray.GetDataEntry(rArray.GetEntryCount()-1).aValue * fScale) *
			(nEnd - this->nMaxAccess);
		return nSum;
	}


	template< typename A, typename D >
	template< typename X >
	void xuCsArrayIterator<A,D>::Follow(
		const xuCsArrayIterator<A,X>& rIter )
	{
		nCurrent = rIter.GetPos();
		if (GetRangeStart() <= nCurrent && nCurrent <= GetRangeEnd())
			; 
		else if (nCurrent > GetRangeEnd())
		{
			A nPos = nCurrent;  
			bool bAdv;
			do
			{
				bAdv = NextRange();
			} while (bAdv && GetRangeEnd() < nPos);
			nCurrent = nPos;
		}
		else
			nIndex = rArray.Search( nCurrent);
	}


	template< typename A, typename D, typename S >
	xuCoupledCsArrayIterator<A,D,S>::xuCoupledCsArrayIterator(
		const xuBitMaskCsArray<A,D> & rArray1, A nStart, A nEnd,
		const D& rBitMaskP, const D& rMaskedCompareP,
		const xuCsArray<A,S> & rArray2 )
		: //aIter1(rArray1, nStart, nEnd)
		//, aIter2(rArray2, nStart, nEnd )
		pIter1(new xuCsArrayIterator<A,D>(rArray1,nStart, nEnd))
		, pIter2(new xuCsArrayIterator<A,S>(rArray2,nStart, nEnd))
		, rBitMask( rBitMaskP )
		, rMaskedCompare( rMaskedCompareP )
	{
		InitLimits();
		 
	}
	
	template< typename A, typename D, typename S >
	xuCoupledCsArrayIterator<A,D,S>::~xuCoupledCsArrayIterator()
	{
		if(pIter1)delete pIter1;
		if(pIter2)delete pIter2;
	}



	template< typename A, typename D, typename S >
	void xuCoupledCsArrayIterator<A,D,S>::InitLimits()
	{
		bool bFound = true;
		bool bMoved = false;
		while (bFound && ((*(*pIter1) & rBitMask) != rMaskedCompare))
		{
			bFound = pIter1->NextRange();
			bMoved = true;
		}
		if (bMoved && bFound)
			pIter2->Follow( (*pIter1));
	}

	template< typename A, typename D, typename S >
	void xuCoupledCsArrayIterator<A,D,S>::NewLimits( A nStart, A nEnd )
	{
		pIter1->NewLimits( nStart, nEnd);
		pIter2->NewLimits( nStart, nEnd);
		InitLimits();
	}


	template< typename A, typename D, typename S >
	bool xuCoupledCsArrayIterator<A,D,S>::NextRange()
	{
		bool bAdv;
		if (pIter1->GetRangeEnd() <= pIter2->GetRangeEnd())
		{
			do
			{
				bAdv = pIter1->NextRange();
			} while (bAdv && ((*(*pIter1) & rBitMask) != rMaskedCompare));
			if (bAdv)
				pIter2->Follow( (*pIter1));
		}
		else
		{
			do
			{
				bAdv = pIter2->NextRange();
			} while (bAdv && pIter2->GetRangeEnd() < pIter1->GetRangeStart());
			if (bAdv)
				pIter1->Follow( (*pIter2));     
		}
		return operator bool();
	}


	template< typename A, typename D, typename S >
	void xuCoupledCsArrayIterator<A,D,S>::Resync( A nPos )
	{
		pIter1->Resync( nPos);
		pIter2->Resync( nPos);
		InitLimits();
	}


	template class XUTIL_API xuCsArray< INT32, INT32>;           // css class
	template class XUTIL_API xuCsArray< INT32, USHORT>;           // heights, base class
	template class XUTIL_API xuSummableCsArray<INT32, USHORT>;   // heights
	template class XUTIL_API xuSummableCsArray<INT32, INT32>;  
	template class XUTIL_API xuCsArray< INT32, BYTE>;             // flags, base class
	template class XUTIL_API xuBitMaskCsArray< INT32, BYTE>;      // flags
	template XUTIL_API unsigned long xuBitMaskCsArray< INT32,
		BYTE>::SumCoupledArrayForCondition( INT32, INT32, const BYTE&, const BYTE&,
		const xuSummableCsArray< INT32, USHORT>&) const;
	template XUTIL_API unsigned long xuBitMaskCsArray< INT32,
		BYTE>::SumScaledCoupledArrayForCondition( INT32, INT32, const BYTE&,
		const BYTE&, const xuSummableCsArray< INT32, USHORT>&,
		double) const;
	template XUTIL_API void xuCsArrayIterator< INT32, USHORT>::Follow(
		const xuCsArrayIterator< INT32, BYTE>&);
	template class XUTIL_API xuCoupledCsArrayIterator< INT32, BYTE, USHORT>;

}}
#pragma pop_macro("max")
#pragma pop_macro("min")


