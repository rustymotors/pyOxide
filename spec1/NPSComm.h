/*************************************************************************
 File Name:     NPSComm.h
 Author:        John W. Fawcett
 Date Created:  12/11/97
 Current Owner: John W. Fawcett

 Purpose:       Class header file for NPSComm - TCP and UDP communications
 Notes:

 Modification Log
 Who     |  Date      |  What & Why
 --------------------------------------------------------------------------
         |            |  Original Code
 wsantee | 01/16/1998 |  Default last param on SendToSocket to FALSE.
 drs     | 01/11/1999 |  Added #ifdef __cplusplus so C can include defs in file
 sep     | 10/22/1999 |  Removed # include <cstdlib>

            Copyright 1997-1999 Electronic Arts, Inc.
 *************************************************************************/
//
// $Header: /export/home/cvsroot/NPS/src/Common/NPSLib/NPSComm.h,v 1.17 2001/01/13 20:15:50 abakken Exp $
//
// $Log: NPSComm.h,v $
// Revision 1.17  2001/01/13 20:15:50  abakken
// removed Log.h because of conflicts with NPSLog.h
//
// Revision 1.16  2001/01/13 19:43:19  abakken
// syncing with VSS
//
// Revision 1.15  2000/11/02 03:55:05  abakken
// added GetRemoteIpPort to the Unix side
//
// Revision 1.14  2000/09/01 00:59:13  abakken
// merged in Jakes changes from the PC side.
//
// Revision 1.13  2000/07/17 23:06:11  jrepp
// * changed defines to make source more genericly unix happy
// * fixed permissons problem with INSTALL in the Makefile
// * cleaned up some misc. warnings
//
// Revision 1.12  2000/06/26 22:04:54  fawcett
// New stuff in for Rebroadcaster 2 open comm channel
//
// Revision 1.11  2000/04/13 21:40:08  fawcett
// Merged in changes from external servers.  Also made NPSDll work under Windows and
// brought those changes over.
//
// Revision 1.10  2000/03/08 16:21:27  abakken
// fixed broken build
//
// Revision 1.9  2000/03/08 01:28:09  fawcett
// Added stuff to support new CD key and Game Version tables
//
// Revision 1.8  2000/03/06 16:30:49  abakken
// ported to linux
//
// Revision 1.7  2000/02/25 01:08:42  abakken
// redid the seeding of the random number generator to include the thread id
//
// Revision 1.6  2000/02/25 00:12:19  abakken
// ran through indent
//

#ifndef _NPS_COMM_H
#define _NPS_COMM_H

#if !defined NPSCOMM_LATENCY_AND_BANDWIDTH && defined _DEBUG
# define NPSCOMM_LATENCY_AND_BANDWIDTH (0)
#endif
#undef NPSCOMM_LATENCY_AND_BANDWIDTH

//#ifdef __cplusplus                 // Added by DRS to make this C Compatible.
//# include <vector>
//#endif

#include <sys/types.h>

#if defined (WIN32)
# include <winsock.h>
# include <process.h>
# include <time.h>
# define syslog(a) ;
#else
# include <sys/utsname.h>
# include <sys/socket.h>
# include <unistd.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <errno.h>
# include <sys/time.h>
# include <syslog.h>
# include <sys/stat.h>
# include <stdio.h>
#endif


#ifdef NPSCOMM_LATENCY_AND_BANDWIDTH
# include <NPSRWLock.h>
#endif

#include <NPSGen.h>

#if defined (sun) || defined (linux)
# ifndef INADDR_NONE
#  define INADDR_NONE           (0xFFFFFFFF)
# endif
#endif

#define HOST_NAME_LEN      100
#define PORT_LEN           100
#define OPT_LEN            5
#define COMM_BUFFER_LEN    256
#define MAX_MSG_LEN        0xFFFF

#ifdef NPSCOMM_LATENCY_AND_BANDWIDTH
typedef enum _NPSCommGroup
{
  NPSCOMM_GROUP_A,
  NPSCOMM_GROUP_B,
  NPSCOMM_MAX_GROUP,
}
NPSCommGroup;

typedef struct _NPSCommSendParams
{
  unsigned long baseLatency;       // delay between sends in microseconds
  unsigned long deltaLatency;      // +/- random range in microseconds
  unsigned long maxSendRate;       // maximum send rate in bits/second
}
NPSCommSendParams;

// ip address in network byte order.
// i couldn't find the standard macro to do this, so... here's one.
# define MAKE_S_ADDR(a,b,c,d)    (((a) << 24) | ((b) << 16) | ((c) << 8) | (d))
# define NPSCOMM_INFINITE_BANDWIDTH  0xFFFFFFFFUL
#endif // NPSCOMM_LATENCY_AND_BANDWIDTH

#if !defined (WIN32)
// SEP
// const int INVALID_SOCKET =    -1;
// const int SOCKET_ERROR =      -1;
# define INVALID_SOCKET -1
# define SOCKET_ERROR -1
# ifndef FALSE
#  define FALSE 0
# endif
# ifndef TRUE
#  define TRUE 1
# endif
#endif


// Solaris8 redefined the socklen_t to be a uint32_t instead of a int.

#if defined (SOLARIS8)
  typedef int SOCKET;
  typedef uint32_t socklen_t;
 #elif defined (sun)
  typedef int SOCKET;
  typedef int socklen_t;
 #elif defined (linux)
  typedef int SOCKET;
 #elif defined (WIN32)
  typedef int socklen_t;
#endif

#define STRINGDATA  TRUE
#define PEEKMSGSIZE  4 * sizeof(short)

typedef struct _SockBuffer
{
  SOCKET SocketId;
  long DataSize;
  char *Data;
  long LastRead;
  long Copied;
}
SockBuffer;

#ifdef __cplusplus                 // Added by DRS to make this C Compatible.

#ifdef NPSCOMM_LATENCY_AND_BANDWIDTH
typedef std::vector < struct in_addr >NPSCommSockAddrVector;
#endif

class NPSComm
{
public:
  NPSComm (void);

  ~NPSComm(void);

#ifdef NPSCOMM_LATENCY_AND_BANDWIDTH
  void AddAddrToGroup (NPSCommGroup group, struct in_addr *ipAddress)
  {
    // this function assumes you have NOT added the same address to
    // multiple groups.  if you have, it will always find it in the
    // LOWEST #'d group!!!  this function also keep track of both
    // groups, so that if we expand beyond two groups, the code just
    // needs updating of one constant.  this means searching is more
    // expensive, but...  OH WELL! It's just for debugging!
    m_GroupAddrLock[group].lockForWrite ();
    m_GroupAddr[group].push_back (*ipAddress);
    m_GroupAddrLock[group].release ();
  }

  NPSCommGroup FindGroup (struct in_addr *ipAddress)
  {
    int i;
    bool found = FALSE;

    for (i = NPSCOMM_GROUP_A; i < NPSCOMM_MAX_GROUP && !found; i++)
    {
      NPSCommSockAddrVector::iterator it;

      m_GroupAddrLock[i].lockForRead ();
      for (it = m_GroupAddr[i].begin (); it != m_GroupAddr[i].end (); it++)
      {
        if (memcmp (ipAddress, it, sizeof (struct in_addr)) == 0)
        {
          found = TRUE;
          break;
        }
      }
      m_GroupAddrLock[i].release ();
    }

    // returns NPSCOMM_MAX_GROUP if not found!
    return (NPSCommGroup) i;
  }

  void SetGroupSendLatencyAndBandwidth (
                                         // delay between sends in microseconds
                                         NPSCommGroup group,
                                         unsigned long baseLatency,
                                         // +/- random range in microseconds
                                         unsigned long deltaLatency,
                                         // maximum send rate in bits/second
                                         unsigned long maxSendRate)
  {
    m_Group[group].baseLatency = baseLatency;
    deltaLatency = (deltaLatency > (baseLatency >> 1)) ?
      (baseLatency >> 1) : deltaLatency;
    m_Group[group].deltaLatency = deltaLatency;
    m_Group[group].maxSendRate = maxSendRate;
  }
#endif

  // IP Location stuff
  NPSSTATUS GetMyName (char *HostName);
  NPSSTATUS GetHostIPAddress (char ipAddress[NPS_IPADDR_LEN]);
  int MaxSocketsInFdSet ()
  {
    return m_Maxfds;
  }

  //------------------------- Non-Wrappers -----------------------------
  // Connection (Server)
  NPSSTATUS BuildLocalSocket (int Port, SOCKET * LocalInMsgSock,
                              struct sockaddr_in * LocalInServer);

  NPSSTATUS BuildLocalSocketNoPort (int *InPort, SOCKET * LocalSock,
                                    struct sockaddr_in *LocalServer);

  NPSSTATUS BuildLocalSocketRanged (int *InPort, int Min, int Max,
                                    SOCKET * LocalSock,
                                    struct sockaddr_in *LocalServer);

  NPSSTATUS WaitForSocketConnection (SOCKET LocalSock, SOCKET * Client,
                                     struct timeval *TimeOut,
                                     struct sockaddr_in *RemoteAddr = NULL,
                                     socklen_t *pAddrLen = NULL);

  NPSSTATUS WaitForSocketAccept (SOCKET LocalSock, SOCKET * Client,
                                 struct timeval *TimeOut,
                                 struct sockaddr_in *RemoteAddr = NULL,
                                 socklen_t *pAddrLen = NULL);

  NPSSTATUS NPSComm::CompleteSocketAccept (SOCKET * NewSockFd,
                                           struct timeval *TimeOut);

  NPSSTATUS WaitForSocketConnection (SOCKET LocalSock, SOCKET * Client,
                                     struct sockaddr_in *RemoteAddr = NULL,
                                     socklen_t *pAddrLen = NULL);

  NPSSTATUS CloseSocket (SOCKET * Close);

  // Connection (Client)
  NPSSTATUS BuildRemoteSocket (char *HostName, int Port, SOCKET * LocalSock,
                               struct sockaddr_in *LocalServer);

  NPSSTATUS BuildRemoteSocketByAddr (char *IPAddress, int Port,
                                     SOCKET * LocalSock,
                                     struct sockaddr_in *LocalServer);

  NPSSTATUS ConnectSocket (SOCKET LocalSock, struct sockaddr_in *LocalServer);
  NPSSTATUS ConnectUDPSocket (SOCKET LocalSock, struct sockaddr_in *LocalServer);

  // Note:  Socket connections are synchronous unless you specifically use an
  //        IOCTL on the socket to set them as non-blocking.  If tyou do this,
  //        then you MUST use this routine to complete the connection
  //        (handshakes, etc.).
  NPSSTATUS CompleteAsynchConnection (SOCKET LocalSock);

  // Set socket options
  NPSSTATUS SetSocketOption (SOCKET LocalSocket, int Option, int Value);

  // Send and receive
  NPSSTATUS WaitForSocket (SOCKET LocalSocket, struct timeval *MyTimeOut,
                           NPS_LOGICAL SelectForWrite = FALSE);

  fd_set *CreateSocketFdSet (SOCKET * Sockets, int nSockets);

  NPS_LOGICAL IsSocketSet (SOCKET Socket, fd_set * FdSet);

  NPSSTATUS WaitForMultipleSockets (fd_set * FdSet, struct timeval *TimeOut,
                                    NPS_LOGICAL SelectForWrite = FALSE);

  NPSSTATUS ReceiveFromSocket (SOCKET * LocalSocket, char *Message,
                               int MsgLen, int flags = 0);

  NPSSTATUS SendToSocket (SOCKET * LocalSocket, char *MyMessage,
                          long MsgSize = 0, NPS_LOGICAL stringdata = FALSE);

  NPSSTATUS PeekOnSocket (SOCKET * LocalSocket, char *PeekMessage,
                          int MsgSize = PEEKMSGSIZE);

  int _NPSComm_Select_Write (fd_set * pFdWriteSet,

                             struct timeval *pConnTimeOut);
  //------------------------------------------------------------
  //
  // functions for UDP sockets
  //
  //------------------------------------------------------------

  void BuildSockaddrByAddrUDP (char *RemoteHostIP,
                               int Port,
                               struct sockaddr_in *pServer,
                               socklen_t SockAddrLen);

  NPSSTATUS BuildSockaddrByNameUDP (char *RemoteHostName,
                                    int Port,
                                    struct sockaddr_in *pServer,
                                    socklen_t SockAddrLen);

  NPSSTATUS BuildLocalSocketUDP (int Port,
                                 SOCKET * LocalInMsgSock,
                                 struct sockaddr_in *LocalInServer);

  NPSSTATUS BuildLocalSocketNoPortUDP (int *InPort,
                                       SOCKET * InSock,
                                       struct sockaddr_in *InServer);

  NPSSTATUS BuildLocalSocketRangedUDP (int *InPort, int Min, int Max,
                                       SOCKET * InSock,
                                       struct sockaddr_in *InServer);

  NPSSTATUS SendToSocketUDP (SOCKET * LocalSocket,
                             struct sockaddr *pSockAddr,
                             socklen_t SockAddrLen,
                             char *MyMessage, long MsgSize);


  /**
   * Receives a message on a UDP socket.
   *
   * @param LocalSocket
   *
   * @param pSockAddr
   *
   * @param pSockAddrLen
   *
   * @param MyMessage    This buffer must be at least as large as the MsgSize parameter says it is.
   *
   * @param MsgSize      Must be large enough to hold the entire message, else any partial read throws
   *                     out the remainder of the datagram.
   *
   * @return <UL>
   *         <LI>>0 : The number of bytes copied into the MyMessage parameter.
   *         <LI> 0 : Connection closed
   *         <LI><0 : An NPSSTATUS error message (NPS_ERR,NPS_SHORT_READ)
   *         </UL>
   */
  NPSSTATUS ReceiveFromSocketUDP (SOCKET * LocalSocket,
                                  struct sockaddr *pSockAddr,
                                  socklen_t *pSockAddrLen,
                                  char *MyMessage, int MsgSize);

  NPSSTATUS PeekOnSocketUDP (SOCKET * LocalSocket,
                             struct sockaddr *pSockAddr,
                             socklen_t *pSockAddrLen,
                             char *PeekMessage, int MsgSize);

  // Set and clear asynchronous mode for a socket
  NPSSTATUS SetSocketSync (SOCKET * LocalSocket);
  NPSSTATUS SetSocketAsync (SOCKET * LocalSocket);

  // Inlines
  NPS_INLINE int GetCommError (void)
  {
    return m_LastErrno;
  }

  NPS_INLINE char *GetMyIp (void)
  {
    return szOurIpAddress;
  }

  NPS_INLINE void GetMyInAddr (struct in_addr *ipAddress)
  {
    *ipAddress = OurIpAddress;
  }

  NPS_INLINE void NPSComm_SetLastError (void)
  {
#if defined (WIN32)
    m_LastErrno = WSAGetLastError ();
#else
    m_LastErrno = errno;
    fflush (stdout);
    perror ("SetLastError:");
    fprintf (stderr, "SetLastError (2): %d %s\n", errno, strerror (errno));
    fflush (stderr);
    fflush (stdout);
    if (m_LastErrno == EMFILE)
      _DumpFds ();
#endif
  }
  NPSSTATUS GetRemoteIpPort (int fd, // in
                             char *str, // in - users IP is copied here
                             const int str_len, // in length of buffer str
                             int *port); // out - users Port

private:
  struct in_addr OurIpAddress;
  char szOurIpAddress[16];
  int m_LastErrno;
  int m_Maxfds;
  unsigned int m_RandomSeed;

#ifdef NPSCOMM_LATENCY_AND_BANDWIDTH
  NPSCommSendParams m_Group[NPSCOMM_MAX_GROUP];
  NPSCommSockAddrVector m_GroupAddr[NPSCOMM_MAX_GROUP];
  NPS_RWLock m_GroupAddrLock[NPSCOMM_MAX_GROUP];

  unsigned long _CalcSleepTime (size_t length, NPSCommGroup group, int protocol);
#endif
  void _DumpFds (void);
  NPSSTATUS _WS_Startup ();
  NPSSTATUS _WS_Cleanup ();
  NPSSTATUS _CheckMessageAvailable (SOCKET * LocalSocket);
  static bool _IsValidFd (int fdnum);
  int _Send (int socket, const char *buffer, size_t length, int flags);
  int _SendTo (int socket, const char *message, size_t length, int flags,
               const struct sockaddr *dest_addr, size_t dest_len);
  NPSSTATUS _HandshakeSockets (SOCKET LocalSock,

                               struct sockaddr_in *SockInfo);
  int _NPSComm_Select (fd_set * pFdReadSet, struct timeval *pConnTimeOut);
  int _Recv (SOCKET sock, char *pBuf, int buflen, int flags);
  int _RecvFrom (SOCKET sock, char *pBuf, int buflen, int flags,
                 struct sockaddr *pSockAddr, socklen_t *pSockAddrLen);

};

#endif

#endif // _NPS_COMM_H
