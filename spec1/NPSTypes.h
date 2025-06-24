/**
 * File Name:     NPSTypes.h
 * Author:        John W. Fawcett
 * Date Created:  12/11/97
 *
 * Purpose:       General type declarations for the Network Programming System
 *
 *           Copyright 1997, 1998 Electronic Arts, Inc.
 */
#ifndef _NPS_TYPES_H_
#define _NPS_TYPES_H_

#define NPS_INLINE inline

// I hate to put this here, but DAMMIT this stupid STL stuff generates tons
// of warnings when you turn on browser info.
#ifdef WIN32
  #pragma warning(disable: 4786)    // disable browser info warning for STL stuff
#endif

#define DeleteObj(object) { if (object){delete object; object = NULL;} }
#define DeleteObjArray(object) { if (object){delete [] object; object = NULL;}}

/**
 * Data types
 */
typedef short               NPS_LOGICAL;
typedef unsigned short      NPS_OPCODE;
typedef unsigned short      NPS_MSGLEN;
typedef unsigned long       NPS_USERID; // This one is only for the game server
typedef char               *NPS_PSTR;
typedef unsigned long       NPS_LOGINID; // This is their true EA login ID
typedef unsigned long       NPS_USERCOUNT;
typedef long                NPS_SERVID;
typedef long                NPS_COMMID;
typedef unsigned long       NPS_EACOMID;

#ifndef NPS_HEADER_LENGTH
  #define NPS_HEADER_LENGTH (sizeof(NPS_OPCODE) + sizeof(NPS_MSGLEN))
#endif

// Logical values
#ifndef FALSE
  #define FALSE               0
#endif

#ifndef TRUE
  #define TRUE                1
#endif

#ifdef JWF_NO
  #ifdef  min
    #undef  min
  #endif             /* min */
  #define min(a, b)       ((a) < (b) ? (a) : (b))

  #ifdef  max
    #undef  max
  #endif             /* max */
  #define max(a, b)       ((a) < (b) ? (b) : (a))
#endif


/**
 * A callback function passed by the client to the dll.  This callback is called
 * during network delays, allowing client to update their GUI/Audio/etc. and
 * provides a means for the client to break out of the currently executing
 * method.
 * @since NPS 1.0
 */
#ifdef WIN32
typedef long (__cdecl *tfIdleCallBack)( void *Context );
#else
typedef long (*tfIdleCallBack)( void *Context );
#endif

enum
{
  // Length defines
  NPS_LAST_NAME_LEN         =  32,
  NPS_FIRST_NAME_LEN        =  24,
  NPS_ADDRESS_LEN           =  64,
  NPS_CITY_LEN              =  32,
  NPS_STATE_LEN             =   5,
  NPS_ZIP_LEN               =  10,
  NPS_COUNTRY_LEN           =   9,
  NPS_PHONE_LEN             =  20,
  NPS_FAX_LEN               =  20,
  NPS_EMAIL_LEN             =  64,
  NPS_USERNAME_LEN          =  32,
  NPS_PASSWORD_LEN          =  17,
  NPS_GAMENAME_LEN          =  64,
  NPS_VERSION_LEN           =  32,
  NPS_MAX_GAME_BLOB         = 512,
  NPS_SERVERTYPE_LEN        =  32,
  NPS_IPADDR_LEN            =  16,
  NPS_ROWID_LEN             =  30,
  NPS_URL_LEN               =  64,
  NPS_SERIAL_NO_LEN         =  32,
  NPS_SYSTEMNAME_LEN        =  64,
  NPS_LOBBY_PARSE_LEN       =  64,
  NPS_DESCRIPTION_LEN       =  64,
  NPS_SKU_LEN               =  32,
  NPS_CDKEY_LEN             =  32,
  NPS_HASHED_KEY_LEN        =  16,
  NPS_HASHED_PASSWORD_LEN   = 400,
  NPS_SALT_LEN              = 20,
  NPS_SYSTEM_MESSAGE_LEN    = 256,
  NPS_CHAT_MESSAGE_LEN      = 256,

  NPS_MAILTITLE_LEN         = 80,
  NPS_MAILBODY_LEN          = 2048,

  NPS_COMMANDLINE_LEN       = 256,
  NPS_GAMELAUNCHCOMMAND_LEN = 128,
  NPS_PERSONAL_BLOB_LEN     = 256,
  NPS_PICTURE_BLOB_LEN      = 1,
  NPS_RIFF_NAME_LEN         = 32,
  NPS_BUG_REPORT_LEN        = 1024,

  NPS_USERDATA_LEN         = 64,
  NPS_HOSTNAME_LEN         = 64,
  NPS_CHANNEL_DATA_SIZE    = 256,

  NPS_SERVER_RESERVED_LEN = 512,

  // Removed 4 bytes for pointer to SlotList struct
  // Removed another 4 for the ChannelMaster
  NPS_CHANNEL_RESERVED_LEN = 504,
  NPS_CLIENT_RESERVED_LEN  = 512,

  // Removed 4 bytes for channel master
  NPS_RIFF_RESERVED_LEN    = 508,

  NPS_FILENAME_LEN         = 256,

  NPS_HS_GENERIC_STRING_LEN =128,
  NPS_HS_FIELD_DESCR_LEN    = 32,

  NPS_SKU_REGISTRY_LEN      = 256,

  NPS_UNIX_USER_LEN         = 32,
  NPS_UNIX_PASSWORD_LEN     = 32,
  NPS_ADMIN_EMAILS_LEN      = 128,
  NPS_MODULO_DESCRIPTION_LEN  = 256,
  NPS_PERSONALEVEL_DESCRIPTION_LEN  = 32,
  NPS_SESSION_KEY_LEN  = 32,
  NPS_MAX_UDP_PACKET_LEN = 512

};

// Common types
typedef unsigned long             NPS_BUDDYLISTID;
typedef unsigned long             NPS_CUSTOMERID;
typedef unsigned long             NPS_GAMEID;
typedef unsigned long             NPS_GAMEUSERID;
typedef unsigned long             NPS_SERVERDATAID;
typedef unsigned long             NPS_LOBBYID;
typedef unsigned long             NPS_ROOMID;
typedef unsigned long             NPS_MAILID;
typedef unsigned long             NPS_SHARDID;


// current status of a customer
typedef enum _NPSCUSTOMERSTATUS
{
  NPS_CUSTINACTIVE                  = 0,
  NPS_CUSTBANNED                    = 1,
  NPS_CUSTWARNED                    = 2,
  NPS_CUSTACTIVE                    = 3
} NPSCUSTOMERSTATUS;


/**
 * Status values.  These are returned by most ALL NPS methods.
 */
typedef enum _NPSSTATUS
{
  NPS_OK                            =   0,
  NPS_ERR                           =  -1,
  NPS_TIMEOUT                       =  -2,
  NPS_ERR_DUP_USER                  =  -3,
  NPS_ERR_SERVER_FULL               =  -4,
  NPS_ERR_CHANNEL_ALREADY_OPEN      =  -5,
  NPS_NOT_CONNECTED                 =  -6,
  NPS_BAD_COMM_CHANNEL              =  -7,
  NPS_BAD_PARAM                     =  -8,
  NPS_NO_MESSAGES                   =  -9,
  NPS_DATABASE_ERROR                = -10,
  NPS_USER_NOT_FOUND                = -11,
  NPS_GAME_NOT_FOUND                = -12,
  NPS_DUP_GAME                      = -13,
  NPS_NO_BUDDIES                    = -14,
  NPS_GAME_INVALID                  = -15,
  NPS_GAME_USER_NOT_FOUND           = -16,
  NPS_NO_MORE_PERSONAS              = -17,
  NPS_NO_MORE_BUDDIES               = -18,
  NPS_NO_MORE_GAMES                 = -19,
  NPS_SERVER_NOT_FOUND              = -20,
  NPS_DUP_SERVER                    = -21,
  NPS_NO_MORE_SERVERS               = -22,
  NPS_ACCESS_DENIED                 = -23,
  NPS_NO_MORE_GAMECOMPUTERS         = -24,
  NPS_NO_MORE_ROOMS                 = -25,
  NPS_NO_MORE_RECORDS               = -26,
  NPS_NOT_IMPLEMENTED               = -27,
  NPS_INVALID_SOCKET                = -28,
  NPS_NO_DATA_AVAIL                 = -29,
  NPS_OUT_OF_MEMORY                 = -30,
  NPS_BUILD_SOCKET_FAILED           = -31,
  NPS_CONNECT_SOCKET_FAILED         = -32,
  NPS_PASSWORD_CHECK_FAILED         = -33,
  NPS_INCOMPATABLE_VERSIONS         = -34,
  NPS_SCORE_NOT_FOUND               = -35,
  NPS_NO_SCORES                     = -36,
  NPS_NO_MORE_SCORES                = -37,
  NPS_REGISTRY_NOT_FOUND            = -38,
  NPS_NO_MORE_COMPUTERS             = -39,
  NPS_SOCKET_LIMIT_EXCEEDED         = -40,
  NPS_READY_LIST_FULL               = -41,

  // Database errors
  NPS_DB_GENERIC_ERROR              = -42,
  NPS_DB_BAD_COLUMN                 = -43,
  NPS_DB_REQUIRED_COLUMN_MISSING    = -44,
  NPS_DB_INVALID_CURSOR_STATE       = -45,
  NPS_DB_CONNECTION_TIMEOUT         = -46,
  NPS_DB_CONNECTION_CLOSED          = -47,
  NPS_DB_CANNOT_OPEN_CONNECTION     = -48,
  NPS_DB_NETWORK_ERROR              = -49,
  NPS_DB_NO_MEMORY                  = -50,

  // Client canceled a connection
  NPS_CLIENT_CANCELED               = -51,
  NPS_NOT_ENOUGH_PLAYERS            = -52,
  NPS_NO_COMPUTERS_AVAILABLE        = -53,
  NPS_DONT_HAVE_RWLOCK              = -54,

  // Generic database return errors
  NPS_DUP_RECORD                    = -55,
  NPS_RECORD_NOT_FOUND              = -56,

  // Errors brought over from the UserClient
  NPS_BAD_SUBSYSTEM_INSTALL         = -57,
  NPS_SUBSYSTEM_FAILURE             = -58,
  NPS_PARAMETERS_INVALID            = -59, // One or more params are invalid
  NPS_PARAMETER_1_INVALID           = -60, // Param 1 is invalid
  NPS_PARAMETER_2_INVALID           = -61, // Param 2 is invalid
  NPS_PARAMETER_3_INVALID           = -62, // Param 3 is invalid
  NPS_PARAMETER_4_INVALID           = -63, // Param 4 is invalid
  NPS_PARAMETER_5_INVALID           = -64, // Param 5 is invalid
  NPS_ROOM_NOT_CREATED              = -65, // ??? Room Server not valid ???
  NPS_NO_USERS                      = -66, // *** NOT USED ***
  NPS_NO_ROOMS                      = -67, // There are no rooms in this room server
  NPS_NO_LOBBIES                    = -68, // There are no lobbies available.
  NPS_NO_USERS_IN_ROOM              = -69, // Room Empty, should not happen.
  NPS_NO_USERS_IN_LOBBY             = -70, // *** NOT USED ***
  NPS_TIME_OUT_EXCEEDED             = -71, // Response from server not received, timeout, unknown results.
  NPS_ROOM_NOT_FOUND                = -72, // Could not find specified room.
  NPS_LOBBY_NOT_FOUND               = -73, // Could not find specified lobby.
  NPS_DUPLICATE_ROOM                = -74, // *** NOT USED ***
  NPS_LOBBY_NOT_CONNECTED           = -75, // Room Server NOT connected, cannot change rooms.
  NPS_NO_READIED_USERS              = -76, // Can't get ready list, no one is ready.
  NPS_TOO_MANY_USERS                = -77, // Too many users.
  NPS_ROOM_CHANGE_FAILED            = -78, // ChangeToRoom
  NPS_COULD_NOT_CREATE_ROOM         = -79, // Room creation error.
  NPS_ALREADY_INITIALIZED           = -80, // Don't call init twice
  NPS_NO_CALLBACKS_INITIALIZED      = -81, // Callbacks must be initialized before login

  // Errors from the line tester
  NPS_ANALYZE_NOT_INITIALIZED       = -82, // Must call AnalyzeInit first
  NPS_ANALYZE_ICMP_ERROR            = -83, // Icmp function returned an error
  NPS_ANALYZE_LINE_TROUBLE          = -84, // Unknown trouble or too many transmission errors
  NPS_ANALYZE_BUFFER_TOO_SMALL      = -85, // Throughput test using buffer larger than the max buffer size
  NPS_ANALYZE_PROTOCOL_INIT_ERROR   = -86, // Initialization of Packet Send/Receive failed
  NPS_ANALYZE_PROTOCOL_CLOSE_ERROR  = -87, // Close of Packet Send/Receive failed
  NPS_ANALYZE_PROTOCOL_SEND_ERROR   = -88, // Packet Send failed
  NPS_ANALYZE_PROTOCOL_RECEIVE_ERROR = -89, // Packet Receive failed
  NPS_ANALYZE_PROTO_ERROR           = -90, // Bad Packet Protocol Information
  NPS_ANALYZE_PING_ERROR            = -91, // Ping/Send/Receive Error
  NPS_ANALYZE_PING_TIMEOUT          = -92, // Ping/Send/Receive Timeout

  // New ones
  NPS_INCOMPATIBLE_SUBSYSTEM        = -99,
  NPS_TOO_MANY_RIFFS                = -100,
  NPS_NO_MORE_TOP_PLAYERS           = -101,
  NPS_NO_EACUSTOMERPRIVS            = -102,
  NPS_NO_MORE_EACUSTOMERPRIVS       = -103,
  NPS_ERR_DUP_GREG                  = -104,
  NPS_CUSTOMERPRIVEXISTS            = -105,
  NPS_NOT_AUTHORIZED                = -106,  //For beta / game registration verification
  NPS_SHORT_READ                    = -107,
  NPS_NO_MORE_LEADER_BOARD          = -108,
  NPS_NO_MONITORS                   = -109,
  NPS_NO_MORE_MONITORS              = -110,
  NPS_NO_STATISTICS                 = -111,
  NPS_NO_MORE_STATISTICS            = -112,
  NPS_NO_MODULOS                    = -113,
  NPS_NO_MORE_MODULOS               = -114,
  NPS_MODULO_NOT_FOUND              = -115,
  NPS_NO_PERSONALEVELDEFS           = -116,
  NPS_NO_MORE_PERSONALEVELDEFS      = -117,
  NPS_PERSONALEVELDEF_NOT_FOUND     = -118,
  NPS_NO_SYSTEM_MESSAGES            = -119,
  NPS_NO_MORE_SYSTEM_MESSAGES       = -120,

  /**
   * AuthLogin Errors
   */
  NPS_AUTH_LOADLIB_ERROR            = -122, // Couldn't load the dll
  NPS_AUTH_INTERNAL_ERROR           = -123, // Something with the Auth internals, contact EA.com
  NPS_AUTH_ERROR       							= -124, // Generic Auth error

	/**
	 * Available error codes
	 */
  NPS_PERSONAS_PER_SHARD_LIMIT_REACHED = -125,
  NPS_PERSONA_CREATION_DISABLED_ON_SHARD = -126,
  NPS_LOGIN_USER_BANNED = -127,
  NPS_NOT_USED4 		= -128,
  NPS_NOT_USED5     = -129,
  NPS_NOT_USED6  		= -130,
  NPS_NOT_USED7     = -131,
  NPS_NOT_USED8     = -132,

  /**
   * Crypto Errors
   */
  NPS_CRYPTO_ERR                    = -133,
  NPS_CRYPTO_NOT_INITIALIZED        = -134,
  NPS_CRYPTO_INVALID_KEY            = -135,
  NPS_CRYPTO_EXPIRED_KEY            = -136,

  NPS_ERR_ILLEGAL_USERNAME          = -137,
  NPS_ERR_INVALID_USERNAME          = -138,
  NPS_ERR_RESERVED_USERNAME         = -139,
  NPS_CRYPTO_NO_VALIDATION_SERVER   = -140,

  NPS_CRC_MISMATCH                  = -141,  // executable crc didn't match login server crc.
  NPS_RESTRICTED_ACCESS             = -142,  // your user class is currently restricted from logging in to this server.
  NPS_DIRTY_WORD                    = -143,  // didn't make it past the censors (illegal token)
  NPS_TOKEN_INVALID                 = -146,  // invalid characters in token
  NPS_TOKEN_RESERVED                = -147,  // token conflicts with reserved word list

  NPS_MAX_ERROR                     = -144,

  // these are for the rebroadcaster.  There are no "global error message
  // strings" for these errors, so MAKE SURE you leave these *after*
  // NPS_MAX_ERROR.
  NPS_HANDLER_FORCES_BREAK          = 444,
  NPS_REBUILD_FDSET                 = 555

}NPSSTATUS;


const char * NPS_StatusToString( NPSSTATUS );
const char * NPS_OpcodeToString( NPS_OPCODE );

// apps can recover from these errors and retry the database operation
#define FIRST_DB_RECOVERABLE_ERROR   NPS_DB_INVALID_CURSOR_STATE
#define LAST_DB_RECOVERABLE_ERROR   NPS_DB_NETWORK_ERROR

// this is kind of a backwords macro - the signs of the comparisons are
// reversed from what seems natural because the errors are negative numbers
#define IS_DB_RECOVERABLE_ERROR(a) ((a<=FIRST_DB_RECOVERABLE_ERROR)&&(a>=LAST_DB_RECOVERABLE_ERROR))
enum
{
  RETURN_OK                         =  0,
  RETURN_ERR                        = -1,
  RETURN_TIMEOUT                    = -2
};

typedef enum
{
  NPSTCP                            = 33,
  NPSUDP                            = 44,
  NPSUDPPREF                        = 55,
  NPSTCP_UDPPREF                    = 66
} NPS_PROTOCOL;

#define NPSREADPROFILE                  (1)
#define NPSWRITEPROFILE                 (2)

#define NPS_GS_PORT                     (500)

enum
{
  // Users that don't specify their Id start in this range
  NPSUser_AutoBase            = 0x80000000,

  // Users that are monitoring (vs playing) start in this range
  NPSUser_AutoMonitorBase     = 0x40000000,

  // Child processes that connect back use this range
  NPSUser_AutoProcMonitorBase = 0x20000000,

  // SysOp user ids - these users are ALWAYS invisible
  NPSUser_Admin               = 0x10000000,

  // SysOp user ids - these users are ALWAYS invisible
  NPSUser_AdminVisible        = 0x08000000
};

// Reserved ID's for MONITORS -- ? Why not give these things actuall
// accounts? DRS
#define NPS_MONITORID                   (0xFFFFDFFEUL)
#define NPS_MONITORBASE                 (0xFFFFBFFEUL)

// Reserved ID's for SERVERS -- ? Why not give these things actuall
// accounts? DRS
#define NPS_SERVERID                    (0xFFFFFFFEUL)
#define NPS_SERVERBASE                  (0xFFFFDFFFUL)

#define NPS_USERIDMAX               (0xFFFFBFFEUL)

// !!!THIS IS IN SECONDS!!!
#define NPS_HEARTBEAT_TIMEOUT_DEFAULT   (60)    // or 1 minute, currently

/**
 * NO OTHER COMMANDS SHALL BE LESS THAN 255 (FF) and NO COMMANDS
 * SHALL USE ANY OF THESE COMBINATIONS IN THEIR HIGH ORDER BYTES!!
 *
 * Commands to direct game messages
 */
typedef enum
{
  // short game message opcodes
  NPS_MIN_GAME_MESSAGE_OPCODE          = 0x80,
  NPS_SEND_ALL                         = NPS_MIN_GAME_MESSAGE_OPCODE,
  NPS_SEND_PLUG_IN                     = 0x81,
  NPS_SEND_GAME_READY_LIST             = 0x82,

  NPS_SEND_LIST                        = 0x83,
  NPS_SEND_NOT_LIST                    = 0x84,

  NPS_SEND_BUDDY                       = 0x85,
  NPS_SEND_SINGLE                      = 0x86,
  NPS_SEND_NOT_SINGLE                  = 0x87,
  NPS_GAME_MESSAGE                     = 0x88,

  NPS_MAX_SHORT_GAME_MESSAGE_OPCODE    = NPS_GAME_MESSAGE,

  // The following enums are for INTERNAL USE ONLY!  Users should
  // use the above opcodes ONLY!

  // long/logged game message opcodes
  NPS_SEND_ALL_LONG                    = 0x89,
  NPS_SEND_ALL_LONG_LOGGED             = 0x8A,
  NPS_SEND_PLUG_IN_LONG                = 0x8B,
  NPS_SEND_PLUG_IN_LONG_LOGGED         = 0x8C,
  NPS_SEND_GAME_READY_LIST_LONG        = 0x8D,
  NPS_SEND_GAME_READY_LIST_LONG_LOGGED = 0x8E,

  NPS_SEND_LIST_LONG                   = 0x8F,
  NPS_SEND_LIST_LONG_LOGGED            = 0x90,
  NPS_SEND_NOT_LIST_LONG               = 0x91,
  NPS_SEND_NOT_LIST_LONG_LOGGED        = 0x92,

  NPS_SEND_BUDDY_LONG                  = 0x93,
  NPS_SEND_BUDDY_LONG_LOGGED           = 0x94,
  NPS_SEND_SINGLE_LONG                 = 0x95,
  NPS_SEND_SINGLE_LONG_LOGGED          = 0x96,
  NPS_SEND_NOT_SINGLE_LONG             = 0x97,
  NPS_SEND_NOT_SINGLE_LONG_LOGGED      = 0x98,
  NPS_GAME_MESSAGE_LONG                = 0x99,
  NPS_GAME_MESSAGE_LONG_LOGGED         = 0x9A,

  NPS_MAX_LONG_GAME_MESSAGE_OPCODE     = NPS_GAME_MESSAGE_LONG_LOGGED,
  NPS_MAX_GAME_MESSAGE_OPCODE          = NPS_MAX_LONG_GAME_MESSAGE_OPCODE
}teNPS_LOBBY_MESSAGES;

// System wide error message
#define NPS_SYSTEM_ERROR                 0xfff
typedef struct
{
  NPS_OPCODE OpCode;    // The OpCode that had the error
  NPSSTATUS Err;    // The error code.  See above
  long ServerId;  // Optional -- the ServerId of the server that had the error
  long CommId;    // Optional -- The CommId of the channel that had the error
}
NPS_SysErr;

/**
 *  Commands from the game client to the game server
 *
 * ALL NEW COMMANDS MUST GO AT THE END OR THE REBROADCASTER, NPSDLL AND
 * NPSCLIENT DLL MUST BE IN SYNC.
 */
typedef enum
{
  NPS_LOGIN                           = 0x100,
  NPS_GET_USER_LIST                   = 0x101,
  NPS_GET_MY_USER_DATA                = 0x102, // not used!
  NPS_SET_MY_USER_DATA                = 0x103,
  NPS_LOG_OFF_SERVER                  = 0x104,
  NPS_CLOSE_COMM_CHANNEL              = 0x105,
  NPS_OPEN_COMM_CHANNEL               = 0x106,
  NPS_GET_CLIENT_COUNT                = 0x107,
  NPS_START_GAME                      = 0x108, // Unisex.
  NPS_READY_FOR_GAME                  = 0x109, // Unisex.
  NPS_START_GAME_SERVER               = 0x10A,
  NPS_SET_SLEEP_STATE                 = 0x10B,
  NPS_GET_SERVER_INFO                 = 0x10C,
  NPS_SET_COMM_FLAGS                  = 0x10D,
  NPS_GET_READY_LIST                  = 0x10E,
  NPS_SEND_SERVER_LIST                = 0x10F,
  NPS_SET_COMM_CHANNEL_RATE           = 0x110,
  NPS_SET_HEARTBEAT_TIMEOUT           = 0x111,
  NPS_GET_HEARTBEAT_TIMEOUT           = 0x112,
  NPS_SET_CHANNEL_DATA                = 0x113,
  NPS_FILE_START                      = 0x114,
  NPS_FILE_DATA                       = 0x115,
  NPS_FILE_COMPLETED                  = 0x116,
  NPS_BOOT_USER_FROM_CHANNEL          = 0x117,
  NPS_LOCATE_USER                     = 0x118,
  NPS_ENABLE_FILTER                   = 0x119,
  NPS_DISABLE_FILTER                  = 0x11A,
  NPS_SLEEP_SERVER                    = 0x11B,
  NPS_WAKE_SERVER                     = 0x11C,
  NPS_TERMINATE_GAME_SERVER           = 0x11D,
  NPS_SEND_SKU_REGISTRY               = 0x11E,
  NPS_SET_READY_FOR_GAME              = 0x11F,
  NPS_LOGIN_RESP                      = 0x120,
  NPS_SOCKET_RECONNECT                = 0x121,
  NPS_SET_SLOT                        = 0x122,
  NPS_GET_SLOT_LIST                   = 0x123,
  NPS_SET_CHANNEL_CLOSED              = 0x124,
  NPS_UDP_STATUS                      = 0x125,
  NPS_GET_USER_INFO                   = 0x126,
  NPS_GET_MASTER_LIST                 = 0x127,
  NPS_GET_MINI_USER_LIST              = 0x128,
  NPS_UDP_FAILURE                     = 0x129,
  NPS_BUDDYLIST_REFRESH               = 0x12A,
  NPS_BUDDYLIST_ADD_USERS             = 0x12B,
  NPS_BUDDYLIST_REMOVE_USERS          = 0x12C
}teNPS_LOBBYCLIENT_COMMANDS;

/**
 * Commands from the game server to the game client
 *
 * ALL NEW COMMANDS MUST GO AT THE END OR THE REBROADCASTER, NPSDLL AND
 * NPSCLIENT DLL MUST BE IN SYNC.
 */
typedef enum
{
  NPS_FORCE_LOGOFF             = 0x201,
  NPS_USER_LEFT                = 0x202, // User Left the Server
  NPS_USER_JOINED              = 0x203, // User Joined the Server
  NPS_USER_INFO                = 0x204,
  NPS_SYSTEM_ALERT             = 0x205, // NOT USED ANY MORE.
  NPS_CLIENT_COUNT             = 0x206,
  NPS_ACK                      = 0x207, // DLL HIDES THIS MESSAGE FROM USER CALLBACK
  NPS_USER_LEFT_CHANNEL        = 0x208, // A user left this Channel
  NPS_CHANNEL_CLOSED           = 0x209, // Channel Close (different than CHANNEL_DELETED?)
  NPS_DUP_USER                 = 0x20A, // Duplictae user, user already online.
  NPS_SERVER_FULL              = 0x20B, // Too many users on server
  NPS_USER_JOINED_CHANNEL      = 0x20C, // A user joined this channel
  NPS_SERVER_INFO              = 0x20D,
  NPS_CHANNEL_CREATED          = 0x20E, // Channel Added
  NPS_CHANNEL_DELETED          = 0x20F, // Channel Removed
  NPS_READY_LIST               = 0x210, // List of all Readied users in Channel
  NPS_USER_LIST                = 0x211, // List of all users in Channel
  NPS_SERVER_LIST              = 0x212,
  NPS_CHANNEL_DENIED           = 0x213, // Incorrect password.
  NPS_CHANNEL_GRANTED          = 0x214, // You have joined the channel.
  NPS_CHANNEL_CONDITIONAL      = 0x215, // You have joined an opaque channel, but cannot ready.
  NPS_SERVER_REDIRECT          = 0x216, // NOT USED AT THIS TIME.
  NPS_HEARTBEAT                = 0x217, // DLL HIDES THIS MESSAGE FROM USER CALLBACK
  NPS_HEARTBEAT_TIMEOUT        = 0x218, // DLL HIDES THIS MESSAGE FROM USER CALLBACK
  NPS_CHANNEL_UPDATE           = 0x219, // Update to some or all channel information
  NPS_FORCE_LEAVE_CHANNEL      = 0x21A, // DLL HIDES THIS MESSAGE FROM USER CALLBACK
  NPS_USER_LOCATION            = 0x21B, // Result of Find Command.
  NPS_GAME_SERVER_STARTED      = 0x21C, // For Room Servers, support for IsRacing flag
  NPS_GAME_SERVER_TERMINATED   = 0x21D, // For Room Servers, support for IsRacing flag
  NPS_VERSIONS_DIFFERENT       = 0x21E,
  NPS_SEND_VERSION_STRING      = 0x21F,
  NPS_GAME_SKU_REGISTRY_KEY    = 0x220,
  NPS_PLUGIN_ACK               = 0x221,
  NPS_SERVER_CRASHED           = 0x222, // sent to clients (users and monitors) when server is going away because of a bad signal
  NPS_OPEN_COMM_CHANNEL_ACK    = 0x223,
  NPS_GAME_SERVER_STATE_CHANGE = 0x224,
  NPS_SLOT_UPDATE              = 0x225,
  NPS_SLOT_LIST                = 0x226,
  NPS_CHANNEL_MASTER           = 0x227,
  NPS_CHANNEL_MASTER_LIST      = 0x228,
  NPS_MINI_USER_LIST           = 0x229,
  NPS_INVALID_KEY              = 0x22A,
  NPS_NO_VALIDATION_SERVER     = 0x22B,
  NPS_INC_MINI_USER_LIST       = 0x22C,
  NPS_DEC_MINI_USER_LIST       = 0x22D,
  NPS_BUDDY_LEFT               = 0x22E,
  NPS_BUDDYLIST_UPDATE         = 0x22F,
  NPS_OK_TO_LOGIN              = 0x230,
  NPS_Q_POSITION               = 0x231
}teNPS_LOBBYSERVER_COMMANDS;

// Commands within the game client or within the game server

// ALL NEW COMMANDS MUST GO AT THE END OR THE REBROADCASTER, NPSDLL AND
// NPSCLIENT DLL MUST BE IN SYNC.
enum
{
  NPS_SERVER_DISCONNECTED     = 0x2a0,
  NPS_INTERNAL_CONNECT        = 0x2a1,
  NPS_SET_SERVER_INFO         = 0x2a2,
  NPS_NOP                     = 0x2a3
};

// Commands from the monitor to a game server
enum
{
  NPS_ATTACH                  = 0x301,
  NPS_SEND_RIFF_LIST          = 0x302,
  NPS_OPEN_RIFF               = 0x303,
  NPS_MONITOR_COMMAND         = 0x304,
  NPS_MONITOR_MESSAGE         = 0x305, // Monitor to Game Client as well
  NPS_ATTACH_ACK              = 0x306,
  NPS_LOGIN_ACK               = 0x307,
  NPS_ATTACH_RESP             = 0x308,
  NPS_SEND_GAME_SERVERS_LIST  = 0x309,
  NPS_RESTART_ROOM_SERVER     = 0x30A,
  NPS_SEND_STATISTICS         = 0x30B,
  NPS_SEND_MINI_RIFF_LIST     = 0x30C,
  NPS_GAG_USER                = 0x30D,
  NPS_UNGAG_USER              = 0x30E,
  NPS_SET_MAX_USERS           = 0x30F,
  NPS_SET_MAX_CHANNELS        = 0x310
};

// Commands from a game server to the monitor
enum
{
  NPS_RIFF_LIST               = 0x401,
  NPS_GAME_SERVERS_LIST       = 0x402,
  NPS_STATISTICS              = 0x403,
  NPS_MINI_RIFF_LIST          = 0x404
};

/**
 * Commands from the login client to the login server
 * ALL NEW COMMANDS MUST GO AT THE END OR LOGINCLIENT, LOGIN CGI &
 * NPSCLIENT DLL MUST BE IN SYNC.
 */
typedef enum
{
  NPS_USER_LOGIN              = 0x501,
  NPS_GAME_LOGIN              = 0x502,
  NPS_REGISTER_GAME_LOGIN     = 0x503,
  NPS_SET_GAME_BLOB           = 0x504,
  NPS_GET_NEXT_SERVER         = 0x505,
  NPS_NEW_EA_ACCOUNT          = 0x506,
  NPS_NEW_GAME_ACCOUNT        = 0x507,
  NPS_UPDATE_EA_ACCOUNT       = 0x508,
  NPS_UPDATE_GAME_ACCOUNT     = 0x509,
  NPS_LOCATE_PLAYER           = 0x50A,
  NPS_GET_BUDDY_LIST          = 0x50B,
  NPS_ADD_BUDDY_LIST          = 0x50C,
  NPS_DELETE_BUDDY_LIST       = 0x50D,
  NPS_CLEAR_BUDDY_LIST        = 0x50E,
  NPS_REGISTER_GAME_LOGOUT    = 0x50F,
  NPS_GET_GENERIC_HS_DATA     = 0x510,
  NPS_PUT_GENERIC_HS_DATA     = 0x511,
  NPS_DELETE_GAME_PERSONA     = 0x512,
  NPS_READ_SERVER_DATA_LIST   = 0x513,
  NPS_DELETE_GENERIC_HS_DATA  = 0x514,
  NPS_GET_PLAYER_RANK         = 0x515,
  NPS_GET_TOP_PLAYERS         = 0x516,
  NPS_ADD_BUDDY_BY_NAME       = 0x517,
  NPS_GET_BUDDY_INFO          = 0x518,
  NPS_GET_PERSONA_INFO        = 0x519,
  NPS_GET_LEADER_BOARD        = 0x51A,

  /**
   * the SRP authentication requires handshaking.  The user sends his/her
   * username (no passwd).  The server sends back the users N, G and S.
   * The user then sends A and the server replys with B.  Now both sides
   * have a shared secret and we only have to verify each side.
   */
  NPS_SRP_USERNAME            = 0x51B,
  NPS_SRP_NGS                 = 0x51C,
  NPS_SRP_A                   = 0x51D,
  NPS_SRP_B                   = 0x51E,
  NPS_SRP_USER_VERIFY         = 0x51F,
  NPS_SRP_SERVER_VERIFY       = 0x520,

  /* now back to our regularly scheduled enumerations */
  NPS_FILE_BUG_REPORT         = 0x521,
  NPS_GET_GENERIC_FIELD_RANKING = 0x522,

  NPS_SEND_EMAIL              = 0x523,
  NPS_RECEIVE_EMAIL           = 0x524,
  NPS_DELETE_EMAILS           = 0x525,
  NPS_LIST_EMAILS             = 0x526,

  NPS_AAI_REQUEST             = 0x530,
  NPS_AAI_CRACK               = 0x531,

  NPS_GET_PERSONA_MAPS        = 0x532,

  NPS_VALIDATE_PERSONA_NAME   = 0x533,
  NPS_CHECK_TOKEN             = 0x534,
  NPS_GET_USER_STATUS         = 0x535

}teNPS_LOGINCLIENT_COMMANDS;

/**
 * Commands from the login server to the login client
 *
 * ALL NEW COMMANDS MUST GO AT THE END OR LOGINCLIENT, LOGIN CGI &
 * NPSCLIENT DLL MUST BE IN SYNC.
 */
typedef enum
{
  NPS_USER_VALID             = 0x601,
  NPS_USER_INVALID           = 0x602,
  NPS_GAME_BLOB              = 0x603,
  NPS_NO_GAME_BLOB           = 0x604,
  NPS_NEXT_SERVER            = 0x605,
  NPS_EA_ACCOUNT_INFO        = 0x606,
  NPS_GAME_ACCOUNT_INFO      = 0x607,
  NPS_BUDDY_LIST             = 0x608,
  NPS_PLAYER_LOCATION        = 0x609,
  NPS_DB_ERROR               = 0x60A,
  NPS_HS_DATA                = 0x60B,
  NPS_USER_DELETED           = 0x60C,
  NPS_HS_DELETED             = 0x60D,
  NPS_PLAYER_RANK            = 0x60E,
  NPS_TOP_PLAYERS            = 0x60F,
  NPS_USER_NOT_AUTHORIZED    = 0x610,
  NPS_INVALID_GAME           = 0x611,
  NPS_NO_GAME_USER           = 0x612,
  NPS_GAME_ACCOUNT_COUNT     = 0x613,
  NPS_BUDDY_LIST_COUNT       = 0x614,
  NPS_BUDDY_INFO             = 0x615,
  NPS_PERSONA_INFO           = 0x616,
  NPS_LEADER_BOARD           = 0x617,
  NPS_LEADER_BOARD_NO_CHARS  = 0x618,

  NPS_IN_GAME_EMAIL          = 0x619,
  NPS_SEND_EMAIL_ACK         = 0x61a,
  NPS_IGE_LIST               = 0x61b,

  NPS_AAI_REQUEST_REPLY      = 0x620,

  NPS_COMM_ERROR             = 0x621,
  NPS_USER_CREATED           = 0x622,

  // returned when NPS refuses authorization to this class of user
  NPS_AUTH_DENIED            = 0x623,
  NPS_PERSONA_DELETION_DENY  = 0x624,

  NPS_DUP_PERSONA            = 0x625,
  NPS_USER_BANNED            = 0x626,     // usee has been banned in GLS.

  NPS_MISSING_AUTHENTICATOR  = 0x630,
  NPS_MISSING_AAI_URL,
  NPS_AAI_EXCEPTION,
  NPS_AAI_INTERNAL_ERROR,

  NPS_ILLEGAL_TOKEN,                      // token failed the feelth test.
  NPS_INVALID_TOKEN,                      // token contains invalid characters
  NPS_RESERVED_TOKEN,                     // token contains text that voilated reserved word list.

  NPS_INVALID_CRC,                        // client-side crc did not match server-side crc.

  NPS_SHARD_PERSONA_LIMIT_REACHED = 0x640,  // we are only allowed one persona per shard.
  NPS_SHARD_PERSONA_CREATION_DISABLED = 0x641, // persona creation on this shard is disabled.

  NPS_LS_OPERATION_TIMEOUT,               // the current operation took too much d@mned time and got nuked.


  NPS_UNDEFINED_ERROR        = 0x650
}teNPS_LOGINSERVER_COMMANDS;

// Commands server-to-server
enum
{
  NPS_KILL_SERVER            = 0x701,
  NPS_INIT_SERVER            = 0x702,
  NPS_ATTACH_SERVER          = 0x703,
  NPS_CHANNEL_COUNT          = 0x704,
  // NPS_EXTERNAL_SOURCE should never be used for an actual sent message
  // this is only a key for the externalmessage queue
  NPS_EXTERNAL_SOURCE        = 0x705,
  NPS_LOGIN_COUNT            = 0x706
};

// Commands from the server daemon to the director, and back
enum
{
  NPS_UPDATE_SERVER_STATISTICS     = 0x781,
  NPS_INITIAL_SERVER_STATISTICS    = 0x782,
  NPS_SERVERDAEMON_UPDATE_RATE     = 0x783
};

// Command within a DLL
enum
{
  NPS_DLL_INTERNAL            = 0x901
};

// TODO pull this define after I have convinced myself that crypto works.
#define DO_CRYPTO 1

// leave room to grow...
enum
{
  NPS_CRYPTO_PUB_KEY         = 0x1001, //! use RSA public key

  NPS_CRYPTO_DES_CBC         = 0x1101, //! use DES_CBC

  NPS_CRYPTO_DES_CBC_ID      = 0x1201  //! use DES_CBC, userid NOT encrypted
};


// commands between client and PQServer
enum {
  NPS_PQS_CLIENT_REQUEST         = 0x1301,    // client connect to PQS message.
  NPS_PQS_QUEUE_DATA             = 0x1302,    // data from PLS (user x/y)
  NPS_PQS_ACCEPT                 = 0x1303     // accepted to the PLS.
};


// DAG 10/28/98 - added this so that we can get textual descriptions of an
// command constant use THIS interface ALWAYS to get a string from a
// command constant.
#ifdef __cplusplus
extern "C"
{
#endif

  const char *NPSCommandToString (int command);
  const char *NPSErrorToString   (int err);

#define NPSCMD_STRING(command)  CommandToString(command)

#ifdef __cplusplus
}
#endif

// Language identifiers for DBAdmin
enum
{
  NPS_LANG_GERMAN           = 0x0407,
  NPS_LANG_ENGLISH_US       = 0x0409,
  NPS_LANG_SPANISH          = 0x040a,
  NPS_LANG_FRENCH           = 0x040c,
  NPS_LANG_JAPANESE         = 0x0411,
  NPS_LANG_SWEDISH          = 0x041d,
  NPS_LANG_UNKNOWN          = 0xffff // Put valid languages before this entry
};

// Version extraction methods for DBAdmin
enum
{
  NPS_VERSION_FROM_REGISTRY = 0,
  NPS_VERSION_FROM_FILE     = 1,
  NPS_VERSION_FROM_UNKNOWN  = 2  // Put valid methods before this one
};

/**
 * These flags change the way the channel behaves
 */
typedef enum
{
  /*** This is a private, password protected channel*/
  NPS_PRIVATE_CHANNEL   = 1<<0,

  /*** This is a public channel*/
  NPS_PUBLIC_CHANNEL    = 1<<1,

  /**
   * This is an opaque channel.  Users may enter, but can only ready if
   * they have the password.
   */
  NPS_OPAQUE_CHANNEL    = 1<<2,

  /*** This is a permanent channel*/
  NPS_PERMANENT_CHANNEL = 1<<3,

  /*** This is a transient channel (collapses when the last user leaves)*/
  NPS_TRANSIENT_CHANNEL = 1<<4,

  /*** Set the GameReady flag for this user*/
  NPS_SET_GAME_READY    = 1<<5,

  /*** Clear the GameReady flag for this user*/
  NPS_CLEAR_GAME_READY  = 1<<6,

  /*** Don't let other users know about this channel*/
  NPS_HIDE_CHANNEL      = 1<<7,

  /*** Let everyone see this channel*/
  NPS_EXPOSE_CHANNEL    = 1<<8,

  /**
   * If message coalescing, send when SendRate expires OR when a second
   * message from any user is received
   */
  NPS_SEND_ON_DUP       = 1<<9,

  /**
   * If message coalescing, send when SendRate expores regardless of second
   * messages  - second messages are added to the group and sent at that
   * time
   */
  NPS_SEND_ON_TIME      = 1<<10,

  /**
   * (only valid when the channel is first created) - Launch a game server
   * for this channel (not for game use)
   */
  NPS_AUTOLAUNCH_SERVER = 1<<11,

  /*** Use this to coallesce messages in the server*/
  NPS_SET_SERVER_RATE   = 1<<12,

  /*** Use this to coallesce messages in the client*/
  NPS_SET_DLL_RATE      = 1<<13,

  /*** Use this to disable message backlogging*/
  NPS_DISABLE_BACKLOG   = 1<<14,

  /*** Use this to enable message backlogging*/
  NPS_ENABLE_BACKLOG    = 1<<15
}teNPS_COMM_CHANNEL_FLAGS;

/**
 * These flags change the way the server behaves
 */
typedef enum
{
  /**
   * Use a polling read (application calls NPSReadGameMessage)
   * rather than default callback mechanism
   */
  NPS_POLLING_READ                  = 1<<0,
  /*** Use default callback mechanism for game messages*/
  NPS_STANDARD_READ                 = 1<<1,
  /*** Send messages from a seperate thread (slightly faster return from NPSSendMessage)*/
  NPS_THREADED_SEND                 = 1<<2,
  /*** Use default method for sending messages*/
  NPS_STANDARD_SEND                 = 1<<3,
  /*** (LAN only) Respond to broadcast requests*/
  NPS_ACCEPT_HOST_ANNOUNCE          = 1<<4,
  /*** (LAN only) Don't respond to broadcast requests*/
  NPS_DENY_HOST_ANNOUNCE            = 1<<5,
  /*** Don't allow any more users into this server*/
  NPS_LOCK_USER_CONNECTIONS         = 1<<6,
  /*** Allow users into this channel*/
  NPS_UNLOCK_USER_CONNECTIONS       = 1<<7,
  /*** Enable checksum processing (DEBUG only -- slows down system)*/
  NPS_ENABLE_CHECKSUM               = 1<<8,
  /*** Disable checksum processing (default)*/
  NPS_DISABLE_CHECKSUM              = 1<<9,
  /*** Filter messages from the server (no user messages)*/
  NPS_FILTER_MESSAGES               = 1<<10,
  /*** Disable filtering*/
  NPS_RELEASE_MESSAGES              = 1<<11,
  /*** Enable sleep mode on the server*/
  NPS_SERVER_SLEEP                  = 1<<12,
  /*** Wake the server back up again*/
  NPS_SERVER_AWAKE                  = 1<<13,
  NPS_OLD_PROTOCOL                  = 1<<14,
  /*** User is gagged on this server - no chat */
  NPS_GAGGED                        = 1<<15
}teNPS_SERVER_FLAGS;

// Slot flags
typedef enum
{
  NPS_SLOT_ANY  = 1,   // Allow any slot
  NPS_SLOT_ONLY = 2    // Only the requested slot
}teNPS_SLOT_FLAGS;

// High score falgs
#define NPS_READ_HS_LOBBY             1
#define NPS_READ_HS_ROOM              2
#define NPS_READ_HS_PERM_GAME         4
#define NPS_READ_HS_GAME              8
#define NPS_READ_HS_TOURNEY          16

// ServerIsRunning flags
#define NPS_SERVER_NOT_RUNNING        1
#define NPS_SERVER_START_PENDING      2
#define NPS_SERVER_RUNNING            4
#define NPS_CHANNEL_CLOSED_MASK       8

#endif // _NPS_TYPES_H_
