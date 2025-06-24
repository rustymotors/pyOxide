// Name: NPS_Utils.h
//
// Author: Fraser Hutchinson <fhutchinson@ea.com>
//
// Purpose: utilitarian routines
//
// Notes:


#if !defined ( NPS_UTILS_H_ )
#define  NPS_UTILS_H_

#if !defined( _REENTRANT)
#define _REENTRANT
#endif

#include <string.h>
#include <time.h>


#if defined ( WIN32 )
#include <winsock.h>
#else
#include <arpa/inet.h>  // for ntohx() and htonx()
#include <sys/isa_defs.h>  // _BIG_ENDIAN
#endif

#if !defined ( _BIG_ENDIAN )
#define NPS_LITTLE_ENDIAN
#else
#define NPS_BIG_ENDIAN
#endif


class NPS_TimeString {
public:

  NPS_TimeString( time_t = 0 );
  NPS_TimeString( const NPS_TimeString & );

  time_t                getTime() const;
  const char *          set(time_t );
  void                  now();
  const char *          str() const;

protected:

  void                  makeString();

private:

  enum {
    maxStringLength_ = 32
  };

  time_t                time_;
  char                  buffer_[maxStringLength_];
};





//#include <NPS_Types.h>
typedef char                int8;  // equiv to byte
typedef unsigned char       uint8; // equiv to ubyte
typedef short               int16;
typedef unsigned short      uint16;

typedef long                 int32;
typedef unsigned long        uint32;

#if !defined ( WIN32 )
typedef long long           int64;
typedef unsigned long long  uint64;
#endif
typedef float               float32;
typedef double              double64;



inline unsigned int     hton( unsigned int val );

inline int16            hton( int16 val );
inline uint16           hton( uint16 val );

inline bool             hton( bool val );

inline int32            hton( int32 val );
inline uint32           hton( uint32 val );

#if !defined ( WIN32 )
inline int64            hton( int64 val );
inline uint64           hton( uint64 val );
#endif
inline float32          hton( float32 val );
inline double64         hton( double64 val );



inline unsigned int     ntoh( unsigned int val );
inline int              ntoh( int val );

inline int16            ntoh( int16 val );
inline uint16           ntoh( uint16 val );

inline bool             ntoh( bool val );

inline int32            ntoh( int32 val );
inline uint32           ntoh( uint32 val );

#if !defined ( WIN32 )
inline int64            ntoh( int64 val );
inline uint64           ntoh( uint64 val );
#endif

inline float32          ntoh( float32 val );
inline double64         ntoh( double64 val );







// this all should be templated......
char *
reverse_byte_order( char *in, uint16 len );


inline bool
hton( bool val ) {
  return hton( (uint32)val );
}

inline bool
ntoh( bool val ) {
  return ntoh( (uint32)val );
}

// coercion from signed to unsigned

inline int16
hton( int16 val )  {
  return hton( (uint16)val );
}


inline unsigned int
hton( unsigned int val )  {
  return hton( (uint32)val );
}

inline int32
hton( int32 val )  {
  return hton( (uint32)val );
}

#if !defined ( WIN32 )
inline int64
hton( int64 val )  {
  return hton( (uint64)val );
}
#endif

inline int16
ntoh( int16 val )  {
  return ntoh( (uint16)val );
}


inline int
ntoh( int val ) {
  return ntoh( (int32)val );
}


inline unsigned int
ntoh( unsigned int val ) {
  return ntoh( (uint32)val );
}

inline int32
ntoh( int32 val )  {
  return ntoh( (uint32)val );
}

#if !defined ( WIN32 )
inline int64
ntoh( int64 val )  {
  return ntoh( (uint64)val );
}
#endif

// implementation.
inline uint16
hton( uint16 val ) {
  return htons(val);
}

inline uint32
hton( uint32 val ) {
  return htonl(val);
}

#if !defined ( WIN32 )
inline uint64
hton( uint64 val ) {
#if defined ( NPS_LITTLE_ENDIAN )
  reverse_byte_order( reinterpret_cast<char *>(&val), sizeof(val) );
#endif
  return val;
}
#endif


inline float32
hton( float32 val ) {
#if defined ( NPS_LITTLE_ENDIAN )
  reverse_byte_order( reinterpret_cast<char *>(&val), sizeof(val) );
#endif
  return val;
}


inline double64
hton( double64 val ) {
#if defined ( NPS_LITTLE_ENDIAN )
  reverse_byte_order( reinterpret_cast<char *>(&val), sizeof(val) );
#endif
  return val;
}



inline uint16
ntoh( uint16 val ) {
  return ntohs(val);
}

inline uint32
ntoh( uint32 val ) {
  return ntohl(val);
}

#if !defined ( WIN32 )
inline uint64
ntoh( uint64 val ) {
#if defined ( NPS_LITTLE_ENDIAN )
  reverse_byte_order( reinterpret_cast<char *>(&val), sizeof(val) );
#endif
  return val;
}
#endif


inline float32
ntoh( float32 val ) {
#if defined ( NPS_LITTLE_ENDIAN )
  reverse_byte_order( reinterpret_cast<char *>(&val), sizeof(val) );
#endif
  return val;
}

inline double64
ntoh( double64 val ) {
#if defined ( NPS_LITTLE_ENDIAN )
  reverse_byte_order( reinterpret_cast<char *>(&val), sizeof(val) );
#endif
  return val;
}





inline
NPS_TimeString::NPS_TimeString( time_t t )
: time_(0)
{
  buffer_[0] = buffer_[maxStringLength_] = '\0';
  set(t);
}

inline
NPS_TimeString::NPS_TimeString( const NPS_TimeString &ts )
  : time_(0)
{
  buffer_[0] = buffer_[maxStringLength_] = '\0';
  set(ts.time_);
}


inline time_t
NPS_TimeString::getTime() const {
  return time_;
}


inline void
NPS_TimeString::now() {
  set( time(NULL) );
}

inline const char *
NPS_TimeString::str() const {
  return buffer_;
}





#endif // #ifndef ( NPS_UTILS_H_ )
