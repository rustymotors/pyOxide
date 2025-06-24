/**
* @author jfawcett@ea.com
* @author cwalters@ea.com
* @author abakken@ea.com
*
* @version 2.0
*
* Created:  1/10/97
*/

#ifndef _NPSDLL_H_
#define _NPSDLL_H_

#include "MessageTypes.h"
#include "NPSDll_Types.h"

#ifndef NO_VERSION
/*** Major.API.Struct.Code*/
const char * const NPSDLL_VERSION_ID = "4.5.0.0";
#endif

//#ifdef __cplusplus
//#define NPSAPI_Interface class
//#else
#define NPSAPI_Interface  struct
#define NPSAPI_Method     public: virtual

//#endif

#ifdef DOXYGEN
/**
 * The NPS API is now contained within this structure which is passed to the user
 * through a call to NPSAPI_GetInterface().
 *
 * @author cwalters@ea.com
 * @version 4.2.0.0
 * @since 4.2.0.0
 */
class cNPSAPI
#else
NPSAPI_Interface cNPSAPI
#endif
{
  /**
   * Check the version.
   *
   * @param version_string Use the string constant NPSDLL_VERSION_ID
   *
   * @return (BOOL) TRUE if versions match
   */
  NPSAPI_Method BOOL
    NPSDLL_CheckVersionString (char *version_string);

  /**
   * Creates a connection to a lobby server.
   *
   * @param arg        Connection information
   *
   * @return (NPS_SERVID) A unique identifier for this user's connection to the server if
   *         greater than or equal to 0.  If less than 0, value represents an NPSSTATUS error
   *         code
   * @since NPS 1.0
   */
  NPSAPI_Method NPS_SERVID
    NPSConnectToServer (NPSArg_ConnectToServer * arg);

  /**
   * Adds user to a room (doesn't use slots).
   *
   * @param arg        Connection information
   *
   * @param SlotNumber  The user requested slot number (-1 for any)
   *
   * @param SlotFlags   Informs the server how to act if the slot is full
   *
   * @return (NPS_COMMID)  The comm Id of the new room or if less than 0, the NPSSTATUS of
   *         the error.
   */
  NPSAPI_Method NPS_COMMID
    NPSAddCommChannel (NPSArg_AddCommChannel * arg,long SlotNumber=-1L,teNPS_SLOT_FLAGS SlotFlags= NPS_SLOT_ANY);

  /**
   * Request to be assigned to a slot position
   *
   * @param ServerId   The server ID for the parent channel
   *
   * @param CommId     The room Id
   *
   * @param SlotNumber The user requested slot number (-1 for any)
   *
   * @param SlotFlags  Flags that tell the server how to act if the slot is full
   *
   * @return (NPSSTATUS)
   */
  NPSAPI_Method NPSSTATUS
    NPSRequestSlotPosition (NPS_SERVID ServerId,NPS_COMMID CommId,long SlotNumber,unsigned long SlotFlags);

  /**
   * Request your slot position be freed, but stay connected to room
   *
   * @param ServerId The server ID for the parent channel
   *
   * @param CommId   Room Id
   *
   * @return (NPSSTATUS)
   */
  NPSAPI_Method NPSSTATUS
    NPSFreeSlotPosition (NPS_SERVID ServerId,NPS_COMMID CommId);

  /**
   * Request the server to open or close comm channel join access.  Monitor the
   * command message callback for acknowledgement that the room was opened or closed.
   *
   * @param ServerId The parent room server
   *
   * @param CommId   The room to open or close
   *
   * @param IsClosed TRUE to close, FALSE to open
   *
   * @return (NPSSTATUS) NPS_OK if request was queued to send.
   */
  NPSAPI_Method NPSSTATUS
    NPSSetChannelClosed (NPS_SERVID ServerId,NPS_COMMID CommId,NPS_LOGICAL IsClosed);

  // Return the list of users at slots on a specific channel
  // Inputs:
  //    ServerId:  The server ID for the parent channel
  //    CommId:    The communications channel you wish to query (zero for the
  //                      primary channel)
  // Outputs:
  //    Status:    NPS_ERR for failure, otherwise NPS_OK
  NPSAPI_Method NPSSTATUS
    NPSGetSlotList (NPS_SERVID ServerId,NPS_COMMID CommId);

  // Connect to the server.  The first connection for a user MUST be TCP.
  // Inputs:
  //    ServerId:  The server ID for the parent channel
  //    CommId:    The communications channel you wish to query (zero for the
  //                      primary channel)
  // Outputs:
  //    Protocol:  NPSTCP or NPSUDP
  //    CommRiff:  The Riff for this channel
  //    HostName:  Host name of server
  //    Port:      Port connected on
  //    UserData:  User info struct
  NPSAPI_Method NPSSTATUS
    NPSQueryCommChannel (NPS_SERVID ServerId,NPS_COMMID CommId,int *Protocol,char *CommRIFF,char *HostName,int *Port,NPS_UserInfo *UserData);

  /**
   * Close a logical communications channel (a.k.a. a room).  If the channel is TCP
   * the socket doesn't close because channel zero is always active while the server
   * is connected (call NPSLogOffServer to close the TCP channel).
   * If the channel is UDP, and the last UDP channel open, the UDP socket is marked
   * for closure (socket closure always happens in the listening thread, so the socket
   * may still be open for some time after this method returns success).
   *
   * @param ServerId The server the comm channel is on
   *
   * @param CommId   The Id of the room to leave.  If zero, this will return immediatly with NPS_OK,
   *                 and do nothing (to close channel zero, you must log off the server).
   *
   * @return (NPSSTATUS)
   */
  NPSAPI_Method NPSSTATUS
    NPSCloseCommChannel (NPS_SERVID ServerId,NPS_COMMID CommId);

  /**
   * Close the connection to a server.  The connection is immediately removed from
   * the connection queue.  This method does not wait for a server reply.
   *
   * @param ServerId The server to disconnect from.  This is the Id returned from NPSConnectToServer
   *
   * @return (NPSSTATUS) NPS_OK if message was sent to server
   *
   * @see NPSAPI::NPSConnectToServer# NPSConnectToServer
   */
  NPSAPI_Method NPSSTATUS
    NPSLogOffServer (NPS_SERVID ServerId);

  /**
   * Request the user list for a comm channel (a.k.a. Room).  Monitor the command
   * callback for a NPS_USER_LIST message.
   *
   * @param ServId server ID returned from NPSConnectToServer
   *
   * @param CommId The room Id
   *
   * @return (NPSSTATUS) NPS_OK if the request was sent.
   */
  NPSAPI_Method NPSSTATUS
    NPSGetUserList (NPS_SERVID ServId,NPS_COMMID CommId);

  /**
   * Request the Mini user list for the specified room.  Monitor the command callback
   * for a NPS_MINI_USER_LIST message.
   *
   * @param ServId The roomserver Id
   *
   * @param CommId The room Id
   *
   * @return (NPSSTATUS) NPS_OK if the request is queued to send
   */
  NPSAPI_Method NPSSTATUS
    NPSGetMiniUserList (NPS_SERVID ServId,NPS_COMMID CommId);

  /**
   * Requests the number of users on a RoomServer. Monitor the command callback for
   * a NPS_USER_COUNT message.
   *
   * @param ServerId The room server to query
   *
   * @return (NPSSTATUS) NPS_OK if the message was queued for send.
   */
  NPSAPI_Method NPSSTATUS
    NPSGetUserCount (NPS_SERVID ServerId);

  /**
   * Request the user count of a room.  Monitor the command message callback for a
   * reply.
   *
   * @param ServerId The server on which the room lives
   *
   * @param CommId   The commId of the room to get user count from.  If 0, the control channel count
   *                 is returned which is the number of users connected to the server.
   *
   * @return (NPSSTATUS) NPS_OK if the request was sent
   */
  NPSAPI_Method NPSSTATUS
    NPSGetCommChannelCount (NPS_SERVID ServerId,NPS_COMMID CommId);

  // Force start the game
  // inputs:
  //   ServerId:   The server ID for the parent channel
  //   CommId:     The CommId for the game
  //   UseReadyList: If TRUE, the NPS_START_GAME command will be sent only to
  //                 those players who have called NPSReadyForGame
  // outputs:
  //   The results of this command will come back throught the registered
  //   command callback.
  NPSAPI_Method NPSSTATUS
    NPSStartGame (NPS_SERVID ServerId,NPS_COMMID CommId,NPS_LOGICAL UseReadyList);

  // Send a game ready message
  // inputs:
  //   ServerId:   The server ID for the parent channel
  //   CommId:     The CommId for the game
  //   IsReady:    TRUE if the player is ready to play
  // outputs:
  //   This command has no outputs.
  NPSAPI_Method NPSSTATUS
    NPSReadyForGame (NPS_SERVID ServerId,NPS_COMMID CommId,NPS_LOGICAL IsReady);

  // Launch a transient game server
  // inputs:
  //   ServerId:   The server ID for the parent channel
  //   CommId:     The server iD
  // outputs:
  //   The results of this command will come back throught the registered
  //   command callback.    You will receive a NPS_SERVER_INFO message
  //   containing the server connection information for the launched
  //   game server
  NPSAPI_Method NPSSTATUS
    NPSStartGameServer (NPS_SERVID ServerId,NPS_COMMID CommId);

  // Launch a transient game server on a specified host
  // inputs:
  //   ServerId:   The server ID for the parent channel
  //   CommId:     The server iD
  //   Host:       IP Address of desired host
  // outputs:
  //   The results of this command will come back throught the registered
  //   command callback.    You will receive a NPS_SERVER_INFO message
  //   containing the server connection information for the launched
  //   game server
  NPSAPI_Method NPSSTATUS
    NPSStartGameServerOnHost (NPS_SERVID ServerId,NPS_COMMID CommId,char *Host);

  // Terminate a transient game server
  // inputs:
  //   ServerId:   The server ID for the parent channel
  //   CommId:     The server iD
  // outputs:
  //   The results of this command will come back throught the registered
  //   command callback.    You will receive a NPS_GAME_SERVER_TERMINATED message
  //   containing the CommId for the terminated
  //   game server
  NPSAPI_Method NPSSTATUS
    NPSTerminateGameServer (NPS_SERVID ServerId,NPS_COMMID CommId);

  /**
   * Get users local copy of their user data for a server.  User data can be different
   * between servers.
   *
   * @param ServerId The server to query
   *
   * @param UserData The user data stored locally on the given server
   *
   * @return (NPSSTATUS)
   */
  NPSAPI_Method NPSSTATUS
    NPSGetMyUserData (NPS_SERVID ServerId,NPS_UserInfo *UserData);

  /**
   * Send a user data update to the server.  The users name and ID are not changed.
   * The data is not sent if it does not differ from the local copy store for the
   * given server.
   *
   * @param ServerId The server to change the user data on
   *
   * @param UserData The new user data
   *
   * @return (NPSSTATUS) NPS_OK if the message is queued to send
   */
  NPSAPI_Method NPSSTATUS
    NPSSetMyUserData (NPS_SERVID ServerId,NPS_UserInfo *UserData);

  // Get my user data from the server
  // Inputs:
  //    ServerId:  The primary communications channel for this user
  // Outputs:
  //    UserData:  The user data for the user
  //    NPSSTATUS: Status
  NPSAPI_Method NPSSTATUS
    NPSGetUserData (NPS_SERVID ServerId,NPS_USERID UserId,NPS_UserInfo *UserData);

  // Get my user data from the server.  Server returns a NPS_USER_DATA message
  // Inputs:
  //    ServerId:  The primary communications channel for this user
  // Outputs:
  //    UserData:  The user data for the user
  //    NPSSTATUS: Status
  NPSAPI_Method NPSSTATUS
    NPSGetUserInfo (NPS_SERVID ServerId,NPS_USERID UserId);

  // Send a message to the other players
  // Inputs:
  //    ServerId:  The server ID for the parent channel
  //    CommId:    The communications channel to send the message on
  //    Type:      Message type {NPS_SEND_ALL | NPS_SEND_SINGLE | NPS_SEND_LIST
  //                             | NPS_SEND_PLUG_IN | NPS_SEND_BUDDY }
  //    UserList:  List of user IDs to send the message to if NPS_SEND_LIST
  //    UserListLength: Length of UserList
  //    Blob:      Message to send (Binary Large Object)
  //    BlobLength: Length of blob
  // Outputs:
  //    NPSSTATUS: Status
  //
  // NPSSendLoggedMessage is the same as SendMessage except that a log of the
  // message will be made as it passes through the server.
  NPSAPI_Method NPSSTATUS
    NPSSendMessage (NPS_SERVID ServId,NPS_COMMID CommId,int Type,const NPS_USERID *UserList,int UserListLength,const char *Blob,int BlobLength);

  NPSAPI_Method NPSSTATUS
    NPSSendLoggedMessage (NPS_SERVID ServId,NPS_COMMID CommId,int Type,NPS_USERID *UserList,int UserListLength,const char *Blob,int BlobLength);

  // Send a server command
  // Inputs:
  //    ServerId:  The server ID for the parent channel
  //    CommId:    The communications channel to send the message on
  //    Cmd:       The command to send to the server (see rebroadcaster_types.h)
  //    Blob:      Arguments for the command
  //    BlobLength: The length of the blob
  NPSAPI_Method NPSSTATUS
    NPSSendCommand (NPS_SERVID ServerId,NPS_COMMID CommId,int Cmd,void *Blob=NULL,int BlobLength=0);

  /**
   * Return the last socket error we had
   *
   * @return The error code.  You'll have to decipher the meaning
   */
  NPSAPI_Method int
    NPSGetLastCommError (void);

  /**
   * Initialize Comm channel profile
   *
   * @param ServId      The server ID for the parent channel
   *
   * @param CommId      The communications channel to send the message on
   *
   * @param ReadOrWrite Set to NPSREADPROFILE or NPSWRITEPROFILE
   *
   * @return (NPSSTAUS)
   */
  NPSAPI_Method NPSSTATUS
    NPSInitializeCommProfile (NPS_SERVID ServId,NPS_COMMID CommId,int ReadOrWrite);

  // Get Comm Profile Data
  // Inputs:
  //    ServerId:  The server ID for the parent channel
  //    CommId:    The communications channel to send the message on
  //    Object:    A pointer to a PktProfileObject in which to store the
  //               information
  //    ReadOrWrite: Set ReadOrWrite to NPSREADPROFILE or NPSWRITEPROFILE
  // Outputs:
  //    An error if you've specified an invalid server
  NPSAPI_Method NPSSTATUS
    NPSGetCommProfile (NPS_SERVID ServId,NPS_COMMID CommId,PktProfileObject *Object,int ReadOrWrite);

  // Monitor functions -- not for applications

  // Get the list of communications channels on the server
  // Inputs:
  //    ServerId:  The server ID for the parent channel
  // Outputs:
  //    An error if you've specified an invalid server
  NPSAPI_Method NPSSTATUS
    NPSGetRIFFList (NPS_SERVID ServerId);

  // Get the list of communications channels on the server
  // Inputs:
  //    ServerId:  The server ID for the parent channel
  // Outputs:
  //    An error if you've specified an invalid server
  NPSAPI_Method NPSSTATUS
    NPSGetMiniRIFFList (NPS_SERVID ServerId);

  /**
   * Requests a list of all game servers.  Reply returned through given servers
   * command message callback.
   *
   * @param ServerId The parent server of the game servers that will be returned
   *
   * @return (NPSSTATUS) NPS_OK if request was sent to the server
   */
  NPSAPI_Method NPSSTATUS
    NPSGetGameServersList (NPS_SERVID ServerId);

  // Return Rebroadcaster Statistics
  // Inputs:
  //    ServerId:  The server ID for the parent channel
  // Outputs:
  //    An error if you've specified an invalid server
  NPSAPI_Method NPSSTATUS
    NPSGetStatistics (NPS_SERVID ServerId);

  // Restarts a Room Server
  // Inputs:
  //    ServerId:  The server ID for the room to restart
  NPSAPI_Method NPSSTATUS
    NPSRestartRoomServer (NPS_SERVID ServerId,NPS_SERVID RoomServerId);

  // Return userinfo for the users on the server
  // Inputs:
  //    ServerId:  The server ID for the parent channel
  //    NumUsers:  The maximum number of users to get data for
  // Outputs:
  //    NumUsers:  The number of users returned (max = NumUsers input)
  //    UserData:  NPS_UserInfo structs for up to NumUsers players
  // Returns:
  //    An error if you've specified an invalid server
  NPSAPI_Method NPSSTATUS
    NPSGetUsersOnServer (NPS_SERVID ServerId,int *NumUsers,NPS_UserInfo UserData[]);

  // Get the ServerInfo for a given room/game server
  // Inputs:
  //    ServerId:  The server ID for the parent channel
  //    CommId:    The communications channel to send the message on
  // Outputs:
  //    The results of this command will come back throught the registered
  //    command callback.    You will receive a NPS_SERVER_INFO message
  //    containing the server connection information for the launched
  //    game server
  // Returns:
  //    An error if you've specified an invalid server
  NPSAPI_Method NPSSTATUS
    NPSGetServerInfo (NPS_SERVID ServId,NPS_COMMID CommId);

  // Set comm channel flags
  // Inputs:
  //    ServerId:  The server ID for the parent channel
  //    CommId:    The communications channel to send the message on
  //    Flags:     The flags to set.  See NPSTypes.h for valid flags
  // Outputs:
  //    An error if you've specified an invalid server or invalid channel
  NPSAPI_Method NPSSTATUS
    NPSSetCommChannelFlags (NPS_SERVID ServerId,NPS_COMMID CommId,unsigned long Flags);

  /**
   * Get comm channel flags stored locally.
   *
   * @param ServerId The server ID returned from NPSConnectToServer
   *
   * @param CommId   The comm channel to query
   *
   * @param Flags    (out) Returns the flags that are set locally
   *
   * @return
   */
  NPSAPI_Method NPSSTATUS
    NPSGetCommChannelFlags (NPS_SERVID ServerId,NPS_COMMID CommId,unsigned long *Flags);

  // Set flags for server behaviour
  // Inputs:
  //    ServerId:  The server ID
  //    Flags:     The flags to set.  See NPSTypes.h for valid flags
  // Outputs:
  //    An error if you've specified an invalid server
  NPSAPI_Method int
    NPSSetServerFlags (NPS_SERVID ServerId,unsigned long Flags);

  // Get the users on a comm channel that have set the GAME_READY flag
  // Inputs:
  //    ServerId:  The server ID for the parent channel
  //    CommId:    The communications channel to send the message on
  // Outputs:
  //    Data is returned through the command callback.  Messages of type
  //    NPS_READY_FOR_GAME will be returned
  NPSAPI_Method NPSSTATUS
    NPSGetReadyList (NPS_SERVID ServerId,NPS_COMMID CommId);

  // Set the client side select () timeout rate
  NPSAPI_Method void
    NPSSetTimeoutRate (int Sec,int Usec);

  // Send the list of all attached servers the primary server knows about
  NPSAPI_Method NPSSTATUS
    NPSGetServerList (NPS_SERVID ServerId);

  /**
   * Server coallesc data on a comm channel.
   *
   * @param ServId The server ID for the parent channel
   *
   * @param CommId The communications channel to limit
   *
   * @param Rate   The rate, in milliseconds, to send the data
   *
   * @param Flags  Must have One of
   *               <UL>
   *               <LI>NPS_SET_SERVER_RATE -- Have the server gather the messages for distribution
   *               <LI>NPS_SET_DLL_RATE    -- Collect outgoing messages in the DLL
   *               and send them at the specified interval.  BITWISE OR with:
   *               <UL>
   *               <LI>NPS_SEND_ON_DUP -- Send if we get a second message
   *               from a user (regardless of content),
   *               or when the time runs out.  NOTE: This
   *               is only valid withNPS_SET_SERVER_RATE.
   *               Sending with NPS_SET_DLL_RATE will cause an error return.
   *               <LI>NPS_SEND_ON_TIME -- Send when the time runs out.  All messages between time
   *               intervals will be buffered.
   *               </UL>
   *               </UL>
   *
   * @return
   *
   * @see NPSSetServerFlags
   */
  NPSAPI_Method NPSSTATUS
    NPSSetCommChannelRate (NPS_SERVID ServId,NPS_COMMID CommId,long Rate,unsigned long Flags);

  // Set the heartbeat timeout for the DLL or the server
  // Inputs:
  //    ServerId:  The server ID for the parent channel
  //    Flags:     Must have One of:
  //                     NPS_SET_SERVER_RATE -- Set the timeout on the server
  //                     NPS_SET_DLL_RATE -- Set the timeout in the DLL
  //    Timeout:   The new heartbeat timeout in seconds.
  // Outputs:
  //    None
  NPSAPI_Method NPSSTATUS
    NPSSetHeartbeatTimeout (NPS_SERVID ServerId,unsigned long Flags,unsigned long Timeout);

  // Get the heartbeat timeout for the DLL or the server
  // Inputs:
  //    ServerId:  The server ID for the parent channel
  //    Flags:     Must have One of:
  //                     NPS_SET_SERVER_RATE -- Set the timeout on the server
  //                     NPS_SET_DLL_RATE -- Set the timeout in the DLL
  //    Timeout:   The current heartbeat timeout in seconds.
  // Outputs:
  //    None
  NPSAPI_Method NPSSTATUS
    NPSGetHeartbeatTimeout (NPS_SERVID ServerId,unsigned long Flags,unsigned long *Timeout);

  // Change the channel data for a channel you belong to
  // Inputs:
  //    ServerId:  The server ID for the parent channel
  //    ChannelInfo: The new channel data.
  // Outputs:
  //    None.  This routine will send a request to the server to change the
  //     channel.
  //    The channel will only really be changed in response to a message from the
  //    server.
  NPSAPI_Method NPSSTATUS
    NPSSetCommChannelData (NPS_SERVID ServerId,NPS_ChannelInfo *ChannelInfo);

  /**
   * Request the comm channel data (a.k.a. room data) from the server.  Monitor the
   * CommandMsg callback for a reply.
   *
   * @param ServerId Server the room is on
   *
   * @param CommId   The comm Id of the room to get the data for
   *
   * @return (NPSSTATUS) NPS_OK if the request was sent
   * @since NPS 1.0
   *
   * @deprecated Not Implemented
   */
  NPSAPI_Method NPSSTATUS
    NPSGetCommChannelData (NPS_SERVID ServerId,NPS_COMMID CommId);

  /**
   * Set the callbacks for the given server
   *
   * @param ServerId   The server to query
   *
   * @param ServerData (NPS_ServerCallbackInfo) The servers callbacks
   *
   * @return (NPSSTATUS) NPS_OK if Server was found and callbacks set
   */
  NPSAPI_Method NPSSTATUS
    NPSSetServerData (NPS_SERVID ServerId, const NPS_ServerCallbackInfo *CallbackInfo);

  /**
   * Get the callbacks currently set for the given server
   *
   * @param ServerId   The server to query
   *
   * @param ServerData (NPS_ServerCallbackInfo) The servers callbacks
   *
   * @return (NPSSTATUS) NPS_OK if ServerData was filled in
   */
  NPSAPI_Method NPSSTATUS
    NPSGetServerData (NPS_SERVID ServerId,NPS_ServerCallbackInfo *CallbackInfo);

  /**
   * Send a file to the server plug in point.
   *
   * @param ServerId The server to send the file to
   *
   * @param FileName The dir/name of the file to send
   *
   * @return (NPSSTATUS)
   */
  NPSAPI_Method NPSSTATUS
    NPSSendFileToPlugIn (NPS_SERVID ServerId,char *FileName);

  /**
   * Force a user out of a room
   *
   * @param ServerId The server the comm channel is on
   *
   * @param CommId   The room the user is in
   *
   * @param UserId   The user to force out
   *
   * @return (NPSSTATUS)
   */
  NPSAPI_Method NPSSTATUS
    NPSBootUserFromChannel (NPS_SERVID ServerId,NPS_COMMID CommId,NPS_USERID UserId);

  /**
   * Force a user off a server
   *
   * @param UserId   The user to force out
   *
   * @return (NPSSTATUS)
   */
  NPSAPI_Method NPSSTATUS
    NPSForceLogoff (NPS_SERVID ServerId, NPS_USERID UserId);

  /**
   * Ask the server to tell you all the comm channels a player is attached to
   *
   * @param ServerId The server to search on
   *
   * @param UserId   user to find
   *
   * @return NPS_OK if query was sent
   */
  NPSAPI_Method NPSSTATUS
    NPSLocatePlayer (NPS_SERVID ServerId,NPS_USERID UserId);

  /**
   * Request the list of master user Id's for all rooms on the server.
   * List is returned in the command callback.
   *
   * @param ServId (NPS_SERVID) server to retrieve the list for.
   *
   * @return (NPSSTATUS) NPS_OK if no problems sending the request to the server.
   */
  NPSAPI_Method NPSSTATUS
    NPSGetMasterList (NPS_SERVID ServId);

  /**
   * Gag a user on this server - this will need to be repeated for other servers
   *
   * @param ServerId The server the user is on
   *
   * @param UserId   The user to gag
   *
   * @return (NPSSTATUS)
   */
  NPSAPI_Method NPSSTATUS
    NPSGagUser (NPS_SERVID ServerId, NPS_USERID UserId);

  /**
   * Ungag a user on this server - this will need to be repeated for other servers
   *
   * @param ServerId The server the user is on
   *
   * @param UserId   The user to ungag
   *
   * @return (NPSSTATUS)
   */
  NPSAPI_Method NPSSTATUS
    NPSUngagUser (NPS_SERVID ServerId, NPS_USERID UserId);

  /**
   * Set the max number of users allowed to enter this server
   *
   * @param ServerId The server to limit
   *
   * @param Count What the limit is (0 for no limit)
   *
   * @return (NPSSTATUS)
   */
  NPSAPI_Method NPSSTATUS
    NPSSetMaxUsers (NPS_SERVID ServerId, long count);

  /**
   * Set the max number of channels for this server
   *
   * @param ServerId The server to limit
   *
   * @param Count What the limit is (0 for no limit)
   *
   * @return (NPSSTATUS)
   */
  NPSAPI_Method NPSSTATUS
    NPSSetMaxChannels (NPS_SERVID ServerId, long count);

  /**
   * Get a string description of an NPSSTATUS error
   *
   * @param status
   *
   * @return (const char *) Pointer to an error string
   */
  NPSAPI_Method const char *
    NPSGetErrorString (NPSSTATUS status);

  /**
   * This method adds/removes entries to/from the PLS async user tracking list.
   *
   * @param ServerId Server ID of the primary lobby.
   *
   * @param numIds   Number of IDs passed in
   *
   * @param buddyIds The IDs to add/remove from the list
   *
   * @param addUsers (BOOL)
   *                 <UL>
   *                 <LI>TRUE:  Add IDs to the list.
   *                 <LI>FALSE: Remove IDs from the list.
   *                 </UL>
   *
   * @return (NPSSTATUS) NPS_OK if message is queued to send.
   */
  NPSAPI_Method NPSSTATUS
    NPSBuddyList_Modify(NPS_SERVID ServerId, long numIds, NPS_USERID * buddyIds, BOOL addUsers);

  /**
   * Request the current data for all users the the PLS user tracking list.
   *
   * @param ServerId The ID of the PLS
   *
   * @return (NPSSTATUS) NPS_OK if the request was queued for send.
   */
  NPSAPI_Method NPSSTATUS
    NPSBuddyList_Refresh(NPS_SERVID ServerId);

};

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef DYNAMIC_NPS_DLL

  // ***************************** Function declarations **********************
  // SEP
#ifdef sun
#define DLL_PROCESS_ATTACH    1
#define DLL_THREAD_ATTACH     2
#define DLL_THREAD_DETACH     3
#define DLL_PROCESS_DETACH    4
#define THREAD_PRIORITY_ABOVE_NORMAL  1
#define THREAD_PRIORITY_NORMAL          2

  NPSRebroadcasterDllExport BOOL APIENTRY
    DllMain (DWORD ul_reason_for_call);
#endif

  NPSRebroadcasterDllExport cNPSAPI* NPSCDECL
    NPSAPI_GetInterface (void);
  NPSRebroadcasterDllExport void  NPSCDECL
    NPSAPI_ReleaseInterface (void);

#else  // DYNAMIC_NPS_DLL
  typedef cNPSAPI*  (NPSCDECL *fNPSAPI_GetInterface)(void);
  typedef void      (NPSCDECL *fNPSAPI_ReleaseInterface)(void);

#endif // DYNAMIC_NPS_DLL

#ifdef __cplusplus
}
#endif

#endif // _NPSDLL_H_
