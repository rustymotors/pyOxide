//
// Name:
//  MessageTypes.h
//
// Designer:
//  Andrew Bakken <abakken>
//
// Copyright (c) 2000 by Electronic Arts.
// All Rights Reserved.
//
// $Header: /export/home/cvsroot/NPS/src/Common/NPSLib/MessageTypes.h,v 1.1 2001/01/12 18:12:00 abakken Exp $
//
//

#ifndef _REBROADCASTER2_MESSAGE_TYPES_H_
#define _REBROADCASTER2_MESSAGE_TYPES_H_

#include <stdio.h>

#include "NPSTypes.h"
#include "NPSDll_Types.h"
#include "NPSPacketProfile.h"

typedef struct _NPS_UserId_List
{
  unsigned long NumUsers;
  NPS_USERID    * UserId;               // User ID - persona id
}
NPS_UserId_List;

typedef struct _NPS_UserId_UserData
{
  NPS_USERID UserId;               // User ID - persona id
  char UserData[NPS_USERDATA_LEN]; // User controllable data (64)
}
NPS_UserId_UserData;

typedef struct _NPS_UserInfo
{
  NPS_USERID UserId;               // User ID - persona id
  char UserName[NPS_USERNAME_LEN]; // User name (32)
  char UserData[NPS_USERDATA_LEN]; // User controllable data (64)
}
NPS_UserInfo;

typedef struct _NPS_MiniUserInfo
{
  NPS_USERID UserId;               // User ID (long)
  char UserName[NPS_USERNAME_LEN]; // User name (32)
}
NPS_MiniUserInfo;

typedef struct _NPS_LoginInfo
{
  NPS_UserInfo UserData;           // The UserInfo for the login
  NPS_CUSTOMERID CustomerId;       // The global ID across all of NPS
  char keyHash [NPS_HASHED_KEY_LEN];
  char HostName[NPS_HOSTNAME_LEN]; // Name of the users computer (64)
  char MyIpAddr[NPS_IPADDR_LEN];   // IP address of the users computer (16)
  unsigned long Flags;             // The initial flags for the server (4)
  char Version[NPS_VERSION_LEN + 1]; // Version string (33)
}
NPS_LoginInfo;

typedef struct _NPS_DllPktTimeMarker
{
  unsigned long msecs;             // Milliseconds (4)
  unsigned long seconds;           // Seconds (4)
}
NPS_DllPktTimeMarker;

typedef struct _NPS_MsgList
{
  char *Message;                   // Pointer to the message (4)
  struct _NPS_MsgList *Next;
  struct _NPS_MsgList *Prev;
}
NPS_MsgList;

typedef struct _NPS_CommData
{
  NPS_COMMID CommId;               // The ID of this channel (4)
  char Riff[NPS_RIFF_NAME_LEN];    // The RIFF of this channel (32)
  long SlotNumber;                 // The slot for this user on this channel (4)
  long SlotFlags;                  // Flags that say how to act if a slot is
                                   // unavailable (4)

  // These next 3 fields are not used on the server
  SOCKET Socket;                   // The socket this channel uses (4)
  struct sockaddr_in pServer;      // If UDP, the server info (16)
  struct sockaddr_in pClient;      // If UDP, the client info (16)

  long Port;                       // The port this channel connected on (4)
  unsigned long Protocol;          // NPSTCP or NPSUDP (4)
  NPS_USERID UserId;               // ID of the owner of this channel (4)
  short ConnectedUsers;            // Number of people on this channel (2)
  short OpenChannels;              // Number of channels on this server (2)
  NPS_LOGICAL CanReady;            // Able to ready for a game (2)
  NPS_LOGICAL GameReady;           // Is the player ready for a game (2)
  NPS_LOGICAL IsMaster;            // Are we the master in this game (2)
  short ChannelType;               // NPS_PUBLIC_CHANNEL, NPS_PRIVATE_CHANNEL,
                                   //  or NPS_OPAQUE_CHANNEL (2)
  PktProfileObject *ReadProfile;   // Receive statistics (4)
  PktProfileObject *WriteProfile;  // Transmit statistics (4)
  struct _serverdata *ParentServer; // The server that owns this channel (4)
  char Password[NPS_PASSWORD_LEN]; // The password if a private channel (8)
  NPS_LOGICAL DisableBacklog;      // TRUE if backlogging messages is (2)
  char GameServerIsRunning;        // Game server state for this channel (1)
  char Dummy;                      // Alignment placeholder (1)
  NPS_LOGICAL LaunchGameServer;    // TRUE if a game server should be launched
                                   //  at channel create (2)
  unsigned short MaxReadyPlayers;  // Max players allowed to ready (2)
  long SKU;                        // SKU of the game requesting launch (4)
  long SendRate;                   // If rate limiting, this is the rate. (4)
  NPS_DllPktTimeMarker LastTimeSent; // The last time a message was sent (8)
  char ChannelData[NPS_CHANNEL_DATA_SIZE]; // User settable channel info (256)
  unsigned long Flags;             // Flags associated with this channel (4)
  NPS_LOGICAL MessageOnList;       // If rate limiting, this flag indicates
                                   //  that a message is already on the pending
                                   //  message list for this user
  NPS_LOGICAL IsBeingRemoved;      // TRUE if in process of being removed (2)
  NPS_MsgList *MessagesToSend;     // Linked list.  Can't use the STL because
                                   //  this file has to be C compatible (4)
  NPS_MSGLEN MetaMsgLen;           // Total length of all messages in
                                   //  MessagesToSend (2)
  short NumMetaMsg;                // Number of messages in MessagesToSend (2)
  NPS_DllPktTimeMarker LastMessageTime; // Last time a message was received
                                        //  from this user (16)
  FILE *CurrentFile;               // If sending a file, this is the handle (4)
  char CurrentFileName[64];        // File name (64)
  struct _NPS_CommData *Prev;
  struct _NPS_CommData *Next;
}
NPS_CommData;

typedef struct _NPS_FileStartInfo
{
  char FileName[NPS_FILENAME_LEN];
  long NumBytes;
}
NPS_FileStartInfo;

typedef struct _NPS_FileData
{
  long SegmentNumber;
  char BinBuf[1000];
  long NumBytes;
}
NPS_FileData;

typedef struct _ReadyForGameInfo
{
  NPS_COMMID CommId;
  NPS_LOGICAL LocalIsReady;
  NPS_LOGICAL Padding;
}
ReadyForGameInfo;

typedef struct _NPS_Login
{
  NPS_USERID connectionID;
  char userName[NPS_USERNAME_LEN];
  char userData[NPS_USERDATA_LEN]; // User controllable data
  unsigned int key;
  unsigned long flags;             // connection flags
  char version[NPS_VERSION_LEN];
}
NPS_Login;

typedef struct _NPS_OpenCommChannelAck
{
  NPS_COMMID commIDAssignedByServer;
  unsigned int key;
}
NPS_OpenCommChannelAck;

typedef struct _NPS_OpenCommChannel
{
  NPS_USERID connectionID;
  NPS_COMMID commID;
  int protocol;
  char riff[NPS_RIFF_NAME_LEN];
  char password[NPS_PASSWORD_LEN];
  char channelData[NPS_CHANNEL_DATA_SIZE];
  unsigned int key;                // their key
  unsigned long flags;             // channel flags
}
NPS_OpenCommChannel;

typedef struct _NPS_GameServerLaunchInfo
{
  NPS_COMMID CommId;               // Comm ID (long)
  char BestHost[NPS_IPADDR_LEN];   // IP address of the users computer (16)
}
NPS_GameServerLaunchInfo;

typedef struct _NPS_ChannelGrantedData
{
  NPS_COMMID CommId;
  long ServerPort;
}
NPS_ChannelGrantedData;

typedef struct _NPS_UDPChannelStatus
{
  NPS_COMMID CommId;
  long       Successful;
}
NPS_UDPChannelStatus;

typedef struct _NPS_SlotRequest
{
  NPS_COMMID       CommId;
  long             SlotNumber;
  unsigned long    SlotFlags;
  NPS_LOGICAL      AddOrRemove;
} NPS_SlotRequest;

typedef struct _NPS_SetChannelClosed
{
  NPS_COMMID       CommId;
  NPS_LOGICAL      IsClosed;
} NPS_SetChannelClosed;

// only used between rebroadcasters for NPS_CHANNEL_CREATED and
// NPS_CHANNEL_DELETED messages
typedef struct _ChannelCountInfo
{
  long ChannelCount;
  NPS_COMMID CommId;
} NPS_ChannelCountInfo;

typedef struct _NPS_RestartRoom
{
  NPS_SERVID RoomServerId;
  NPS_COMMID CommId;
} NPS_RestartRoom;

typedef struct _NPS_Qposition
{
  short position;
  short count;
} NPS_Qposition;

#endif // _REBROADCASTER2_MESSAGE_TYPES_H_
