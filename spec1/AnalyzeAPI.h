// **************************************************************************************************
// ****
// **** AnalyzeAPI
// ****
// **** Written by David R. Sullivan
// ****
// **** Copyright 1998 Electronic Arts, Inc.  All Rights Reserved.
// ****

#ifndef _ANALYZE_API_H_
#define _ANALYZE_API_H_

#include "NPSTypes.h"

#ifdef BUILDING_DLL
#define DllExport    __declspec( dllexport )
#define AAPICDECL
#else
#define DllExport	extern
#ifndef sun
#define AAPICDECL	__cdecl
#else
#define AAPICDECL
#endif
#endif

// **** Defines *************************************************************************************

#define BUFFER_SIZE		(65536*4)		// Default Packet Size			   (DO NOT CHANGE THIS VALUE)
#define PING_TIMEOUT	(3000)			// Default Ping Timeout			   (DO NOT CHANGE THIS VALUE)
#define PACKET_SIZE		(2880)			// Default Packet Size			   (DO NOT CHANGE THIS VALUE)
#define TIME_TO_LIVE	(255)			// Default Time to Live (TTL)	   (DO NOT CHANGE THIS VALUE)

#define ANALYZE_NO_SET	(0)				// Used in SetInternals call

// **** enums ***************************************************************************************

#define ANALYZE_OK						 0
// Moved to NPSTypes.h
/*
enum AnalyzeError
{
	ANALYZE_OK						=  0,	// No Error
	ANALYZE_ERROR					= -1,
	ANALYZE_NOT_INITIALIZED			= -2,	// Must call AnalyzeInit first
	ANALYZE_NOT_ENOUGH_MEMORY		= -3,	// Cannot malloc memory for replyBuffer
	ANALYZE_ICMP_ERROR				= -4,	// Icmp function returned an error
	ANALYZE_LINE_TROUBLE			= -5,	// Unknown trouble or too many transmission errors
	ANALYZE_BUFFER_TOO_SMALL		= -6,	// Throughput test using buffer larger than the max buffer size
	ANALYZE_PROTOCOL_INIT_ERROR		= -7,	// Initialization of Packet Send/Receive failed
	ANALYZE_PROTOCOL_CLOSE_ERROR	= -8,	// Close of Packet Send/Receive failed
	ANALYZE_PROTOCOL_SEND_ERROR		= -9,	// Packet Send failed
	ANALYZE_PROTOCOL_RECEIVE_ERROR	= -10,	// Packet Receive failed
	ANALYZE_PROTO_ERROR				= -11,	// Bad Packet Protocol Information
	ANALYZE_PING_ERROR				= -12,	// Ping/Send/Receive Error
	ANALYZE_PING_TIMEOUT			= -13,	// Ping/Send/Receive Timeout

	ANALYZE_MAX_ERROR				= -14
};
*/

enum AnalyzeProtocols
{
	ANALYZE_NO_PROTO				= 0,	// Error Condition, not valid
	ANALYZE_PROTO_ICMP				= 1,	// Use ICMP Ping Requests as packet protocol
	ANALYZE_PROTO_USER				= 2,	// User Defined Ping Routine

	ANALYZE_MAX_PROTO				= 3
};

// **** Structs *************************************************************************************

typedef struct LatencyInfo				// These values are returned from TestLatency(...)
{
	unsigned short	AveragePingTime;
	unsigned short	LowestPingTime;
	unsigned short	HighestPingTime;
	unsigned short	MeanPingTime;
	unsigned char	QuickHops;
	unsigned char	NumberOfPings;
	unsigned short	NumberOfRetries;

} LatencyInfo;


typedef struct ThroughputInfo			// These values are returned from TestThroughput(...)
{
	unsigned long	Bandwidth;
	unsigned long	BytesTrasmitted;
	unsigned long	RoundTripTime;
	unsigned char	NumberOfPings;
	unsigned short	NumberOfRetries;

} ThroughputInfo;


typedef struct HopInfo					// These values are returned from TestHop(...)
{
	unsigned char NumberOfHops;
	unsigned char NumberOfRetries;

} HopInfo;


typedef struct AnalyzeInternalInfo
{
	unsigned long		BufferSize;		// Maximum size of reply buffer
	unsigned long		PingTimeout;	// Number of milliseconds before a reply read timeout occurs
	unsigned long		PacketSize;		// Number of bytes to transfer in a Bandwidth packet
	unsigned char		TimeToLive;		// Maximum number of Hops kto Travel


	int					PacketProto;	// Type of Packet Protocol to be used.
	int					PacketOverhead;	// Ammount in bytes of send and receive overhead divided by two.

	int	(*PacketInit)(void);	// Init Packet Driver
	int (*PacketClose)(void);	// Close Packet Driver
	int (*PacketPing)(char *dest, int PacketSize, int Milliseconds); // Packet Output Routine

} AnalyzeInternalInfo;

// **** Prototypes **********************************************************************************

#ifndef DYNAMIC_ANALYZE_DLL

#ifdef __cplusplus
extern "C"
{
#endif

//===================================================================================================
//==== AnalyzeInit
//====
//==== creates:	replyPacket			This buffer is created here.
//====

DllExport NPSSTATUS AAPICDECL AnalyzeInit(void);

//===================================================================================================
//==== AnalyzeClose
//====
//==== removes:	replyPacket			This buffer is free up here.
//====

DllExport void AAPICDECL AnalyzeClose(void);

//===================================================================================================
//==== AnalyzeSetInternals
//====
//==== input params:				Pass ANALYZE_NO_SET as a parameter to ignore this parameter
//====
//==== input:	BufferSize			Maximum size of reply buffer
//====			PingTimeout			Number of Milliseconds to wait for the reception of the ping
//====			PacketSize			Size of Packets to use in Throughput test
//====			TimeToLive			Maximum number of hops to travel using ICMP

DllExport int AAPICDECL AnalyzeSetInternals(unsigned long BufferSize, unsigned long PingTimeout, unsigned long PacketSize, unsigned char TimeToLive);

//===================================================================================================
//==== AnalyzeSetProtocol
//====
//==== input:	type				Protocol type (ANALYZE_PROTO_ICMP or ANALYZE_PROTO_USER)
//====			initRoutine			Initialze user defined ping code
//====			closeRoutine		Close/Shutdown user defined pping code
//====			pingRoutine			User defined ping routine
//====			packetOverhead		Ammount of overhead for this packet type (one way)
//====
//====			int UserInit(void);		returns (ANALYZE_OK or ANALYZE_ERROR)
//====			int UserClose(void);	returns (ANALYZE_OK or ANALYZE_ERROR)
//====			int UserPing(char *destin, ulong PacketSize, ulong Milliseconds)
//====
//==== return values from UserPing
//====
//====			ANALYZE_OK				Ping Successful
//====			ANALYZE_PING_ERROR		General Error, Send Error, Etc.
//====			ANALYZE_PING_TIMEOUT	Number of Milliseconds Exceeded.
//====

DllExport int AAPICDECL AnalyzeSetProtocol(int type, void *initRoutine, void *closeRoutine, void *pingRoutine, unsigned int packetOverhead);

//===================================================================================================
//==== AnalyzeProtocolInit
//====
//====		This routine is used to initialize the protocol, usually ammounting to a socket creation
//====
//====	returns:
//====		ANALYZE_OK						Everything is fine.
//====		ANALYZE_PROTOCOL_INIT_ERROR		Something went wrong during init.
//====

DllExport int AAPICDECL AnalyzeProtocolInit(void);

//===================================================================================================
//==== AnalyzeProtocolClose
//====
//====		This routine is used to close the protocol, usually ammounting to a socket close
//====
//====	returns:
//====		ANALYZE_OK						Everything is fine.
//====		ANALYZE_PROTOCOL_CLOSE_ERROR	Something went wrong during close
//====

DllExport int AAPICDECL AnalyzeProtocolClose(void);


//===================================================================================================
//==== AnalyzeProtocolPing
//====
//====		This routine is the lowest level ping that you have access to.
//====
//====	inputs:	destination				IP address for PROTO_ICMP, void * for PROTO_USER
//====			TimeToLive				For ICMP Only, maximum number of hops to travel
//====			PacketSize				Outgoing and Incomming packet size
//====			Milliseconds			Send and Receive Packet Timeout
//====			EchoReply				buffer to put Echo Reply data into.
//====			EchoReplySize			Maximum size of Echo reply data
//====
//====	returns:
//====		ANALYZE_ERROR				General purpose error code.
//====		ICMP ERROR CODES			Refer to entire list in IPExport.h
//====
//====		for UserPing codes are translated to the following.
//====
//====			IP_SUCCESS				Successful Ping
//====			IP_BAD_DESINATION		Non-Successful Ping
//====			IP_REQ_TIMED_OUT		Number of Milliseconds exceeded.
//====

DllExport int AAPICDECL AnalyzeProtocolPing(char *destination, unsigned char TimeToLive, int PacketSize, int Milliseconds, struct icmp_echo_reply *EchoReply, unsigned long EchoReplySize);

//===================================================================================================
//==== AnalyzeLatency
//====
//==== input:	destination			Address to Ping
//====			NumberOfPings		Number of times to ping
//====
//==== output:	result				struct to place results in
//====
//==== returns: AveragePingTime		returns a -1 for error or AveragePing time on success

DllExport int AAPICDECL AnalyzeLatency(char *destination, int NumberOfPings, LatencyInfo *result);

//===================================================================================================
//==== AnalyzeThroughput
//====
//==== input:	destination			Address to Ping
//====			TimeToLive			Maximum number of Hops
//====			NumberOfPings		Number of times to ping
//====			PacketSize			Size of additional "garbage" packet
//====
//==== output:	result				struct to place results in
//====
//==== returns: Bandwidth			returns a -1 for error or Bandwidth time on success

DllExport int AAPICDECL AnalyzeThroughput(char *destination, int NumberOfPings, unsigned long PacketSize, ThroughputInfo *result);

//===================================================================================================
//==== AnalyzeHops
//====
//==== input:	destination			Address to Ping
//====
//==== output:	result				struct to place results in
//====
//==== returns: NumberOfHops		returns a -1 for error or NumberOfHops time on success

DllExport int AAPICDECL AnalyzeHops(char *destination, HopInfo *result);

#ifdef __cplusplus
}
#endif

#endif // DYNAMIC_ANALYZE_DLL


typedef int		(AAPICDECL *fAnalyzeInit)(void);
typedef void	(AAPICDECL *fAnalyzeClose)(void);
typedef int		(AAPICDECL *fAnalyzeSetInternals)(unsigned long BufferSize, unsigned long PingTimeout, unsigned long PacketSize, unsigned char TimeToLive);
typedef int		(AAPICDECL *fAnalyzeSetProtocol)(int type, void *initRoutine, void *closeRoutine, void *pingRoutine, unsigned int packetOverhead);
typedef int		(AAPICDECL *fAnalyzeProtocolInit)(void);
typedef int		(AAPICDECL *fAnalyzeProtocolClose)(void);
typedef int		(AAPICDECL *fAnalyzeProtocolPing)(char *destination, unsigned char TimeToLive, int PacketSize, int Milliseconds, struct icmp_echo_reply *EchoReply, unsigned long EchoReplySize);
typedef int		(AAPICDECL *fAnalyzeLatency)(char *destination, int NumberOfPings, LatencyInfo *result);
typedef int		(AAPICDECL *fAnalyzeThroughput)(char *destination, int NumberOfPings, unsigned long PacketSize, ThroughputInfo *result);
typedef int		(AAPICDECL *fAnalyzeHops)(char *destination, HopInfo *result);


#endif // _ANALYZE_API_H_
