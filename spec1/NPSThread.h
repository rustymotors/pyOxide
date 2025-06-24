/*************************************************************************
 File Name:     NPSThread.h
 Author:        David A. Gregory
 Date Created:  8/5/98
 Current Owner: David A. Gregory

 Purpose: provide a ultra simple platform independent layer on top of unix
          pthreads and NT threads.

 Notes: I've only provide functionality we need right now.  Feel free to add
        additional functionality as needed.

 Modification Log
 Who     |  Date      |  What & Why
 -------------------------------------------------------------------------
         |            |  Original Code

            Copyright 1998 Electronic Arts, Inc.
 *************************************************************************/
//
// $Header: /export/home/cvsroot/NPS/src/Common/NPSLib/NPSThread.h,v 1.8 2001/01/13 19:43:21 abakken Exp $
//
// $Log: NPSThread.h,v $
// Revision 1.8  2001/01/13 19:43:21  abakken
// syncing with VSS
//
// Revision 1.7  2000/09/09 22:24:45  abakken
// cleaned up compiler warnings
//
// Revision 1.6  2000/07/17 23:06:11  jrepp
// * changed defines to make source more genericly unix happy
// * fixed permissons problem with INSTALL in the Makefile
// * cleaned up some misc. warnings
//
// Revision 1.5  2000/03/06 16:36:05  abakken
// partial port to linux
//
// Revision 1.4  2000/02/24 23:29:17  abakken
// ran through indent
//

#ifndef _NPSTHREAD_H_
#define _NPSTHREAD_H_

#if defined (WIN32)
# include <wtypes.h>               // for HANDLE
#define NPSThread_Convention __cdecl
#else
# include <pthread.h>              // for pthread_mutex_t
#define NPSThread_Convention
#endif


// CONSTANTS

#define NPS_THREAD_INVALID  ((NPS_ThreadHandle)0)


// TYPEDEFS

typedef unsigned long NPS_ThreadHandle;
typedef void (NPSThread_Convention * NPS_ThreadFunc) (void *);

extern "C"
{
  typedef void *(* pNPS_ThreadFunc)(void *);
}

#if defined (WIN32)
  typedef HANDLE NPS_MutexHandle;
#else
  typedef long (*LPTHREAD_START_ROUTINE) (void *);
  typedef pthread_mutex_t NPS_MutexHandle;
#endif


// GLOBAL FUNCTION PROTOTYPES

extern NPS_ThreadHandle NPS_ThreadCreate (NPS_ThreadFunc threadFunc,
                                          void *threadData,
                                          const char *description = NULL);

extern NPS_ThreadHandle NPS_ThreadCreateWithPriority (NPS_ThreadFunc
                                                      threadFunc,
                                                      void *threadData,
                                                      const char *description,
                                                      int nPriority);

extern NPS_ThreadHandle NPS_ThreadCreateDetached (NPS_ThreadFunc threadFunc,
                                                  void *threadData,
                                                  const char *description = NULL);
extern void NPS_ThreadTerminate (void *status);

extern void NPS_ThreadWaitForAllToTerminate ();

extern void NPS_Sleep (unsigned long microSeconds);
extern void NPS_Yield ();

extern void NPS_CreateMutex (NPS_MutexHandle *mutexHandle);
extern void NPS_MutexLock (NPS_MutexHandle *mutexHandle);
extern void NPS_MutexUnLock (NPS_MutexHandle *mutexHandle);
extern void NPS_DestroyMutex (NPS_MutexHandle *mutexHandle);

#endif /* _NPSTHREAD_H_ */
