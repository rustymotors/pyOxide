// AUTODOC+
// PROGRAM: NPSUserLogin.h
// AUTODOC-
/*************************************************************************
 File Name:     NPSUserLogin.h
 Author:        John W. Fawcett
 Date Created:  3/23/98
 Current Owner: John W. Fawcett

 Purpose:       Client/Server shared data structures for login server

 Modification Log
 Who    |   Date    |  What & Why
 -------------------------------------------------------------------------
        |           |  Original Code
  SEP   | 27-Oct-99 |  Added ComputerLoadTable/GameServersTable/ComputersTable2

                Copyright 1997-2000 Electronic Arts, Inc.
*************************************************************************/

#ifndef _NPS_USERLOGIN_H
#define _NPS_USERLOGIN_H

#include <NPSTypes.h>

#include <time.h>		// for time_t

typedef struct _AccountData {
   NPS_CUSTOMERID  CustomerId;
   char        LastName[NPS_LAST_NAME_LEN + 1];
   char        FirstName[NPS_FIRST_NAME_LEN + 1];
   char        Address1[NPS_ADDRESS_LEN + 1];
   char        Address2[NPS_ADDRESS_LEN + 1];
   char        City[NPS_CITY_LEN + 1];
   char        State[NPS_STATE_LEN + 1];
   char        PostalCode[NPS_ZIP_LEN + 1];
   char        Phone[NPS_PHONE_LEN + 1];
   char        Country[NPS_COUNTRY_LEN + 1];
   char        EMail[NPS_EMAIL_LEN + 1];
   long        NumLogins;
   char        UserName[NPS_USERNAME_LEN + 1];
   char        Password[NPS_PASSWORD_LEN + 1];
   long        TimeOnLine;
   short       Gender;
   short       Age;
   short       JobType;
   short       IncomeLevel;
   short       NumberComputers;
   short       ComputerTypes;
   short       ModemSpeed;
   short       Ram;
   long        PingTime;
   long        Hops;
   char        Url[NPS_URL_LEN + 1];
   short       FailedLoginAttempts;
   NPS_LOGICAL WantsEANews;
   char        ParentalEMail[NPS_EMAIL_LEN+1];
   NPS_LOGICAL OkToSellName;
   short       GamePurchasesPerYear;
   long        HoursOnlinePerYear;
   time_t      CreateDate;
   time_t      LastAccess;
   time_t      Birthday;
   char        HashedPassword[NPS_HASHED_PASSWORD_LEN+1];
   char        Salt[NPS_SALT_LEN+1];
   short       ModuloIndex;
   time_t      PasswordExpiration;
} AccountData;

#define NUMCOLSACCOUNTDATA 39

//
// CustomerPrivStruct -- jake
// represents the EACustomerPriv table
typedef struct _CustomerPrivData {
   NPS_CUSTOMERID CustomerId;
   NPS_SERVERDATAID ServerDataId;
   int ServerStatusId;
} CustomerPrivData;
#define NUMCOLSCUSTOMERPRIVDATA 3

//
// Bulk load the database with CDKeys, maintain a reference
// count of how many times a key has been used.
typedef struct _CDKeyData
{
	char 							Key[NPS_CDKEY_LEN+1];
	NPS_SERVERDATAID 	ServerDataId;
	long 							RefCount;
	NPS_LOGICAL						IsDisabled;
} CDKeyData;
#define NUMCOLSCDKEY 4

//
// Represents the CDKeyRegistrant table for associating
// cusotmers with the cd key(s) they own or have stolen :)
typedef struct _CDKeyRegistrantData
{
	char 							Key[NPS_CDKEY_LEN+1];
	NPS_CUSTOMERID		CustomerId;
} CDKeyRegistrantData;
#define NUMCOLSCDKEYREGISTRANT 2

typedef struct _UserGameData {
   NPS_CUSTOMERID     CustomerId;
   char           GameUserName[NPS_USERNAME_LEN + 1];
   NPS_SERVERDATAID   ServerDataId;
   time_t         CreateDate;
   time_t         LastLogin;
   long           NumGames;
   NPS_GAMEUSERID     GameUserId;
   short          IsOnSystem;
   time_t         GamePurchaseDate;
   char           GameSerialNumber[NPS_SERIAL_NO_LEN + 1];
   long           TimeOnLine;
   long           TimeInGame;
   char           GameSpecific[NPS_MAX_GAME_BLOB];
   char           PersonalBlob[NPS_PERSONAL_BLOB_LEN];
   char           PictureBlob[NPS_PICTURE_BLOB_LEN];
   NPS_LOGICAL        DND;
   time_t         GameStart;
   char           CurrentKey[NPS_HASHED_PASSWORD_LEN];
   short          PersonaLevel;
  NPS_SHARDID     ShardId;
} UserGameData;

// Because the blobs come from different tables, there are 2
// defines for this structure.  The separation is transparent
// to the end user.
#define NUMCOLSUSERGAMEDATA 16
#define NUMCOLSGENERICBLOB  2

typedef struct _ServerDataTableInfo {
   NPS_SERVERDATAID   ServerDataId;
   char           GameName[NPS_GAMENAME_LEN + 1];
   NPS_LOGICAL        IsMMS;
   NPS_LOGICAL        IsRoomServer;
   NPS_LOGICAL        IsLobbyServer;
   NPS_LOGICAL        IsPermanentGameServer;
   NPS_LOGICAL        IsGameServer;
   NPS_LOGICAL        IsTournamentServer;
   NPS_LOGICAL        UsesGameServer;
   NPS_LOGICAL        UniqueNamesRequired;
   NPS_LOGICAL        IsClonable;
   long           TotalPlayers;
   long           TotalGames;
   long           TotalPlayersAllowed;
   long           TotalGamesAllowed;
   long           TotalRoomsAllowed;
   long           MinPlayersRequiredPerGame;
   long           MaxPlayersAllowedPerGame;
   long           NumberOfCrashes;
   long           UsersCrashed;
   char           GameServerCommand[NPS_COMMANDLINE_LEN + 1];
   char           LobbyParseString[NPS_LOBBY_PARSE_LEN];
   long           UserNameMaxChar;
   long           ReopenThreshold;

} ServerDataTableInfo;

#define NUMCOLSSERVERDATA 24

typedef struct _BuddyList {
   NPS_GAMEUSERID     MyGameId;
   NPS_GAMEUSERID     BuddyGameId;
   NPS_LOGICAL        IsBuddy;
   NPS_LOGICAL        DND;
   NPS_LOGICAL        DNB;
   NPS_LOGICAL        NoEntry;
   NPS_LOGICAL        MuteWhisper;
   NPS_LOGICAL        MuteChat;
} BuddyList;

#define NUMCOLSBUDDYLIST 8

typedef struct _MyBuddy {
   NPS_GAMEUSERID     MyGameId;
   NPS_GAMEUSERID     BuddyGameId;
   NPS_LOGICAL        IsBuddy;
   NPS_LOGICAL        DND;
   NPS_LOGICAL        DNB;
   NPS_LOGICAL        NoEntry;
   NPS_LOGICAL        MuteWhisper;
   NPS_LOGICAL        MuteChat;
} MyBuddy;

#define NUMCOLSMYBUDDY 8

typedef struct _GameData {
   NPS_SERVERDATAID   ServerDataId;
   long           SKU;
   char           GameStartCommand[NPS_GAMELAUNCHCOMMAND_LEN + 1];
} GameData;

#define NUMCOLSGAMEDATA 3

typedef struct _ActiveServerData {
   NPS_SERVERDATAID   ServerDataId;
   long           ServerType;
   long           ServerPort;
   char           ServerIp[NPS_HOSTNAME_LEN + 1];
   long           RamUsed;
   long           IOUsed;
   long           CPUUsed;
   NPS_LOGICAL    IsFull;
   long           CurrentUserCount;
} ActiveServerData;

#define NUMCOLSACTIVESERVERDATA 9

typedef struct _UsersOnLineData
{
	NPS_GAMEUSERID	GameUserId;
	long			ActiveServerId;
} UsersOnLineData;

#define NUMCOLSUSERSONLINEDATA 2

typedef struct _LobbiesToRunData {
   NPS_LOBBYID        LobbyId;
   char           IpAddress[NPS_HOSTNAME_LEN + 1];
   char           LaunchCommand[NPS_COMMANDLINE_LEN + 1];
   NPS_SERVERDATAID   ServerDataId;
   NPS_LOGICAL        IsDisabled;
   int				Port;
   char             ServerName[NPS_GAMENAME_LEN + 1];
   char             UnixUser[NPS_UNIX_USER_LEN + 1];
   char             UnixPassword[NPS_UNIX_PASSWORD_LEN + 1];
   char             AdminEmails[NPS_ADMIN_EMAILS_LEN + 1];
} LobbiesToRunData;

#define NUMCOLSLOBBIESTORUNDATA 10

typedef struct _RoomsToRunData {
   NPS_ROOMID         RoomId;
   NPS_LOBBYID        ParentId;
   char           IpAddress[NPS_HOSTNAME_LEN + 1];
   char           LaunchCommand[NPS_COMMANDLINE_LEN + 1];
   NPS_SERVERDATAID   ServerDataId;
   NPS_SERVERDATAID	GameServerId;
   char             GameRiff[NPS_RIFF_NAME_LEN + 1];
} RoomsToRunData;

#define NUMCOLSROOMSTORUNDATA 7

typedef struct _GameComputersData {
   NPS_SERVERDATAID   ServerDataId;
   char           IpAddress[NPS_IPADDR_LEN + 1];
} GameComputersData;

#define NUMCOLSGAMECOMPUTERSDATA 2

typedef struct _GenericHighScore {
   NPS_CUSTOMERID     GameUserId;
   NPS_SERVERDATAID   ServerDataId;
   long               Integer1;
   long               Integer2;
   long               Integer3;
   long               Integer4;
   long               Integer5;
   long               Integer6;
   long               Integer7;
   long               Integer8;
   long               Integer9;
   long               Integer10;
   long               Integer11;
   long               Integer12;
   long               Integer13;
   long               Integer14;
   long               Integer15;
   long               Integer16;
   double             Double1;
   double             Double2;
   double             Double3;
   double             Double4;
   double             Double5;
   double             Double6;
   double             Double7;
   double             Double8;
   char               String1[NPS_HS_GENERIC_STRING_LEN];
   char               String2[NPS_HS_GENERIC_STRING_LEN];
   char               String3[NPS_HS_GENERIC_STRING_LEN];
   char               String4[NPS_HS_GENERIC_STRING_LEN];
   char               String5[NPS_HS_GENERIC_STRING_LEN];
   char               String6[NPS_HS_GENERIC_STRING_LEN];
   char               String7[NPS_HS_GENERIC_STRING_LEN];
   char               String8[NPS_HS_GENERIC_STRING_LEN];
} GenericHighScore;

#define NUMCOLSGENERICHIGHSCORE 34

typedef struct _GenericHighScoreDescription {
   long              ServerDataId;
   long              FieldIndex;
   long              FieldDataType;
   char              FieldDescription[NPS_HS_FIELD_DESCR_LEN];
} GenericHighScoreDescription;

#define NUMCOLSGENERICHIGHSCOREDESCRIPTION 4

typedef struct _GameSKURegistry {
   long              ServerDataId;
   char              SKURegistry[NPS_SKU_REGISTRY_LEN];
} GameSKURegistry;

#define NUMCOLSGAMESKUREGISTRY 2

typedef struct _BugReportData {
   long           BugId;
   NPS_GAMEUSERID CreatorPersonaId;
   time_t         CreateDate;
   long           Severity;
   char           GameName[NPS_GAMENAME_LEN];
} BugReportData;

#define NUMCOLSBUGREPORTDATA 5

// Typedefs for messages
typedef struct {
   char UserName[NPS_USERNAME_LEN + 1];
   char Password[NPS_PASSWORD_LEN + 1];
   char GameName[NPS_GAMENAME_LEN + 1];
   //unsigned long ServerDataID;  // TODO:  For Future Upgrade
   //char GameName[NPS_GAMENAME_LEN + 1];   Pick one or the other...
} UserLoginStruct;

typedef struct {
   NPS_CUSTOMERID CustomerId;
   char GameName[NPS_GAMENAME_LEN + 1];
} GameLoginStruct;

typedef struct {
   NPS_LOGICAL UseName;
   char GameName[NPS_GAMENAME_LEN + 1];
   char GameUserName[NPS_USERNAME_LEN + 1];
   NPS_GAMEUSERID GameUserId;
} GetPersonaInfoStruct;

typedef struct {
   NPS_GAMEUSERID GameUserId;
   short BlobToUpdate; // Or'd together.  1 = GameBlob, 2 = Personal Blob,
      // 4 = Picture Blob 3 = 1 and 2, 5 = 1 and 4, 6 = 2 and 4, 7 = all 3
   char GameSpecific[NPS_MAX_GAME_BLOB];
   char PersonalBlob[NPS_PERSONAL_BLOB_LEN];
   char PictureBlob[NPS_PICTURE_BLOB_LEN];
} GameBlobStruct;

typedef struct {
   NPS_GAMEUSERID GameUserId;
} RegisterGameLoginStruct;

typedef struct {
   NPS_GAMEUSERID GameUserId;
} DeleteGamePersonaStruct;

typedef struct {
   AccountData NewAccountInfo;
} UpdateEaAccountStruct;

typedef struct {
   UserGameData NewGameAccount;
} UpdateGameAccountStruct;

typedef struct {
   NPS_GAMEUSERID GameUserId;
} RequestBuddyListStruct;

typedef struct {
   NPS_LOGICAL UseName;
   NPS_GAMEUSERID MyGameUserId;
   NPS_GAMEUSERID BuddyGameUserId;
   char BuddyGameUserName[NPS_USERNAME_LEN + 1];
   char GameName[NPS_GAMENAME_LEN + 1];
} GetBuddyInfoStruct;

typedef struct {
   NPS_GAMEUSERID BuddyGameUserId;
   char GameUserName[NPS_USERNAME_LEN + 1];
   NPS_LOGICAL IsOn;
   NPS_LOGICAL IsBuddy;
   NPS_LOGICAL DND;
   NPS_LOGICAL DNB;
   NPS_LOGICAL NoEntry;
   NPS_LOGICAL MuteWhisper;
   NPS_LOGICAL MuteChat;
} BuddyListInfo;

typedef struct {
   NPS_GAMEUSERID MyUserId;
   NPS_GAMEUSERID BuddyUserId;
   NPS_LOGICAL IsBuddy;
   NPS_LOGICAL DND;
   NPS_LOGICAL DNB;
   NPS_LOGICAL NoEntry;
   NPS_LOGICAL MuteWhisper;
   NPS_LOGICAL MuteChat;
} AddBuddyListStruct;

typedef struct {
   NPS_GAMEUSERID MyUserId;
   char BuddyName[NPS_USERNAME_LEN + 1];
   char GameName[NPS_GAMENAME_LEN + 1];
   NPS_LOGICAL IsBuddy;
   NPS_LOGICAL DND;
   NPS_LOGICAL DNB;
   NPS_LOGICAL NoEntry;
   NPS_LOGICAL MuteWhisper;
   NPS_LOGICAL MuteChat;
} AddBuddyByNameStruct;

typedef struct {
   NPS_GAMEUSERID MyUserId;
   NPS_GAMEUSERID BuddyUserId;
} DeleteBuddyListStruct;

typedef struct {
   NPS_GAMEUSERID MyUserId;
} ClearBuddyListStruct;

typedef struct {
   UserGameData TheGameData;
   char GameName[NPS_GAMENAME_LEN + 1];
} AddGamePersonaStruct;


typedef struct {
   char ServerName[NPS_GAMENAME_LEN];
   unsigned short ServerType;
   NPS_GAMEUSERID GameUserId;
   short NumInts;
   short NumFloats;
   short NumChars;
   short IntsToRead[16];
   short FloatsToRead[8];
   short CharsToRead[8];
   long  IntArray[16];
   float FloatArray[8];
   char  CharArray[16][128];
} HighScoreDataStruct;

typedef struct {
   char ServerName[NPS_GAMENAME_LEN];
   unsigned short ServerType;
   NPS_GAMEUSERID GameUserId;
   short NumInts;
   short NumFloats;
   short NumChars;
   short IntsToRead[16];
   short FloatsToRead[8];
   short CharsToRead[8];
   long  IntArray[16];
   float FloatArray[8];
} HighScoreDataStructNoChars;

typedef struct {
   char ServerName[NPS_GAMENAME_LEN];
   unsigned short ServerType;
   NPS_GAMEUSERID GameUserId;
} DeleteHighScoreStruct;

typedef struct {
   long Type;
} RequestServerDataListStruct;

// NPS_GET_PLAYER_RANK
typedef struct {
   NPS_GAMEUSERID GameUserId;
   long Rank;
} PlayerRankInfo;

// NPS_GET_GENERIC_FIELD_RANKING
typedef struct {
   NPS_GAMEUSERID GameUserId;
   char GameName[NPS_GAMENAME_LEN+1];
	 short FieldIndex;
	 short SortAsc;
   long Rank;
} GenericFieldRankingInfo;

typedef struct {
   long NumPlayers;
} TopPlayersRequest;

typedef struct {
   char  GameName[NPS_GAMENAME_LEN];
} GetGameVersionStruct;

// AUTODOC+
typedef struct {
   long  NumScores;			// Maximum scores to return
   char  GameName[NPS_GAMENAME_LEN];    // Server Name of interest
   unsigned short ServerType;		// ServerType of GameName
   short SortField;			// Field to sort by
   NPS_LOGICAL  SortUsingFloat;		// 1 -> Floats, 0-> Integers
   NPS_LOGICAL  SortOrder;		// 1 -> Ascending, 0 -> Descending
   NPS_LOGICAL  ReturnChars;		// 1 -> Yes, 0 -> No
} LeaderBoardRequest;
// AUTODOC-

typedef struct {
   NPS_GAMEUSERID GameUserId;
   char GameUserName[NPS_USERNAME_LEN + 1];
   long Score;
   long Rank;
} PlayerRankListInfo;

// AUTODOC+
typedef struct {
   char GameUserName[NPS_USERNAME_LEN + 1];
   long  IntArray[16];
   float FloatArray[8];
} LeaderBoardListInfoNoChars;

typedef struct {
   char GameUserName[NPS_USERNAME_LEN + 1];
   long  IntArray[16];
   float FloatArray[8];
   char  CharArray[8][128];
} LeaderBoardListInfo;
// AUTODOC-

typedef struct _ComputersData {
   char           IpAddress[NPS_IPADDR_LEN + 1];
   char           SystemName[NPS_SYSTEMNAME_LEN + 1];
   long           SystemType;
   NPS_LOGICAL    IsUp;
   NPS_LOGICAL    EaMachine;
   long           MonitorRate;
   long           NoMaxLoadReadings;
   long           NoCurrentLoadReadings;
   time_t         LastLoadReading;
   time_t         LastServerLaunch;
   long           Realmem;
   char           ContactFirstName[NPS_FIRST_NAME_LEN + 1];
   char           ContactLastName[NPS_LAST_NAME_LEN + 1];
   char           ContactAddr1[NPS_ADDRESS_LEN + 1];
   char           ContactAddr2[NPS_ADDRESS_LEN + 1];
   char           ContactCity[NPS_CITY_LEN + 1];
   char           ContactState[NPS_STATE_LEN + 1];
   char           ContactPostalCode[NPS_ZIP_LEN + 1];
   char           ContactCountry[NPS_COUNTRY_LEN + 1];
   char           ContactPhone[NPS_PHONE_LEN + 1];
   char           ContactFax[NPS_FAX_LEN + 1];
   char           ContactEmail[NPS_EMAIL_LEN + 1];
   long           NPSRemotePort;
   long           MachineScale;
   long           NoMinLoadReadings;
   long           StatisticsRate;
   long           SystemMessageCheckRate;
} ComputersData;

#define NUMCOLSCOMPUTERSDATA 27

typedef struct _GameServersData {
   char           IpAddress[NPS_HOSTNAME_LEN + 1];
   long           GameServerId;
   long           GameServerGroupId;
   NPS_LOGICAL    Enabled;
   NPS_LOGICAL    ScheduleMode;
   long           NoMaxGameServers;
   long           NoCurrentGameServers;
   char           SWVersion[NPS_VERSION_LEN + 1];
   time_t         LastSWUpdate;
} GameServersData;

#define NUMCOLSGAMESERVERSDATA 9

typedef struct _ComputerLoadData {
   char           IpAddress[NPS_IPADDR_LEN + 1];
   time_t         LoadSampleDate;
   long           IPackets;
   long           IErrors;
   long           OPackets;
   long           OErrors;
   long           Collisions;
   long           IBytes;
   long           OBytes;
   long           Avenrun1Min;
   long           Avenrun5Min;
   long           Avenrun15Min;
   long           Availrmem;
   long           Freemem;
   long           SwapTotal;
   long           SwapFree;
   long           SwapUsed;
} ComputerLoadData;

#define NUMCOLSCOMPUTERLOADDATA 17

typedef struct _GameServerGroupData {
   long           GameServerGroupId;
   char           GameServerGroupDescription[NPS_DESCRIPTION_LEN + 1];
} GameServerGroupData;

#define NUMCOLSGAMESERVERGROUPDATA 2

typedef struct _NPSMonitorData {
   char           IpAddress[NPS_IPADDR_LEN + 1];
   long           Port;
   long           Status;
   time_t         LastLogEntry;
   time_t         LastRSRestart;
   time_t         LastPLSRestart;
} NPSMonitorData;

#define NUMCOLSNPSMONITORDATA 6

typedef struct _NPSStatisticsData {
   NPS_SERVERDATAID   ServerDataId;
   time_t             TimeStamp;
   long               NumUsers;
   long               HighWaterMark;
   long               NumSockets;
   long               NumConnections;
} NPSStatisticsData;

#define NUMCOLSNPSSTATISTICSDATA 6

typedef struct _ModuloData {
   short              ModuloIndex;
   char               Modulus[NPS_HASHED_PASSWORD_LEN + 1];
   char               Generator[NPS_SALT_LEN + 1];
   char               Description[NPS_MODULO_DESCRIPTION_LEN + 1];
} ModuloData;

#define NUMCOLSMODULODATA 4

typedef struct _PersonaLevelDefData {
   NPS_SERVERDATAID   ServerDataId;
   short              PersonaLevel;
   char               Description[NPS_PERSONALEVEL_DESCRIPTION_LEN + 1];
} PersonaLevelDefData;

#define NUMCOLSPERSONALEVELDEFDATA 3

typedef struct _NPSSystemMessageData {
   char               Message[NPS_SYSTEM_MESSAGE_LEN + 1];
   time_t             TimeStamp;
   long               DeleteFlag;
   long               DeliveryMode;
   long               DeliveryStatus;
} NPSSystemMessageData;

#define NUMCOLSNPSSYSTEMMESSAGEDATA 5

typedef struct _GameVersionNumberData {
   char Version[NPS_VERSION_LEN];
   NPS_SERVERDATAID   ServerDataId;
} GameVersionNumberData;

#define NUMCOLSGAMEVERSIONDATA 2

#if defined (USE_SRP)
typedef struct {
   char UserName [NPS_USERNAME_LEN + 1];
   char GameName [NPS_GAMENAME_LEN + 1];
} SRP_UsernameData;
#endif /* USE_SRP */

typedef struct _BugReportMessage {
   long           BugId;
   NPS_GAMEUSERID CreatorPersonaId;
   time_t         CreateDate;
   long           Severity;
   char           GameName[NPS_GAMENAME_LEN];
   char           Description[NPS_BUG_REPORT_LEN];
} BugReportMessage;

typedef struct _SendInGameEmailMessage {
   NPS_GAMEUSERID SenderId;
   time_t sentDate;

   // for NPS_SEND_EMAIL, above two should be overridden
   // for NPS_IN_GAME_EMAIL (server response to NPS_RECEIVE_EMAIL),
   //   above two are filled in correctly, but numDestIds (below) is 0

   time_t expiryDate;
   char subject[80];         // fixed length title
   short numDestIds;
   short msgLen;
   NPS_USERID firstDestId;

   // after is an array of GAMEUSERIDs
   // followed by the rest of the message (msgLen bytes total)
} SendInGameEmailMessage;

typedef struct _ReceiveInGameEmailMessage {  // NPS_RECEIVE_EMAIL
   NPS_GAMEUSERID myId;
   NPS_MAILID mailId;
   NPS_LOGICAL headerOnly;
} ReceiveInGameEmailMessage;

typedef struct _ListInGameEmailMessages {    // NPS_LIST_IGES
   NPS_GAMEUSERID myId;
   // should be inferred from connection for security purposes

   NPS_MAILID lastMailId;
} ListInGameEmailsMessage;

typedef struct _DeleteInGameEmailMessages {  // NPS_DELETE_IGES
   NPS_GAMEUSERID myId;
   short numIds;
   NPS_MAILID firstMailId;    // rest of mailids follow
} DeleteInGameEmailsMessage;

typedef struct _SendEmailAckMessage {               // NPS_SEND_EMAIL_ACK
   short nValidIDs;               // let's at least give them some information
} SendEmailAckMessage;

typedef struct _IGEListMessage {                // NPS_IGE_LIST
   short numIds;
   NPS_MAILID firstMailId;   // rest of mailIds (numIds total) follow
} IGEListMessage;

//structure passed to DLL for sending mail
typedef struct _NPS_SendMail
{
	NPS_USERID		fromID;
	NPS_USERID      *toIDs;         //null terminated array of ids
	char            *title;         //max size defined by NPS_MAILTITLE_LEN
	char            *message;       //max size defined by NPS_MAILBODY_LEN
	time_t          expirytime;     //
} NPS_SendMail;

//structure passed from DLL for incoming mail
typedef struct _NPS_IncMail
{
	NPS_MAILID              id;             //server generated on send receipt
	NPS_USERID              fromID;
	char                    fromUserName[NPS_USERNAME_LEN+1];
	char                    title[NPS_MAILTITLE_LEN+1];
	time_t                  sendtime;       //server generated on send receipt
	time_t                  expirytime;     //currently not displayed but could be one day
	NPS_LOGICAL             unread;         //TODO - server record of if header downloaded before
	char                    *message;       //NPS dll alloced and free'd
} NPS_IncMail;

typedef struct _NPS_UserStatus
{
	bool						isBan;
	bool						isGagged;
	NPS_CUSTOMERID	customerId;
	char						sessionKeyStr[NPS_SESSION_KEY_LEN];
	int							sessionKeyLen;
	void *					sessionKey;
} NPS_UserStatus;

#endif // _NPS_USERLOGIN_H
