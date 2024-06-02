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
	_tcscpy_s(profileData[profileDataCount]._tag, MAX_TAG_LENGTH, tag);
	QueryPerformanceCounter(&(profileData[profileDataCount]._begin));
	profileData[profileDataCount]._call = 1;

	++profileDataCount;
}

VOID ProfileEnd(LPCTSTR tag)
{
	static LARGE_INTEGER frequency;
	INT index;
	INT trimIndex;
	LARGE_INTEGER end;

	if (frequency.QuadPart == 0)
		QueryPerformanceFrequency(&frequency);

	for (index = 0; index < profileDataCount; ++index)
	{
		if (profileData[index]._flag == 0)
			continue;

		if (_tcscmp(profileData[index]._tag, tag) != 0)
			continue;

		QueryPerformanceCounter(&end);

		end.QuadPart -= profileData[index]._begin.QuadPart;
		end.QuadPart *= 10000000;
		end.QuadPart /= frequency.QuadPart;

		profileData[index]._total += end.QuadPart;

		for (trimIndex = 0; trimIndex < MAX_TRIM; ++trimIndex)
		{
			if (end.QuadPart > profileData[index]._max[trimIndex] ||
				profileData[index]._max[trimIndex] == 0)
			{
				profileData[index]._max[trimIndex] = end.QuadPart;
				break;
			}

			if (end.QuadPart < profileData[index]._min[trimIndex] ||
				profileData[index]._min[trimIndex] == 0)
			{
				profileData[index]._min[trimIndex] = end.QuadPart;
				break;
			}
		}

		return;
	}
}

int main(void)
{
	while (1)
	{
		PROFILE_BEGIN(_T("Test"));
		PROFILE_END(_T("Test"));
	}

	return 0;
}
