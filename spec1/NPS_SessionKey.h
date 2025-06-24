///////////////////////////////////////////////////////////////////////////
//  File Name:     NPS_SessionKey.h
//  Author:        Fraser Hutchinson
//  Date Created:  4/12/01
//
//  Current Owner: Fraser Hutchinson
//
//  Purpose:       Class wrapper to our session key that will be used for
//                 encrypting client-server communications
//
//  Copyright 1997-2001 Electronic Arts, Inc.
///////////////////////////////////////////////////////////////////////////

#if !defined ( _NPS_SESSION_KEY_H )
#define _NPS_SESSION_KEY_H



#include <NPS_Serialize.h>
#include <NPSTypes.h>

//! class that encapsulates a session key
/*!
  Each key consists of a char buffer (a binary blob), a length of the key,
  and an expiry date.
*/


class NPS_SessionKey : public NPS_Serialize {
public:

  //! default constructor.
  NPS_SessionKey();

  //! Copy constructor., Makes a copy (deep copy) of the input buffer
  NPS_SessionKey( const char *buff, uint16 len, time_t expiry_date = 0 );
  //! destructor
  ~NPS_SessionKey();

  //! assignment operator - makes a deep copy.
  NPS_SessionKey &      operator = ( const NPS_SessionKey & );

  enum {
    maxLength_ = NPS_SESSION_KEY_LEN
  };

  //! returns the maximum length a key may take.
  static const uint16   maxLength() { return maxLength_; }

  //! returns true if the session key is valid (has been set)
  bool                  isValid() const;

  //! returns a pointer to the non-null-terminated key data.
  const char *          key() const;

  //! returns a pointer to the key data, and writes the length to \a len
  const char *          key( uint16 &len ) const;

  //! get the expiration date.
  time_t                expiryDate() const;

  //! Set the key, using \a len bytes of the supplied buffer
  void                  setKey( const char *buf, uint16 len );

  //! Set the key's expiration date. Unsafe to drink after this time
  void                  setExpiryDate( time_t );

  //! returns the key length. will be <= maxLength()
  uint16                length() const;


  // NPS_Serialize virtual methods implementation.
  //! returns the length of the buffer required to serialize this class.
  virtual uint16        serializeSizeOf() const;
  //! perform the serialization.
  virtual void          _doSerialize();
  //! deserialize into this class.
  virtual void          _doDeserialize();

private:

  char                  key_[maxLength_+1];
  uint16                keyLength_;
  time_t                expiryDate_;

};





////////////////////////////////////////////////////////////////////////////
//
//    I N L I N E   M E T H O D S
//
//

inline
NPS_SessionKey::NPS_SessionKey( )
  : keyLength_(0),
    expiryDate_(0)
{
  memset (key_, 0, maxLength_);
}

inline
NPS_SessionKey::NPS_SessionKey( const char *buf, uint16 len, time_t expiry_date )
  : keyLength_(0),
    expiryDate_(expiry_date)
{
  key_[0] = key_[maxLength_] = '\0';
  setKey( buf, len );
}


inline
NPS_SessionKey::~NPS_SessionKey()
{}


inline NPS_SessionKey &
NPS_SessionKey::operator = ( const NPS_SessionKey &sk ) {
  if( this != &sk )
  {
    setKey( sk.key(), sk.keyLength_ );
  }
  return *this;
}

inline const char *
NPS_SessionKey::key() const {
  return key_;
}

inline const char *
NPS_SessionKey::key( uint16 &len ) const {
  len = keyLength_;
  return key_;
}

inline time_t
NPS_SessionKey::expiryDate() const {
  return expiryDate_;
}

inline void
NPS_SessionKey::setKey( const char *ptr, uint16 len ) {
  keyLength_ = (len > maxLength_)? maxLength_ : len;
  memcpy (key_, ptr, keyLength_);
}

inline void
NPS_SessionKey::setExpiryDate( time_t t ) {
  expiryDate_ = t;
}


// returns the length of the buffer required to serialize this class.
inline uint16
NPS_SessionKey::serializeSizeOf() const {
  return NPS_Serialize::serializeSizeOf() +
    _serializeSizeOf( key_, keyLength_ ) +
    _serializeSizeOf( expiryDate_ );
}

// perform the serialization.
inline void
NPS_SessionKey::_doSerialize() {
  _serialize( key_, keyLength_ );
  _serialize( expiryDate_ );
}

// deserialize into this class.
inline void
NPS_SessionKey::_doDeserialize() {
  keyLength_ = _deserialize( key_, maxLength_ );
  _deserialize( expiryDate_ );
}

//XXX fixme!
inline bool
NPS_SessionKey::isValid() const {
  return expiryDate_ > 0;
}


inline uint16
NPS_SessionKey::length() const {
  return keyLength_;
}



#endif // #if !defined ( _NPS_SESSION_KEY_H )
