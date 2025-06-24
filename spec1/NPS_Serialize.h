//  Name: NPS_Serialize.h
//
//  Author: Fraser Hutchinson, 1999 2000 <fhutchinson@ea.com>
//
//  Purpose: base class for all classes supporting serialization.
//
//  Replaces NPSMessageContainer - is now NPS_RawMessage.
//
//  Notes:
//  ------------------
//  This class is meant to be used as the base class for classes that
//  need serialization.
//  NPS_Serialize serializes the class into a int8 stream. Integral
//  values are (optionally, by default) converted to network int8 order.
//  Strings are written as pascal strings.
//
//  This class handles the memory allocation and deallocation of memory
//  required for the serialized buffer.
//
//  Simply derive from the NPS_Serialize and implement the following
//  three methods:
//      virtual uint16        serializeSizeOf() const;
//      virtual void          _doSerialize() = 0;
//      virtual void          _doDeserialize() =0;
//
//

#if !defined ( NPS_SERIALIZE_H_ )
#define NPS_SERIALIZE_H_

// Tune down "forcing to bool" compiler warning.
#if defined ( WIN32 )
#pragma warning(disable: 4800)
#endif

#include <stdlib.h>
#include <iostream>
#include <string.h>  // for memset() et al.
#include <time.h> // for time_t
//#include "NPSLoggable.h"
#include "NPS_Utils.h"

//! Base class for serialization support.
/*!
  This base class provides support for serialization - the process of converting
  the state of a structure or class to a byte stream.

  Example usage:
  \code
  myClass instance( arg1, arg2.... );
  uint16 len = 0;
  const unsigned char *buffer = instance.serialize( len );

  // you can now write the buffer :
  write( sock, buff, len );

  // and release the buffer (or else you'll have a memory leak).
  instance.releaseBuffer( buffer );

  // to deserialize, given a read buffer buff of at least the header::length()
  myClass instance
  instance.deserialize( buff );

  \endcode

  To derive from this class, you will need to reimplement
  \code
  virtual uint16        serializeSizeOf() const;
  virtual void          _doSerialize() = 0;
  virtual void          _doDeserialize() =0;
  \endcode

 */
class NPS_Serialize {
public:

  //! default constructor.
  NPS_Serialize( uint16 id =0, uint16 version =0 );

  //! automatically deserializes the buffer.
  /*!
    \param serialized_buffer
    \param owns_allocation if true, the class will delete allocation when done.
  */
  NPS_Serialize( const unsigned char *serialized_buffer, bool owns_allocation );

  //! copy constructor.
  NPS_Serialize( const NPS_Serialize & );
  //! assignment operator
  NPS_Serialize &       operator = ( const NPS_Serialize & );


  //! destructor. If set as allocation owner, will release memory.
  virtual ~NPS_Serialize();

  //@{
  /*!
   * Perform the serialization
   *
   * Allocates a buffer large enough to serialize to.
   * Caller owns the buffer returned, and should release it with releaseBuffer().
   * \param len the number of bytes written is returned here.
   */
  //@}
  const unsigned char * serialize( uint16 & len) const;
  const unsigned char * serialize( uint16 & len);

  //! Perform the serialization
  /*!
    Uses memory supplied by the caller.  Assumes space is >= serializeSizeOf()
  */
  const unsigned char * serialize( unsigned char *buff, uint16 &len );
  const unsigned char * serialize( unsigned char *buff, uint16 &len ) const;

  //! Perform the deserialization, populating the class from the supplied buffer.
  /*
    populates the class from the serialized stream in \p buff_in
    \param buff_in buffer containing the serialized stream
    \param deallocate if true, release the memory in buff_in
  */
  void                  deserialize( const unsigned char *buff_in,
				     bool deallocate = false );

  //! deallocate the buffer allocated by serialize()
  static void           releaseBuffer( const unsigned char *buffer );

  //! return the message version from the header
  uint16                messageVersion() const;

  //! return the message id (opcode) from the header
  uint16                messageId() const;

  //! set the message version in the header
  void                  setMessageVersion( uint16 );

  //! set the message id (opcode) in the header
  void                  setMessageId( uint16 );

  //! Get the checksum value
  uint32                checksum() const;

  //! Get the sequence number (same as checksum).
  uint32                sequenceNumber() const;

  //! Set the sequence number. Will override checksum. Cannot == 0
  void                  setSequenceNumber( uint32 sn );


  //! if set, the class will assume it owns the allocation.
  /*!
    If garbage collection is on,the class will assume that it should delete memory supplied
    via a deserialize() call.
  */
  void                  setGarbageCollection( bool on = false );

  //! returns true if the class assumes it owns the allocation.
  bool                  isGarbageCollectionSet() const;


  //! header management.
  /*!
    The header is backwards compatible for length and messageId
    This class has no members, but exists for namespacing. It manages a
    serialized buffer, referred to as its allocation.

    The header looks like this:
    \li 2 int8s msgid
    \li 2 int8s total msglen
    \li 2 int8s message version.
    \li 2 int8s reserved
    \li 4 int8s checksum
  */

  struct Header {
    //! Default constructor
    Header();

    //! Copy constructor
    Header( const Header & );

    //! constructor
    /*
      Uses the supplied buffer as its data allocation.
      If owns_allocation is set, it will take responsibility for
      deallocating the memory.
    */
    Header( const unsigned char * );

    //! Copy constructor. Does a shallow copy.
    Header &   operator = ( const Header & );

    //! Assignment operator. Does a shallow copy
    Header &   operator = ( const unsigned char * );

    // Destructor. if \p owns_allocation, it will deallocate the allocation.
    virtual ~Header();

    friend class NPS_Serialize;

    enum {
      size_  = (4*sizeof(uint16)) + sizeof(uint32)   //!< size of the header in bytes.
    };

    //! return the message id
    uint16                id()       const;
    //! returns the version number
    uint16                version()  const;
    //! returns the message length
    uint16                length()   const;
    // returns the checksum
    uint32                checksum() const;
    // returns the checksum
    uint32                sequenceNumber() const;

    //! writes the sequence number as the checksum to the header (nop if allocation is not set).
    void                  setSequenceNumber( uint32 );

  private:

    //! resets everything to a known state.
    void                  clear();
    //! sets the allocation to the supplied buffer
    void                  setAllocation( unsigned char * );
    //! deletes the allocation if \p own_allocation is set
    void                  deleteAllocation();

    //! writes the length to the header (nop if allocation is not set)
    void                  setLength( uint16 );
    //! writes the version to the header (nop if allocation is not set)
    void                  setVersion( uint16 );
    //! writes the message id to the header (nop if allocation is not set)
    void                  setId( uint16 );
    //! writes the checksum to the header (nop if allocation is not set).
    void                  setChecksum( uint32 );

    //@{
    //! returns a pointer to the first byte in allocation following the (logical) header
    unsigned char *       getDataPtr();
    const unsigned char * getDataPtr() const;
    //@}

    //! pointer to the first byte in allocation following the (logical) header
    unsigned char *       data_;

    //! if set, we will deallocate the allocation when finished with it.
    bool                  iOwnTheBuffer_;
  };



  /** @name Derivation
   * the following three methods MUST be implemented by the subclasses,
   * and must call the same method in the immediate base class, except
   * for the root class.
   *
   * To serialize, simply call serialize() to get a pointer to the
   * serialized buffer:
   *
   *  const unsigned char *buff = myObject.serialize();
   *
   *  To populate a class from the serialized buffer:
   *
   *    myObject.deserialize( buff );
   *
   *  Release the buffer's memory by passing it to releaseBuffer():
   *
   *  NPS_Serialize::releaseBuffer( buff );

   * You need not call NPS_Serialize::_doSerialize() or
   * NPS_Serialize::_doDeserialize(), but you MUST call
   * NPS_Serialize::_serializeSizeOf().
   *
   * \code
   *
   *  class myClass : public NPS_Serialize {
   *  public:
   *
   *      virtual uint16 serializeSizeOf() const {
   *          return NPS_SerializeSizeOf()
   *          + _serializeSizeOf( myInt ) +
   *          + _serializeSizeOf( myData );
   *      }
   *      virtual void  _doSerialize() {
   *          _serialize( myInt );
   *          _serialize( myData );
   *      }
   *      virtual void  _doDeserialize() {
   *          _deserialize( myInt );
   *          _deserialize( myData, 256 );
   *      }
   *
   *  private:
   *      int myInt;
   *      char myData[256];
   *  };
   *
   * \endcode
   *
   * If this class were derived from a class derived from NPS_Serialize, you
   * would need to implemented _doSerialize() and _doDeserialize() like so:
   * \code
   *      virtual void  _doSerialize() {
   *          MyParent::_doSerialize();
   *          _serialize( myInt );
   *          _serialize( myInt );
   *          _serialize( myData );
   *      }
   *      virtual void  _doDeserialize() {
   *          MyParent::_doDeserialize();
   *          _deserialize( myInt );
   *          _deserialize( myData, 256 );
   *      }
   * \endcode
   *
   *  Note that the methods in the immediate base class are called.
   *  The order that the members get serialized is not important except
   *  that the order must be the same for doSerialize() and _doDeserialize().
   *
   * \sa NPS_RawMessage class
   */
  //@{
  //! get the size required for the serialization buffer
  virtual uint16        serializeSizeOf() const;
  //! serialize this class up the vtree
  virtual void          _doSerialize() = 0;
  //! deserialize this class up the vtree
  virtual void          _doDeserialize() =0;
  //@}


  /** @name serializeHeader
   *  manipulate the default behavior of serializing the header
   *
   *
   *  This is a short term solution to a design flaw that I have been contemplating
   *  fixing for a while now.
   *  What I have been looking do is rework to NPS_Serialize to contain a subclassable header
   *  instance which determines the bytes stored in the ehader. I will get around to this at
   *  some point.
   *  When using serialized classes as member classes, it is sometimes useful to avoid
   *  the overheader of Header::size_ bytes in serialization. This defeats including the
   *  header during serialization for such classes.
   *  The is optionally set (and cleared) from the following methods:
   *  \li serialize( NPS_Serialize *, bool noHeader )
   *  \li deserialize ( NPS_Serialize *, bool noHeader )
   *  \li serializeSizeOf( const NPS_Serialize *, bool noHeader );
   *
   */
  //@{
  //! returns true if the header will be serialized
  bool                  serializeHeader() const;
  //! set to false if you do not want the header serialized
  void                  setSerializeHeader( bool = true );
  //@}



  //! Convenience class for storing and manipulating serialized buffers.
  class MessageBuffer {
  public:
    //! constructor.
    /*
      \param buff the serialized buffer.
      \param len the length of the buffer.
    */
    MessageBuffer( const unsigned char *buff = NULL,
		   uint16 len = 0 );
    //! destructor. Deallocates \p buffer if non-NULL
    ~MessageBuffer();

    //! returns the message id of the buffer
    uint16                   messageId() const;
    //! returns the message length of the buffer
    uint16                   messageLength() const;
    //! returns a pointer to the buffer
    const unsigned char *    buffer() const;
    //! returns the length of the serialized buffer.
    uint16                   length() const;

    //! assigns both the buffer and the length.
    void                     set( const unsigned char *buff, uint16 len );
    //! deallocates memory.
    void                     clear();
    //! allocates a buiffer of size \p len
    void                     allocate( uint16 len );

  private:

    const unsigned char *    buffer_;
    uint16                   length_;

  };



protected:

  Header                  header_;

  /** @name _serialize
   *
   * Each of following takes a pointer to the buffer to write
   * to, and a reference to the argument to write.
   * For ints, if in_nbo is set (true), then it is assumed that the value
   * is maintained internally in network byte order, and consequently no
   * conversion will be performed.
   *
   * To serialize/deserialize byte streams, use
   *   _serialize( const char *, uint16 len ) and
   *   _deserialize( char *, uint16 len )
   *  These, by default, do not use null-termination.
   */
  //@{
  //! serialize a class instance, (omits header)
  void                  _serialize( const NPS_Serialize * );
  //! serialize a bool to one byte.
  void                  _serialize( bool );
  //! serialize a signed byte
  void                  _serialize( int8 );
  //! serialize an unsigned byte
  void                  _serialize( uint8 );
  //! serialize
  void                  _serialize( unsigned int, bool in_nbo = false );
  void                  _serialize( int16 ,   bool in_nbo = false );
  void                  _serialize( uint16,   bool in_nbo = false );
  void                  _serialize( int,      bool in_nbo = false );
  void                  _serialize( int32,    bool in_nbo = false );
  void                  _serialize( uint32,   bool in_nbo = false );
#if !defined ( WIN32 )
  void                  _serialize( int64,    bool in_nbo = false );
  void                  _serialize( uint64,   bool in_nbo = false );
#endif
  void                  _serialize( float32,  bool in_nbo = false );
  void                  _serialize( double64, bool in_nbo = false );
  //! serialize a 'C' string (assumes null-temination).
  void                  _serialize( const char *str );
  //! serialize a buffer; does not assume null-termination.
  void                  _serialize( const char *buff, uint16 len );
  //@}

  /** @name _deserialize
   *
   * Each of following takes a pointer to the buffer to read from
   * and a reference to the argument to be written to.
   * For ints, if in_nbo is set, then it is assumed that the value is
   * maintained internally in network int8 order, and consequently no
   * conversion will be performed.
   */
  //@{
  //! serialize a class instance, (omits header)
  void                  _deserialize( NPS_Serialize * );
  void                  _deserialize( bool & );
  void                  _deserialize( int8 & );
  void                  _deserialize( uint8 & );
  void                  _deserialize( unsigned int &, bool in_nbo = false );
  void                  _deserialize( int16 & ,   bool in_nbo = false );
  void                  _deserialize( uint16 &,   bool in_nbo = false );
  void                  _deserialize( int &,      bool in_nbo = false );
  void                  _deserialize( int32 &,    bool in_nbo = false );
  void                  _deserialize( uint32 &,   bool in_nbo = false );
#if !defined ( WIN32 )
  void                  _deserialize( int64 &,    bool in_nbo = false );
  void                  _deserialize( uint64 &,   bool in_nbo = false );
#endif
  void                  _deserialize( float32 &,  bool in_nbo = false );
  void                  _deserialize( double64 &, bool in_nbo = false );
  // writes to user-allocated space. Deserializes the entire length as encoded
  // (does not look for null-termination).
  uint16                _deserialize( char *dest,
				      uint16 max_len,
				      bool terminate = true );
  // allocates memory for buffer.
  char *                _deserialize( uint16 &len, char *&dest, bool terminate = false );
  //@}


  /** @name _serializeSizeOf
   *
   * methods to be called when implementing serializeSizeOf()
   */
  //@{
  static uint16         _serializeSizeOf( const NPS_Serialize * );
  static unsigned int   _serializeSizeOf( unsigned int );
  static uint16         _serializeSizeOf( int8 );
  static uint16         _serializeSizeOf( uint8 );
  static uint16         _serializeSizeOf( int16 );
  static uint16         _serializeSizeOf( uint16 );
  static uint16         _serializeSizeOf( int32 );
  static uint16         _serializeSizeOf( uint32 );
  static uint16         _serializeSizeOf( int );

#if !defined ( WIN32 )
  static uint16         _serializeSizeOf( int64 );
  static uint16         _serializeSizeOf( uint64 );
#endif
  static uint16         _serializeSizeOf( float32 );
  static uint16         _serializeSizeOf( double64 );

  //! get the size need to serialize a 'C' string (assumes null-temination).
  static uint16         _serializeSizeOf( const char *str );
  //! does not assumes null-temination, but optionally tests for it. If check_terminationLen is set, use string length if terminated, or len,  whichever is smaller.
  static uint16         _serializeSizeOf( const char *str,
					  uint16 len,
					  bool check_termination = false  );

  //@}


  //!calculate the checksum.
  /*!
    reimplement this method to use this hook to generate the checksum value
    Must be independent of the value on the header's checksum field.
  */
  virtual uint32        generateChecksum() const;

private:

  void                  _serializeRaw( const unsigned char *buff, uint16 len );
  void                  _deserializeRaw( unsigned char *buff, uint16 len );

  unsigned char *       serializedBufferPtr_;

  uint16                messageId_;
  uint16                messageVersion_;

  //! will normally be true, but is optionally cleared when serializing
  bool                  serializeHeader_;

  //! if set, will override checksum. Cannot = 0.
  uint32                sequenceNumber_;

};




//! Simple message class ala NPS_Message
/*!
 *  a simple instance of an NPS_Serialize class that emulates NPS_Message.
 *  creates a copy of the passed buffer.
 *
 * example usage:
 * \code
 *
 * // to create and serialize a message:
 *
 *    char msg_string = "this is the message I want to send";
 *    NPS_RawMessage message( NPS_USER_VALID, msg_string, strlen(msg_string+1 );
 *    uint16 len = 0;
 *    const unsigned char *buffer = message.serialize( len );
 *    write( sock, buffer, len );
 *
 * // to deserialize and display a message:
 *
 *    NPS_RawMessage message;
 *    message.deserialize( buffer );
 *    const unsigned char *msg = reinterpret_cast<const char *>(message.getBlob());
 *    cout << "The message opcode is " << message.messageId()
 *         << ", string:" << msg << endl;
 *
 *   // you would not be responsible for deleting this memory with NPS_RawMessageGC
 *   delete[] msg;
 * \endcode
 *
*/
class NPS_RawMessage : public NPS_Serialize {
public:

  //! default constructor
  /*!
    makes a copy of \p buff
    \param id the message id (NPS_CODE opcode)
    \param len the message len (length of \p buff)
    \param buff the message buffer
  */
  NPS_RawMessage( uint16 id =0, uint16 len = 0, const char *buff = NULL );
  //! destructor.
  ~NPS_RawMessage();

  //! assignment operator, acts like default constructor.
  NPS_RawMessage &      operator = ( const NPS_RawMessage & );

  // returns a pointer to the data blob
  const char *          getBlob() const;
  // generates a message using NPS_Message semantics
  /*!
    makes a copy of \p buff
    \param id the message id (NPS_CODE opcode)
    \param len the message len (length of \p buff)
    \param buff the message buffer
  */
  void                  createMessage( uint16 id,
				       uint16 len = 0,
				       const char *buff = NULL );

  //! reimplemented from NPS_Serialize
  uint16                serializeSizeOf() const;
  //! reimplemented from NPS_Serialize
  void                  _doSerialize();
  //! reimplemented from NPS_Serialize
  void                  _doDeserialize();

protected:

  void                  allocate( uint16 len );
  virtual void          release();

  char *                buffer_;
  uint16                length_;

};


//! same as NPS_Message, but automatically deallocates memory allocated for the message blob
class NPS_RawMessageGC : public NPS_RawMessage {
public:

  NPS_RawMessageGC( uint16 id =0, uint16 len = 0, const char *buff = NULL );

  NPS_RawMessageGC &    operator = ( const NPS_RawMessageGC &msg );
protected:

  void                  release();

};





///////////////////////////////////////////////////////////////////////////////////
//
//    I N L I N E   M E T H O D S
//

inline
NPS_Serialize::NPS_Serialize( const NPS_Serialize &s )
  : messageId_(s.messageId_),
    messageVersion_(s.messageVersion_),
    serializeHeader_(s.serializeHeader_)
{
  header_ = s.header_;
}


inline NPS_Serialize &
NPS_Serialize::operator = ( const NPS_Serialize &s ) {
  if( this != &s ) {
    messageVersion_ =s.messageVersion_;
    messageId_ =s.messageId_;
    header_ = s.header_;
  }
  return *this;
}


inline void
NPS_Serialize::releaseBuffer( const unsigned char *buffer ) {
  delete[] const_cast<char *>(reinterpret_cast<const char *>(buffer));
}


// protected methods.

inline uint16
NPS_Serialize::serializeSizeOf() const {
  // cout << "sizeof header =" << header_.size_ << endl;
  return ( serializeHeader_ )? header_.size_ : 0;
}



// Serialize a data element. These do the actual work
// tres simple, just do necessary conversions and call serializeRaw()
// with the length


// on most systems, bool is an int, on some systems it';s a short.
// we really only need one int8, so let's cast it as a int8.
inline void
NPS_Serialize::_serialize( bool b ) {
  *serializedBufferPtr_++ = (b)? 1 : 0;
}

inline void
NPS_Serialize::_serialize( int8 c ) {
  *serializedBufferPtr_++ = (uint8)c;
}


inline void
NPS_Serialize::_serialize( uint8 c ) {
  *serializedBufferPtr_++ = c;
}

inline void
NPS_Serialize::_serialize( unsigned int val, bool b ) {
  if( !b ) val = hton(val);
  _serializeRaw( reinterpret_cast<const unsigned char *>(&val), sizeof(val) );
}

inline void
NPS_Serialize::_serialize( int16 val, bool b ) {
  if( !b ) val = hton(val);
  _serializeRaw( reinterpret_cast<const unsigned char *>(&val), sizeof(val) );
}


inline void
NPS_Serialize::_serialize( uint16 val, bool b ) {
  if( !b ) val = hton(val);
  _serializeRaw( reinterpret_cast<const unsigned char *>(&val), sizeof(val) );
}

inline void
NPS_Serialize::_serialize( int val, bool b ) {
  if( !b ) val = hton((uint32)val);
  _serializeRaw( reinterpret_cast<const unsigned char *>(&val), sizeof(val) );
}

inline void
NPS_Serialize::_serialize( int32 val, bool b ) {
  if( !b ) val = hton(val);
  _serializeRaw( reinterpret_cast<const unsigned char *>(&val), sizeof(val) );
}

inline void
NPS_Serialize::_serialize( uint32 val, bool b ) {
  if( !b ) val = hton(val);
  _serializeRaw( reinterpret_cast<const unsigned char *>(&val), sizeof(val) );
}

#if !defined ( WIN32 )
inline void
NPS_Serialize::_serialize( int64 val, bool b ) {
  if( !b ) val = hton(val);
  _serializeRaw( reinterpret_cast<const unsigned char *>(&val), sizeof(val) );
}

inline void
NPS_Serialize::_serialize( uint64 val, bool b ) {
  if( !b ) val = hton(val);
  _serializeRaw( reinterpret_cast<const unsigned char *>(&val), sizeof(val) );
}
#endif

inline void
NPS_Serialize::_serialize( float32 val, bool b ) {
  if( !b ) val = hton(val);
  _serializeRaw( reinterpret_cast<const unsigned char *>(&val), sizeof(val) );
}

inline void
NPS_Serialize::_serialize( double64 val, bool b ) {
  if( !b ) val = hton(val);
  _serializeRaw( reinterpret_cast<const unsigned char *>(&val), sizeof(val) );
}

inline void
NPS_Serialize::_serializeRaw( const unsigned char *buff, uint16 len ) {
  memcpy( serializedBufferPtr_, buff, len );
  serializedBufferPtr_ += len;
}


// each of following takes a pointer to the buffer to read from
// and a reference to the argument to be written to. They
// return a pointer to the next readable area.

// on Solaris, bool is an int (! - 15 wasted bits!!!) so, just
// cast as a int8 (char).
inline void
NPS_Serialize::_deserialize( bool &b ) {
  b = (bool)(*serializedBufferPtr_++);
}


inline void
NPS_Serialize::_deserialize( int8 &val ) {
  val = *serializedBufferPtr_++;
}


inline void
NPS_Serialize::_deserialize( unsigned int &val, bool b ) {
  _deserializeRaw( reinterpret_cast<unsigned char *>(&val), sizeof(val) );
  if( !b ) val = ntoh(val);
}

inline void
NPS_Serialize::_deserialize( uint8 &val ) {
  val = *serializedBufferPtr_++;
}

inline void
NPS_Serialize::_deserialize( int16 &val, bool b ) {
  _deserializeRaw( reinterpret_cast<unsigned char *>(&val), sizeof(val) );
  if( !b ) val = ntoh(val);
}

inline void
NPS_Serialize::_deserialize( uint16 &val, bool b ) {
  _deserializeRaw( reinterpret_cast<unsigned char *>(&val), sizeof(val) );
  if( !b ) val = ntoh(val);
}

inline void
NPS_Serialize::_deserialize( int &val, bool b ) {
  _deserializeRaw( reinterpret_cast<unsigned char *>(&val), sizeof(val) );
  if( !b ) val = ntoh(val);
}

inline void
NPS_Serialize::_deserialize( int32 &val, bool b ) {
  _deserializeRaw( reinterpret_cast<unsigned char *>(&val), sizeof(val) );
  if( !b ) val = ntoh(val);
}

inline void
NPS_Serialize::_deserialize( uint32 &val, bool b ) {
  _deserializeRaw( reinterpret_cast<unsigned char *>(&val), sizeof(val) );
  if( !b ) val = ntoh(val);
}

#if !defined ( WIN32 )
inline void
NPS_Serialize::_deserialize( int64 &val, bool b ) {
  _deserializeRaw( reinterpret_cast<unsigned char *>(&val), sizeof(val) );
  if( !b ) val = ntoh(val);
}

inline void
NPS_Serialize::_deserialize( uint64 &val, bool b ) {
  _deserializeRaw( reinterpret_cast<unsigned char *>(&val), sizeof(val) );
  if( !b ) val = ntoh(val);
}
#endif

inline void
NPS_Serialize::_deserialize( float32 &val, bool b ) {
  _deserializeRaw( reinterpret_cast<unsigned char *>(&val), sizeof(val) );
  if( !b ) val = ntoh(val);
}

inline void
NPS_Serialize::_deserialize( double64 &val, bool b ) {
  _deserializeRaw( reinterpret_cast<unsigned char *>(&val), sizeof(val) );
  if( !b ) val = ntoh(val);
}

inline void
NPS_Serialize::_deserializeRaw( unsigned char *dest, uint16 len ) {
  memcpy( dest, serializedBufferPtr_, len );
  serializedBufferPtr_ += len;
}


// methods to be called when implementing serializeSizeOf()

// when serializing an embedded class, we precede the write with the sizeof
inline uint16
NPS_Serialize::_serializeSizeOf( const NPS_Serialize *s ) {
  // cout << "sizeof int8 =" << sizeof(val) << endl;
  return s->serializeSizeOf() + sizeof( uint16 );
}


inline unsigned int
NPS_Serialize::_serializeSizeOf( unsigned int val ) {
  return sizeof(val);
}

inline uint16
NPS_Serialize::_serializeSizeOf( int8 val ) {
  // cout << "sizeof int8 =" << sizeof(val) << endl;
  return sizeof(val);
}

inline uint16
NPS_Serialize::_serializeSizeOf( uint8 val ) {
  // cout << "sizeof uint8 =" << sizeof(val) << endl;
  return sizeof(val);
}

inline uint16
NPS_Serialize::_serializeSizeOf( int16 val ) {
  // cout << "sizeof int16 =" << sizeof(val) << endl;
  return sizeof(val);
}
inline uint16
NPS_Serialize::_serializeSizeOf( uint16 val ) {
  // cout << "sizeof uint16 =" << sizeof(val) << endl;
  return sizeof(val);
}

inline uint16
NPS_Serialize::_serializeSizeOf( int val ) {
  // cout << "sizeof int32 =" << sizeof(val) << endl;
  return sizeof(val);
}

inline uint16
NPS_Serialize::_serializeSizeOf( int32 val ) {
  // cout << "sizeof int32 =" << sizeof(val) << endl;
  return sizeof(val);
}
inline uint16
NPS_Serialize::_serializeSizeOf( uint32 val ) {
  // cout << "sizeof uint32 =" << sizeof(val) << endl;
  return sizeof(val);
}

#if !defined ( WIN32 )
inline uint16
NPS_Serialize::_serializeSizeOf( int64 val ) {
  // cout << "sizeof int64 =" << sizeof(val) << endl;
  return sizeof(val);
}
inline uint16
NPS_Serialize::_serializeSizeOf( uint64 val ) {
  // cout << "sizeof uint32 =" << sizeof(val) << endl;
  return sizeof(val);
}
#endif

inline uint16
NPS_Serialize::_serializeSizeOf( float32 val ) {
  // cout << "sizeof float32 =" << sizeof(val) << endl;
  return sizeof(val);
}
inline uint16
NPS_Serialize::_serializeSizeOf( double64 val ) {
  // cout << "sizeof double64 =" << sizeof(val) << endl;
  return sizeof(val);
}

// assumes null-temination.
inline uint16
NPS_Serialize::_serializeSizeOf( const char *str ) {
  uint16 ret = strlen(str) + sizeof(uint16);
  // cout << "sizeof string =" << ret << endl;
  return ret;
}

inline void
NPS_Serialize::setGarbageCollection( bool b ) {
  header_.iOwnTheBuffer_ = b;
}

inline bool
NPS_Serialize::isGarbageCollectionSet() const {
  return header_.iOwnTheBuffer_;
}

inline uint16
NPS_Serialize::messageVersion() const {
  return messageVersion_;
}

inline uint16
NPS_Serialize::messageId() const {
  return messageId_;
}

inline void
NPS_Serialize::setMessageVersion( uint16 ver ) {
  messageVersion_ = ver;
  header_.setVersion(ver);
}

inline void
NPS_Serialize::setMessageId( uint16 id ) {
  messageId_ = id;
  header_.setId(id);
}




//**************** NPS_Serialize::Header ******************//

inline
NPS_Serialize::Header::Header()
  : data_(NULL),
    iOwnTheBuffer_(false)
{}

inline
NPS_Serialize::Header::Header( const NPS_Serialize::Header & h )
  : data_(NULL),
    iOwnTheBuffer_(false)
{
  *this = h;  // invoke the copy constructor.
}

inline
NPS_Serialize::Header::Header( const unsigned char *alloc )
  : data_(NULL),
    iOwnTheBuffer_(false)
{
  setAllocation( const_cast<unsigned char *>(alloc) );
}


inline NPS_Serialize::Header &
NPS_Serialize::Header::operator = ( const NPS_Serialize::Header &h ) {
  if( h.data_ ) {
    uint16 len = h.length();
    if(len) {
      setAllocation( new unsigned char[len] );
      memcpy( data_, h.data_, len );
    }
  }
  return *this;
}


inline NPS_Serialize::Header &
NPS_Serialize::Header::operator = ( const unsigned char *alloc ) {
  setAllocation( const_cast< unsigned char *>(alloc) );
  return *this;
}



inline
NPS_Serialize::Header::~Header() {
  if( iOwnTheBuffer_ )
    deleteAllocation();
}


inline void
NPS_Serialize::Header::clear() {
  if(data_)
    memset( data_, '\0', size_ );
}

inline uint16
NPS_Serialize::Header::id() const {
  uint16 id = 0;
  if( data_ ) {
    memcpy( &id, data_, sizeof(id) );
  }
  return ntoh(id);
}

inline void
NPS_Serialize::Header::setId( uint16 id ) {
  if( data_ ) {
    id = hton(id);
    memcpy( data_, &id, sizeof(id) );
  }
}


inline uint16
NPS_Serialize::Header::length() const {
  uint16 len = 0;
  if( data_ ) {
    memcpy( &len, data_ + sizeof(uint16), sizeof(len) );
  }
  return ntoh(len);
}

inline void
NPS_Serialize::Header::setLength( uint16 len ) {
  if( data_ ) {
    len = hton(len);
    memcpy( data_ + sizeof(uint16), &len, sizeof(len) );
  }
}

inline uint16
NPS_Serialize::Header::version() const {
  uint16 ver = 0;
  if( data_ ) {
    memcpy( &ver, data_ + (2*sizeof(uint16)), sizeof(ver) );
  }
  return ntoh(ver);
}


inline void
NPS_Serialize::Header::setVersion( uint16 ver ) {
  if( data_ ) {
    ver = hton(ver);
    memcpy( data_ + (2*sizeof(uint16)), &ver, sizeof(ver) );
  }
}



inline uint32
NPS_Serialize::Header::sequenceNumber() const {
  return checksum();
}


inline uint32
NPS_Serialize::Header::checksum() const {
  uint32 sum = 0;
  if( data_ ) {
    memcpy( &sum, data_ + (4*sizeof(uint16)), sizeof(sum) );
  }
  return ntoh(sum);
}


inline void
NPS_Serialize::Header::setChecksum( uint32 sum ) {
  if( data_ ) {
    sum = hton(sum);
    memcpy( data_ + (4*sizeof(uint16)), &sum, sizeof(sum) );
  }
}


inline void
NPS_Serialize::Header::setSequenceNumber( uint32 val ) {
  setChecksum(val);
}


inline void
NPS_Serialize::Header::setAllocation( unsigned char * ptr ) {
  deleteAllocation();
  data_ = ptr;
}


inline void
NPS_Serialize::Header::deleteAllocation() {
  if( iOwnTheBuffer_ && data_ ) {
    delete[] data_;
  }
  data_ = NULL;
}


inline unsigned char *
NPS_Serialize::Header::getDataPtr() {
  return ( data_ )? data_ + size_ : NULL;
}

inline const unsigned char *
NPS_Serialize::Header::getDataPtr() const {
  return ( data_ )? data_ + size_ : NULL;
}





inline uint16
NPS_RawMessage::serializeSizeOf() const {
  return NPS_Serialize::serializeSizeOf() +
    _serializeSizeOf( buffer_, length_, false ); // false -> do not check for termination
}

inline void
NPS_RawMessage::_doSerialize() {
  _serialize( buffer_, length_ );
}

inline void
NPS_RawMessage::_doDeserialize() {
  release();
  buffer_ = _deserialize( length_, buffer_, false );
}



inline
NPS_RawMessageGC::NPS_RawMessageGC( uint16 id, uint16 len, const char *buff )
  : NPS_RawMessage(id,len,buff )
{}

inline NPS_RawMessageGC &
NPS_RawMessageGC::operator = ( const NPS_RawMessageGC &msg ) {
  if( this != &msg ) {
      reinterpret_cast< NPS_RawMessage &>(*this) =
	reinterpret_cast< const NPS_RawMessage &>(msg);
  }
  return *this;
}

inline void
NPS_RawMessageGC::release() {
  delete[] buffer_;
  buffer_ = NULL;
  length_ = 0;
}


// simple hack for now.
inline uint32
NPS_Serialize::generateChecksum() const {

  uint16 a = header_.version();
  uint16 b = header_.length();
  uint32 val = b;
  val += (a << 8);

  return val;
}


inline bool
NPS_Serialize::serializeHeader() const {
  return serializeHeader_;
}

inline void
NPS_Serialize::setSerializeHeader( bool b ) {
  serializeHeader_ = b;
}












//***********  class MessageBuffer *****************//

inline
NPS_Serialize::MessageBuffer::MessageBuffer( const unsigned char *buff,
					      uint16 len )
  : buffer_(buff),
    length_(len)
{}


inline
NPS_Serialize::MessageBuffer::~MessageBuffer() {
  // for some reason, the Visual C++ compiler couldn't figure out how
  // to delete an unsigned char allocation
  delete[] (char *)buffer_;
  buffer_ = NULL;
  length_ = 0;
}



inline uint16
NPS_Serialize::MessageBuffer::messageId() const {
  if( !buffer_ )
    return 0;
  NPS_Serialize::Header header( buffer_ );
  return header.id();
}


inline uint16
NPS_Serialize::MessageBuffer::messageLength() const {
  if( !buffer_ )
    return 0;
  NPS_Serialize::Header header( buffer_ );
  return header.length();
}


inline const unsigned char *
NPS_Serialize::MessageBuffer::buffer() const {
  return buffer_;
}


inline uint16
NPS_Serialize::MessageBuffer::length() const {
  return length_;
}

inline void
NPS_Serialize::MessageBuffer::set( const unsigned char *buff, uint16 len ) {
  delete[] (char *)buffer_;
  buffer_ = buff;
  length_ = len;
}



inline void
NPS_Serialize::MessageBuffer::clear() {
  delete[] (char *)buffer_;
  buffer_ = NULL;
  length_ = 0;
}




inline void
NPS_Serialize::MessageBuffer::allocate( uint16 len ) {
  delete[] (char *)buffer_;
  length_ = len;
  buffer_ = new unsigned char [length_];
}


inline uint32
NPS_Serialize::sequenceNumber() const {
  return sequenceNumber_;
}

inline uint32
NPS_Serialize::checksum() const {
  return header_.checksum();
}


inline void
NPS_Serialize::setSequenceNumber( uint32 cs ) {
  sequenceNumber_ = cs;
  if( header_.data_ ) header_.setChecksum(cs);
}

#endif // #if !defined ( NPS_SERIALIZE_H_ )
