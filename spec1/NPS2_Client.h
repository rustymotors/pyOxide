//====================================================================
// NPS2_Client.h
//==============
//
// Description:
//
// Programmer(s)
//    - C.R.Walters		---	Initial Programming
//
// Project:
//		NPS
//
// Notes:
//		All methods returning NPSSTATUS may return NPS_ERR which is
//		either a non-referenced problem or an unexpected situation.
//		Please contact your NPS Leasen
//		unexpected or  problem with NPS.
//====================================================================
//				 Copyright 2000 Electronic Arts Seattle.
//====================================================================
#ifndef NPS2_CLIENT_H
#define NPS2_CLIENT_H
#include "MessageTypes.h"
#include "NPSClientDLLTypes.h"
#include "NPSUserLogin.h"
#include "cQ.h"

#define NPS2ClientDllExport
#define NPS2C_CDECL

#if 0
#ifdef BUILDING_NPSCLIENT_DLL
	#define NPS2ClientDllExport	//__declspec( dllexport )
	#define NPS2C_CDECL

	#undef DYNAMIC_NPSUSERCLIENT_DLL
#else
	#define NPS2ClientDllExport	__declspec(dllimport)
	#define NPS2C_CDECL	__cdecl
#endif
#endif

//====================================================================
// Generic
//====================================================================
// -------------------------------------------------------------------
// Description:
// 	An object of this type is a required parameter to:
//			cNPS2C_Database
//			cNPS2C_Lobby
//			cNPS2C_Broadcaster
// 	Inherit this class frame and overide the CancelRequest method to
//			1. Update your GUI.
//			2. Query user input.
//			3. Return TRUE to cancel the current task immediately.
//
// Notes:
//		* 	If you cancel a task, the method you were in will return
//			immediately, but the task may actually finish or need to
//			finish.  In such a case, the necessary means to put you
//			in your original state will be queued up and you will may
//			experience a delay while a connection is closed and reinstated.
//			This can also cause a deley in your next task, since the
//			previous task must complete before your next task is engaged.
// -------------------------------------------------------------------
class cNPS2C_Idle_Callback
{
public:
	virtual ~cNPS2C_Idle_Callback(void){};

	virtual BOOL CB_CancelRequest(void){return FALSE;};
};

void NPS2C_SetIdleCallback(cNPS2C_Idle_Callback * callback);


class cNPS2C_Broadcaster_Callbacks
{
public:
	virtual ~cNPS2C_Broadcaster_Callbacks(void){};

	virtual void
		CB_Message(NPS_COMMID commID, NPS_USERID addr, char *msg, NPS_MSGLEN msgLen){return;};
	virtual void
		CB_ConnectionOpened(NPS_COMMID commID, NPS_USERID addr, char *msg, NPS_MSGLEN msgLen){return;};
	virtual void
		CB_ConnectionClosed(NPS_USERID ID){return;};
};

//====================================================================
// Database
//====================================================================
class NPS2ClientDllExport cNPS2C_Account
{
public:
	virtual ~cNPS2C_Account(void){};

	virtual NPSSTATUS
		Account_LogOn(const char * name, const char * password, const char * gameName)=0;
	virtual void
		Account_LogOff(void)=0;
	virtual BOOL
		Account_IsLoggedOn(void)=0;
	virtual const char *
		Account_GetGameName(void)=0;

	// ---------------------------------------------------------------
	// Creates an NPS Account
	// If logged into another account, user will be logged off first.
	// ---------------------------------------------------------------
	virtual NPSSTATUS
		Account_Create(AccountData *)=0;
	virtual NPSSTATUS
		Account_Modify(AccountData *)=0;

	// Must be logged in for this data to be valid.
	// Do not modify the returned data structure.
	// --------------------------------------------
	virtual const AccountData *
		Account_GetAccountData(void)=0;
};

class NPS2ClientDllExport cNPS2C_Persona
{
public:
	virtual ~cNPS2C_Persona(void){};
	//	Populates the persona list with previously created personas
	// stored in the NPS database.  Should be called immediately
	// after loging in.
	// ------------------------------------------------------------
	virtual NPSSTATUS
		Persona_DownloadList(NPS_CUSTOMERID  CustomerId, const char * gameName)=0;
	virtual cQ *
		Persona_List(void)=0;
	// Creates a new persona in the nps database, Selects it,
	//	and attaches to the primary lobby with said persona.
	// ------------------------------------------------------------
	virtual NPSSTATUS
		Persona_Create(char * name)=0;

	//	Removes a persona from the NPS database.
	// ------------------------------------------------------------
	virtual void
		Persona_Delete(NPS_GAMEUSERID ID)=0;
	virtual void
		Persona_Delete(char * name)=0;
	virtual void
		Persona_Delete(void)=0;					// Removes selected.

	// ------------------------------------------------------------
	// Informs the NPS database that the specified persona is now in
	// use.  This sets the time on-line in motion.
	//	Next step is to attach to the primary lobby with the persona.
	// ------------------------------------------------------------
	virtual NPSSTATUS
		Persona_Use(char * name)=0;
	virtual NPSSTATUS
		Persona_UseSelected(void)=0;

	// Stops tracking time on-line for currently selected persona.
	// ------------------------------------------------------------
	virtual NPSSTATUS
		Persona_Release(BOOL wait4reply = FALSE)=0;

	// Locates the specified persona and copies the information into
	// the UserGameData structure passed in if found.  You must first
	// have made a successful call to Persona_GetAll.
	// --------------------------------------------------------------
	virtual BOOL
		Persona_Find(NPS_GAMEUSERID ID, UserGameData * p)=0;
	virtual BOOL
		Persona_Find(char * name, UserGameData * p)=0;

	virtual BOOL
		Persona_Select(NPS_GAMEUSERID ID)=0;

	virtual const UserGameData *
		Persona_GetSelected(void)=0;
};

typedef struct
{
	BuddyListInfo 	buddyInfo;
	NPS_UserInfo	userInfo;
	BOOL				wasOnline;
}tsContact;

typedef void (*tfNPSCB_GetUserInfo)(NPSSTATUS, NPS_UserInfo*);

class NPS2ClientDllExport cNPS2C_Contact
{
public:
	virtual ~cNPS2C_Contact(void){};

	virtual cQ *
		Contact_List(void)=0;

	virtual NPSSTATUS
		Contact_Add(NPS_USERID buddyID, NPS_USERID myID)=0;
	virtual NPSSTATUS
		Contact_Add(const char * buddyName, NPS_USERID myID, const char * gameName)=0;
	virtual void
		Contact_Remove(NPS_USERID buddyID)=0;
	virtual void
		Contact_Remove_Selected(void)=0;
	virtual BOOL
		Contact_Find(NPS_USERID buddyID)=0;
	virtual void
		Contact_RefreshList(NPS_USERID myID)=0;
	virtual void
		Contact_Refresh(tsContact * contact)=0;

	virtual BOOL
		Contact_IsMuted(NPS_USERID buddyID)=0;
	virtual void
		Contact_Mute(const UserChangeInfo * userInfo, BOOL permanent=FALSE)=0;
	virtual void
		Contact_MuteByID(NPS_USERID UserID, BOOL permanent=FALSE)=0;
	virtual void
		Contact_UnMute(NPS_USERID buddyID)=0;

	virtual void
		Contact_GetUserInfo(NPS_USERID id, tfNPSCB_GetUserInfo func)=0;
};

class NPS2ClientDllExport cNPS2C_Mail
{
public:
	virtual ~cNPS2C_Mail(void){};
	virtual	void			Mail_ResetSendList(void)=0;
	virtual	NPSSTATUS		Mail_AddToSendList(NPS_USERID id, BOOL verify=FALSE)=0;
	virtual	NPSSTATUS		Mail_SetSendList(const char *sendList)=0;
	virtual	NPSSTATUS		Mail_Send(const char *title, const char *message, time_t expirytime)=0;
	virtual	NPSSTATUS		Mail_Delete(NPS_MAILID mailID)=0;
	virtual	void			Mail_GetAll(void)=0;
	virtual	void			Mail_Refresh(void)=0;
	virtual	cQ	*			Mail_List(void)=0;
	virtual	char *			Mail_GetValidList(void)=0;
	virtual	char *			Mail_GetErrorList(void)=0;
	virtual void			Mail_PollPending(void)=0;
	virtual BOOL			Mail_IsPending(void)=0;
};

class NPS2ClientDllExport cNPS2C_Database :
	public virtual cNPS2C_Account,
	public virtual cNPS2C_Persona,
	public virtual cNPS2C_Contact,
	public virtual cNPS2C_Mail
{
public:
	virtual ~cNPS2C_Database(void){};
};

//====================================================================
// Lobby
//====================================================================
// main connection
class NPS2ClientDllExport cNPS2C_PrimaryLobbyServer
{
public:
	virtual ~cNPS2C_PrimaryLobbyServer(void){};
	// Download the list of available servers for the given gameName
	// List contains (IP,port) elements
	// ---------------------------------------------------------------
	virtual cQ *
		PLS_GetServerList(const char * gameName)=0;

	virtual NPSSTATUS
		PLS_Connect(char * regKey, const char * userName, NPS_GAMEUSERID userID)=0;
	virtual NPSSTATUS
		PLS_Connect(const char * IP, unsigned long port, const char * userName, NPS_GAMEUSERID userID)=0;
	virtual BOOL
		PLS_IsConnected(void)=0;
	virtual NPSSTATUS
		PLS_DisConnect(BOOL wait4reply=FALSE)=0;

	virtual void
		PLS_Sleep(BOOL)=0;

	virtual void
		PLS_Ping(void)=0;

	virtual int
		PLS_GetLastPing(void)=0;
};
// lobby connection
class NPS2ClientDllExport cNPS2C_RoomServer
{
public:
	virtual ~cNPS2C_RoomServer(void){};

	virtual cQ *
		RS_GetList(void)=0;
	virtual NPSSTATUS
		RS_Join(char * lobbyName)=0;
	virtual NPSSTATUS
		RS_JoinSelected(void)=0;
	virtual BOOL
		RS_IsConnected(void)=0;
	virtual NPSSTATUS
		RS_DisConnect(BOOL wait4reply=FALSE)=0;

	virtual int
		RS_GetNumUsers(char * lobbyName)=0;
	virtual int
		RS_GetNumRooms(char * lobbyName)=0;

	virtual BOOL
		RS_SetSelection(void)=0;
	virtual const RoomInfo	*
		RS_GetSelected(void)=0;

	virtual void
		RS_Sleep(BOOL)=0;

};
// User created rooms
class NPS2ClientDllExport cNPS2C_Room
{
public:
	virtual ~cNPS2C_Room(void){};

	virtual cQ *
		Room_GetList(void)=0;

	// ---------------------------------------------------------------
	virtual NPSSTATUS
		Room_Create(char * name, char * password, void * roomData, int dataSize, int reqSlot, unsigned short maxPlayers)=0;

	// ---------------------------------------------------------------
	virtual NPSSTATUS
		Room_Connect(char * name, char * password, int reqSlot)=0;
	virtual NPSSTATUS
		Room_Connect(char * password,int reqSlot)=0;
	virtual NPSSTATUS
		Room_Connect(int reqSlot)=0;
	virtual NPSSTATUS
		Room_Connect(void)=0;

	// ---------------------------------------------------------------
	virtual BOOL
		Room_IsConnected(void)=0;
	virtual NPSSTATUS
		Room_DisConnect(BOOL wait4reply=FALSE)=0;

	// ---------------------------------------------------------------
	virtual RoomInfo *
		Room_GetRoomInfo(void)=0;
	virtual void
		Room_SendData(void * roomData, int dataSize)=0;
	virtual void
		Room_BootUser(NPS_USERID userID)=0;

	// You should reserve a slot before connecting to the gameserver.
	// slots = 0..x, -1 = no slot
	// ---------------------------------------------------------------
	virtual int
		Room_ReservePlayerSlot(void)=0;
	virtual void
		Room_FreePlayerSlot(void)=0;
	virtual int
		Room_GetPlayerSlot(void)=0;
	virtual NPSSTATUS
		Room_SetReady(BOOL ready, BOOL wait4reply=FALSE)=0;
	virtual void
		Room_EnableRoomReadyPermission(BOOL open)=0;
	virtual BOOL
		Room_IsClosed(void)=0;

	// ---------------------------------------------------------------
	virtual NPS_USERID
		Room_GetHostID(void)=0;
	virtual NPSSTATUS
		Room_GetGameServerInfo_Update(char * roomName=NULL)=0; // NULL roomName uses currently connected room's name.
	virtual const GameStartInfo *
		Room_GetGameServerInfo(void)=0;

	// ---------------------------------------------------------------
	virtual BOOL
		Room_Select(char * roomName)=0;
	virtual BOOL
		Room_SetSelection(void)=0;
	virtual void
		Room_GetData(void * roomData, int size)=0;

	// Resets after call
	// ---------------------------------------------------------------
	virtual BOOL
		Room_IsSelectedRoomDirty(void)=0;
};
// holds list of users in the room (FYI: a Roomlobby is considered a special room)
class NPS2ClientDllExport cNPS2C_User
{
public:
	virtual ~cNPS2C_User(void){};

	virtual void
		User_UpdateList(void)=0;
	virtual cQ *
		User_GetList(void)=0;

	virtual void
		User_SendMyData(const UserDataField * data, int dataSize)=0;
	virtual BOOL
		User_IsMyDataDiff(const UserDataField * data, int dataSize)=0;
};
// chat and user to user binary transfers
typedef enum
{
	kINet_Msg_Type_None=-1,
	kINet_Msg_Type_Connect=0,
	kINet_Msg_Type_Data,
	kINet_Msg_Type_Chat,
	kINet_Msg_Type_LoggedChat
}teINet_Msg_Type;
typedef void (__cdecl *Chat_Callback)(const char *room, const char *user, NPS_USERID userID, const char *msg, NPSCLIENT_CHAT_TYPE flags);

typedef enum
{
	// Initiators
	kINet_Msg_ConnStatus_Close=0,
	kINet_Msg_ConnStatus_RequestOpen,
	kINet_Msg_ConnStatus_ReviewingOpen,
	kINet_Msg_ConnStatus_AcceptOpen,
	kINet_Msg_ConnStatus_Open,
	// Denial
	kINet_Msg_ConnStatus_NotOnline,
	kINet_Msg_ConnStatus_Busy,
	kINet_Msg_ConnStatus_Decline
}teINet_Msg_ConnStatus;

#define	kINetMsg_MaxLen	1024
typedef struct
{
	NPS_USERID				ID;
	char						name[NPS_USERNAME_LEN+1];
	char 						userType;		//Set by SKU
	unsigned short			userDataLen;
	char						userData[kINetMsg_MaxLen];
}tsINet_Msg_Confrere;

typedef struct
{
	BOOL							active;	// TRUE if open or pending (FALSE when explicitly closed or cleared)
	teINet_Msg_ConnStatus	status;
	tsINet_Msg_Confrere		confrere;
}tsINet_Msg_SessionInfo;

class NPS2ClientDllExport cNPS2C_Message
{
public:
	virtual ~cNPS2C_Message(void){};

	virtual void
		Message_HookChatCallback(Chat_Callback)=0;
	virtual BOOL
		Message_Send(NPS_USERID toID, teINet_Msg_Type msgType, char * msg=NULL, unsigned short msgLen=0)=0;
	virtual BOOL
		Message_Connect(NPS_USERID toID, teINet_Msg_ConnStatus status, char userType, const char * name=NULL, char * userData=NULL, unsigned short userDataLen=0)=0;

	virtual void
		Message_PollCurrentSessionInfo(tsINet_Msg_SessionInfo * info)=0;
	virtual BOOL	Message_PollSessionRequest(tsINet_Msg_Confrere * info)=0;

	// Pops session requests of the given user type only
	// -------------------------------------------------
	virtual BOOL
		Message_PopSessionRequest(char userType, tsINet_Msg_Confrere * info)=0;

	virtual void
		Message_ClearCurrentSession(void)=0;
	virtual void
		Message_SetBusy(BOOL isBusy)=0;
};

class NPS2ClientDllExport cNPS2C_Lobby :
	public virtual cNPS2C_PrimaryLobbyServer,
	public virtual cNPS2C_RoomServer,
	public virtual cNPS2C_Room,
	public virtual cNPS2C_User,
	public virtual cNPS2C_Message
{
public:
	virtual ~cNPS2C_Lobby(void){};
};
//====================================================================
// Broadcaster
//====================================================================
typedef enum
{
	kINet_SERVER_NOT_RUNNING = NPS_SERVER_NOT_RUNNING,		// Not currently running or any intent to run
	kINet_SERVER_PENDING  	 =	NPS_SERVER_START_PENDING,	// Intent to find an available server and/or start the broadcaster process on the server.
	kINet_SERVER_RUNNING  	 =	NPS_SERVER_RUNNING,			// All ready, connections allowed as per connection rules.
	kINet_SERVER_CLOSED  	 =	NPS_CHANNEL_CLOSED_MASK		// Server is running, but not allowing any more connections.
}teNPS2C_ServerState;

class NPS2ClientDllExport cNPS2C_Broadcaster
{
public:
	virtual ~cNPS2C_Broadcaster(void){};

	// ----------------------------------------------------------------
	// Attempts to start a new Broadcaster process on an NPS Server.
	// ----------------------------------------------------------------
	virtual NPSSTATUS
		Start(void)=0;

	virtual NPSSTATUS
		Connect(NPS_USERID userID, const char * userName, const GameStartInfo * )=0;
	virtual NPSSTATUS
		DisConnect(BOOL wait4reply=FALSE)=0;
	virtual BOOL
		IsConnected(void)=0;

	virtual BOOL
		GetNextNewPlayer(NPS_UserInfo * userInfo)=0;
	virtual BOOL
		GetNextRemovedPlayer(NPS_UserInfo * userInfo)=0;

   virtual NPSSTATUS
		SendUDP ( NPS_USERID * to, const void * data, int len )=0;
	virtual NPSSTATUS
		SendTCP ( NPS_USERID * to, const void * data, int len )=0;
};

// ----------------------------------------------------------------
//	Aquires and Releases
// ----------------------------------------------------------------

NPS2ClientDllExport cNPS2C_Database * NPS2C_CDECL
	NPS2C_Acquire_Database(const char * gameName, cNPS2C_Idle_Callback * callback);
NPS2ClientDllExport void NPS2C_CDECL
	NPS2C_Release_Database(cNPS2C_Database * obj);

NPS2ClientDllExport cNPS2C_Lobby * NPS2C_CDECL
	NPS2C_Acquire_Lobby(const char * gameName, cNPS2C_Idle_Callback * callback);
NPS2ClientDllExport void NPS2C_CDECL
	NPS2C_Release_Lobby(cNPS2C_Lobby * obj);

NPS2ClientDllExport cNPS2C_Broadcaster * NPS2C_CDECL
	NPS2C_Acquire_Broadcaster(cNPS2C_Broadcaster_Callbacks * callbacks);
NPS2ClientDllExport void NPS2C_CDECL
	NPS2C_Release_Broadcaster(cNPS2C_Broadcaster * obj);

#endif //NPS2_CLIENT_H
