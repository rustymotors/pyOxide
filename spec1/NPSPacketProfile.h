// ******************************************************************************************************
// Module Name:			PktProfile.c
// Written By:			David R. Sullivan
//
// Purpose:				General Purpose throughput monitoring
// Group:				Electonic Arts Online Technology Group
//
// Version Information:
//
//	CREATED:			April 2, 1998 David R. SUllivan
//
// ******************************************************************************************************
// Copyright 1998 Electronic Arts, All Rights Reserved
// ******************************************************************************************************

#ifndef _NPSPACKETPROFILE_H_
#define _NPSPACKETPROFILE_H_

typedef struct PktTimeMarker
{
	unsigned long msecs;
	unsigned long seconds;

} PktTimeMarker;

typedef struct PktProfileObject
{
	// Initialization stuff
	PktTimeMarker	ObjectTimeInit;			// Time Object Initialized

	// Time Values for This Object
	unsigned long	TimeMarker;				// Last Time Marker in MM:SS:MSEC
	unsigned long	CurrentTime;			// Current Time in MM:SS:MSEC
	unsigned long	ElapsedTime;			// Elapsed Time in MM:SS:MSEC

	// Current Stats
	unsigned long	CountThisMarker;		// Number of Counts Sent this Market Period
	unsigned long	UpdatesThisMarker;		// Number of Updates Sent this Marker Period

	// Stats for Last Second of TIme
	unsigned long	CountLastSecond;		// Number of Counts Send in the last second worth of time
	unsigned long	UpdatesLastSecond;		// Number of Updates Sent in the last second.
	unsigned long	TimeLastSecond;			// Time marker for above

	// Stats for Largest Count Per Second
	unsigned long	LargestCountMarker;		// The Largest Number of Counts sent in a market period
	unsigned long	LargestTimeMarker;		// The actual time of Largest marker

	// Stats for Largest Number of Updates Per Second
	unsigned long	LargestUpdatesMarker;	// "" "" Updates.
	unsigned long	LargestPTimeMarker;		// The actual time of the Largest Updates marker

	// Stat Totals
	unsigned long	TotalCount;				// Total Number of Counts written since the last init call
	unsigned long	TotalSeconds;			// Total Number of seconds elapsed since the last init call
	unsigned long	TotalUpdates;			// Total Number of Updates written since the last init call

	// Stat Averagres
	unsigned long	AverageCount;			// Average Number of Counts per second
	unsigned long	AverageUpdates;			// Average Number of Updates per second

	// Stat Means
	unsigned long	MeanCount;				// Average Number of Counts per second
	unsigned long	MeanUpdates;			// Average Number of Updates per second


} PktProfileObject;

#ifdef __cplusplus
extern "C"
{
#endif


// ======================================================================================================
// PktProfileObjectInit();
//
//		Reset the object's profile information
//
// inputs:		PktProfileObject *object	Profile Object to Reset
// returns:		void
// uses:		GetTimeMSecs(), GetTimeMarker();
//
void PktProfileObjectInit(PktProfileObject *object);

// ======================================================================================================
// PktProfileAddCount();
//
//		Add to the objects profile count, incriments the number of Updates
//
// inputs:		PktProfileObject *object	Profile Object to Reset
// returns:		void
//
void PktProfileAddCount(PktProfileObject *object, unsigned long length);

void GetTimeMarker(PktTimeMarker *timeMarker);

#if 0
unsigned long GetTimeMsecs(PktTimeMarker *timeMarker);
unsigned long GetTimeSeconds(PktTimeMarker *timeMarker);
#endif

#ifdef __cplusplus
}
#endif

#endif
