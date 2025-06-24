//
// Name:
//  cSmartPtr.h
//
// Purpose:
//
// Designer:
//
// Copyright (c) 2000 by Electronic Arts.
// All Rights Reserved.
//

#ifndef cSmartPtr_H
#define cSmartPtr_H

#if defined(WIN32)
# include <windows.h>
#else
# include <assert.h>
# include <synch.h>
# include <pthread.h>
# include <thread.h>
#endif
#include <map>

template<class T>
class cSmartPtr
{
public:
  cSmartPtr(T* ptr = 0)
  :m_Ptr(ptr)
  {
    init();
  }
  cSmartPtr(const cSmartPtr& rhs)
  :m_Ptr(rhs.m_Ptr)
  {
    init();
  }
  ~cSmartPtr()
  {
    if (m_Ptr)
      m_Ptr->decRef();
  }

  cSmartPtr&
    operator=(const cSmartPtr& rhs)
  {
    if (m_Ptr != rhs.m_Ptr)
    {
      if (m_Ptr)
        m_Ptr->decRef();
      m_Ptr = rhs.m_Ptr;
      init();
    }
    return(*this);
  }

  T*
    operator->()const
  {
    return(m_Ptr);
  }
  T&
    operator*()const
  {
    return(*m_Ptr);
  }

private:
  T*
    m_Ptr;

  void
    init()
  {
    if (m_Ptr==NULL)
      return;
    m_Ptr->incRef();
  }
};


/*!
 * For UNIX machines, this class needs the following defined in the
 * programs main:
 *    pthread_mutex_t cRefCount::m_Mutex = PTHREAD_MUTEX_INITIALIZER;
 */
class cRefCount
{
public:
  /**
   */
  void incRef(void);

  /**
   */
  void decRef(void);

protected:
  cRefCount(void);
  cRefCount(const cRefCount& rhs);
  cRefCount& operator=(const cRefCount& rhs);
  virtual ~cRefCount(void);

private:
  volatile long
    m_Count;
#if ! defined (WIN32)
  static pthread_mutex_t m_Mutex;
#endif

};

inline void cRefCount::incRef(void)
{
#if defined(WIN32)
  InterlockedIncrement((long*)&m_Count);
#else
  pthread_mutex_lock (&cRefCount::m_Mutex);
  assert (m_Count >= 0);
  ++m_Count;
  pthread_mutex_unlock (&cRefCount::m_Mutex);
#endif
  //std::cerr<<"cRefCount inc, post inc cnt = "<<m_Count<<std::endl;
}

inline void cRefCount::decRef(void)
{
#if defined(WIN32)
  if (InterlockedDecrement ((long*) &m_Count) == 0)
  {
    //std::cerr<<"cRefCount delete this"<<std::endl;
    delete this;
  }

#else
  pthread_mutex_lock (&cRefCount::m_Mutex);
  assert (m_Count > 0);
  --m_Count;
  if (m_Count == 0)
  {
    //std::cerr<<"cRefCount delete this"<<std::endl;
    pthread_mutex_unlock (&cRefCount::m_Mutex);
    delete this;
  }
  else
  {
    pthread_mutex_unlock (&cRefCount::m_Mutex);
  }
#endif
}

inline cRefCount::cRefCount(void)
  : m_Count(0)
{
  //std::cerr<<"cRefCount constructor"<<std::endl;
}

inline cRefCount::cRefCount(const cRefCount& rhs)
  : m_Count(0)
{
  //std::cerr<<"cRefCount copy constructor"<<std::endl;
}

inline cRefCount& cRefCount::operator=(const cRefCount& rhs)
{
  //std::cerr<<"cRefCount = op"<<std::endl;
  return(*this);
}

inline cRefCount::~cRefCount(void)
{
  //std::cerr<<"~cRefCount"<<std::endl;
};


template <class T_NodeData>
class cNode
{
private:
  struct sNode_Data:public cRefCount
  {
    T_NodeData * mData;
    void
      init(T_NodeData * data)
    { mData = data;}
    sNode_Data(T_NodeData *data)
    { init(data);}
    sNode_Data(const sNode_Data& rhs)
    { init(rhs.mData);}
    ~sNode_Data()
    { delete mData;}
  };
  cSmartPtr<sNode_Data> m_SmartPtr;

public:
  cNode (T_NodeData *data)
  :m_SmartPtr(new sNode_Data(data))
  {}
  cNode (void)
  :m_SmartPtr()
  {}

  T_NodeData *
    getData (void)
  { return(m_SmartPtr->mData);}
};

template <class T_Key, class T_NodeData>
class cMap
{
public:
  cMap(void)
  {
		InitLock();
  }
  virtual
    ~cMap(void)
  {
    Lock();
    tMap::iterator it;
    if (!mMap.empty())
    {
      for (it = mMap.begin(); it != mMap.end();)
      {
        Remove(it->first);
        it = mMap.begin();
      }
    }
    UnLock();
		DeleteLock();
  }
  virtual bool
    Add(T_Key &key, T_NodeData * data)
  {
    if (!data)
      return(FALSE);
    Lock();
    mMap.insert(tMap::value_type(key,*data));
    UnLock();
    return(TRUE);
  }

  virtual bool
    Remove(const T_Key &key, T_NodeData &data)
  {
    Lock();
    if (Find(key,data))
    {
      mMap.erase(key);
      UnLock();
      return (TRUE);
    }
    UnLock();
    return(FALSE);
  }

	virtual bool
    Remove(const T_Key &key)
  {
    Lock();
    if (Find(key))
    {
      mMap.erase(key);
      UnLock();
      return (TRUE);
    }
    UnLock();
    return(FALSE);
  }
  virtual bool
    Find(const T_Key &key, T_NodeData &data)
  {
    Lock();
    if (!mMap.empty())
    {
      tMap::iterator it = mMap.find(key);
      if (it != mMap.end())
      {
        data = (*it).second;
        UnLock();
        return (TRUE);
      }
    }
    UnLock();
    return(FALSE);
  }
  virtual bool
    Find(const T_Key &key)
  {
    Lock();
    if (!mMap.empty())
    {
      tMap::iterator it = mMap.find(key);
      if (it != mMap.end())
      {
        UnLock();
        return (TRUE);
      }
    }
    UnLock();
    return(FALSE);
  }
  virtual int
    Count(void)const{return(mMap.size());}

protected:
  virtual void
    InitLock(void)
	{
#if defined(WIN32)
		InitializeCriticalSection(&mMutex);
#else
#endif
	}
	virtual void
		DeleteLock(void)
	{
#if defined(WIN32)
    DeleteCriticalSection(&mMutex);
#else
#endif
	}
  virtual void
    Lock(void)
	{
#if defined(WIN32)
		EnterCriticalSection(&mMutex);
#else
#endif
	}
  virtual void
    UnLock(void)
	{
#if defined(WIN32)
		LeaveCriticalSection(&mMutex);
#else
#endif
	}

  typedef std::map<T_Key, T_NodeData> tMap;
	tMap
		mMap;

private:
#if defined(WIN32)
  CRITICAL_SECTION
    mMutex;
#else
#endif

};


#endif // cSmartPtr_H
