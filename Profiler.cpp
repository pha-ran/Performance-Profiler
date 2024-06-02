#pragma warning (disable : 6387)

//#undef UNICODE
//#undef _UNICODE

#include "Profiler.h"
#include <stdio.h>
#include <tchar.h>
#include <time.h>

#define MAX_TAG_LENGTH		48
#define MAX_PROFILE_DATA	32

struct ProfileData
{
	LONGLONG		_flag;
	TCHAR			_tag[MAX_TAG_LENGTH];
	LARGE_INTEGER	_begin;
	LONGLONG		_total;
	LONGLONG		_call;
};

static ProfileData	profileData[MAX_PROFILE_DATA];
static INT			profileDataCount;

VOID ProfileReset(VOID)
{
	INT index;

	for (index = 0; index < MAX_PROFILE_DATA; ++index)
	{
		profileData[index]._flag = 0;
		profileData[index]._tag[0] = NULL;
		profileData[index]._begin.QuadPart = 0;
		profileData[index]._total = 0;
		profileData[index]._call = 0;
	}

	profileDataCount = 0;
}

VOID ProfileSave(VOID)
{
	__time64_t time;
	struct tm Tm;
	TCHAR name[28];
	FILE* file;
	INT index;

	_time64(&time);
	_localtime64_s(&Tm, &time);

	_stprintf_s(
		name,
		28,
		_T("profile_%04d%02d%02d_%02d%02d%02d.txt"),
		Tm.tm_year + 1900,
		Tm.tm_mon + 1,
		Tm.tm_mday,
		Tm.tm_hour,
		Tm.tm_min,
		Tm.tm_sec
	);

	_tfopen_s(&file, name, _T("w"));

	_ftprintf(
		file,
		_T("%14s\t%12s\t%s\n\n"),
		_T("avg (ns)"),
		_T("call"),
		_T("tag")
	);

	for (index = 0; index < profileDataCount; ++index)
	{
		_ftprintf(
			file,
			_T("%14lld\t%12lld\t%s\n"),
			profileData[index]._total / profileData[index]._call,
			profileData[index]._call,
			profileData[index]._tag
		);
	}

	fclose(file);
}

VOID ProfileBegin(LPCTSTR tag)
{
	INT index;

	for (index = 0; index < profileDataCount; ++index)
	{
		if (_tcscmp(profileData[index]._tag, tag) != 0)
			continue;

		QueryPerformanceCounter(&(profileData[index]._begin));

		return;
	}

	_tcscpy_s(profileData[profileDataCount]._tag, MAX_TAG_LENGTH, tag);
	QueryPerformanceCounter(&(profileData[profileDataCount]._begin));

	++profileDataCount;
}

VOID ProfileEnd(LPCTSTR tag)
{
	static LARGE_INTEGER frequency;
	INT index;
	LARGE_INTEGER end;

	if (frequency.QuadPart == 0)
		QueryPerformanceFrequency(&frequency);

	for (index = 0; index < profileDataCount; ++index)
	{
		if (_tcscmp(profileData[index]._tag, tag) != 0)
			continue;

		QueryPerformanceCounter(&end);

		end.QuadPart -= profileData[index]._begin.QuadPart;
		end.QuadPart *= 1000000000;
		end.QuadPart /= frequency.QuadPart;
		profileData[index]._total += end.QuadPart;
		profileData[index]._call += 1;

		return;
	}
}

int main(void)
{
	while (1)
	{
		PROFILE_BEGIN(_T("for i"));
		for (INT i = 0; i < 10000; ++i)
		{
			PROFILE_BEGIN(_T("for j"));
			for (INT j = 0; j < 10000; ++j)
			{
				INT K = 0;
				K = 1;
			}
			PROFILE_END(_T("for j"));
		}
		PROFILE_END(_T("for i"));

		PROFILE_BEGIN(_T("Test 1000"));
		Sleep(1000);
		PROFILE_END(_T("Test 1000"));

		PROFILE_BEGIN(_T("PROFILE"));
		PROFILE_END(_T("PROFILE"));

		if (GetAsyncKeyState(VK_F1))
			ProfileSave();

		if (GetAsyncKeyState(VK_F5))
			ProfileReset();
	}

	return 0;
}
