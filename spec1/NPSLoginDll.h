/*
 * Name:
 *  NPSLoginDll.h
 *
 *
 * Copyright (c) 2000 by Electronic Arts.
 *
 */

#ifndef _NPS_LOGIN_DLL_H
#define _NPS_LOGIN_DLL_H

#ifndef WIN32
  #include "port.h"
  #define __stdcall
#else
  #include <windows.h>
  #include <winbase.h>
#endif

#include <time.h>
#include "NPSTypes.h"
#include "NPSUserLogin.h"

#ifdef BUILDING_DLL
  #define NPSLoginDllExport    __declspec( dllexport )
  #define NPSCDECL
#else
  #define NPSLoginDllExport
  #ifdef WIN32
    #define NPSCDECL __cdecl
  #endif
#endif

#define AUTH_MAX_TICKET_LEN 128

#define NPSLoginAPI_Interface struct
#define NPSLoginAPI_Method    public: virtual

/**
 * This constant is used to determine compatibility of the client with the DLL, and
 * the DLL with the server.  From left to right, the decimal level mean the following:
 * <UL>
 * <LI>1. Major Version
 * <LI>2. API Version
 * <LI>3. Structures Version
 * <LI>4. Code Version
 * </UL>
 * @since NPS 1.0.0.0
 */
#define NPSLOGINCLIENTDLL_VERSION_ID    "2.0.0.4"   // Major.API.Struct.Code


#ifdef DOXYGEN

/**
 * The NPSLoginAPI is now contained within this structure which is passed to the user
 * through a call to NPSAPI_GetInterface().
 *
 * @author cwalters@ea.com
 * @author fhutchinson@ea.com
 * @version 4.3.0.0
 * @since 4.3.0.0
 */
class cNPSLoginAPI
#else
NPSLoginAPI_Interface cNPSLoginAPI
#endif
{
	/**
	 * Log a user into NPS Login Server.  Checks that user has an EA.com Account.
	 *
	 * @param UserName     EA.com user name
	 *
	 * @param Password     EA.com password
	 *
	 * @param aaiServiceId EA.com Service ID (special identifier bestowed to a SKU by EA.com)
	 *
	 * @param crc          32-bit crc which must match that on the server side to authenticate
	 *
	 * @param UserStatus   (NPS_UserStatus) filled in by this method on successful return.
	 *
	 * @param errTxt       Error string written to by authlogin.dll if this method returns NPS_AUTH_ERROR.
	 *
	 * @param url          (char[]) Error URL written to by authlogin.dll if this method returns NPS_AUTH_ERROR.
	 *
	 * @param IdleCallBack (can be NULL)
	 *
	 * @param Context      (can be NULL)
	 *
	 * @return (NPSSTATUS)
	 */
  NPSLoginAPI_Method NPSSTATUS
    NPSUserLogin( const char * UserName, const char * Password, const char * aaiServiceId, unsigned int crc, NPS_UserStatus & UserStatus, char errTxt[512], char url[512], tfIdleCallBack IdleCallBack, void * Context );

	/**
	 * Authenticate a user against the EA.com AAI server, and get the ticket.
	 *
	 * @param UserName     EA.com user name
	 *
	 * @param Password     EA.com password
	 *
	 * @param aaiServiceId EA.com Service ID (special identifier bestowed to a SKU by EA.com)
	 *
	 * @param outTicket    EA.com ticket if successful
	 *
	 * @param errTxt       Error string provided by Auth V3.
	 *
	 * @param url          URL provided by Auth V3 that points to a page explaining how to remedy the Auth
	 *                     error.
	 *
	 * @return (NPSSTATUS)
	 */
  NPSLoginAPI_Method NPSSTATUS
    NPSAuthenticateUser(const char * UserName, const char * Password, const char * aaiServiceId, char outTicket[AUTH_MAX_TICKET_LEN],	char errTxt[512], char url[512]);

  /**
   * Log a user out of a game and off the system
   *
   * @param CustomerId   (NPS_CUSTOMERID) The ID of the customer as held by NPS
   *
   * @param IdleCallBack
   *
   * @param Context
   *
   * @return (NPSSTATUS) NPS_OK if log off message was sent to the server
   */
  NPSLoginAPI_Method NPSSTATUS
    NPSLogOutUser(NPS_CUSTOMERID CustomerId,tfIdleCallBack IdleCallBack,void *Context);

  /**
   * Log a persona off the system
   *
   * @param GameUserId   (NPS_GAMEUSERID) The persona ID of the persona to log off
   *
   * @param IdleCallBack
   *
   * @param Context
   *
   * @return (NPSSTATUS) NPS_OK if the message was sent to the server
   */
  NPSLoginAPI_Method NPSSTATUS
    NPSLogOutGameUser(NPS_CUSTOMERID CustomerId,
                      NPS_GAMEUSERID GameUserId,
                      NPS_SHARDID    ShardId,
                      tfIdleCallBack IdleCallBack,
                      void *         Context);

  /**
   *
   * @param CustomerId
   *
   * @param GameName
   *
   * @param FirstPersona
   *
   * @param IdleCallBack
   *
   * @param Context
   *
   * @return (NPSSTATUS)
   */
  NPSLoginAPI_Method NPSSTATUS
    NPSGetFirstGamePersona(NPS_CUSTOMERID CustomerId,
                           char *GameName,
                           UserGameData *FirstPersona,
                           tfIdleCallBack IdleCallBack,
                           void *Context);

  /**
   *
   * @param NextPersona
   *
   * @param IdleCallBack
   *
   * @param Context
   *
   * @return (NPSSTATUS)
   */
  NPSLoginAPI_Method NPSSTATUS
    NPSGetNextGamePersona(UserGameData *NextPersona,tfIdleCallBack IdleCallBack,void *Context);

  /**
   *
   * @param GameUserId
   *
   * @param IdleCallBack
   *
   * @param Context
   *
   * @return (NPSSTATUS)
   */
  NPSLoginAPI_Method NPSSTATUS
    NPSSelectGamePersona(NPS_CUSTOMERID CustomerId,
                         NPS_GAMEUSERID GameUserId,
                         NPS_SHARDID shard_id,
                         tfIdleCallBack IdleCallBack,
                         void *Context);

  /**
   * Return the persona information for a specific user
   *
   * @param GameUserId
   *
   * @param PersonInfo
   *
   * @param IdleCallBack
   *
   * @param Context
   *
   * @return (NPSSTATUS)
   */
  NPSLoginAPI_Method NPSSTATUS
    NPSGetPersonaInfo(NPS_GAMEUSERID GameUserId,
                      UserGameData *PersonInfo,
                      tfIdleCallBack IdleCallBack,
                      void *Context);

  /**
   *
   * @param GameUserName
   *
   * @param GameName
   *
   * @param PersonInfo
   *
   * @param IdleCallBack
   *
   * @param Context
   *
   * @return (NPSSTATUS)
   */
  NPSLoginAPI_Method NPSSTATUS
    NPSGetPersonaInfoByName(char *GameUserName,
                            char *GameName,
                            UserGameData *PersonInfo,
                            tfIdleCallBack IdleCallBack,
                            void *Context);

	/**
	 * Create a game persona for an account.
	 *
	 * @param LoginId             EA account ID (returned from NPSUserLogin)
	 *
	 * @param GameName
	 *
	 * @param UserGameInfo        The user to create
	 *
	 * @param MaxPersonasPerShard (unsigned long *) Can be NULL.  If not NULL, then value will be set when a
	 *                            NPS_SHARD_PERSONA_LIMIT_REACHED error message is returned.
	 *
	 * @param IdleCallBack
	 *
	 * @param Context
	 *
	 * @return (NPSSTATUS)
	 *         <UL>
	 *         <LI>NPS_OK: Persona Created
	 *         <LI>NPS_ERR_DUP_USER: Persona name is taken
	 *         <LI>NPS_USER_NOT_FOUND: Invalid customer ID
	 *         <LI>NPS_DATABASE_ERROR
	 *         <LI>NPS_NO_MORE_PERSONAS: user has maxed out personas
	 *         <LI>NPS_ERR_INVALID_USERNAME: supplied name contains invalid characters
	 *         <LI>NPS_ERR_ILLEGAL_USERNAME: supplied name failed filth filter test
	 *         <LI>NPS_ERR_RESERVED_USERNAME: supplied name violated the reserved word list
	 *         <LI>NPS_BAD_COMM_CHANNEL
	 *         <LI>NPS_PERSONAS_PER_SHARD_LIMIT_REACHED
	 *         <LI>NPS_PERSONA_CREATION_DISABLED_ON_SHARD
	 *         <LI>NPS_ERR
	 *         </UL>
	 *
	 * @see NPSUserLogin.h
	 */
  NPSLoginAPI_Method NPSSTATUS
    NPSCreateGamePersona(NPS_LOGINID LoginId,
                         char *GameName,
                         UserGameData *UserGameInfo,
												 unsigned long *MaxPersonasPerShard,
                         tfIdleCallBack IdleCallBack,
                         void *Context);

  /**
   * Update an existing game persona.
   *
   * @param NewPersona
   *
   * @param IdleCallBack
   *
   * @param Context
   *
   * @return (NPSSTATUS)
   */
  NPSLoginAPI_Method NPSSTATUS
    NPSUpdateGamePersona(UserGameData *NewPersona,
                         tfIdleCallBack IdleCallBack,
                         void *Context);

  /**
   * Remove a user's persona from the database.
   *
   * @param GameUserId
   *
   * @param IdleCallBack
   *
   * @param Context
   *
   * @return (NPSSTATUS) NPS_OK on success.
   */
  NPSLoginAPI_Method NPSSTATUS
    NPSDeleteGamePersona(NPS_CUSTOMERID,
                         NPS_GAMEUSERID GameUserId,
                         tfIdleCallBack IdleCallBack,
                         void *Context);

  /**
   * Get the buddy list for a given user game profile.
   * There may be NPS_MAX_BUDDIES buddies for a single persona.
   * This method only returns the first buddy
   * The remaining buddies are stored locally for future retreival
   *
   * @param GameUserId   Persona ID of the buddy list owner
   *
   * @param FirstBuddy   (BuddyListInfo) Filled in with the info of the first buddy if success.
   *
   * @param IdleCallBack
   *
   * @param Context
   *
   * @return <UL>
   *         <LI>NPS_OK we found a buddy
   *         <LI>NPS_NO_MORE_BUDDIES no buddies were found for this persona
   *         <LI>NPS_USER_NOT_FOUND the persona was not found
   *         <LI>NPS_ERR the server returned something other than the buddy count.
   *         </UL>
   *
   * @see NPSGetNextBuddy
   */
  NPSLoginAPI_Method NPSSTATUS
    NPSGetFirstBuddy(NPS_GAMEUSERID GameUserId,
                     BuddyListInfo *FirstBuddy,
                     tfIdleCallBack IdleCallBack,
                     void *Context);

  /**
   * Retrieves the next buddy from the internal buddy list generated in NPSGetFirstBuddy()
   *
   * @param Buddy
   *
   * @param IdleCallBack
   *
   * @param Context
   *
   * @return <UL>
   *         <LI>NPS_OK we found a buddy
   *         <LI>NPS_NO_MORE_BUDDIES no buddies were found for this persona
   *         <LI>NPS_ERR the server returned something other than the buddy count.
   *         </UL>
   *
   *
   * @see NPSGetFirstBuddy
   */
  NPSLoginAPI_Method NPSSTATUS
    NPSGetNextBuddy(BuddyListInfo *Buddy,tfIdleCallBack IdleCallBack,void *Context);

  /**
   * Return a BuddyListInfo structure for a particular persona
   *
   * @param MyGameUserId
   *
   * @param BuddyGameUserId
   *
   * @param TheBuddy
   *
   * @param IdleCallBack
   *
   * @param Context
   *
   * @return
   */
  NPSLoginAPI_Method NPSSTATUS
    NPSGetBuddyInfo(NPS_GAMEUSERID MyGameUserId,
                    NPS_GAMEUSERID BuddyGameUserId,
                    BuddyListInfo *TheBuddy,
                    tfIdleCallBack IdleCallBack,
                    void *Context);

  /**
   * Return a BuddyListInfo structure for a particular persona
   *
   * @param MyGameUserId
   *
   * @param GameUserName
   *
   * @param GameName
   *
   * @param TheBuddy
   *
   * @param IdleCallBack
   *
   * @param Context
   *
   * @return
   */
  NPSLoginAPI_Method NPSSTATUS
    NPSGetBuddyInfoByName(NPS_GAMEUSERID MyGameUserId,
                          char *GameUserName,
                          char *GameName,
                          BuddyListInfo *TheBuddy,
                          tfIdleCallBack IdleCallBack,
                          void *Context);

  /**
   * Add a user to the buddy list for a given user.
   *
   * @param NewBuddy     Pass in the user's LoginId,
   *                     the game name, the user's current game user name, and the game user
   *                     name of the buddy you want to add.
   *
   * @param IdleCallBack
   *
   * @param Context
   *
   * @return
   */
  NPSLoginAPI_Method NPSSTATUS
    NPSAddToBuddyList(AddBuddyListStruct *NewBuddy,tfIdleCallBack IdleCallBack,void *Context);

  /**
   * Add a user to the buddy list for a given user.
   *
   * @param NewBuddy     Pass in the user's Persona Name,
   *                     the game name, the user's current game user name, and the game user
   *                     name of the buddy you want to add.
   *
   * @param GameName
   *
   * @param IdleCallBack
   *
   * @param Context
   *
   * @return
   */
  NPSLoginAPI_Method NPSSTATUS
    NPSAddToBuddyListByName(AddBuddyByNameStruct *NewBuddy,
                            char *GameName,
                            tfIdleCallBack IdleCallBack,
                            void *Context);

  /**
   * Delete an entry from the persona's buddy list.
   *
   * @param MyGameUserId
   *
   * @param BuddyGameUserId
   *
   * @param IdleCallBack
   *
   * @param Context
   *
   * @return
   */
  NPSLoginAPI_Method NPSSTATUS
    NPSRemoveFromBuddyList(NPS_GAMEUSERID MyGameUserId,
                           NPS_GAMEUSERID BuddyGameUserId,
                           tfIdleCallBack IdleCallBack,
                           void *Context);

  /**
   * Clear the entire buddy list for a user.
   * This will nuke all entries in the buddy list.
   *
   * @param GameUserId
   *
   * @param IdleCallBack
   *
   * @param Context
   *
   * @return
   */
  NPSLoginAPI_Method NPSSTATUS
    NPSClearBuddyList(NPS_GAMEUSERID GameUserId,
                      tfIdleCallBack IdleCallBack,
                      void *Context);

  /**
   * Return the database version string for this game
   *
   * @param GameName
   *
   * @param GameVersion
   *
   * @param IdleCallBack
   *
   * @param Context
   *
   * @return
   */
  NPSLoginAPI_Method NPSSTATUS
    NPSGetGameVersion(char *GameName,
                      char *GameVersion,
                      tfIdleCallBack IdleCallBack,
                      void *Context);


  /**
   * set the servername and port explicitly for the library
   *
   * @param hostname Domain name of the login server
   *
   * @param port     Port of the login server
   *
   * @return
   */
  NPSLoginAPI_Method void
    NPSSetLoginServer(char *hostname, short port);

  /**
   * Checks the version of this build
   *
   * @param version_string A version string in the following format 1.1.1.1
   *
   * @return TRUE if version are similiar at the Major.API.Struture level.
   */
  NPSLoginAPI_Method BOOL
    NPSCheckVersionString(char *version_string);

  /**
   * Retrieves a users mail IDs from the login server.
   *
   * @param firstID      The first mailID in the range to get (0=all)
   *
   * @param lastID       Last mail ID in the range to get
   *
   * @param myId         The GameUserID of the personas mailbox being accessed
   *
   * @param IdleCallBack
   *
   * @param Context      A generic pointer that is passed to the Idle/Cancel callback
   *
   * @return The NPSSTATUS of the get operation.  NPS_OK on success.
   *
   * @see NPSLogin\Src\InGameEmail# NPSGetFirstEmail
   * @see NPSLogin\Src\InGameEmail# NPSGetNextEmail
   * @since NPS2
   */
  NPSLoginAPI_Method NPSSTATUS
    NPSGetMail(NPS_MAILID firstID,
               NPS_MAILID lastID,
               NPS_GAMEUSERID myId,
               tfIdleCallBack IdleCallBack,
               void *Context);

  /**
   * Sends an email to a list of users.
   *
   * @param item
   *
   * @param IdleCallBack
   *
   * @param Context
   *
   * @return (NPSSTATUS) NPS_OK if the message was successfully received by the login server.
   * @since NPS 2.0.0.0
   */
  NPSLoginAPI_Method NPSSTATUS
    NPSSendMail(NPS_SendMail *item,
                tfIdleCallBack IdleCallBack,
                void *Context);

  /**
   *
   * @param item
   *
   * @param myId
   *
   * @param headerOnly
   *
   * @param IdleCallBack
   *
   * @param Context
   *
   * @return
   */
  NPSLoginAPI_Method NPSSTATUS
    NPSGetFirstMail(NPS_IncMail *item,
                    NPS_GAMEUSERID myId,
                    NPS_LOGICAL headerOnly,
                    tfIdleCallBack IdleCallBack,
                    void *Context);

  /**
   * Gets the mail message from the server, for the next mailid stored in the
   * state buffer.  NPSGetMail and NPSGetFirstMail should have been called before
   * call this function.
   *
   * @param item         A pointer to a preallocated NPS_IncMail structure that will be filled out on
   *                     a sucessful return.
   *
   * @param myId         GameUserId for person you are retrieving mail for
   *
   * @param headerOnly
   *
   * @param IdleCallBack
   *
   * @param Context
   *
   * @return (NPSSTATUS) NPS_OK on success. NPS_NO_MESSAGES if a privious call to NPSGetMail
   *         wasn't made, failed, or return no message ID's.
   */
  NPSLoginAPI_Method NPSSTATUS
    NPSGetNextMail(NPS_IncMail *item,
                   NPS_GAMEUSERID myId,
                   NPS_LOGICAL headerOnly,
                   tfIdleCallBack IdleCallBack,
                   void *Context);

  /**
   *
   * @param deleteID
   *
   * @param myId
   *
   * @param IdleCallBack
   *
   * @param Context
   *
   * @return
   */
  NPSLoginAPI_Method NPSSTATUS
    NPSDeleteMail(NPS_MAILID deleteID,
                  NPS_GAMEUSERID myId,
                  tfIdleCallBack IdleCallBack,
                  void *Context);


  /**
   * Get a ticket for the specified user from the EAS internal AAI server.
   * You must set up the ip and port to point to the correct server
   * (this is NOT the same server as the LoginServer)
   *
   * @param username     username to authenticate
   *
   * @param password
   *
   * @param gamename     The gamename for the SKU as given by NPS.  Name is used to look up SKUS
   *                     inServiceID.
   *
   * @param ticket       buffer for ticket (out) must be at least NPS_AAIRequestReply::MAX_CONTEXT_LENGTH bytes
   *
   * @param IdleCallBack
   *
   * @param Context
   *
   * @return NPS_OK success, and the ticket is written to the parameter 'ticket'
   */
  NPSLoginAPI_Method NPSSTATUS
    NPS_AAI_RequestEAS ( const char *      username,
                         const char *      password,
                         const char *      gamename,
                         char *            ticket,
                         tfIdleCallBack    IdleCallBack = NULL,
                         void *            Context      = NULL );



  /**
   * Retrieve the persona list for the specified user from the global login database
   *
   * @param personaInfo  A reference to a UserGameData pointer. An array of the appropriate size will be
   *     allocated and returned, filled with persona entries. It is the caller's responbsibility to
   *     delete[] the memory for this array. PersonaInfo will be NULL if no personas were found.
   *
   * @param record_count (out) the number of records found will be written here (the size of the
   *     allocated array)
   *
   * @param ClientCallBack
   *
   * @param Context
   *
   * @return NPS_GAME_ACCOUNT_INFO on success
   */
  NPSLoginAPI_Method NPSSTATUS
    NPSGetPersonaMaps( NPS_CUSTOMERID customerId,
                       UserGameData *&  PersonaInfo,
                       unsigned short &record_count,
                       tfIdleCallBack  ClientCallBack,
                       void *          Context);



  /**
   * Retrieve the maximum numbers of personas a user may have at any one time.
   * This value is only set on failure to create a persona when a user has maxed out on
   * persona 'slots'.
   *
   * @return the maximum number of personas
   */
  NPSLoginAPI_Method unsigned long
    NPSGetMaxPersonaCount() const { return (maxPersonaCount_);}


  /**
   * Retrieve the value in seconds of the persona moratorium enforced in the GLD.
   * This value is only set on failure to delete a persona when the moratorium on that persona
   * is still in force at the GLD
   *
   * @return the number of seconds to the moratorium.
   */
  NPSLoginAPI_Method unsigned long
    NPSGetPersonaMoratorium() const { return (personaMoratorium_);}


  /**
   * Retrieve the persona list for the specified user from the global login database
   *
   * @param logged_in_As  A reference to persona id that will be filled in with the persona
   *     ID the user is currently logged in as.
   *
   * @param user_Status user status structure where the customer id and session key are written to
   *
   * @param ClientCallBack
   *
   * @param Context
   *
   * @return NPS_USER_VALID  on success
   */
  NPSLoginAPI_Method NPSSTATUS
  NPSGetUserStatus( NPS_CUSTOMERID customerId,
		    NPS_GAMEUSERID &logged_in_as,
		    NPS_UserStatus &user_status,
		    tfIdleCallBack  ClientCallBack,
		    void *          Context);



  //! Performs a token check on the supplied string.
  /**
   *    Perform a token check on the supplied string.
   *    @param token              the token to check
   *    @param checkCharacters    check characters for validity
   *    @param checkDirty_        text filter check
   *    @param checkReserved_     check against reserved word list
   *    @param ClientCallBack
   *    @param Context

   *    @return
   *    NPS_ACK            token check(s) succeeded.
   *    NPS_DIRTY_WORD     check failed filth test.
   *    NPS_TOKEN_INVALID  check failed - invalid characters in token
   *    NPS_TOKEN_RESERVED check failed - reserved word violation
   *    NPS_TIMEOUT        operation took to long to complete
   *    NPS_ERR            some other error occurred
   */

  NPSLoginAPI_Method NPSSTATUS
  NPSCheckToken( const char *    token,             // token to check
		 bool            checkCharacters,
		 bool            checkDirty,
		 bool            checkReserved,
		 tfIdleCallBack  ClientCallBack,
		 void *          Context );



  //! Performs a validation check on the supplied username.
  /**
   *    Performs a validation check on the supplied username. Does
   *    checks that the user name not already used on this shard.
   *    checks validity of token (filth test, validity test, reserved word test)
   *    in the future, will check against reserved persona list.
   *    It does NOT test persona limits!
   *
   *    @param customerId         user's customer id
   *    @param username           proposed user name (persona name)
   *    @param gamename           game name.
   *    @param ClientCallBack
   *    @param Context
   *
   *
   * @return
   *    NPS_USER_VALID     validation succeeded
   *    NPS_ERR_DUP_USER   persona name already in use on this shard.
   *    NPS_DIRTY_WORD     check failed filth test.
   *    NPS_TOKEN_INVALID  check failed - invalid characters in token
   *    NPS_TOKEN_RESERVED check failed - reserved word violation
   *    NPS_TIMEOUT        operation took to long to complete
   *    NPS_ERR            some other error occurred
   */

  NPSLoginAPI_Method NPSSTATUS
  NPSValidatePersonaName( NPS_CUSTOMERID  customer_id,
			  const char *    persona_name,
			  const char *    game_name,
			  tfIdleCallBack  ClientCallBack,
			  void *          Context );

private:

  static unsigned long personaMoratorium_;
  static unsigned long maxPersonaCount_;
};


#ifdef __cplusplus
extern "C"
{
#endif

#ifndef DYNAMIC_NPS_LOGIN_DLL

  NPSLoginDllExport cNPSLoginAPI* NPSCDECL
    NPSLoginAPI_GetInterface (const char * AuthLoginDllpath = NULL, const char *inAuthLoginBaseService=NULL, const char *inAuthLoginServer=NULL);
  NPSLoginDllExport void  NPSCDECL
    NPSLoginAPI_ReleaseInterface (void);

#else  // DYNAMIC_NPS_LOGIN_DLL
  typedef cNPSLoginAPI*
    (NPSCDECL *fNPSLoginAPI_GetInterface)(const char * AuthLoginDllpath, const char *inAuthLoginBaseService=NULL, const char *inAuthLoginServer=NULL);
  typedef void
    (NPSCDECL *fNPSLoginAPI_ReleaseInterface)(void);

#endif // DYNAMIC_NPS_LOGIN_DLL

#ifdef __cplusplus
}
#endif






#endif // _NPS_LOGIN_DLL_H
