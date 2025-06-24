//====================================================================
// cQ.h
//=====
//
// Description:
//    A Generic list data structure.
//
// Programmer(s)
//    - C.R.Walters     ---   Initial Programming
//
// Project:
//    NFS III
//
//====================================================================
//           Copyright 1998 Electronic Arts Seattle.
//====================================================================
// $Header: /export/home/cvsroot/NPS/src/Common/NPSLib/cQ.h,v 1.4 2001/01/13 19:43:23 abakken Exp $
//
// $Log: cQ.h,v $
// Revision 1.4  2001/01/13 19:43:23  abakken
// syncing with VSS
//
// Revision 1.3  2001/01/13 00:51:24  abakken
// added CriticalSection code for unix
//
// Revision 1.2  2001/01/12 07:23:20  abakken
// first port to unix
//

#ifndef cQ_H
#define cQ_H

#include <stdio.h>

#include "NPSDll_Types.h"

#ifdef WIN32
# include "windows.h"
#else

# include "CriticalSection.h"
#endif

#include "assert.h"

// -------------------------------------------------------------------
// Types
// -------------------------------------------------------------------
#define kQ_NoNodes        -1  // For use with tsQinfo.maxSelections
#define kQ_UnlimitedNodes  0  // For use with tsQinfo.maxItems & maxSelections

typedef enum
{
   kQnode_LeftIsGreater = -1,
   kQnode_Equal=0,
   kQnode_RightIsGreater=1
}teQ_CompareResults;

typedef struct tsQnode
{
   struct tsQnode  * prev;
   struct tsQnode  * next;
   struct tsQnode  * tmpNext; // Used for sorting
   BOOL            isSelected;
   void            * data;
}tsQnode;

typedef enum
{
   kQ_FIFO,
   kQ_LIFO,
   kQ_Ascend,
   kQ_Descend
}teQorder;

typedef struct
{
   int   itemSize;      // # chars in an item, 0=pointer copy, dynamic sizing requires user defined "sizeOf" callback.
   int   maxItems;      // # items allowed, 0=unlimited. If maxItems is reached, addItem() will delete oldest.
   int   maxSelections; // # items selectable, -1=none 0=unlimited n>0=n.  If maxItems is reached no more will be allowed.
   BOOL  threadSafe;    // set False only if list cannot be reentered
}tsQinfo;

typedef enum
{
   kQ_ToggleSelect,
   kQ_Select,
   kQ_DeSelect
}teQ_SelType;


// ===================================================================
// cQ
// =====
// -------------------------------------------------------------------
// cQ(tsQinfo * listInfo=NULL)
//    Initializes list basics.  If parameter is passed, fully inits list.
// -------------------------------------------------------------------
// Init(tsQinfo * listInfo=NULL)
//    Fully inits list
// -------------------------------------------------------------------
// void  Iterate(teQorder, void * callerInfo = NULL, tfList_CB_Iterate * pfIterate = NULL, const void * criteria = NULL, tfList_CB_Compare * pfCompare = NULL);
//    use criteria to pass additional info to the compare callback.
//    usefull for information such as "col num".
// -------------------------------------------------------------------
// BOOL  Node_Find_Selected(void * item);
//
// -------------------------------------------------------------------
// BOOL  Node_Find_N (const void * criteria, int N, teQorder order, void * item, tfList_CB_Compare * pfCompare=NULL,  void * filtercriteria=NULL, tfList_CB_Filter * pfFilter=NULL);
//
// Purpose:
//    Find an item in the list.
//    Copy that item to the given memory location.
//
// Input:
//    tsList   If Null, or it's reserved member ptr is Null, function
//             returns FALSE.
//    N        The item number to retrieve (as per sort order)
//    order    Order in which to sort items.
//    item     If Null, it is ignored. Else if function returns TRUE
//             then "item" contains the found contents.  If "item"
//             is not large enough to store the contents of the found
//             Item, the results are undefined (i.e. that's a bug)
//
//   void * filtercriteria
//  tfList_CB_Filter * pfFilter
//          If a filter and criteria are specified, may ignore item based upon
//          pfFilter() callback results (TRUE to accept, FALSE to ignore);
// Return:
//    True if an Item exists in the Nth position.
//
// -------------------------------------------------------------------
// BOOL  Node_Get(const void * criteria, void * item = NULL);
//
// Purpose:
//    Find an item in the list.
//    Copy that item to the given memory location.
//    Delete that item from the list.
//
// Input:
//    tsList   If Null, or it's reserved member ptr is Null, function
//             returns FALSE.
//    criteria If Null, list head is deleted.  Else Item must be found
//             via the "find" callback.  If criteria is defined but the
//             "find" callback isn't, returns FALSE.
//    item     If Null, it is ignored. Else if function returns TRUE
//             then "item" contains the deleted contents.  If "item"
//             is not large enough to store the contents of the deleted
//             Item, the results are undefined (i.e. that's a bug)
//
// Return:
//    True if an Item was deleted.
//
// Description:
//    If criteria is non-Null, and Item is found; Item is copied into
//    "item" parameter if "item" is non-Null, and Item is deleted from
//    the list.
//    If criteria is Null, the Item on the lists head is copied into
//    "item" parameter it "item" is non-Null, and Item is deleted from
//    the list.
// -------------------------------------------------------------------
// ===================================================================
class cForeignIterator
{
public:
   virtual BOOL
      Iterator(const void * item, int count, BOOL isSelected, void * callerInfo)=0;
};
class cForeignComparison
{
public:
   virtual teQ_CompareResults
      Comparor(const void * leftItem, const void * rightItem, const void * criteria)=0;
};
class cForeignFinder
{
public:
   virtual BOOL
      Finder(const void * criteria, const void * item)=0;
};
class cForeignModifier
{
public:
   virtual BOOL
      Modifier(const void * criteria, const void * item)=0;
};
class cForeignFilter
{
public:
   virtual BOOL
      Filter(const void * item, const void * filtercriteria)=0;
};

class cQ
{
public:
   cQ(tsQinfo * listInfo=NULL);
   virtual ~cQ(void);
   void  Init(tsQinfo * listInfo);

   // List level methods
   // ------------------
   void  Iterate(teQorder order = kQ_FIFO, void * callerInfo = NULL, cForeignIterator * pForeign_Iterator = NULL, const void * criteria = NULL, cForeignComparison * pfCompare = NULL);
   void  DeleteAll(void);
   int   Count(void);
   int   Count_Selected(void);

	// Calling IsDirty is the ONLY way to clear the dirty flag
	// SetDirty(), and any Add/Remove/Modify or Delete method
	// will set the list's dirty flag.

   BOOL  IsDirty(void);				// Returns True if the dirty flag is set, and always sets the flag to FALSE
   BOOL  SetDirty(void); 			// Manually set the dirty flag

   // Node level methods
   // ------------------
   BOOL  Node_Add(void * item, char * memName = (char *) "listnode");
   BOOL  Node_Select(const void * criteria, teQ_SelType selType, cForeignFinder * pfFind=NULL);
   BOOL  Node_Modify(const void * criteria, void * item, cForeignModifier * pfModify = NULL);
   BOOL  Node_Delete(const void * criteria, cForeignFinder * pfFind = NULL);

   BOOL  Node_Find(const void * criteria, void * item=NULL, cForeignFinder * pfFind = NULL);
   BOOL  Node_Find_Selected(void * item);	 // This fucntion only works if only one item can be selected at a time. see tsQinfo
   BOOL  Node_Find_N (const void * criteria, int N, teQorder order, void * item, cForeignComparison * pfCompare=NULL,  void * filtercriteria=NULL, cForeignFilter * pfFilter=NULL);
   BOOL  Node_Get(const void * criteria, void * item = NULL);

protected:
   // These are visible for cross list locking.
   // Extreme care should be taken not to dead lock because you forgot to unlock.
   // ---------------------------------------------------------------------------
   void  Lock(void);
   void  UnLock(void);

   // used in sorted iterations
   // -------------------------
   virtual teQ_CompareResults
         NodeData_Comparor(const void * /*leftItem*/, const void * /*rightItem*/, const void * /*criteria*/){assert(0);return (kQnode_Equal);}

   // used in get and update methods, ret=True if items match
   // -------------------------------------------------------
   virtual BOOL
         NodeData_Finder(const void * /*criteria*/, const void * /*item*/){assert(0);return (FALSE);}

   // used in iterate, ret=True to continue & False to stop iteration
   // ---------------------------------------------------------------
   virtual BOOL
         NodeData_Iterator(const void * /*item*/, int /*count*/, BOOL /*isSelected*/, void * /*callerInfo*/){assert(0);return (FALSE);}

   // used to get size of node when "listInfo.itemSize" is set to zero at list creation
   // ---------------------------------------------------------------------------------
   virtual int
         NodeData_SizeOf(const void * item);

   virtual void
         NodeData_Modify(/*const*/ void * /*newItem*/, void * /*oldItem*/){assert(0);return;}

   virtual void *
         NodeData_Alloc(void * item, char * memName = (char *) "listnode");

   virtual void
         NodeData_Free(void * item);

   virtual void
         NodeData_Copy(void * dest, const void * src, int length);

private:
   tsQinfo     m_Qinfo;
   tsQnode     * m_Head;
   tsQnode     * m_Tail;
   int         m_Num;
   int         m_NumSelected;
   CRITICAL_SECTION
               m_Mutex;
   BOOL        m_MutexInited;
   BOOL        m_Dirty;

   void        Help_RemoveNode (tsQnode * node);
   tsQnode *   Help_FindNode(const void * criteria, cForeignFinder * pfFind=NULL);
   void        Help_SelectNode (tsQnode * node, teQ_SelType selType);
   inline void Help_InitCriticalSection(void);
   inline void Help_DeleteCriticalSection(void);
   inline void Help_EnterCriticalSection(void);
   inline void Help_LeaveCriticalSection(void);
};
#endif //cQ_H
