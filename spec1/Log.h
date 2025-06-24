/**
* Utility to write file logs.
*
* @author: cwalters@ea.com
*
* Created:6/27/2001
*
*/

#ifndef LOG_H
#define LOG_H

#ifdef WIN32
# include <windows.h>
# include <io.h>
#else
# define NO_PRINT_LOG
#endif

#include <fstream>
#include <sys/stat.h>

#ifdef NO_PRINT_LOG

#define Log_Begin(__b__)        {}
#define Log_End()               {}
#define Log(__b__)              {}
#define Log_FileVersion(__b__)  {}
#define Log_Hex(__b__, __d__)   {}

#else

class cLog:public std::ofstream
{
public:
  cLog(const char * fileName="Log.txt", BOOL forceWrite=FALSE);
  virtual
    ~cLog(void);

  void
    fileVersion(char * name);
  void
    appVersion(void);
  void
    hexDump(char * data, int bytes);

  void
    timeStamp(void);
  void
    lock(void);
  void
    unlock(void);
  void	setWriteLog(BOOL set);
  BOOL	canWriteLog(void) {return(m_WriteLog);};

private:
	BOOL	m_WriteLog;
  CRITICAL_SECTION
    m_Mutex;
};
extern cLog * cLogObj;

#ifndef Log
#define Log(__b__){if(cLogObj){if(cLogObj->canWriteLog()){cLogObj->lock();*cLogObj << __b__ << std::flush;cLogObj->unlock();}}}
#endif

#endif

#endif // LOG_H
