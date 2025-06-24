#ifndef NPSPush_H
#define NPSPush_H

#include "windows.h"

#ifdef BUILDING_DLL
  #define NPSPushDllExport    __declspec( dllexport )
  #define NPSCDECL
#else
  #define NPSPushDllExport
  #ifdef WIN32
    #define NPSCDECL __cdecl
  #endif
#endif


#define kPush_ManifestFileName    "manifest.txt"
#define kPush_LockedFileName      "locked.txt"
#define kPush_ReLocFileName       "ReLoc.txt"
#define kPush_ErrFileName         "Err_Log.txt"
#define kPush_PBA_LogName         "PBA_Log.txt"
#define kPush_PushDLL_LogName     "Push_Log.txt"
#define kPush_PushDLL_LogName_Old   "Push_Log_Old.txt"
#define kPush_StagingDir          "Stage"

#define kPush_MaxStr 256

typedef enum
{
	kPush_SPECIAL_GetDetails=-2,
	kPush_NoUpdates=-1,
	kPush_Initializing=0,
	kPush_Preparing,
	kPush_Checking4Updates,
	kPush_Updating,
	kPush_InstallingUpdates,
	kPush_Complete,

	// All error codes need to be >= kPush_Error
	// -----------------------------------------
	kPush_Error,									// Unexpected error
	kPush_Error_FileInUse,				// need to run PBA.EXE to finish patch
	kPush_Error_Connection,				// connection was broke, try again
	kPush_Error_Transmission,			// connection was broke, try again
	kPush_Error_DiskFull,					// users disk is too full to perform operation
	kPush_Error_No_Such_Channel,	// channel doesn't exist on transmitter
	kPush_Error_Server_Busy,
	kPush_Error_Transmitter,
	kPush_Error_NeedRePrepared,
	kPush_Error_Certificate,
	kPush_Error_NoSSLSupport,
	kPush_Error_InvalidWinsock,
	kPush_Error_UserCanceled


}tePush_Status;

typedef enum
{
	kPush_Pkg_NPS=0,
	kPush_Pkg_NPSPush,
	kPush_Pkg_PBA,
	kPush_Pkg_Max
}tePush_SystemPkg;

typedef struct
{
	/**
	 * Base directory to install this package
	 */
	char  dir[kPush_MaxStr];

	/**
	 * Path and Pkg name as known by the marimba server
	 */
	char  name[kPush_MaxStr];
	/**
	 * e.g. "transtest.ea.com" (NULL terminated, no quotes)
	 */
	char  serverURL[kPush_MaxStr];
	/**
	 * e.g. "80" (NULL terminated, no quotes)
	 */
	char  serverPort[10];
	/**
	 * Recommeded value is FALSE...
	 * Set TRUE if don't want to verify file integrity.
	 * Setting this to true is faster but patch errors are not handled properly
	 */
	bool  noVerify;
	/**
	 * TRUE causes update to leave download in holding area.
	 * Spawn PBA to continue operation.
	 */
#if 0
	BOOL  stopBeforeInstall;
#endif

	/**
	 * 0=FULL Bandwidth, bytesPerSecond
	 */
	int   maxBandwidth;
	char  authPath[kPush_MaxStr];
	char  authTicket[kPush_MaxStr];
}tsPush_Pkg;

typedef enum
{
	kPush_FA_None=0,
	kPush_FA_Connecting, //UPDATE_CONNECTING
	kPush_FA_WaitForResponse, //UPDATE_WAITING
  kPush_FA_CheckSum, //UPDATE_BUILD
  kPush_FA_VerifyMissing,//VERIFY_MISSING
  kPush_FA_VerifyModified,//VERIFY_MODIFIED
  kPush_FA_FoundModified,//UPDATE_UPDATE
  kPush_FA_FoundMissing,//UPDATE_CREATE
  kPush_FA_Transfering,//UPDATE_TRANSFER
  kPush_FA_Reconstructing,//UPDATE_INSTALL
  kPush_FA_Relocating,

}tePush_FileAction;



typedef struct
{
	unsigned long 		byteTotal;
	unsigned long 		byteRecv;
	short         		mrbErr;
	tePush_Status 		status;// set to kPush_SPECIAL_GetDetails to get fileAction
  tePush_FileAction fileAction;
  char          		currentFile[kPush_MaxStr];
}tsPush_Status;

#define NPSPushAPI_Interface  struct
#define NPSPushAPI_Method     public: virtual

NPSPushAPI_Interface cNPSPushAPI
{
	NPSPushAPI_Method bool
    EnQPatch(tsPush_Pkg * pkg);

  NPSPushAPI_Method void
    DeQPatch(char * channelName);

	NPSPushAPI_Method void
    StartUpdates(void);

	NPSPushAPI_Method tePush_Status
		GetStatus(tsPush_Status * status);

	NPSPushAPI_Method void
    Abort(void);
};

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef DYNAMIC_NPS_PUSH_DLL
  cNPSPushAPI* NPSPushAPI_GetInterface (void);
  void  NPSPushAPI_ReleaseInterface (void);
#else
  typedef cNPSPushAPI*
    (NPSCDECL *fNPSPushAPI_GetInterface)(void);
  typedef void
    (NPSCDECL *fNPSPushAPI_ReleaseInterface)(void);

#endif

#ifdef __cplusplus
}
#endif



#endif // NPSPush_H
