#pragma once
#include "windows.h"

#define PROFILE

#ifdef PROFILE
	#define PROFILE_RESET()		ProfileReset()
	#define PROFILE_SAVE()		ProfileSave()
	#define	PROFILE_BEGIN(TAG)	ProfileBegin(TAG)
	#define	PROFILE_END(TAG)	ProfileEnd(TAG)
#else
	#define PROFILE_RESET()
	#define PROFILE_SAVE()
	#define PROFILE_BEGIN(TAG)
	#define PROFILE_END(TAG)
#endif

VOID ProfileReset(VOID);
VOID ProfileSave(VOID);
VOID ProfileBegin(LPCTSTR tag);
VOID ProfileEnd(LPCTSTR tag);
