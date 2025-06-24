///////////////////////////////////////////////////////////////////////////
//  File Name:     GLD_UserStatus.h
//  Author:        Fraser Hutchinson
//  Date Created:  4/12/01
//
//  Current Owner: Fraser Hutchinson
//
//  Purpose:
//
//  Copyright 1997-2001 Electronic Arts, Inc.
///////////////////////////////////////////////////////////////////////////



#if !defined ( _GLDP_USER_STATUS_H_ )
#define  _GLDP_USER_STATUS_H_

#include <NPSTypes.h>
#include <NPS_Serialize.h>
#include <NPS_SessionKey.h>
#include "GLD_UserAction.h"
#include <GLDP_BaseMessage.h>




//! Class that encapusulates a request for the user status.


class GLDP_UserStatusRequest: public GLDP_CustomerId {
public:

  //! default constructor
  /*
    \param c_id the id of the customer in question
    \param cache_is_Dirty if set, mark cache as dirty, effectively causing update
  */

  enum OP {
    useCache_ = 0,
    refreshCache_,
    clearCacheEntry_,
    clearCache_
  };


  GLDP_UserStatusRequest( NPS_CUSTOMERID c_id = 0, OP op = useCache_ );

  //! copy constructor. make a deep copy
  GLDP_UserStatusRequest( const GLDP_UserStatusRequest & );

  //!destructor
  ~GLDP_UserStatusRequest();

  //! assignment operator.
  GLDP_UserStatusRequest & operator = ( const GLDP_UserStatusRequest & );

  //! return true is cache_is_dirty flag is set.
  OP                    operation() const;

  //! set cache_is_dirty flag
  void                  setOperation( OP );

  //! reimplemented from NPS_Serialized.
  virtual uint16        serializeSizeOf() const;
  //! reimplemented from NPS_Serialized.
  virtual void          _doSerialize();
  //! reimplemented from NPS_Serialized.
  virtual void          _doDeserialize();

private:

  OP                    operation_;

};



//! Class that contains user status information. Returned on login
/*!
 *  This class contains the current state of the user, including the
 * session key and expiration date and ban and gag records.
 * Typically, it will be returned to the client upon login to the LoginServer,
 * and periodically requested by the rebroadcasters.
 *
 */

class GLDP_UserStatus : public NPS_Serialize {
public:

  //! default constructor
  /*!
    \param customer_id the id of the customer in question
    \param persona_id the id of the persona (gameuser)
  */
  GLDP_UserStatus( NPS_CUSTOMERID customerId_= 0,
		   NPS_GAMEUSERID persona_id = 0 );

  /*!
    \param customer_id the id of the customer in question
    \param persona_id the id of the persona (gameuser)
    \param key the session key
    \param ban the ban record
    \param gag the gag record
  */
  GLDP_UserStatus( NPS_CUSTOMERID          customerId,
		   NPS_GAMEUSERID          persona_id,
		   const NPS_SessionKey &  key,
		   const GLD_UserAction &  ban,
		   const GLD_UserAction &  gag );

  //! copy constructor
  GLDP_UserStatus( const GLDP_UserStatus & );

  //! assignment operator. Performs a deep copy.
  GLDP_UserStatus &     operator = ( const GLDP_UserStatus & );

  //! returns true if the user is authorized to play this game
  bool                          isAuthorized() const;

  //! returns the NPS customer id
  uint32                        customerId() const;

  //! returns the NPS persona id
  uint32                        personaId() const;

  //! returns NULL if not banned, otherwise a pointer to the ban record
  const GLD_UserAction *        ban() const;

  //! returns NULL if not gagged, otherwise a pointer to the gag record
  const GLD_UserAction *        gag() const;

  //! returns the sessionKey.
  const NPS_SessionKey &        sessionKey() const;

  //! rteturns true if this instance came from the proxy cache
  bool                          isCacheHit() const;

  //! assign the customer id.
  void                          setCustomerId( NPS_CUSTOMERID );
  //! assign the persona id
  void                          setPersonaId( NPS_GAMEUSERID );
  //! assigns the session key.
  void                          setSessionKey( const NPS_SessionKey & );

  //! assign the ban record
  void                          setBan( const GLD_UserAction & );

  //! assign the gag record
  void                          setGag( const GLD_UserAction & );

  //! set as a cache hit.
  void                          setAsCacheHit( bool );

  //! calc the buffer size required for serialization
  virtual uint16                serializeSizeOf() const;
  //! serialize this class
  virtual void                  _doSerialize();
  //! deserialise this class
  virtual void                  _doDeserialize();

protected:


private:

  GLD_UserAction                ban_;
  GLD_UserAction                gag_;

  NPS_CUSTOMERID                customerId_;
  NPS_GAMEUSERID                personaId_;
  bool                          isCacheHit_;
  NPS_SessionKey                sessionKey_;

};



////////////////////////////////////////////////////////////////////////////
//
//    I N L I N E   M E T H O D S
//

// ****************** class GLDP_UserStatusRequest ***************

inline
GLDP_UserStatusRequest::GLDP_UserStatusRequest( NPS_CUSTOMERID              c_id,
						GLDP_UserStatusRequest::OP  op )
  : GLDP_CustomerId( c_id ),
    operation_(op)
{}

inline
GLDP_UserStatusRequest::GLDP_UserStatusRequest( const GLDP_UserStatusRequest &usr )
  : GLDP_CustomerId( reinterpret_cast< const GLDP_CustomerId &>(usr) ),
    operation_( usr.operation_ )
{}


inline
GLDP_UserStatusRequest::~GLDP_UserStatusRequest()
{}


inline GLDP_UserStatusRequest &
GLDP_UserStatusRequest::operator = ( const GLDP_UserStatusRequest &usr ) {
  if( this != &usr ) {
    reinterpret_cast<GLDP_CustomerId &>(*this) =
      reinterpret_cast<const GLDP_CustomerId &>(usr);
    operation_  = usr.operation_;
  }
  return *this;
}


inline GLDP_UserStatusRequest::OP
GLDP_UserStatusRequest::operation() const {
  return operation_;
}


inline void
GLDP_UserStatusRequest::setOperation( GLDP_UserStatusRequest::OP op ) {
  operation_ = op;
}


inline uint16
GLDP_UserStatusRequest::serializeSizeOf() const {
  return GLDP_CustomerId::serializeSizeOf() +
    _serializeSizeOf( (uint32)operation_ );
}


inline void
GLDP_UserStatusRequest::_doSerialize() {
  GLDP_CustomerId::_doSerialize();
  _serialize( (uint32)operation_ );
}

//! reimplemented from NPS_Serialized.
inline void
GLDP_UserStatusRequest::_doDeserialize() {
  GLDP_CustomerId::_doDeserialize();
  _deserialize( (uint32 &)operation_ );
}


//***************** class GLDP_UserStatus **********************

inline
GLDP_UserStatus::GLDP_UserStatus( NPS_CUSTOMERID customer_id,
				  NPS_GAMEUSERID persona_id )
  : customerId_(customer_id),
    personaId_(persona_id)
{}

inline
GLDP_UserStatus::GLDP_UserStatus( NPS_CUSTOMERID          customer_id,
				  NPS_GAMEUSERID          persona_id,
				  const NPS_SessionKey &  key_in,
				  const GLD_UserAction &  ban_in,
				  const GLD_UserAction &  gag_in )
  : ban_(ban_in),
    gag_(gag_in),
    customerId_(customer_id),
    personaId_(persona_id),
    isCacheHit_(false),
    sessionKey_(key_in)
{}



inline
GLDP_UserStatus::GLDP_UserStatus( const GLDP_UserStatus &us )
  : ban_         ( us.ban_ ),
    gag_         ( us.gag_ ),
    customerId_  ( us.customerId_ ),
    personaId_   ( us.personaId_ ),
    isCacheHit_  ( us.isCacheHit_ ),
    sessionKey_  ( us.sessionKey_ )
{
}

// assignment operator. Performs a deep copy.
inline GLDP_UserStatus &
GLDP_UserStatus::operator = ( const GLDP_UserStatus &us ) {
  if( this != &us ) {
    ban_         = us.ban_;
    gag_         = us.gag_;
    customerId_  = us.customerId_;
    personaId_   = us.personaId_;
    isCacheHit_  = us.isCacheHit_;
    sessionKey_  = us.sessionKey_;
  }
  return *this;
}

// returns true if the user is authorized to play this game
inline bool
GLDP_UserStatus::isAuthorized() const {
  return sessionKey_.isValid();
}

// returns the NPS customer id
inline uint32
GLDP_UserStatus::customerId() const {
  return customerId_;
}


// returns the NPS customer id
inline uint32
GLDP_UserStatus::personaId() const {
  return personaId_;
}

// returns NULL if not banned, otherwise a pointer to the ban record
inline const GLD_UserAction *
GLDP_UserStatus::ban() const {
  return ( ban_.isValid() )? &ban_ : NULL;
}

// returns NULL if not gagged, otherwise a pointer to the gag record
inline const GLD_UserAction *
GLDP_UserStatus::gag() const {
  return ( gag_.isValid() )? &gag_ : NULL;
}

// returns the sessionKey.
inline const NPS_SessionKey &
GLDP_UserStatus::sessionKey() const {
  return sessionKey_;
}

inline bool
GLDP_UserStatus::isCacheHit() const {
  return isCacheHit_;
}


// assign the customer id.
inline void
GLDP_UserStatus::setCustomerId( NPS_CUSTOMERID id ) {
  customerId_ = id;
}

// assign the persona id
inline void
GLDP_UserStatus::setPersonaId( NPS_GAMEUSERID id ) {
  personaId_ = id;
}

// assigns the session key.
inline void
GLDP_UserStatus::setSessionKey( const NPS_SessionKey &key ) {
  sessionKey_ = key;
}

// assign the ban record
inline void
GLDP_UserStatus::setBan( const GLD_UserAction &action ) {
  ban_ = action;
}

// assign the gag record
inline void
GLDP_UserStatus::setGag( const GLD_UserAction &action ) {
  gag_ = action;
}

inline void
GLDP_UserStatus::setAsCacheHit( bool b ) {
  isCacheHit_ = b;
}


// calc the buffer size required for serialization
inline uint16
GLDP_UserStatus::serializeSizeOf() const {
  return NPS_Serialize::serializeSizeOf() +
    _serializeSizeOf( (uint32)customerId_ ) +
    _serializeSizeOf( (uint32)personaId_ ) +
    _serializeSizeOf( isCacheHit_ ) +
    _serializeSizeOf( &ban_ ) +
    _serializeSizeOf( &gag_ ) +
    _serializeSizeOf( &sessionKey_ );
}

// serialize this class
inline void
GLDP_UserStatus::_doSerialize() {
  _serialize( (uint32)customerId_ );
  _serialize( (uint32)personaId_ );
  _serialize( isCacheHit_ );
  _serialize( &ban_ );
  _serialize( &gag_ );
  _serialize( &sessionKey_ );
}


inline void
GLDP_UserStatus::_doDeserialize() {
  _deserialize( (uint32 &)customerId_ );
  _deserialize( (uint32 &)personaId_ );
  _deserialize( isCacheHit_ );
  _deserialize( &ban_ );
  _deserialize( &gag_ );
  _deserialize( &sessionKey_ );
}



#endif  // #if !defined ( _GLDP_USER_STATUS_H_ )
