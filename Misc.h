#pragma once

const float OUTLINE_WIDTH = 3;
const float LINE_WIDTH = 1;

typedef void* (*CreateInterfaceFn) (const char *pName, int *pReturnCode);
typedef void (*pfnDemoCustomDataCallback) (unsigned char *pData, size_t iSize);

typedef struct PlayerInfo_s
{
	__int64         unknown;            //0x0000 
	union
	{
		__int64       steamID64;          //0x0008 - SteamID64
		struct
		{
			__int32     xuidLow;
			__int32     xuidHigh;
		};
	};
	char            szName[128];        //0x0010 - Player Name
	int             userId;             //0x0090 - Unique Server Identifier
	char            szSteamID[20];      //0x0094 - STEAM_X:Y:Z
	char            pad_0x00A8[0x10];   //0x00A8
	unsigned long   iSteamID;           //0x00B8 - SteamID 
	char            szFriendsName[128];
	bool            fakeplayer;
	bool            ishltv;
	unsigned int    customFiles[4];
	unsigned char   filesDownloaded;
} PlayerInfo_t;

enum EntityFlags : int
{
	FL_ONGROUND = (1 << 0),
	FL_DUCKING = (1 << 1),
	FL_WATERJUMP = (1 << 2),
	FL_ONTRAIN = (1 << 3),
	FL_INRAIN = (1 << 4),
	FL_FROZEN = (1 << 5),
	FL_ATCONTROLS = (1 << 6),
	FL_CLIENT = (1 << 7),
	FL_FAKECLIENT = (1 << 8)
};

enum CmdButtons {
	in_attack = (1 << 0),
	in_jump = (1 << 1),
	in_duck = (1 << 2),
	in_forward = (1 << 3),
	in_back = (1 << 4),
	in_use = (1 << 5),
	in_cancel = (1 << 6),
	in_left = (1 << 7),
	in_right = (1 << 8),
	in_moveleft = (1 << 9),
	in_moveright = (1 << 10),
	in_attack2 = (1 << 11),
	in_run = (1 << 12),
	in_reload = (1 << 13),
	in_alt1 = (1 << 14),
	in_alt2 = (1 << 15),
	in_score = (1 << 16),
	in_speed = (1 << 17),
	in_walk = (1 << 18),
	in_zoom = (1 << 19),
	in_weapon1 = (1 << 20),
	in_weapon2 = (1 << 21),
	in_bullrush = (1 << 22),
	in_grenade1 = (1 << 23),
	in_grenade2 = (1 << 24)
};

class CUserCmd
{
public:
	virtual ~CUserCmd() {};
	int       command_number;     // For matching server and client commands for debugging
	int       tick_count;         // The tick the client created this command
	Vector    viewangles;         // Player instantaneous view angles.
	Vector    aimdirection;       // 
	float     forwardmove;        // Forward velocity
	float     sidemove;           // Sideways velocity
	float     upmove;             // Upward velocity
	int       buttons;            // Attack button states
	byte      impulse;            // Impulse command issued.
	int       weaponselect;       // Current weapon id
	int       weaponsubtype;      // 
	int       random_seed;        // For shared random functions
	short     mousedx;            // Mouse accum in x from create move
	short     mousedy;            // Mouse accum in y from create move
	bool      hasbeenpredicted;   // Client only, tracks whether we've predicted this command at least once
	char      pad_0x4C[0x18];
};

inline int iGetNewAllocationCount(int iAllocationCount, int iGrowSize, int iNewSize, int iBytesItem) {
	if (iBytesItem) {
		iAllocationCount = ((1 + ((iNewSize - 1) / iGrowSize)) * iBytesItem);
	}
	else {
		if (!iAllocationCount) {
			// Compute an allocation which is at least as big as a cache line...
			iAllocationCount = (31 + iBytesItem) / iBytesItem;
		}

		while (iAllocationCount < iNewSize) {
			iAllocationCount *= 2;
		}
	}

	return iAllocationCount;
}

template< class T, class I = int >
class CUtlMemory
{
	T* m_tMemory;
	int m_iAllocationCount;
	int m_iGrowSize;
public:
	bool m_bIsValidIndex(I iIndex) const
	{
		long lConvert = iIndex;
		return (lConvert >= 0) && (lConvert < m_iAllocationCount);
	}

	T& operator[](I iIndex);
	const T& operator[](I iIndex) const;

	T* m_tBase()
	{
		return m_tMemory;
	}

	int m_iNumAllocated() const
	{
		return m_iAllocationCount;
	}

	void Grow(int iNumber)
	{
		assert(iNumber > 0);

		int iOldAllocationCount = m_iAllocationCount;
		int iAllocationRequested = m_iAllocationCount + iNumber;
		int iNewAllocationCount = iGetNewAllocationCount(m_iAllocationCount, m_iGrowSize, iAllocationRequested, sizeof(T));

		if ((int)(I)iNewAllocationCount < iAllocationRequested)
		{
			if ((int)(I)iNewAllocationCount == 0 && (int)(I)(iNewAllocationCount - 1) >= iAllocationRequested)
			{
				--iNewAllocationCount;
			}
			else
			{
				if ((int)(I)iAllocationRequested != iAllocationRequested)
				{
					assert(0);
					return;
				}
				while ((int)(I)iNewAllocationCount < iAllocationRequested)
				{
					iNewAllocationCount = (iNewAllocationCount + iAllocationRequested) / 2;
				}
			}
		}

		m_iAllocationCount = iNewAllocationCount;

		if (m_tMemory)
		{
			unsigned char* pChar = new unsigned char[m_iAllocationCount * sizeof(T)];

			memcpy(pChar, m_tMemory, iOldAllocationCount * sizeof(T));
			m_tMemory = (T*)pChar;
		}
		else
		{
			m_tMemory = (T*)new unsigned char[m_iAllocationCount * sizeof(T)];
		}
	}
};

template< class T, class I >
T& CUtlMemory< T, I >::operator[](I iIndex)
{
	assert(m_bIsValidIndex(iIndex));
	return m_tMemory[iIndex];
}

template< class T, class I >
const T& CUtlMemory< T, I >::operator[](I iIndex) const
{
	assert(m_bIsValidIndex(iIndex));
	return m_tMemory[iIndex];
}

template< class T >
void Destruct(T* tMemory)
{
	tMemory->~T();
}

template< class T >
T* Construct(T* tMemory)
{
	return ::new(tMemory) T;
}

template< class T >
T* CopyConstruct(T* tMemory, T const& tSource)
{
	return ::new(tMemory) T(tSource);
}

template< class T, class A = CUtlMemory< T > >
class CUtlVector
{
	typedef A aAllocator;
	typedef T* tIterator;
	typedef const T* tConstIterator;
	aAllocator m_aMemory;
	int m_iSize;
	T* m_tElements;
public:
	T& operator[](int iIndex);
	const T& operator[](int iIndex) const;

	T& m_tElement(int iIndex)
	{
		return m_aMemory[iIndex];
	}

	const T& m_tElement(int iIndex) const
	{
		assert(m_bIsValidIndex(iIndex));
		return m_aMemory[iIndex];
	}

	T* m_tBase()
	{
		return m_aMemory.m_tBase();
	}

	int m_iCount() const
	{
		return m_iSize;
	}

	void RemoveAll()
	{
		for (int i = m_iSize; --i >= 0; )
		{
			Destruct(&m_tElement(i));
		}

		m_iSize = 0;
	}

	bool m_bIsValidIndex(int iIndex) const
	{
		return (iIndex >= 0) && (iIndex < m_iSize);
	}

	void ShiftElementsRight(int iElement, int iNumber = 1)
	{
		assert(m_bIsValidIndex(iElement) || (m_iSize == 0) || (iNumber == 0));

		int iNumberToMove = m_iSize - iElement - iNumber;
		if ((iNumberToMove > 0) && (iNumber > 0))
		{
			memmove(&m_tElement(iElement + iNumber), &m_tElement(iElement), iNumberToMove * sizeof(T));
		}
	}

	void ShiftElementsLeft(int iElement, int iNumber = 1)
	{
		assert(m_bIsValidIndex(iElement) || (m_iSize == 0) || (iNumber == 0));

		int iNumberToMove = m_iSize - iElement - iNumber;
		if ((iNumberToMove > 0) && (iNumber > 0))
		{
			memmove(&m_tElement(iElement), &m_tElement(iElement + iNumber), iNumberToMove * sizeof(T));
		}
	}

	void GrowVector(int iNumber = 1)
	{
		if (m_iSize + iNumber > m_aMemory.m_iNumAllocated())
		{
			m_aMemory.Grow(m_iSize + iNumber - m_aMemory.m_iNumAllocated());
		}

		m_iSize += iNumber;
	}

	int m_iInsertBefore(int iElement) {
		assert((iElement == m_iCount()) || m_bIsValidIndex(iElement));

		GrowVector();
		ShiftElementsRight(iElement);
		Construct(&m_tElement(iElement));
		return iElement;
	}

	int m_iAddToHead()
	{
		return m_InsertBefore(0);
	}

	int m_iAddToTail()
	{
		return m_InsertBefore(m_iSize);
	}

	int m_InsertBefore(int iElement, const T& tSource)
	{
		assert((m_tBase() == 0) || (&tSource < m_tBase()) || (tSource >= (m_tBase() + m_iCount())));
		assert((iElement == m_iCount()) || m_bIsValidIndex(iElement));

		GrowVector();
		ShiftElementsRight(iElement);
		CopyConstruct(&m_tElement(iElement), tSource);
		return iElement;
	}

	int m_iAddToTail(const T& tSource)
	{
		return m_InsertBefore(m_iSize, tSource);
	}

	int m_iFind(const T& tSource) const
	{
		for (int i = 0; i < m_iCount(); i++)
		{
			if (m_tElement(i) == tSource)
			{
				return i;
			}
		}
		return -1;
	}

	void Remove(int iElement)
	{
		Destruct(&m_tElement(iElement));
		ShiftElementsLeft(iElement);
		--m_iSize;
	}

	bool m_bFindAndRemove(const T& tSource) {
		int iElement = m_iFind(tSource);
		if (iElement != -1)
		{
			Remove(iElement);
			return true;
		}
		return false;
	}

	// For STD consistancy, ignoring notation
	tIterator begin()
	{
		return m_tBase();
	}

	tConstIterator begin() const
	{
		return m_tBase();
	}

	tIterator end() {
		return m_tBase() + m_iCount();
	}

	tConstIterator end() const
	{
		return m_tBase() + m_iCount();
	}
};

template< typename T, class A >
T& CUtlVector< T, A >::operator[](int iIndex) {
	assert(iIndex < m_iSize);
	return m_aMemory[iIndex];
}

template< typename T, class A >
const T& CUtlVector< T, A >::operator[](int iIndex) const {
	assert(iIndex < m_iSize);
	return m_aMemory[iIndex];
}
