/**
  * @author: jfawcett@ea.com
  * @author: abakken@ea.com
  * @author: cwalters@ea.com
  *
  * Created: 7/1/98
  *
  */
#ifndef _NPS_DLL_TYPES_H
#define _NPS_DLL_TYPES_H

#include "NPSTypes.h"

#ifdef WIN32
  #include <winsock.h>              // for SOCKET typedef
#else
  #include <NPSComm.h>              // for SOCKET typedef
#endif

#ifdef WIN32
  #ifdef BUILDING_DLL
    #define NPSRebroadcasterDllExport    __declspec( dllexport )
    #define NPSCDECL
  #else
    #define NPSRebroadcasterDllExport
    #define NPSCDECL __cdecl
  #endif
#else
  #define NPSRebroadcasterDllExport
  #define NPSCDECL

  #define BOOL  bool

  #ifndef TRUE
    #define TRUE  true
  #endif

  #ifndef FALSE
    #define FALSE false
  #endif

  #define APIENTRY
  #define HANDLE int
  #define DWORD long
  #define LPVOID void *
#endif



/*** Game message callback function*/
typedef void (NPSCDECL * tfGameMessageCallback) (NPS_SERVID ServerId,
                                                 NPS_COMMID CommId,
                                                 NPS_USERID SendingUser,
                                                 NPS_OPCODE Command,
                                                 char       *Blob,
                                                 int        BlobLen,
                                                 void       *UserGameContext);

/*** Command callback function*/
typedef void (NPSCDECL * tfCommandMessageCallback) (NPS_SERVID ServerId,
                                                    NPS_OPCODE Command,
                                                    char       *Blob,
                                                    int         BlobLen,
                                                    void       *UserCommandContext);


/**
 * Contains callbacks used to deliver messages.
 *
 * @see NPSConnectToServer
 * @see NPSSetServerData
 * @see NPSGetServerData
 * @since NPS 1
 */
typedef struct _NPS_ServerCallbackInfo
{
  /**
   * Messages received from other players are delivered through this
   * callback method.  Involked from a separate thread.
   */
  tfGameMessageCallback
    GameMsg;

  /**
   * Context always passed to GameMessage
   */
  void *
    GameMsgContext;

  /**
   * Messages received from the server are delivered through this
   * callback method.  Involked from a separate thread.
   */
  tfCommandMessageCallback
    CommandMsg;

  /**
   * Context always passed to CommandMessage
   */
  void *
    CommandMsgContext;

} NPS_ServerCallbackInfo;

/**
 * NPS_ClientInfo is information about the client can be set on the initial
 * server connection, and change (as is the case for UserData) once connected.<BR>
 * <BR>
 *
 * @see GetUsersOnServer
 * @see GetUserData
 * @see GetMyUserData
 * @see QueryCommChannel
 * @see SetMyUserData
 * @see ConnectToServer
 * @see NPS_USER_JOINED
 * @see NPS_USER_INFO
 * @see NPS_USER_LIST
 */
typedef struct _NPS_ClientInfo
{
  /**
   * The permanent name the user will have on this server
   */
  char
    UserName[NPS_USERNAME_LEN];

  /**
   * Assigned user ID (or 0 for auto assign).  Once connected to a server,
   * this value remains constant
   */
  NPS_USERID
    UserId;

  /**
   * User defined data blob
   */
  char
    UserData[NPS_USERDATA_LEN];
} NPS_ClientInfo;

typedef struct _NPS_SlotList
{
  NPS_COMMID     CommId;        /*** The CommId of this channel*/
  NPS_USERID     UserId;        /*** The UserId of the user*/
  long           SlotNumber;    /*** The user's slot number*/
  NPS_LOGICAL    AddUser;       /*** TRUE if this is an add, FALSE if this is a remove*/
  struct _NPS_SlotList *Next;
  struct _NPS_SlotList *Prev;
} NPS_SlotList;

/**
 * NPS_ChannelInfo are those things that a client can change about a comm
 * channel.
 *
 * @see OpenCommChannel
 * @see SetCommChannelData
 * @see NPS_CHANNEL_UPDATE
 * @since NPS 1
 */
typedef struct _NPS_ChannelInfo
{
  // The ID of this channel (USERS DON'T CHANGE)*/
  NPS_COMMID CommId;

  // The RIFF of this channel*/
  char Riff[NPS_RIFF_NAME_LEN];

  // NPSTCP or NPSUDP (USERS DON'T CHANGE)*/
  unsigned long Protocol;

  // Port to build the socket on.  Set to 0 for none.*/
  int RequestedPort;

  // Number of people on this channel (USERS DON'T CHANGE)*/
  short ConnectedUsers;

  /**
   * For a primary lobby, the number of channels the room this server
   * represents has open
   */
  short OpenChannels;

  /*** The password for a private comm channel*/
  char Password[NPS_PASSWORD_LEN];

  /*** SKU of the game requesting launch*/
  long SKU;

  /*** If rate limiting, this is the rate*/
  long SendRate;

  /*** User settable channel information*/
  char ChannelData[NPS_CHANNEL_DATA_SIZE];

  /*** TRUE if the user is a member of channel*/
  NPS_LOGICAL UserIsConnected;

  /*** Max number of players allowed*/
  unsigned short MaxReadyPlayers;

  /*** List of used slots on this channel*/
  NPS_SlotList *SlotList;

  /*** The UserId that owns this channel*/
  NPS_USERID MasterUserId;

  /**
   * Possible values:
   * <UL>
   * <LI>NPS_SERVER_NOT_RUNNING
   * <LI>NPS_SERVER_START_PENDING
   * <LI>NPS_SERVER_RUNNING
   * </UL>
   */
  char GameServerIsRunning;

  /**
   * Possible Values:
   * <UL>
   * <LI>NPS_PUBLIC_CHANNEL
   * <LI>NPS_PRIVATE_CHANNEL
   * <LI>NPS_OPAQUE_CHANNEL
   * </UL>
   */
  short ChannelType;

} NPS_ChannelInfo;

/**
 * Flags that can be set for a comm channel
 * <UL>
 * <LI>NPS_PRIVATE_CHANNEL   - This is a private, password protected channel
 * <LI>NPS_PUBLIC_CHANNEL    - This is a public channel
 * <LI>NPS_PERMANENT_CHANNEL - This is a permanent channel
 * <LI>NPS_TRANSIENT_CHANNEL - This is a transient channel
 *                             (collapses when the last user leaves)
 * <LI>NPS_SET_GAME_READY    - Set the GameReady flag for this user
 * <LI>NPS_CLEAR_GAME_READY  - Cleat the GameReady flag for this user
 * <LI>NPS_HIDE_CHANNEL      - Don't let other users know about this channel
 * <LI>NPS_EXPOSE_CHANNEL    - Let everyone see this channel
 * <LI>NPS_SEND_ON_DUP       - If message coalescing, send when SendRate expires
 *                             OR when a second message from any user is received
 * <LI>NPS_SEND_ON_TIME      - If message coalescing, send when SendRate expores
 *                             regardless of second messages (second messages are
 *                             added to the group and sent at that time)
 * <LI>NPS_AUTOLAUNCH_SERVER - Launch a game server for this channel (not for game use)
 * <LI>NPS_SET_SERVER_RATE - Use this to coallesce messages in the server
 * <LI>NPS_SET_DLL_RATE - Use this to coallesce messages in the client
 * </UL>
 * NPS_RiffInfo is the message passed back in response to GetRiffList (NPS_RIF_LIST).
 * @since NPS 1
 */
typedef struct _NPS_RiffInfo
{
  /*** The RIFF name*/
  char Riff[NPS_RIFF_NAME_LEN];

  /*** The protocol of this RIFF*/
  unsigned long Protocol;

  /*** The CommId of the RIFF*/
  NPS_COMMID CommId;

  /*** Password for this channel*/
  char Password[NPS_PASSWORD_LEN];

  /*** NPS_PUBLIC_CHANNEL, NPS_PRIVATE_CHANNEL, or NPS_OPAQUE_CHANNEL*/
  short ChannelType;

  /*** Number of users connected to this channel (USERS DON'T CHANGE)*/
  short ConnectedUsers;

  /*** The number of channels the room has open*/
  short OpenChannels;

  /*** TRUE if the user is a member of this channel*/
  NPS_LOGICAL UserIsConnected;

  /*** User settable channel information*/
  char ChannelData[NPS_CHANNEL_DATA_SIZE];

  /*** The number of players with the GAME_READY flag set*/
  unsigned short NumberOfReadyPlayers;

  /*** The max number of ready players allowed*/
  unsigned short MaxReadyPlayers;

  /*** The UserID of the owner of the channel*/
  NPS_USERID MasterUserId;

  /*** NPS_SERVER_NOT_RUNNING, NPS_SERVER_START_PENDING, or NPS_SERVER_RUNNING*/
  char GameServerIsRunning;

} NPS_RiffInfo;

typedef struct _NPS_MiniRiffInfo
{
  /*** The RIFF name*/
  char Riff [NPS_RIFF_NAME_LEN];

  /*** The CommId of the RIFF*/
  NPS_COMMID CommId;

  /*** Number of users connected to this channel (USERS DON'T CHANGE)*/
  short ConnectedUsers;

} NPS_MiniRiffInfo;


/**
 * NPS_GameServersInfo is the message passed back in response to
 * GetGameServersList (NPS_GAME_SERVERS_LIST).
 */
typedef struct _NPS_GameServersInfo
{
  char GameServerGroupDescription[NPS_DESCRIPTION_LEN + 1];
  char GameServerName[NPS_SYSTEMNAME_LEN + 1];
  char GameServerIp[NPS_HOSTNAME_LEN + 1];
} NPS_GameServersInfo;

typedef struct _NPS_ClientCount
{
  /*** The ID of the channel*/
  NPS_COMMID CommId;

  /*** The number of people on it (TODO THIS NEEDS TO BE A SHORT!)*/
  NPS_USERCOUNT Count;
} NPS_ClientCount;

typedef struct _NPS_ChannelClosed
{
  /*** The ID of the channel*/
  NPS_COMMID CommId;

  /*** The reason it was closed*/
  int ErrCode;
} NPS_ChannelClosed;

typedef struct _NPS_UserLeftChannel
{
  /*** The User ID of the player that left*/
  NPS_USERID UserId;

  /*** The channel they left*/
  NPS_COMMID CommId;
} NPS_UserLeftChannel;

typedef struct _NPS_UserJoinedChannel
{
  /*** The permanent name the user will have on this server */
  char UserName [NPS_USERNAME_LEN];

  /*** The User ID of the player that joined*/
  NPS_USERID UserId;

  /*** The channel they joined*/
  NPS_COMMID CommId;

  /*** User defined data blob */
  char UserData [NPS_USERDATA_LEN];

} NPS_UserJoinedChannel;

typedef struct _NPS_RunningServerInfo
{
  /*** The RIFF of the comm channel attached to the server*/
  char Riff[NPS_RIFF_NAME_LEN];

  /*** The CommId of the comm channel attached to the server*/
  NPS_COMMID CommId;

  /*** The IP address of the machine the server's running on*/
  char IpAddress[NPS_IPADDR_LEN];

  /*** The connection port for the server*/
  unsigned long Port;

  /*** For a game server only, the assigned user ID*/
  NPS_USERID UserId;

  /*** Number of players in this game*/
  NPS_USERCOUNT NumberOfPlayers;

  /*** The socket to send messages on (parent -> child)*/
  SOCKET ServerSocket;

  struct _NPS_RunningServerInfo *Prev;

  struct _NPS_RunningServerInfo *Next;
} NPS_RunningServerInfo;

typedef struct _NPS_StartGame
{
  /*** The CommId of the channel for the game*/
  NPS_COMMID CommId;

  /**
   * if set, only rebroadcast to users who have their ready flag set
   *
   * @see NPSSetGameReady
   */
  NPS_LOGICAL UseReadyList;

  /*** Make this structure byte aligned*/
  NPS_LOGICAL Padding;
} NPS_StartGame;

typedef struct _NPS_ChannelCreated
{
  /*** The CommId of the new channel*/
  NPS_COMMID CommId;

  /*** The RIFF of the new channel*/
  char Riff[NPS_RIFF_NAME_LEN];

  /*** The protocol the new channel is using*/
  unsigned long Protocol;

  /*** User settable channel information*/
  char ChannelData[NPS_CHANNEL_DATA_SIZE];

  /*** NPS_PUBLIC_CHANNEL, NPS_PRIVATE_CHANNEL, or NPS_OPAQUE_CHANNEL*/
  short ChannelType;

  unsigned short MaxReadyPlayers;
} NPS_ChannelCreated;

typedef struct _NPS_SetCommChannelFlags
{
  /*** The CommId of the channel you want to affect*/
  NPS_COMMID CommId;

  /*** The flags we want to set on this channel*/
  unsigned long Flags;
} NPS_SetCommChannelFlags;

typedef struct _NPS_ReadyForGame
{
  NPS_COMMID CommId;
  NPS_USERID UserId;
  NPS_LOGICAL IsReady;
  NPS_LOGICAL IsMaster;
} NPS_ReadyForGame;

/**
 * Actual RIFF list data follows this in the message
 */
typedef struct _NPS_RiffListheader
{
  unsigned long StructSize;
  unsigned long NumRiffs;
} NPS_RiffListHeader;

/**
 * Game Servers list data follows this header
 */
typedef struct _NPS_GameGerversListheader
{
  unsigned long StructSize;
  unsigned long NumServers;
} NPS_GameServersListHeader;

typedef struct _NPS_Statisticsheader
{
  long NumUsers;
  long HighWaterMark;
  long NumSockets;
} NPS_StatisticsHeader;

typedef struct _NPS_CommChannelRateHeader
{
  /*** The CommId of the channel to be limited*/
  NPS_COMMID CommId;

  /*** The rate, in milliseconds*/
  long Rate;

  /*** Either NPS_SEND_ON_DUP or NPS_SEND_ON_TIME*/
  unsigned long Flags;
} NPS_CommChannelRateHeader;

typedef struct _NPS_BootUserFromChannel
{
  /*** The CommId of the channel*/
  NPS_COMMID CommId;

  /*** The UserId of the user to be booted out*/
  NPS_USERID UserId;
} NPS_BootUserFromChannel;

typedef struct _NPS_ForceLeaveChannel
{
  /*** The CommId of the channel you're being kicked out of*/
  NPS_COMMID CommId;

  /*** The UserId of the user that kicked them out*/
  NPS_USERID UserId;
} NPS_ForceLeaveChannel;

typedef struct _NPS_LocateUser
{
  /*** The UserId of the user you're looking for*/
  NPS_USERID UserId;
} NPS_LocateUser;

typedef struct _NPS_UserLocation
{
  /*** The UserId of the user*/
  NPS_USERID UserId;

  /*** The number of CommIds the user has open*/
  long NumCommId;

  /*** (actual CommIds follow this header)*/
} NPS_UserLocation;

typedef struct _NPS_GameServerStateInfo
{
  /*** Channel for the server's information*/
  NPS_COMMID CommId;

  /*** for NPS_SERVER_START_PENDING, the UserId of the user that sent it*/
  NPS_USERID UserId;

  /*** NPS_SERVER_NOT_RUNNING, NPS_SERVER_START_PENDING, or NPS_SERVER_RUNNING*/
  char GameServerState;

} NPS_GameServerStateInfo;

typedef struct _NPS_UserListHeader
{
  NPS_COMMID CommId;
  NPS_USERCOUNT NumberOfUsers;
} NPS_UserListHeader;

typedef struct _NPS_SlotListHeader
{
  NPS_COMMID      CommId;
  NPS_USERCOUNT   NumberOfUsers;
}
NPS_SlotListHeader;

typedef struct _NPS_SlotInfo
{
  NPS_COMMID     CommId;        /*** The CommId of this channel*/
  NPS_USERID     UserId;        /*** The UserId of the user*/
  long           SlotNumber;    /*** The user's slot number*/
  NPS_LOGICAL    AddUser;       /*** TRUE if this is an add, FALSE if this is a remove*/
} NPS_SlotInfo;

typedef struct _NPS_ChannelMaster
{
  NPS_COMMID      CommId;
  NPS_USERID      MasterUserId;
} NPS_ChannelMaster;


typedef struct _NPSArg_ConnectToServer
{
  /**
   * The Domain Name or IP Address of the server to connect
   */
  char
    HostName[NPS_HOSTNAME_LEN];
  /**
   * The Port to connect on
   */
  int
    Port;
  /**
   * Special instructions about how the server should behave and/or
   * set defaults
   */
  unsigned long
    ServerFlags;

  /**
   * The initial client descripiton sent to the server
   */
  NPS_ClientInfo
    ClientInfo;

  /**
   * Callbacks the server delivers messages to
   */
  NPS_ServerCallbackInfo
    ServerCallbacks;

  /**
   * Called during connection wait states.  Return TRUE to cancel the connection
   * attempt.  Involked from a separate thread so it is not safe to
   * use this method to update GUI's.
   */
  tfIdleCallBack
    IdleCallback;
  /**
   * Context always passed to idleCallback
   */
  void *
    IdleContext;

#if defined (DO_CRYPTO)
  /*
   * The session key from the Login client/dll.
   * (NPS_SessionKey)
   */
  void *skey;

  /*
   * The customer ID from the Login client/dll.  This is NOT the persona
   * for this shard.
   */
  NPS_CUSTOMERID CustomerID;

  /*
   * The shard ID from the Login client/dll.
   */
  NPS_SHARDID ShardID;
#endif

  /*
   * Should only set to TRUE when connecting to the PLS.
   */
	bool	QUse;

	/*
   * Position in connection queue, 0 if not set yet.
   */
	volatile short * QPostion;

	/*
   * Number of people waiting in connection queue, 0 if not set yet.
   */
	volatile short * QCount;

}NPSArg_ConnectToServer;

typedef struct _NPSArg_AddCommChannel
{
  /**
   * The Id returned by NPSConnectToServer
   */
  NPS_SERVID
    ServerId;
  /**
   * NPSTCP or NPSUDP
   */
  int
    Protocol;
  /**
   * The ID of this channel
   */
  NPS_COMMID
    CommId;
  /**
   * Special instructions about how the channel should behave and/or
   * set defaults
   */
  unsigned long
    ChannelFlags;
  /**
   * The RIFF of this channel
   */
  char
    Riff[NPS_RIFF_NAME_LEN];
  /**
   * Port to build the socket on.  Set to 0 for none.
   */
  int
    Port;
  /**
   * The password for a private comm channel
   */
  char
    Password[NPS_PASSWORD_LEN];
  /**
   * SKU of the game requesting launch
   */
  long
    SKU;
  /**
   * If rate limiting, this is the rate
   */
  long
    SendRate;
  /**
   * User settable channel information
   */
  char
    ChannelData[NPS_CHANNEL_DATA_SIZE];
  /**
   * Max number of players allowed
   */
  unsigned short
    MaxReadyPlayers;
  /**
   * Possible Values:
   * <UL>
   * <LI>NPS_PUBLIC_CHANNEL
   * <LI>NPS_PRIVATE_CHANNEL
   * <LI>NPS_OPAQUE_CHANNEL
   * </UL>
   */
  short
    ChannelType;
  /**
   * Called during connection wait states.  Return TRUE to cancel the connection
   * attempt.  Involked from a separate thread so it is not safe to
   * use this method to update GUI's.
   */
  tfIdleCallBack
    IdleCallback;
  /**
   * Context always passed to idleCallback
   */
  void *
    IdleContext;
}NPSArg_AddCommChannel;

#endif // _NPS_DLL_TYPES_H
