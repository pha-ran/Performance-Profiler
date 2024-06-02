#include "Profiler.h"

struct ProfileData
{
	LONGLONG		_flag;
	TCHAR			_tag[32];
	LARGE_INTEGER	_begin;
	LONGLONG		_total;
	LONGLONG		_call;
	LONGLONG		_max[2];
	LONGLONG		_min[2];
};

ProfileData profileData[32];

VOID ProfileReset(VOID)
{
	;
}

VOID ProfileSave(VOID)
{
	;
}

VOID ProfileBegin(LPCTSTR tag)
{
	;
}

VOID ProfileEnd(LPCTSTR tag)
{
	;
}
