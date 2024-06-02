#include "Profiler.h"
#include <tchar.h>

#define MAX_TAG_LENGTH		32
#define MAX_TRIM			2
#define MAX_PROFILE_DATA	32

struct ProfileData
{
	LONGLONG		_flag;
	TCHAR			_tag[MAX_TAG_LENGTH];
	LARGE_INTEGER	_begin;
	LONGLONG		_call;
	LONGLONG		_total;
	LONGLONG		_max[MAX_TRIM];
	LONGLONG		_min[MAX_TRIM];
};

static ProfileData	profileData[MAX_PROFILE_DATA];
static INT			profileDataCount;

VOID ProfileReset(VOID)
{
	INT index;
	INT trimIndex;

	for (index = 0; index < MAX_PROFILE_DATA; ++index)
	{
		profileData[index]._flag = 0;
		profileData[index]._tag[0] = NULL;
		profileData[index]._begin.QuadPart = 0;
		profileData[index]._call = 0;
		profileData[index]._total = 0;
		
		for (trimIndex = 0; trimIndex < MAX_TRIM; ++trimIndex)
		{
			profileData[index]._max[trimIndex] = 0;
			profileData[index]._min[trimIndex] = 0;
		}
	}

	profileDataCount = 0;
}

VOID ProfileSave(VOID)
{
	;
}

VOID ProfileBegin(LPCTSTR tag)
{
	INT index;

	for (index = 0; index < profileDataCount; ++index)
	{
		if (profileData[index]._flag == 0)
			continue;

		if (_tcscmp(profileData[index]._tag, tag) != 0)
			continue;

		QueryPerformanceCounter(&(profileData[index]._begin));
		profileData[index]._call += 1;

		return;
	}

	profileData[profileDataCount]._flag = 1;
	_tcscpy(profileData[profileDataCount]._tag, tag);
	QueryPerformanceCounter(&(profileData[profileDataCount]._begin));
	profileData[profileDataCount]._call = 1;

	++profileDataCount;
}

VOID ProfileEnd(LPCTSTR tag)
{
	static LARGE_INTEGER frequency;
	INT index;

	if (frequency.QuadPart == 0)
		QueryPerformanceFrequency(&frequency);

	for (index = 0; index < profileDataCount; ++index)
	{
		;
	}
}
