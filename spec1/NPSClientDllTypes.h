/**
	*
	* NPSClientDLL Types
	*
	* Written by David R. Sullivan
	*
	* Copyright 1998 Electronic Arts, Inc.  All Rights Reserved.
	*/

#ifndef _NPSCLIENTDLLTYPES_H_
#define _NPSCLIENTDLLTYPES_H_

#include "AnalyzeAPI.h"
#include "NPSDll_Types.h"

#define NPSCLIENTDLL_VERSION_ID		"2.0.0.1"		// Major.API.Struct.Code

#define NPSCLIENT_ERROR NPSSTATUS

/**
 * This is only for NPSUserClient (Test Client) to show the user what the default is.
 */
#define LOBBY_SERVER				"NPS-Dev3.ea.com\0"
#define LOBBY_PORT					(8888)	// Default Lobby port to connect with.

#define MASTER_APPPATH_KEY		"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths"
#define MASTER_CLIENT_KEY			"SOFTWARE\\Electronic Arts\\Network Play System\\NPSUSERCLIENT"
#define MASTER_SYSTEM_KEY			"SOFTWARE\\Electronic Arts\\Network Play System\\SYSTEM"
#define LOBBY_IP_KEY					"LobbyServerIP"
#define LOBBY_PORT_KEY				"LobbyServerPort"
#define HEARTBEAT_KEY					"Heartbeats"
#define PROTOCOL_KEY					"Protocol"
#define DATABASE_KEY					"DATABASE_MODE"
#define FORCED_USERID_KEY			"FORCE_USER_ID"
#define SERVER_TIMEOUT_KEY		"SERVER_TIMEOUT"
#define VERBOSE_LEVEL_KEY			"VERBOSE_LEVEL"

#define MAX_RIFFS							(256)			// Maximum Lobbies and/or Rooms
#define MAX_USERS_IN_ROOM			(256)			// Maximum Users Supported
#define MAX_SERVER_RETRY			(3)				// Number of times NPSConnectToServer is called if it fails.

/**
 * This enum is used in the ConnectionLost callback
 */
typedef enum NPSC_SERVER_TYPE
{
	NPS_ROOM,
	NPSC_ROOM_SERVER,
	NPSC_LOBBY_SERVER,
} NPSC_SERVER_TYPE;

// This enum is used in the ConnectionLost callback
typedef enum NPSC_DISCONNECT_REASON
{
	NPSC_FORCED_LOGOFF,
	NPSC_LOST_CONNECTION,
	NPSC_BOOTED,
} NPSC_DISCONNECT_REASON;

// This enum is used when searching for a user name, user id, comm name, comm id, lobby name, or lobby id.
typedef enum NPSC_FINDRIFF
{
	FINDRIFF_NOLOAD  				= 0,				 	// Do not attempt to reload the RIFF list
	FINDRIFF_LOADIFNOTFOUND = 1,					// Load the RIFF list if the RIFF we are searching for is not found
	FINDRIFF_LOADALWAYS 		= 2					 	// Always load the RIFF list before we search for it
} NPSC_FINDRIFF;

// This enum is the users response to an Idle callback.
typedef enum NPSCLIENT_IDLE_STATUS
{
	NPSC_NO_CANCEL 	= 0,								// Idle callback did not cancel
	NPSC_CANCEL_OPERATION,							// If possible, Cancel the pending operation gracefully (allow user to continue using services)
	NPSC_MAX_IDLE
} NPSCLIENT_IDLE_STATUS;

// This enum is for chat type and priority.  NPSC_SYSTEM_x messages are filters based on the users desired security level
typedef enum NPSCLIENT_CHAT_TYPE
{
	NPSC_SYSTEM_3 = -3,								// Used for IncommingSystemMessage (these are levels of detail)
	NPSC_SYSTEM_2 = -2,								// Used for IncommingSystemMessage (these are levels of detail)
	NPSC_SYSTEM_1 = -1,								// Used for IncommingSystemMessage (these are levels of detail)
	NPSC_SYSTEM   =  0,								// System Chat to User (this is actual Chat)
	NPSC_ALL,													// All Users see this
	NPSC_WHISPER,											// A list of users see this
	NPSC_BUDDY,												// A buddy see's this
	NPSC_ALL_LOGGED,
	NPSC_WHISPER_LOGGED,
	NPSC_BUDDY_LOGGED,
	NPSC_MAX_CHAT_TYPE
} NPSCLIENT_CHAT_TYPE;

// This enum identifies what operation a change callback uses.  User in LobbyUserChange, RoomUserChange, RoomChange and LobbyChange callbacks.
typedef enum NPSCLIENT_CHANGE						// These flags are used in the flags field of the callback function
{
	NPSC_ADD_ITEM = 0,								// Add a new item to the list
	NPSC_REMOVE_ITEM,									// Remove an item from the list
	NPSC_UPDATE_ITEM,									// Update or Change an Items Data
	NPSC_ADD_SELECTED_ITEM,						// Add a selected Item (Room that user himself created)
	NPSC_MAX_CHANGE
} NPSCLIENT_CHANGE;

// This enum identifies how the PING system is going to operate.
typedef enum NPSCLIENT_PING
{
	NPSC_PING_NOW = 0,								// Request an Immediate Ping
	NPSC_PING_AUTO,										// Auto-Ping in Low Bandwidth Times
	NPSC_PING_NEVER,									// Never Auto-Ping or Interval Ping
	NPSC_PING_INTERVAL,								// Ping at this Interval
	NPSC_MAX_PING
} NPSCLIENT_PING;

// This enum identifies the type of lobby services the client selects.
typedef enum NPSLOBBY_MODE
{
	NPSC_LOBBY_AS_ROOM = 0,							// Default Mode of NPSClientDLL, Lobby is a Room nothing more
	NPSC_LOBBY_NEVER_LEAVE,							// When LeaveRoom the user is automatically placed in the room "LOBBY"
	NPSC_LOBBY_HIDE_NAME,								// Hides the Lobby name from the API user as well as never leaves
	NPSC_MAX_LOBBY_MODE
} NPSLOBBY_MODE;


// ===============================================================
// Callback Structs
// ===============================================================

#pragma pack( push, 1 )

typedef struct UserDataField
{
	char UserData[NPS_USERDATA_LEN];
} UserDataField;

// This structure is passed to the RoomUserChange and LobbyUserChange callbacks
typedef struct UserChangeInfo
{
	NPS_USERID    UserID;							// ID of user that is changing.
	char      		UserName[NPS_USERNAME_LEN];		// Name of user that is changing
	UserDataField UserData;						// User controllable data
	int       		initFlag;						// If TRUE, initialize the UserList before this record.
	BOOL					isReady;
	BOOL					isMaster;
} UserChangeInfo;

#pragma pack( pop )

typedef struct ChannelInfo
{
	char           Riff[NPS_RIFF_NAME_LEN];	 // The RIFF name
	unsigned long  Protocol;									// The protocol of this RIFF
	NPS_COMMID     CommId;									// The CommId of the RIFF
	char           Password[NPS_PASSWORD_LEN];// Password for this channel
	short          ChannelType;							 // NPS_PUBLIC_CHANNEL, NPS_PRIVATE_CHANNEL,
																					 // or NPS_OPAQUE_CHANNEL
	short          ConnectedUsers;						// Number of users connected to this channel (USERS DON'T CHANGE)
	short          OpenChannels;							// Number of open channels in the server represented by this channel
	NPS_LOGICAL    UserIsConnected;						// TRUE if the user is a member of this channel
	char           ChannelData[NPS_CHANNEL_DATA_SIZE]; // User settable channel information
	unsigned short NumberOfReadyPlayers;			// The number of players with the GAME_READY flag set
	unsigned short MaxReadyPlayers;						// The maximum number of players allowed to ready
	NPS_SlotList   *SlotList;
	NPS_USERID     MasterUserId;
	char           Reserved[NPS_CHANNEL_RESERVED_LEN];	// Initialize to NULL, please
	char           GameServerIsRunning;				// NPS_SERVER_NOT_RUNNING, NPS_SERVER_START_PENDING, or
																						// NPS_SERVER_RUNNING for this channel
} ChannelInfo, RoomInfo;


/**
 * This structure is passed to the RoomChange and LobbyChange callbacks
 */
typedef struct RoomChangeInfo
{
	int       initFlag;						// If TRUE, initialize the RoomList before processing this record
	RoomInfo  Info;
} RoomChangeInfo, LobbyChangeInfo;

/**
 * This structure is passed to the GameStart callback
 */
typedef struct GameStartInfo
{
	NPS_USERID  PlayerNumber;						// PlayerIndex.
	int         NumberOfPlayers;					// Total number of human players
	int         GameServerPort;						// Port to connect to Game Server on...
	char        GameServerIpAddress[NPS_HOSTNAME_LEN + 1]; // IP Address of Game Server.
	char        UserName[NPS_USERNAME_LEN];			// Name of user that is changing
	NPS_USERID  UserId;								// UserID of yourself.
	char        CurrentRoomName[NPS_RIFF_NAME_LEN+1];	// Room Name that you are in...
} GameStartInfo;

/**
 * This defines the list callbacks which are used to maintain user, lobby and room lists
 */
typedef void (__cdecl *tfRoomUserChange_CB)(void *context, UserChangeInfo *info, NPSCLIENT_CHANGE flags);
typedef void (__cdecl *tfLobbyUserChange_CB)(void *context, UserChangeInfo *info, NPSCLIENT_CHANGE flags);
typedef void (__cdecl *tfRoomChange_CB)(void *context, RoomChangeInfo *room, NPSCLIENT_CHANGE flags);
typedef void (__cdecl *tfLobbyChange_CB)(void *context, LobbyChangeInfo *lobby, NPSCLIENT_CHANGE flags);

typedef struct LIST_CBK
{
	tfRoomUserChange_CB		RoomUserChange;
	tfLobbyUserChange_CB	LobbyUserChange; // *** RESERVED FOR FUTURE USE ***
	tfRoomChange_CB				RoomChange;
	tfLobbyChange_CB			LobbyChange;
	void *Context;
} LIST_CBK;


/**
 * This defines the chat system and internal system message callbacks
 */
typedef void (__cdecl *tfUserMessage_CB)(void *context, const char *room, const char *user, NPS_USERID userID, const char *msg, NPSCLIENT_CHAT_TYPE flags);
typedef void (__cdecl *tfSystemMessage_CB)(void *context, const char *msg, NPSCLIENT_CHAT_TYPE flags);

typedef struct CHAT_CBK
{
	tfUserMessage_CB		IncommingChatLine;
	tfSystemMessage_CB  IncommingSystemMessage;
	void *Context;
} CHAT_CBK;

/**
 * These defines are the bitmasks for the UserReadyToPlay callback's RF_Flags
 */
#define RF_MASTER	0x100							// this player is the master of the room
#define RF_READY	0x200							// this player is ready

/**
 * This defines the status callbacks, these callbacks are use to provide status about users, games and the overall service
 */
typedef	void (__cdecl *tfUserReady_CB)(void *context, NPS_USERID UserID, int RF_Flags);	// **** I may roll this up into UserChange ****
typedef	void (__cdecl *tfConnectionLost_CB)(void *context, NPSC_DISCONNECT_REASON reason, NPSC_SERVER_TYPE stype);
typedef	void (__cdecl *tfSleepyTime_CB)(void *context, NPS_USERID UserID, BOOL sleep);			// Called before and after game start
typedef	BOOL (__cdecl *tfGetUserCommandLine_CB)(void *context, NPS_USERID UserID, char *CommandLine, long *CommandLineLength);	// Called when StrartGameServer(TRUE) is used
typedef	void (__cdecl *tfStartGame_CB)(void *context, GameStartInfo GameInfo);					// Called when StartGameServer(FALSE) is used;
typedef	void (__cdecl *tfGameStartInProgress_CB)(void *context);									// Called when an NPS_GAME_SERVER_PENDING message is received.
typedef	NPSCLIENT_IDLE_STATUS (__cdecl *tfIdle_CB)(void *context);								// Called during Process Messages Loop
typedef struct STATUS_CBK
{
	tfUserReady_CB 						UserReadyToPlay;
	tfConnectionLost_CB				ConnectionLost;
	tfSleepyTime_CB						SleepyTime;
	tfGetUserCommandLine_CB		GetUserCommandLine;
	tfStartGame_CB						StartGame;
	tfGameStartInProgress_CB	GameStartInProgress;
	tfIdle_CB									Idle;
	void *Context;
} STATUS_CBK;


/**
 * Callbacks used to search for users, rooms and lobbies.
 */
typedef	void (__cdecl *tfLobbyFound_CB)(void *context, char *riff, NPS_COMMID CommID);
typedef	void (__cdecl *tfRoomFound_CB)(void *context,  char *riff, NPS_COMMID CommID);
typedef	void (__cdecl *tfUserFound_CB)(void *context,  char *name, NPS_USERID UserID);
typedef struct FIND_CBK
{
	tfLobbyFound_CB LobbyFound;
	tfRoomFound_CB	RoomFound;
	tfUserFound_CB	UserFound;
	void *Context;
} FIND_CBK;

#endif
