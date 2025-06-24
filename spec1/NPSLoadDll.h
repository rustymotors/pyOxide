// **************************************************************************************************
// ****
// **** NPSLoadDLL
// ****
// **** Written by David R. Sullivan
// ****
// **** Copyright 1998 Electronic Arts, Inc.  All Rights Reserved.
// ****

#ifndef NPSLOADDLL_H
#define NPSLOADDLL_H

#ifdef DYNAMIC_NPS_DLL
#include "NPSDll.h"
#endif

#ifdef DYNAMIC_NPS_LOGIN_DLL
#include "NPSLoginDll.h"
#endif

#ifdef DYNAMIC_ANALYZE_DLL
#include "AnalyzeAPI.h"
#endif

#ifdef DYNAMIC_PUSH_DLL
#include "NPSPush.h"
#endif

#include <windows.h>

#define NPSDLL_PATH_LEN				(128)
#define NPSDLL_IP_STRING			(256)

typedef enum NPSDllTypes
{
	NPSDLLTYPE_MIN = 0,
	NPSDLLTYPE_REBROADCASTER=NPSDLLTYPE_MIN,
	NPSDLLTYPE_LOGIN_CLIENT,
	NPSDLLTYPE_ANALYZEAPI,
	NPSDLLTYPE_PUSH,
	NPSDLLTYPE_MARIMBA,
	NPSDLLTYPE_MAX
} NPSDllTypes;

typedef struct
{
	char				LoginIP[NPSDLL_IP_STRING];
	char				LobbyIP[NPSDLL_IP_STRING];
	char				PatchIP[NPSDLL_IP_STRING];
	unsigned long	LoginPort;
	unsigned long	LobbyPort;
	unsigned long	PatchPort;
}tsNPSServerLocations;

#ifdef __cplusplus
extern "C"
{
#endif


extern BOOL __cdecl NPSLoadDll(NPSDllTypes type, const char * path);
extern BOOL __cdecl NPSIsDllLoaded(NPSDllTypes type);
extern BOOL __cdecl NPSUnloadDll(NPSDllTypes type);
extern BOOL __cdecl NPSSpawnPBA(const char * pbaPath, char * cmdLineArgs);

// ***********************************************************************************
// NPSDll Commands
// ***********************************************************************************

#ifdef DYNAMIC_NPS_DLL
extern fNPSAPI_GetInterface							pNPSAPI_GetInterface;
extern fNPSAPI_ReleaseInterface					pNPSAPI_ReleaseInterface;
#endif

// ***********************************************************************************
// LoginClient Commands
// ***********************************************************************************

#ifdef DYNAMIC_NPS_LOGIN_DLL
//extern fNPSLoginAPI_GetInterface				pNPSLoginAPI_GetInterface;
//extern fNPSLoginAPI_ReleaseInterface		pNPSLoginAPI_ReleaseInterface;
#endif

// ***********************************************************************************
// Analyze API Commands
// ***********************************************************************************

#ifdef DYNAMIC_ANALYZE_DLL
//extern fNPSIcmpAPI_GetInterface				pNPSIcmpAPI_GetInterface;
//extern fNPSIcmpAPI_ReleaseInterface		pNPSIcmpAPI_ReleaseInterface;
extern fAnalyzeInit								AnalyzeInit;
extern fAnalyzeClose							AnalyzeClose;
extern fAnalyzeSetInternals						AnalyzeSetInternals;
extern fAnalyzeSetProtocol						AnalyzeSetProtocol;
extern fAnalyzeProtocolInit						AnalyzeProtocolInit;
extern fAnalyzeProtocolClose					AnalyzeProtocolClose;
extern fAnalyzeProtocolPing						AnalyzeProtocolPing;
extern fAnalyzeLatency							AnalyzeLatency;
extern fAnalyzeThroughput						AnalyzeThroughput;
extern fAnalyzeHops								AnalyzeHops;
#endif

// ***********************************************************************************
// PatchDLL Commands
// ***********************************************************************************
#ifdef DYNAMIC_PUSH_DLL
//extern fNPSPushAPI_GetInterface			pNPSPushAPI_GetInterface;
//extern fNPSPushAPI_ReleaseInterface		pNPSPushAPI_ReleaseInterface;
#endif

#ifdef __cplusplus
}
#endif

#endif //NPSLOADDLL_H
